#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import importlib.util
import json
import math
import sys
from pathlib import Path

from reportlab.lib import colors
from reportlab.lib.pagesizes import landscape, letter
from reportlab.pdfgen import canvas
from reportlab.pdfbase.pdfmetrics import stringWidth

ROOT = Path(__file__).resolve().parents[1]
for name, rel in [("extract_gbsp", "tools/extract_gbsp.py"), ("render_annotated_maps", "tools/render_annotated_maps.py")]:
    spec = importlib.util.spec_from_file_location(name, ROOT / rel)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    assert spec.loader
    spec.loader.exec_module(mod)

eg = sys.modules["extract_gbsp"]
ra = sys.modules["render_annotated_maps"]

STYLE = {
    "START": (colors.HexColor("#1f6feb"), colors.HexColor("#08306b")),
    "VICTIM": (colors.HexColor("#008246"), colors.HexColor("#00441b")),
    "ALT": (colors.HexColor("#55b978"), colors.HexColor("#006d2c")),
    "FIRE": (colors.HexColor("#dc281e"), colors.HexColor("#7f0000")),
    "HOSE": (colors.HexColor("#0070be"), colors.HexColor("#045a8d")),
    "VALVE": (colors.HexColor("#00a0b4"), colors.HexColor("#005824")),
    "EXIT": (colors.HexColor("#e6a000"), colors.HexColor("#8c510a")),
    "DOOR": (colors.HexColor("#785014"), colors.HexColor("#3f2704")),
    "TRIGGER": (colors.HexColor("#ffd92f"), colors.HexColor("#8c6d31")),
    "PROP": (colors.HexColor("#9ecae1"), colors.HexColor("#08519c")),
}
CLEAR_CATEGORIES = {"START", "VICTIM", "ALT", "FIRE", "HOSE", "VALVE", "EXIT", "DOOR"}
FULL_CATEGORIES = set(STYLE)


