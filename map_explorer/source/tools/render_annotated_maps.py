#!/usr/bin/env python3
"""Render clear annotated top-down SVGs from extracted Genesis3D GBSP maps."""
from __future__ import annotations

import argparse
import csv
import html
import importlib.util
import json
import math
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EXTRACTOR = ROOT / "tools" / "extract_gbsp.py"
spec = importlib.util.spec_from_file_location("extract_gbsp", EXTRACTOR)
eg = importlib.util.module_from_spec(spec)
sys.modules["extract_gbsp"] = eg
assert spec.loader
spec.loader.exec_module(eg)

IMPORTANT_CLASSES = {
    "RCS_PlayerRescue",
    "RCS_VictimRescue",
    "RCS_VictimAltSpot",
    "RCS_FireRescue",
    "RCS_PropModel",
    "RCS_EscapeArrow",
    "RCS_BoundsTrigger",
    "RCS_BModel",
}

CATEGORY_STYLE = {
    "START": {"fill": "#1f6feb", "stroke": "#08306b", "shape": "diamond"},
    "VICTIM": {"fill": "#2ca25f", "stroke": "#00441b", "shape": "circle"},
    "ALT": {"fill": "#99d8c9", "stroke": "#006d2c", "shape": "circle"},
    "FIRE": {"fill": "#e34a33", "stroke": "#7f0000", "shape": "circle"},
    "HOSE": {"fill": "#2b8cbe", "stroke": "#045a8d", "shape": "square"},
    "VALVE": {"fill": "#41b6c4", "stroke": "#005824", "shape": "square"},
    "EXIT": {"fill": "#fed976", "stroke": "#8c510a", "shape": "triangle"},
    "DOOR": {"fill": "#8c6d31", "stroke": "#3f2704", "shape": "rect"},
    "TRIGGER": {"fill": "#ffd92f", "stroke": "#8c6d31", "shape": "square"},
    "PROP": {"fill": "#9ecae1", "stroke": "#08519c", "shape": "square"},
}


def parse_vec(text):
    if not text or text == "<null>":
        return None
    parts = str(text).replace(",", " ").split()
    if len(parts) < 3:
        return None
    try:
        return tuple(float(p) for p in parts[:3])
    except ValueError:
        return None


def get_location(ent):
    return parse_vec(ent.get("ZZ_location")) or parse_vec(ent.get("location")) or parse_vec(ent.get("origin"))


def clean_name(value):
    if not value or value == "<null>":
        return ""
    return str(value)


def category_for(ent):
    cls = ent.get("classname", "")
    name = clean_name(ent.get("A_Name") or ent.get("%name%"))
    lname = name.lower()
    scripts = " ".join(v for k, v in ent.items() if k.startswith("S_Script"))
    if cls == "RCS_PlayerRescue":
        return "START"
    if cls == "RCS_VictimRescue":
        return "VICTIM"
    if cls == "RCS_VictimAltSpot":
        return "ALT"
    if cls == "RCS_FireRescue":
        return "FIRE"
    if cls == "RCS_EscapeArrow":
        return "EXIT"
    if cls == "RCS_BoundsTrigger":
        return "TRIGGER"
    if cls == "RCS_BModel" and ("door" in lname or "door" in scripts.lower()):
        return "DOOR"
    if cls == "RCS_PropModel":
        if "hose" in lname or "touchhose" in scripts.lower():
            return "HOSE"
        if "valve" in lname or "makevalve" in scripts.lower():
            return "VALVE"
        if "bonus" in scripts.lower():
            return None
        return "PROP"
    return None


def natural_id(ent, fallback):
    for key in ("A_Name", "%name%", "A_Victim", "C_Model", "C_AnimModel0"):
        value = clean_name(ent.get(key))
        if value:
            return value
    return str(fallback)


