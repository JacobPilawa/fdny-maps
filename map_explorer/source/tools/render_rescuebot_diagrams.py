#!/usr/bin/env python3
from __future__ import annotations

import json
import math
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
import importlib.util
import sys

ROOT = Path(__file__).resolve().parents[1]
EXTRACTED = ROOT / "extracted"
DATA_MAPS = ROOT / "FDNY Firefighter 1.0" / "FireFighter American Hero" / "Data" / "Maps"

spec = importlib.util.spec_from_file_location("render_annotated_maps", ROOT / "tools" / "render_annotated_maps.py")
ra = importlib.util.module_from_spec(spec)
sys.modules["render_annotated_maps"] = ra
assert spec.loader
spec.loader.exec_module(ra)

spec2 = importlib.util.spec_from_file_location("extract_gbsp", ROOT / "tools" / "extract_gbsp.py")
eg = importlib.util.module_from_spec(spec2)
sys.modules["extract_gbsp"] = eg
assert spec2.loader
spec2.loader.exec_module(eg)


def font(size: int, bold: bool = False):
    cands = [
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Verdana Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Verdana.ttf",
    ]
    for c in cands:
        try:
            return ImageFont.truetype(c, size)
        except Exception:
            pass
    return ImageFont.load_default()


def load_map_geom(bsp: Path, entity_ys: list[float] | None = None):
    _, _, payloads = eg.read_chunks(bsp)
    faces = eg.parse_faces(payloads.get(11, b""))
    verts = eg.vecs(payloads.get(14, b""))
    idx = list(__import__("struct").unpack("<" + "i" * (len(payloads.get(13, b"")) // 4), payloads.get(13, b""))) if 13 in payloads else []
    textures = eg.parse_textures(payloads.get(18, b""))
    texinfo = eg.parse_texinfo(payloads.get(17, b""))
    planes = eg.parse_planes(payloads.get(10, b""))
    floors, walls = ra.classify_geometry(faces, verts, idx, planes, texinfo, textures)
    floors = [f for f in floors if f["area"] >= 200]
    if entity_ys:
        low = min(entity_ys) - 160
        high = max(entity_ys) + 160
        nearby = [f for f in floors if low <= f["y"] <= high]
        if nearby:
            floors = nearby

    wall_segments = []
    seen = set()
    for wall in walls:
        pts = wall["points"]
        xz = [(p[0], p[2]) for p in pts]
        best = None
        for i, p1 in enumerate(xz):
            for p2 in xz[i + 1 :]:
                d = (p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2
                if best is None or d > best[0]:
                    best = (d, p1, p2)
        if not best or best[0] < 80 * 80:
            continue
        _, a, b = best
        qa = (round(a[0] / 16) * 16, round(a[1] / 16) * 16)
        qb = (round(b[0] / 16) * 16, round(b[1] / 16) * 16)
        key = tuple(sorted([qa, qb]))
        if key in seen:
            continue
        seen.add(key)
        wall_segments.append((qa, qb))
    return floors, wall_segments


def ccw(a, b, c):
    return (c[1] - a[1]) * (b[0] - a[0]) > (b[1] - a[1]) * (c[0] - a[0])


def segments_intersect(a, b, c, d):
    return ccw(a, c, d) != ccw(b, c, d) and ccw(a, b, c) != ccw(a, b, d)


def wall_blocks_trace(player, victim, wall_segments):
    start = (player["x"], player["z"])
    end = (victim["x"], victim["z"])
    return any(segments_intersect(start, end, a, b) for a, b in wall_segments)


def stage_rescuebot_disabled(anns):
    for ent in anns:
        raw = ent.get("raw", {})
        scripts = " ".join(str(v).lower() for k, v in raw.items() if k.startswith("S_Script"))
        if "setrescuebot 0" in scripts:
            return True
    return False


def gather_stage_victim_orders():
    rows = []
    for d in sorted(EXTRACTED.iterdir()):
        if not d.is_dir():
            continue
        p = d / "annotated_entities.json"
        if not p.exists():
            continue
        anns = json.loads(p.read_text())
        victims = [a for a in anns if a.get("category") == "VICTIM"]
        if not victims:
            continue
        victims = sorted(victims, key=lambda x: x.get("entity_index", 0))
        for i, v in enumerate(victims, 1):
            rows.append({
                "stage": d.name,
                "order": i,
                "entity_index": v.get("entity_index"),
                "label": v.get("label"),
                "name": v.get("name"),
                "x": round(float(v.get("x", 0.0)), 1),
                "z": round(float(v.get("z", 0.0)), 1),
            })
    return rows


def stage_groups(rows):
    out = {}
    for r in rows:
        out.setdefault(r["stage"], []).append(r)
    for k in out:
        out[k] = sorted(out[k], key=lambda x: x["order"])
    return out


def write_stage_order_files(rows):
    out_json = EXTRACTED / "rescuebot_stage_victim_order.json"
    out_csv = EXTRACTED / "rescuebot_stage_victim_order.csv"
    out_json.write_text(json.dumps(rows, indent=2), encoding="utf-8")

    import csv

    with out_csv.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["stage", "order", "entity_index", "label", "name", "x", "z"])
        w.writeheader()
        for r in rows:
            w.writerow(r)


def draw_stage_explainer(stage: str):
    anns = json.loads((EXTRACTED / stage / "annotated_entities.json").read_text())
    victims = sorted([a for a in anns if a.get("category") == "VICTIM"], key=lambda x: x.get("entity_index", 0))
    if not victims:
        return
    start = next(a for a in anns if a.get("category") == "START")
    rescuebot_disabled = stage_rescuebot_disabled(anns)

    entity_ys = [float(a.get("y", 0.0)) for a in victims + [start]]
    floors, walls = load_map_geom(DATA_MAPS / f"{stage}.bsp", entity_ys)

    xs, zs = [], []
    for f in floors:
        for x, _y, z in f["points"]:
            xs.append(x)
            zs.append(z)
    for a, b in walls:
        xs += [a[0], b[0]]
        zs += [a[1], b[1]]
    for a in victims + [start]:
        xs.append(a["x"])
        zs.append(a["z"])

    minx, maxx = min(xs), max(xs)
    minz, maxz = min(zs), max(zs)
    pad = 90
    panel = 700
    scale = (2200 - 2 * pad) / max(maxx - minx, maxz - minz)
    W = int((maxx - minx) * scale + 2 * pad + panel)
    H = int((maxz - minz) * scale + 2 * pad)

    def tx(x):
        return pad + (x - minx) * scale

    def ty(z):
        # Match the displayed orientation of the ReportLab/PDF topdown maps.
        # PIL has Y increasing downward, while the PDF canvas has Y increasing upward,
        # so using z-minz here reproduces the PDF view on screen.
        return pad + (z - minz) * scale

    def draw_base(draw):
        draw.rectangle((0, 0, W, H), fill=(245, 246, 240))
        for f in sorted(floors, key=lambda item: item["y"]):
            pts = [(tx(x), ty(z)) for x, _y, z in f["points"]]
            draw.polygon(pts, fill=(223, 230, 219), outline=(192, 196, 186))
        for a, b in walls:
            draw.line((tx(a[0]), ty(a[1]), tx(b[0]), ty(b[1])), fill=(36, 42, 44), width=3)
        draw.rectangle((W - panel, 0, W, H), fill=(255, 255, 255), outline=(170, 170, 170), width=2)

    # Diagram 1: order map with plain-language legend
    img1 = Image.new("RGB", (W, H), (255, 255, 255))
    d1 = ImageDraw.Draw(img1)
    draw_base(d1)

    # player start
    sx, sz = tx(start["x"]), ty(start["z"])
    d1.polygon([(sx, sz - 16), (sx + 16, sz), (sx, sz + 16), (sx - 16, sz)], fill=(32, 110, 220), outline=(8, 56, 128), width=2)
    d1.text((sx + 18, sz - 26), "START", font=font(20, True), fill=(8, 56, 128))

    for i, v in enumerate(victims, 1):
        x, y = tx(v["x"]), ty(v["z"])
        marker_fill = (125, 125, 125) if rescuebot_disabled else (42, 162, 75)
        marker_outline = (75, 75, 75) if rescuebot_disabled else (10, 74, 25)
        d1.ellipse((x - 14, y - 14, x + 14, y + 14), fill=marker_fill, outline=marker_outline, width=2)
        d1.text((x, y), str(i), anchor="mm", font=font(18, True), fill=(255, 255, 255))
        d1.line((x + 14, y - 14, x + 48, y - 36), fill=(70, 70, 70), width=2)
        d1.text((x + 52, y - 38), f"V{i}", anchor="ls", font=font(18, True), fill=(20, 20, 20))

    px = W - panel + 24
    py = 20
    d1.text((px, py), f"{stage}: RescueBot Candidate Order", font=font(30, True), fill=(20, 20, 20)); py += 42
    d1.text((px, py), "Static BSP view: these are victim candidates, not proven live states.", font=font(16), fill=(70, 70, 70)); py += 24
    d1.text((px, py), "Runtime eligibility still needs victim+0x120 state, flag, LOS, and distance.", font=font(16), fill=(70, 70, 70)); py += 26
    if rescuebot_disabled:
        d1.text((px, py), "Stage script disables RescueBot: setrescuebot 0", font=font(16, True), fill=(160, 40, 35)); py += 28
    d1.text((px, py), "Order source: RCS_VictimRescue entity_index order", font=font(16), fill=(80, 80, 80)); py += 30
    headers = ["#", "Label", "Name", "entity_index", "X", "Z"]
    d1.text((px, py), "  ".join(headers), font=font(16, True), fill=(30, 30, 30)); py += 24
    for i, v in enumerate(victims, 1):
        row = f"{i:>1}  {v['label']:<4}  {v['name']:<12}  {v['entity_index']:>4}  {int(v['x']):>5}  {int(v['z']):>5}"
        d1.text((px, py), row, font=font(15), fill=(35, 35, 35)); py += 22

    py += 8
    d1.text((px, py), "Legend", font=font(18, True), fill=(30, 30, 30)); py += 24
    d1.polygon([(px + 10, py + 4), (px + 20, py + 14), (px + 10, py + 24), (px, py + 14)], fill=(32, 110, 220), outline=(8, 56, 128))
    d1.text((px + 30, py + 4), "Player spawn/start", font=font(16), fill=(40, 40, 40)); py += 28
    legend_fill = (125, 125, 125) if rescuebot_disabled else (42, 162, 75)
    legend_outline = (75, 75, 75) if rescuebot_disabled else (10, 74, 25)
    legend_text = "RescueBot disabled in this stage" if rescuebot_disabled else "Static victim candidate (runtime state unknown)"
    d1.ellipse((px, py, px + 20, py + 20), fill=legend_fill, outline=legend_outline, width=2)
    d1.text((px + 30, py + 2), legend_text, font=font(16), fill=(40, 40, 40)); py += 32

    notes = [
        "Top-down projection uses engine X/Z.",
        "Higher horizontal surfaces are drawn over lower ones.",
        "Runtime scan may differ if containers reorder entities.",
    ]
    for n in notes:
        d1.text((px, py), n, font=font(16), fill=(55, 55, 55)); py += 24

    out1 = EXTRACTED / stage / "rescuebot_explainer_order.png"
    img1.save(out1, quality=95)

    # Diagram 2: trigger checks from concrete player point
    img2 = Image.new("RGB", (W, H), (255, 255, 255))
    d2 = ImageDraw.Draw(img2)
    draw_base(d2)

    # concrete player location: near center of stage bounds for readability
    player = {"x": (minx + maxx) * 0.5, "z": (minz + maxz) * 0.5}
    pxp, pyp = tx(player["x"]), ty(player["z"])
    d2.ellipse((pxp - 10, pyp - 10, pxp + 10, pyp + 10), fill=(30, 80, 200), outline=(8, 40, 110), width=2)
    d2.text((pxp + 16, pyp - 16), "Player example position", font=font(18, True), fill=(8, 40, 110))

    # 600-unit radius
    r = 600.0 * scale
    d2.ellipse((pxp - r, pyp - r, pxp + r, pyp + r), outline=(35, 95, 210), width=3)
    d2.text((pxp + r + 10, pyp), "600u distance gate", font=font(16, True), fill=(35, 95, 210))

    rows = []
    for i, v in enumerate(victims, 1):
        x, y = tx(v["x"]), ty(v["z"])
        dist = math.dist([player["x"], player["z"]], [float(v["x"]), float(v["z"])])
        pass_dist = dist >= 600.0
        col = (125, 125, 125) if rescuebot_disabled else ((42, 162, 75) if pass_dist else (210, 70, 60))
        d2.ellipse((x - 14, y - 14, x + 14, y + 14), fill=col, outline=(20, 20, 20), width=2)
        d2.text((x, y), str(i), anchor="mm", font=font(18, True), fill=(255, 255, 255))
        rows.append((i, v, dist, pass_dist))

    px = W - panel + 24
    py = 20
    d2.text((px, py), f"{stage}: Static Check Inputs", font=font(30, True), fill=(20, 20, 20)); py += 42
    d2.text((px, py), "This plot can show static distance only.", font=font(17), fill=(70, 70, 70)); py += 24
    d2.text((px, py), "Runtime state and flag checks are not in the BSP data.", font=font(17), fill=(70, 70, 70)); py += 26
    d2.text((px, py), f"Player example at X={int(player['x'])}, Z={int(player['z'])}", font=font(16), fill=(70, 70, 70)); py += 28
    d2.text((px, py), "Per-victim distance gate from FUN_00455230:", font=font(16, True), fill=(20, 20, 20)); py += 24
    d2.text((px, py), "requires distance >= 600.0 game units", font=font(16), fill=(40, 40, 40)); py += 30

    d2.text((px, py), "#   victim    distance   static distance", font=font(16, True), fill=(20, 20, 20)); py += 24
    for i, v, dist, ok in rows:
        status = "DISABLED" if rescuebot_disabled else ("PASS" if ok else "FAIL")
        color = (90, 90, 90) if rescuebot_disabled else ((25, 120, 45) if ok else (170, 35, 30))
        line = f"{i:<2}  {v['name']:<8}  {dist:>7.1f}u    {status}"
        d2.text((px, py), line, font=font(15), fill=color); py += 22

    py += 8
    checks = [
        "Runtime-only checks before rescue can fire:",
        "1) victim+0x120 in {4,7}: runtime victim-state gate",
        "2) victim[0x1fd] == 0: disqualifying flag must be clear",
        "3) LOS gate: direct trace must be blocked (not visible)",
        "4) RescueBot idle and cooldown expired",
        "5) mission-state gate allows update (training is suppressed)",
        "Static map data alone cannot mark final eligibility.",
    ]
    for c in checks:
        d2.text((px, py), c, font=font(14), fill=(45, 45, 45)); py += 20

    out2 = EXTRACTED / stage / "rescuebot_explainer_checks.png"
    img2.save(out2, quality=95)

    # Diagram 3: simplified LOS/blockage visualization.
    img3 = Image.new("RGB", (W, H), (255, 255, 255))
    d3 = ImageDraw.Draw(img3)
    draw_base(d3)

    d3.ellipse((pxp - 12, pyp - 12, pxp + 12, pyp + 12), fill=(30, 80, 200), outline=(8, 40, 110), width=2)
    d3.text((pxp + 16, pyp - 16), "Player LOS sample point", font=font(18, True), fill=(8, 40, 110))
    los_rows = []
    for i, v in enumerate(victims, 1):
        x, y = tx(v["x"]), ty(v["z"])
        dist = math.dist([player["x"], player["z"]], [float(v["x"]), float(v["z"])])
        blocked = wall_blocks_trace(player, v, walls)
        distance_ok = dist >= 600.0
        if rescuebot_disabled:
            line_col = (125, 125, 125)
            status = "stage disabled"
        elif blocked and distance_ok:
            line_col = (218, 115, 30)
            status = "blocked + far"
        elif blocked:
            line_col = (170, 120, 35)
            status = "blocked, too close"
        elif distance_ok:
            line_col = (55, 130, 200)
            status = "clear + far"
        else:
            line_col = (155, 155, 155)
            status = "clear, too close"
        d3.line((pxp, pyp, x, y), fill=line_col, width=3)
        d3.ellipse((x - 14, y - 14, x + 14, y + 14), fill=line_col, outline=(20, 20, 20), width=2)
        d3.text((x, y), str(i), anchor="mm", font=font(18, True), fill=(255, 255, 255))
        los_rows.append((i, v, dist, status))

    px = W - panel + 24
    py = 20
    d3.text((px, py), f"{stage}: Approximate LOS / Blocked Trace", font=font(27, True), fill=(20, 20, 20)); py += 42
    d3.text((px, py), "Code polarity: a clear trace fails RescueBot.", font=font(17), fill=(70, 70, 70)); py += 24
    d3.text((px, py), "RescueBot continues when the trace is blocked.", font=font(17), fill=(70, 70, 70)); py += 28
    d3.text((px, py), "This plot approximates blockage using rendered walls.", font=font(15), fill=(85, 85, 85)); py += 22
    d3.text((px, py), "Actual game check uses Genesis3D collision.", font=font(15), fill=(85, 85, 85)); py += 30

    legend = [
        ((218, 115, 30), "blocked + >=600u: static gates pass"),
        ((170, 120, 35), "blocked but <600u: distance fails"),
        ((55, 130, 200), "clear + >=600u: LOS gate fails"),
        ((155, 155, 155), "clear + <600u: both fail"),
    ]
    for col, text in legend:
        d3.line((px, py + 9, px + 30, py + 9), fill=col, width=5)
        d3.text((px + 40, py), text, font=font(15), fill=(45, 45, 45))
        py += 24
    py += 8
    d3.text((px, py), "#   victim       distance   approximate trace", font=font(16, True), fill=(20, 20, 20)); py += 24
    for i, v, dist, status in los_rows:
        line = f"{i:<2}  {v['name']:<12} {dist:>7.1f}u   {status}"
        d3.text((px, py), line, font=font(14), fill=(45, 45, 45))
        py += 20

    out3 = EXTRACTED / stage / "rescuebot_explainer_los.png"
    img3.save(out3, quality=95)


def main():
    rows = gather_stage_victim_orders()
    write_stage_order_files(rows)
    for stage in sorted(stage_groups(rows)):
        draw_stage_explainer(stage)
    print("Wrote rescuebot diagrams (all stages) and stage victim order files.")


if __name__ == "__main__":
    main()