def load_bsp(path: Path):
    _, _, payloads = eg.read_chunks(path)
    models = eg.parse_models(payloads.get(1, b""))
    faces = eg.parse_faces(payloads.get(11, b""))
    verts = eg.vecs(payloads.get(14, b""))
    vert_indices = []
    if 13 in payloads:
        import struct
        vert_indices = list(struct.unpack("<" + "i" * (len(payloads[13]) // 4), payloads[13]))
    textures = eg.parse_textures(payloads.get(18, b""))
    texinfo = eg.parse_texinfo(payloads.get(17, b""))
    planes = eg.parse_planes(payloads.get(10, b""))
    _, entities = eg.parse_entities(payloads.get(16, b""))
    floors, walls = ra.classify_geometry(faces, verts, vert_indices, planes, texinfo, textures)
    anns = ra.build_annotations(entities)
    model_by_name = {}
    for ent in entities:
        if ent.get("classname") != "%Model%":
            continue
        name = ent.get("%name%")
        try:
            model_idx = int(ent.get("Model", "-1"))
        except ValueError:
            continue
        if name and 0 <= model_idx < len(models):
            model_by_name[name] = models[model_idx]
    for ann in anns:
        model_name = ann.get("raw", {}).get("C_Model")
        model = model_by_name.get(model_name or "")
        if model:
            ann["model_bounds"] = {
                "mins": model["mins"],
                "maxs": model["maxs"],
                "origin": model["origin"],
                "model_name": model_name,
            }
    return floors, walls, anns


def reduce_walls(walls):
    segments = []
    seen = set()
    for wall in walls:
        pts = wall["points"]
        xz = [(p[0], p[2]) for p in pts]
        best = None
        for i, p1 in enumerate(xz):
            for p2 in xz[i + 1:]:
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
        segments.append((qa, qb))
    return segments


def bounds(floors, wall_segments, anns):
    xs, zs = [], []
    for p in floors:
        if p["area"] < 220:
            continue
        for x, _y, z in p["points"]:
            xs.append(x); zs.append(z)
    for a, b in wall_segments:
        xs += [a[0], b[0]]; zs += [a[1], b[1]]
    for a in anns:
        xs.append(a["x"]); zs.append(a["z"])
    return min(xs), max(xs), min(zs), max(zs)


def draw_centered_text(c, x, y, text, size=7, color=colors.black):
    c.setFillColor(color)
    c.setFont("Helvetica-Bold", size)
    c.drawCentredString(x, y - size / 3, text)


def truncate(text, width, font="Helvetica", size=6):
    text = str(text or "")
    if stringWidth(text, font, size) <= width:
        return text
    while text and stringWidth(text + "...", font, size) > width:
        text = text[:-1]
    return text + "..."


def marker_shape(c, cat, x, y, r, fill, stroke):
    c.setFillColor(fill)
    c.setStrokeColor(stroke)
    c.setLineWidth(0.9)
    if cat == "START":
        path = c.beginPath()
        path.moveTo(x, y + r)
        path.lineTo(x + r, y)
        path.lineTo(x, y - r)
        path.lineTo(x - r, y)
        path.close()
        c.drawPath(path, fill=1, stroke=1)
    elif cat == "EXIT":
        path = c.beginPath()
        path.moveTo(x, y + r)
        path.lineTo(x + r, y - r)
        path.lineTo(x - r, y - r)
        path.close()
        c.drawPath(path, fill=1, stroke=1)
    elif cat in {"DOOR", "TRIGGER", "PROP", "HOSE", "VALVE"}:
        c.rect(x - r, y - r, r * 2, r * 2, fill=1, stroke=1)
    else:
        c.circle(x, y, r, fill=1, stroke=1)


def place_pdf_labels(anns, tx, ty, cell=22):
    placed = []
    occupied = set()
    priority = {"START": 0, "VICTIM": 1, "ALT": 2, "EXIT": 3, "HOSE": 4, "VALVE": 5, "FIRE": 6, "DOOR": 7, "TRIGGER": 8, "PROP": 9}
    offsets = [(14, 14), (14, -18), (-62, 14), (-62, -18), (26, 34), (26, -38), (-82, 34), (-82, -38), (0, 54), (0, -58)]
    for a in sorted(anns, key=lambda x: (priority.get(x["category"], 99), x["z"], x["x"])):
        px, py = tx(a["x"]), ty(a["z"])
        lx, ly = px + 14, py + 14
        for ox, oy in offsets:
            cx, cy = int((px + ox) / cell), int((py + oy) / cell)
            if (cx, cy) not in occupied:
                lx, ly = px + ox, py + oy
                occupied.add((cx, cy))
                break
        b = dict(a)
        b.update({"px": px, "py": py, "lx": lx, "ly": ly})
        placed.append(b)
    return placed


def draw_map(c, title, floors, walls, anns, x0, y0, w, h, mode="clear"):
    categories = FULL_CATEGORIES if mode == "full" else CLEAR_CATEGORIES
    clear = [a for a in anns if a["category"] in categories]
    wall_segments = reduce_walls(walls)
    minx, maxx, minz, maxz = bounds(floors, wall_segments, clear)
    legend_w = 260 if mode == "full" else 235
    map_area_w = w - legend_w - 14
    pad = 28
    scale = min((map_area_w - pad * 2) / max(maxx - minx, 1), (h - pad * 2) / max(maxz - minz, 1))
    map_w = (maxx - minx) * scale
    map_h = (maxz - minz) * scale
    ox = x0 + (map_area_w - map_w) / 2
    oy = y0 + (h - map_h) / 2
    def tx(x): return ox + (x - minx) * scale
    # PDF sheets use a conventional top-down map view: larger engine Z values
    # appear toward the top of the page instead of reading bottom-up.
    def ty(z): return oy + (maxz - z) * scale

    c.setFillColor(colors.HexColor("#f8fafc"))
    c.rect(x0, y0, w, h, fill=1, stroke=0)
    c.setStrokeColor(colors.HexColor("#d1d5db"))
    c.setLineWidth(0.35)
    gx = math.floor(minx / 500) * 500
    while gx <= maxx:
        c.line(tx(gx), ty(minz), tx(gx), ty(maxz)); gx += 500
    gz = math.floor(minz / 500) * 500
    while gz <= maxz:
        c.line(tx(minx), ty(gz), tx(maxx), ty(gz)); gz += 500

    c.setFillColor(colors.Color(0.88, 0.90, 0.87, alpha=0.34))
    c.setStrokeColor(colors.Color(0.65, 0.67, 0.64, alpha=0.24))
    c.setLineWidth(0.2)
    for p in floors:
        if p["area"] < 300:
            continue
        pts = [(tx(x), ty(z)) for x, _y, z in p["points"]]
        if len(pts) < 3:
            continue
        path = c.beginPath()
        path.moveTo(*pts[0])
        for pt in pts[1:]:
            path.lineTo(*pt)
        path.close()
        c.drawPath(path, fill=1, stroke=1)

    c.setStrokeColor(colors.HexColor("#111827"))
    c.setLineWidth(1.4)
    for a, b in wall_segments:
        c.line(tx(a[0]), ty(a[1]), tx(b[0]), ty(b[1]))

    if mode == "full":
        placed = place_pdf_labels(clear, tx, ty)
        for a in placed:
            if a["category"] != "TRIGGER" or "model_bounds" not in a:
                continue
            mins = a["model_bounds"]["mins"]
            maxs = a["model_bounds"]["maxs"]
            x1, x2 = sorted([tx(mins[0]), tx(maxs[0])])
            y1, y2 = sorted([ty(mins[2]), ty(maxs[2])])
            c.setFillColor(colors.Color(1.0, 0.85, 0.18, alpha=0.10))
            c.setStrokeColor(colors.Color(0.55, 0.42, 0.10, alpha=0.62))
            c.setLineWidth(0.9)
            c.setDash(5, 3)
            c.rect(x1, y1, x2 - x1, y2 - y1, fill=1, stroke=1)
            c.setDash()
        for a in placed:
            cat = a["category"]
            fill, stroke = STYLE[cat]
            marker_shape(c, cat, a["px"], a["py"], 5.4, fill, stroke)
            draw_centered_text(c, a["px"], a["py"] + 0.3, a["label"], 5.2, colors.black)
    else:
        # Draw fire and doors first, then larger readable route-planning labels.
        for a in clear:
            cat = a["category"]
            fill, stroke = STYLE[cat]
            x, y = tx(a["x"]), ty(a["z"])
            if cat == "FIRE":
                marker_shape(c, cat, x, y, 2.4, fill, stroke)
            elif cat == "DOOR":
                marker_shape(c, cat, x, y, 3.0, fill, stroke)

        for a in clear:
            cat = a["category"]
            if cat in {"FIRE", "DOOR"}:
                continue
            fill, stroke = STYLE[cat]
            x, y = tx(a["x"]), ty(a["z"])
            marker_shape(c, cat, x, y, 5.0, fill, stroke)
            draw_centered_text(c, x, y + 9, a["label"], 6.5, fill)

    c.setFillColor(colors.HexColor("#111827"))
    c.setFont("Helvetica-Bold", 15)
    c.drawString(x0 + 12, y0 + h - 18, title)
    c.setFont("Helvetica", 7)
    source = "topdown_annotated.svg" if mode == "full" else "topdown_stage_clear.svg"
    c.drawString(x0 + 12, y0 + 8, f"Map uses same decoded entities as {source}; table below is annotated_entities.csv.")

    # Legend panel mirrors the topdown_annotated.svg category legend.
    lx = x0 + map_area_w + 14
    c.setFillColor(colors.white)
    c.setStrokeColor(colors.HexColor("#cbd5e1"))
    c.setLineWidth(0.8)
    c.rect(lx, y0, legend_w, h, fill=1, stroke=1)
    c.setFillColor(colors.HexColor("#111827"))
    c.setFont("Helvetica-Bold", 18)
    c.drawString(lx + 14, y0 + h - 28, "Legend")
    c.setFont("Helvetica", 9)
    counts = {}
    for a in clear:
        counts[a["category"]] = counts.get(a["category"], 0) + 1
    legend_rows = [
        ("START", "Player/start"),
        ("VICTIM", "Survivor base"),
        ("ALT", "RNG alt spot"),
        ("FIRE", "Fire dot"),
        ("HOSE", "Hose"),
        ("VALVE", "Valve"),
        ("EXIT", "Exit arrow"),
        ("DOOR", "Door/model"),
    ]
    if mode == "full":
        legend_rows += [("TRIGGER", "Trigger/bounds"), ("PROP", "Prop model")]
    yy = y0 + h - 58
    for cat, desc in legend_rows:
        fill, stroke = STYLE[cat]
        c.setFillColor(fill); c.setStrokeColor(stroke); c.setLineWidth(0.7)
        c.rect(lx + 16, yy - 8, 11, 11, fill=1, stroke=1)
        c.setFillColor(colors.HexColor("#111827")); c.setFont("Helvetica", 9)
        c.drawString(lx + 34, yy - 6, f"{cat}: {desc} ({counts.get(cat, 0)})")
        yy -= 18
    yy -= 12
    c.setFont("Helvetica-Bold", 10)
    c.drawString(lx + 14, yy, "Label meaning")
    yy -= 16
    c.setFont("Helvetica", 8)
    for line in ["S# start", "V# survivor base", "A# RNG alt spot", "E# exit", "H# hose", "W# valve", "F# fire", "D# door/model"]:
        c.drawString(lx + 16, yy, line)
        yy -= 13
    yy -= 10
    c.setFont("Helvetica-Bold", 10)
    c.drawString(lx + 14, yy, "Coordinate bounds")
    yy -= 16
    c.setFont("Helvetica", 8)
    for line in [f"X: {minx:.0f} to {maxx:.0f}", f"Z: {minz:.0f} to {maxz:.0f}", "Projection: X/Z top-down"]:
        c.drawString(lx + 16, yy, line)
        yy -= 13


def read_rows(csv_path: Path):
    with csv_path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def draw_table(c, rows, x0, y0, w, row_h=11):
    cols = [
        ("label", 38), ("category", 58), ("entity_index", 42), ("class", 82),
        ("name", 100), ("x", 46), ("y", 46), ("z", 46), ("victim_link", 88), ("scripts", None),
    ]
    fixed = sum(width for _name, width in cols if width is not None)
    widths = [(name, width if width is not None else max(120, w - fixed)) for name, width in cols]
    header_h = 15
    y = y0
    c.setFillColor(colors.HexColor("#111827"))
    c.setFont("Helvetica-Bold", 10)
    c.drawString(x0, y, "Entity Table: annotated_entities.csv")
    y -= 16
    c.setFont("Helvetica-Bold", 6.2)
    c.setFillColor(colors.HexColor("#e5e7eb"))
    c.rect(x0, y - header_h + 3, w, header_h, fill=1, stroke=0)
    c.setFillColor(colors.HexColor("#111827"))
    x = x0
    for name, width in widths:
        c.drawString(x + 2, y - 7, name)
        x += width
    y -= header_h
    c.setFont("Helvetica", 5.7)
    for i, row in enumerate(rows):
        if i % 2 == 0:
            c.setFillColor(colors.HexColor("#f3f4f6"))
            c.rect(x0, y - row_h + 2, w, row_h, fill=1, stroke=0)
        c.setFillColor(colors.HexColor("#111827"))
        x = x0
        for name, width in widths:
            c.drawString(x + 2, y - 7, truncate(row.get(name, ""), width - 4, size=5.7))
            x += width
        y -= row_h
    c.setStrokeColor(colors.HexColor("#9ca3af"))
    c.setLineWidth(0.3)
    c.rect(x0, y + row_h - 2, w, header_h + len(rows) * row_h, fill=0, stroke=1)
    return y


def render(name: str, bsp: Path, csv_path: Path, out_pdf: Path, mode: str = "clear"):
    floors, walls, anns = load_bsp(bsp)
    rows = read_rows(csv_path)
    if mode == "full":
        page_w = 1900
        map_h = 1700
        row_h = 12
    else:
        page_w = 1320
        map_h = 1100
        row_h = 12
    margin = 28
    table_h = 42 + 15 + len(rows) * row_h
    page_h = margin * 2 + map_h + 26 + table_h
    out_pdf.parent.mkdir(parents=True, exist_ok=True)
    c = canvas.Canvas(str(out_pdf), pagesize=(page_w, page_h))
    c.setTitle(f"{name} {mode} map with entity table")
    y_map = page_h - margin - map_h
    title_mode = "full annotated" if mode == "full" else "clear"
    draw_map(c, f"{name} - {title_mode} top-down map", floors, walls, anns, margin, y_map, page_w - margin * 2, map_h, mode=mode)
    draw_table(c, rows, margin, y_map - 32, page_w - margin * 2, row_h=row_h)
    c.save()
    print(json.dumps({"map": name, "mode": mode, "out": str(out_pdf), "rows": len(rows), "page_size": [page_w, page_h]}, indent=2))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("name")
    ap.add_argument("bsp", type=Path)
    ap.add_argument("csv", type=Path)
    ap.add_argument("out", type=Path)
    ap.add_argument("--mode", choices=["clear", "full"], default="clear")
    args = ap.parse_args()
    render(args.name, args.bsp, args.csv, args.out, mode=args.mode)

if __name__ == "__main__":
    main()