def load_bsp(path):
    _, chunks, payloads = eg.read_chunks(path)
    models = eg.parse_models(payloads.get(1, b""))
    faces = eg.parse_faces(payloads.get(11, b""))
    verts = eg.vecs(payloads.get(14, b""))
    vert_indices = list(__import__("struct").unpack("<" + "i" * (len(payloads.get(13, b"")) // 4), payloads.get(13, b""))) if 13 in payloads else []
    textures = eg.parse_textures(payloads.get(18, b""))
    texinfo = eg.parse_texinfo(payloads.get(17, b""))
    planes = eg.parse_planes(payloads.get(10, b""))
    _, entities = eg.parse_entities(payloads.get(16, b""))
    return chunks, models, faces, verts, vert_indices, textures, texinfo, planes, entities


def polygon_area_xz(points):
    area = 0.0
    for i, (x1, _y1, z1) in enumerate(points):
        x2, _y2, z2 = points[(i + 1) % len(points)]
        area += x1 * z2 - x2 * z1
    return abs(area) / 2.0


def face_texture(face, texinfo, textures):
    if not (0 <= face["texinfo"] < len(texinfo)):
        return ""
    ti = texinfo[face["texinfo"]]
    tid = ti.get("texture", -1)
    if 0 <= tid < len(textures):
        return textures[tid]["name"].lower()
    return ""


def classify_geometry(faces, verts, vert_indices, planes, texinfo, textures):
    floors = []
    walls = []
    for face in faces:
        pts = eg.face_vertices(face, vert_indices, verts)
        if len(pts) < 3:
            continue
        tex = face_texture(face, texinfo, textures)
        if tex.startswith("sky_"):
            continue
        plane = planes[face["plane_num"]] if 0 <= face["plane_num"] < len(planes) else None
        n = plane["normal"] if plane else [0.0, 0.0, 0.0]
        area = polygon_area_xz(pts)
        if abs(n[1]) > 0.62 and area > 24:
            y = sum(p[1] for p in pts) / len(pts)
            floors.append({"points": pts, "y": y, "tex": tex, "area": area})
        elif abs(n[1]) < 0.28:
            xs = [p[0] for p in pts]
            zs = [p[2] for p in pts]
            ys = [p[1] for p in pts]
            if max(ys) - min(ys) >= 32 and (max(xs) - min(xs) > 8 or max(zs) - min(zs) > 8):
                walls.append({"points": pts, "y": sum(ys) / len(ys), "tex": tex})
    return floors, walls


def build_annotations(entities):
    counters = {k: 0 for k in CATEGORY_STYLE}
    anns = []
    for idx, ent in enumerate(entities):
        cat = category_for(ent)
        if not cat:
            continue
        loc = get_location(ent)
        if not loc:
            continue
        counters[cat] += 1
        prefix = {
            "START": "S", "VICTIM": "V", "ALT": "A", "FIRE": "F", "HOSE": "H", "VALVE": "W",
            "EXIT": "E", "DOOR": "D", "TRIGGER": "T", "PROP": "P",
        }[cat]
        label = f"{prefix}{counters[cat]}"
        scripts = [v for k, v in sorted(ent.items()) if k.startswith("S_Script") and v and v != "<null>"]
        anns.append({
            "entity_index": idx,
            "label": label,
            "category": cat,
            "class": ent.get("classname", ""),
            "name": natural_id(ent, idx),
            "x": loc[0], "y": loc[1], "z": loc[2],
            "scripts": " | ".join(scripts),
            "victim_link": ent.get("A_Victim", ""),
            "raw": ent,
        })
    return anns


def bounds_from(floors, walls, anns):
    xs, zs = [], []
    for item in floors + walls:
        for x, _y, z in item["points"]:
            xs.append(x); zs.append(z)
    for a in anns:
        xs.append(a["x"]); zs.append(a["z"])
    return min(xs), max(xs), min(zs), max(zs)


def svg_poly(points, tx, ty):
    return " ".join(f"{tx(x):.1f},{ty(z):.1f}" for x, _y, z in points)


def svg_wall_path(points, tx, ty):
    # Project vertical polygons; use their horizontal bounding segment for visual clarity.
    xz = [(p[0], p[2]) for p in points]
    best = None
    for i, p1 in enumerate(xz):
        for p2 in xz[i + 1:]:
            d = (p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2
            if best is None or d > best[0]:
                best = (d, p1, p2)
    if not best or best[0] < 16:
        return ""
    _, p1, p2 = best
    return f'<line x1="{tx(p1[0]):.1f}" y1="{ty(p1[1]):.1f}" x2="{tx(p2[0]):.1f}" y2="{ty(p2[1]):.1f}" />'


def place_labels(anns, tx, ty):
    placed = []
    cell = 28
    occupied = set()
    priority = {"START": 0, "VICTIM": 1, "ALT": 2, "EXIT": 3, "HOSE": 4, "VALVE": 5, "FIRE": 6, "DOOR": 7, "TRIGGER": 8, "PROP": 9}
    for a in sorted(anns, key=lambda x: (priority.get(x["category"], 99), x["z"], x["x"])):
        px, py = tx(a["x"]), ty(a["z"])
        offsets = [(14, -14), (14, 18), (-54, -14), (-54, 18), (24, -34), (24, 34), (-72, -34), (-72, 34), (0, -54), (0, 54)]
        lx, ly = px + 12, py - 12
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


def write_tables(out_dir, anns):
    fields = ["label", "category", "entity_index", "class", "name", "x", "y", "z", "victim_link", "scripts"]
    with (out_dir / "annotated_entities.csv").open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for a in anns:
            w.writerow({k: a.get(k, "") for k in fields})
    important = [a for a in anns if a["category"] not in {"TRIGGER", "PROP"}]
    with (out_dir / "important_entities.csv").open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for a in important:
            w.writerow({k: a.get(k, "") for k in fields})
    (out_dir / "annotated_entities.json").write_text(json.dumps(anns, indent=2), encoding="utf-8")
    (out_dir / "important_entities.json").write_text(json.dumps(important, indent=2), encoding="utf-8")


def write_svg(out_path, title, floors, walls, anns):
    minx, maxx, minz, maxz = bounds_from(floors, walls, anns)
    map_w, map_h = maxx - minx, maxz - minz
    panel_w = 430
    pad = 70
    target_w = 2200
    scale = (target_w - panel_w - pad * 2) / max(map_w, 1)
    height = max(1200, int(map_h * scale + pad * 2))
    width = int(map_w * scale + pad * 2 + panel_w)
    def tx(x): return pad + (x - minx) * scale
    def ty(z): return pad + (maxz - z) * scale

    yvals = [f["y"] for f in floors] or [0]
    ymin, ymax = min(yvals), max(yvals)
    placed = place_labels(anns, tx, ty)
    counts = {}
    for a in anns:
        counts[a["category"]] = counts.get(a["category"], 0) + 1

    with out_path.open("w", encoding="utf-8") as f:
        f.write(f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">\n')
        f.write('<defs>\n')
        f.write('<style><![CDATA[text{font-family:Verdana,DejaVu Sans,sans-serif}.small{font-size:18px}.label{font-size:16px;font-weight:700}.panel{font-size:17px}.title{font-size:34px;font-weight:800}.subtitle{font-size:18px}.wall{stroke:#111827;stroke-width:3.2;stroke-linecap:round;opacity:.88}.grid{stroke:#d1d5db;stroke-width:1}.floor{stroke:#64748b;stroke-width:.7}.leader{stroke:#111827;stroke-width:1.2;opacity:.55}.labelbox{fill:#fff;stroke:#111827;stroke-width:1.2;rx:4;opacity:.92}]]></style>\n')
        f.write('</defs>\n')
        f.write('<rect width="100%" height="100%" fill="#f8fafc"/>\n')
        f.write(f'<text x="32" y="42" class="title">{html.escape(title)} annotated top-down map</text>\n')
        f.write(f'<text x="32" y="70" class="subtitle">Projection: engine X/Z, vertical axis Y. Grid: 500 engine units. Labels correspond to annotated_entities.csv.</text>\n')

        # Grid.
        gx0 = math.floor(minx / 500) * 500
        gx1 = math.ceil(maxx / 500) * 500
        gz0 = math.floor(minz / 500) * 500
        gz1 = math.ceil(maxz / 500) * 500
        x = gx0
        while x <= gx1:
            f.write(f'<line class="grid" x1="{tx(x):.1f}" y1="{ty(gz0):.1f}" x2="{tx(x):.1f}" y2="{ty(gz1):.1f}"/>\n')
            if int(x) % 1000 == 0:
                f.write(f'<text class="small" x="{tx(x)+3:.1f}" y="{ty(gz0)-6:.1f}" fill="#64748b">X {int(x)}</text>\n')
            x += 500
        z = gz0
        while z <= gz1:
            f.write(f'<line class="grid" x1="{tx(gx0):.1f}" y1="{ty(z):.1f}" x2="{tx(gx1):.1f}" y2="{ty(z):.1f}"/>\n')
            if int(z) % 1000 == 0:
                f.write(f'<text class="small" x="{tx(gx0)+3:.1f}" y="{ty(z)-3:.1f}" fill="#64748b">Z {int(z)}</text>\n')
            z += 500

        # Floors by elevation.
        for poly in sorted(floors, key=lambda p: p["area"], reverse=True):
            y = poly["y"]
            t = (y - ymin) / (ymax - ymin or 1)
            # Higher surfaces slightly warmer, lower surfaces slightly cooler.
            r = int(226 + 22 * t); g = int(232 + 12 * t); b = int(240 - 30 * t)
            opacity = 0.54 if poly["area"] > 120 else 0.34
            f.write(f'<polygon class="floor" points="{svg_poly(poly["points"], tx, ty)}" fill="rgb({r},{g},{b})" fill-opacity="{opacity:.2f}"/>\n')

        f.write('<g class="walls">\n')
        for wall in walls:
            line = svg_wall_path(wall["points"], tx, ty)
            if line:
                f.write(line.replace('/>', ' class="wall"/>') + '\n')
        f.write('</g>\n')

        # Entity markers.
        for a in placed:
            style = CATEGORY_STYLE[a["category"]]
            px, py = a["px"], a["py"]
            fill, stroke = style["fill"], style["stroke"]
            if style["shape"] == "circle":
                radius = 10 if a["category"] != "FIRE" else 8
                f.write(f'<circle cx="{px:.1f}" cy="{py:.1f}" r="{radius}" fill="{fill}" stroke="{stroke}" stroke-width="2"/>\n')
            elif style["shape"] == "diamond":
                f.write(f'<polygon points="{px:.1f},{py-13:.1f} {px+13:.1f},{py:.1f} {px:.1f},{py+13:.1f} {px-13:.1f},{py:.1f}" fill="{fill}" stroke="{stroke}" stroke-width="2"/>\n')
            elif style["shape"] == "triangle":
                f.write(f'<polygon points="{px:.1f},{py-13:.1f} {px+14:.1f},{py+12:.1f} {px-14:.1f},{py+12:.1f}" fill="{fill}" stroke="{stroke}" stroke-width="2"/>\n')
            else:
                f.write(f'<rect x="{px-8:.1f}" y="{py-8:.1f}" width="16" height="16" fill="{fill}" stroke="{stroke}" stroke-width="2"/>\n')
            f.write(f'<line class="leader" x1="{px:.1f}" y1="{py:.1f}" x2="{a["lx"]:.1f}" y2="{a["ly"]:.1f}"/>\n')
            text = html.escape(a["label"])
            bw = max(30, len(text) * 10 + 10)
            f.write(f'<rect class="labelbox" x="{a["lx"]-4:.1f}" y="{a["ly"]-18:.1f}" width="{bw}" height="23"/>\n')
            f.write(f'<text class="label" x="{a["lx"]:.1f}" y="{a["ly"]:.1f}" fill="#111827">{text}</text>\n')

        # Panel / legend.
        px = width - panel_w + 22
        f.write(f'<rect x="{width-panel_w:.1f}" y="0" width="{panel_w}" height="{height}" fill="#ffffff" stroke="#cbd5e1"/>\n')
        f.write(f'<text x="{px}" y="44" class="title" font-size="26">Legend</text>\n')
        y = 82
        legend_names = [("START", "Player/start"), ("VICTIM", "Survivor"), ("ALT", "Alt survivor spot"), ("FIRE", "Fire"), ("HOSE", "Hose"), ("VALVE", "Valve"), ("EXIT", "Exit arrow"), ("DOOR", "Door/model"), ("TRIGGER", "Trigger zone"), ("PROP", "Other prop")]
        for cat, desc in legend_names:
            st = CATEGORY_STYLE[cat]
            f.write(f'<rect x="{px}" y="{y-14}" width="18" height="18" fill="{st["fill"]}" stroke="{st["stroke"]}" stroke-width="2"/>\n')
            f.write(f'<text x="{px+30}" y="{y}" class="panel">{cat}: {html.escape(desc)} ({counts.get(cat,0)})</text>\n')
            y += 28
        y += 18
        f.write(f'<text x="{px}" y="{y}" class="panel" font-weight="700">Coordinate Bounds</text>\n')
        y += 26
        for line in [f'X: {minx:.0f} to {maxx:.0f}', f'Z: {minz:.0f} to {maxz:.0f}', f'Y floors: {ymin:.0f} to {ymax:.0f}']:
            f.write(f'<text x="{px}" y="{y}" class="panel">{html.escape(line)}</text>\n')
            y += 24
        y += 18
        f.write(f'<text x="{px}" y="{y}" class="panel" font-weight="700">Important Labels</text>\n')
        y += 26
        for a in placed:
            if a["category"] not in {"START", "VICTIM", "ALT", "HOSE", "VALVE", "EXIT"}:
                continue
            line = f'{a["label"]} {a["category"]}: {a["name"]}'
            if len(line) > 43:
                line = line[:40] + '...'
            f.write(f'<text x="{px}" y="{y}" class="panel">{html.escape(line)}</text>\n')
            y += 22
            if y > height - 28:
                break
        f.write('</svg>\n')


def render(map_name, bsp_path, out_dir):
    out_dir.mkdir(parents=True, exist_ok=True)
    chunks, models, faces, verts, vert_indices, textures, texinfo, planes, entities = load_bsp(bsp_path)
    floors, walls = classify_geometry(faces, verts, vert_indices, planes, texinfo, textures)
    anns = build_annotations(entities)
    write_tables(out_dir, anns)
    clear_anns = [a for a in anns if a["category"] not in {"TRIGGER", "PROP"}]
    write_svg(out_dir / "topdown_annotated.svg", map_name + " full", floors, walls, anns)
    write_svg(out_dir / "topdown_stage_clear.svg", map_name + " clear", floors, walls, clear_anns)
    print(json.dumps({"map": map_name, "annotations": len(anns), "clear_annotations": len(clear_anns), "floors": len(floors), "walls": len(walls), "out": str(out_dir / "topdown_stage_clear.svg")}, indent=2))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("map_name")
    ap.add_argument("bsp", type=Path)
    ap.add_argument("out_dir", type=Path)
    args = ap.parse_args()
    render(args.map_name, args.bsp, args.out_dir)

if __name__ == "__main__":
    main()
