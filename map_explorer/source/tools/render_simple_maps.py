#!/usr/bin/env python3
from __future__ import annotations

import argparse
import importlib.util
import json
import math
import sys
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont, ImageFilter

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("extract_gbsp", ROOT / "tools" / "extract_gbsp.py")
eg = importlib.util.module_from_spec(spec); sys.modules["extract_gbsp"] = eg; spec.loader.exec_module(eg)
spec2 = importlib.util.spec_from_file_location("render_annotated_maps", ROOT / "tools" / "render_annotated_maps.py")
ra = importlib.util.module_from_spec(spec2); sys.modules["render_annotated_maps"] = ra; spec2.loader.exec_module(ra)

LABEL_CATS = {"START", "VICTIM", "ALT", "HOSE", "VALVE", "EXIT"}
MARKER = {
    "START": (31, 111, 235),
    "VICTIM": (0, 130, 70),
    "ALT": (75, 180, 120),
    "FIRE": (220, 40, 30),
    "HOSE": (0, 110, 190),
    "VALVE": (0, 160, 180),
    "EXIT": (230, 160, 0),
    "DOOR": (120, 80, 20),
}


def font(size, bold=False):
    candidates = [
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Verdana Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Verdana.ttf",
    ]
    for c in candidates:
        try:
            return ImageFont.truetype(c, size)
        except Exception:
            pass
    return ImageFont.load_default()


def load(path):
    _, _, payloads = eg.read_chunks(path)
    return {
        "models": eg.parse_models(payloads.get(1, b"")),
        "faces": eg.parse_faces(payloads.get(11, b"")),
        "verts": eg.vecs(payloads.get(14, b"")),
        "idx": list(__import__("struct").unpack("<" + "i" * (len(payloads.get(13, b"")) // 4), payloads.get(13, b""))) if 13 in payloads else [],
        "textures": eg.parse_textures(payloads.get(18, b"")),
        "texinfo": eg.parse_texinfo(payloads.get(17, b"")),
        "planes": eg.parse_planes(payloads.get(10, b"")),
        "entities": eg.parse_entities(payloads.get(16, b""))[1],
    }


def classify(g):
    floors, walls = ra.classify_geometry(g["faces"], g["verts"], g["idx"], g["planes"], g["texinfo"], g["textures"])
    # Strip decorative micro-polygons aggressively for diagrams.
    floors = [p for p in floors if p["area"] >= 220]
    wall_segments = []
    seen = set()
    for wall in walls:
        pts = wall["points"]
        xz = [(p[0], p[2]) for p in pts]
        best = None
        for i, p1 in enumerate(xz):
            for p2 in xz[i + 1:]:
                d = (p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2
                if best is None or d > best[0]: best = (d, p1, p2)
        if not best or best[0] < 80 * 80:
            continue
        _, a, b = best
        # Quantize to collapse duplicate coplanar wall fragments.
        qa = (round(a[0] / 16) * 16, round(a[1] / 16) * 16)
        qb = (round(b[0] / 16) * 16, round(b[1] / 16) * 16)
        key = tuple(sorted([qa, qb]))
        if key in seen:
            continue
        seen.add(key)
        wall_segments.append((qa, qb))
    return floors, wall_segments


def bounds(floors, wall_segments, anns):
    xs=[]; zs=[]
    for p in floors:
        for x,_,z in p["points"]: xs.append(x); zs.append(z)
    for a,b in wall_segments:
        xs += [a[0], b[0]]; zs += [a[1], b[1]]
    for a in anns:
        xs.append(a["x"]); zs.append(a["z"])
    return min(xs), max(xs), min(zs), max(zs)


def draw_label(draw, xy, text, fill, anchor="lm", size=24):
    f = font(size, True)
    x,y = xy
    bbox = draw.textbbox((x,y), text, font=f, anchor=anchor)
    pad=5
    box = (bbox[0]-pad,bbox[1]-pad,bbox[2]+pad,bbox[3]+pad)
    draw.rounded_rectangle(box, radius=5, fill=(255,255,255,235), outline=(30,30,30,220), width=2)
    draw.text((x,y), text, font=f, fill=fill, anchor=anchor)


def render_one(name, bsp, out_dir, filename, include_cats, compact_labels=True):
    g=load(bsp)
    floors, wall_segments = classify(g)
    anns = ra.build_annotations(g["entities"])
    clear = [a for a in anns if a["category"] in include_cats]
    minx,maxx,minz,maxz=bounds(floors, wall_segments, clear)
    mapw, maph=maxx-minx, maxz-minz
    pad=110
    target=2500
    scale=(target-pad*2)/max(mapw,maph,1)
    W=int(mapw*scale+pad*2+430); H=int(maph*scale+pad*2)
    W=max(W,1200); H=max(H,900)
    panel_x=W-400
    def tx(x): return pad+(x-minx)*scale
    def ty(z): return pad+(maxz-z)*scale

    img=Image.new('RGBA',(W,H),(250,250,246,255))
    draw=ImageDraw.Draw(img,'RGBA')
    # grid 500 units, subtle
    gx=math.floor(minx/500)*500
    while gx<=maxx:
        col=(210,210,205,150) if gx%1000 else (185,185,180,180)
        draw.line((tx(gx),ty(minz),tx(gx),ty(maxz)),fill=col,width=1)
        gx+=500
    gz=math.floor(minz/500)*500
    while gz<=maxz:
        col=(210,210,205,150) if gz%1000 else (185,185,180,180)
        draw.line((tx(minx),ty(gz),tx(maxx),ty(gz)),fill=col,width=1)
        gz+=500

    floor_layer=Image.new('RGBA',(W,H),(0,0,0,0)); fd=ImageDraw.Draw(floor_layer,'RGBA')
    for p in floors:
        poly=[(tx(x),ty(z)) for x,_,z in p['points']]
        fd.polygon(poly, fill=(224,228,222,150), outline=(170,175,170,60))
    floor_layer=floor_layer.filter(ImageFilter.SMOOTH)
    img.alpha_composite(floor_layer)
    draw=ImageDraw.Draw(img,'RGBA')

    # walls
    for a,b in wall_segments:
        draw.line((tx(a[0]),ty(a[1]),tx(b[0]),ty(b[1])), fill=(20,24,28,210), width=4)

    # Doors under labels.
    for a in clear:
        if a['category']=='DOOR':
            x,y=tx(a['x']),ty(a['z'])
            draw.rectangle((x-7,y-7,x+7,y+7), fill=MARKER['DOOR']+(230,), outline=(60,35,0,255), width=2)

    # Fires small unlabeled to reduce clutter.
    for a in clear:
        if a['category']=='FIRE':
            x,y=tx(a['x']),ty(a['z'])
            draw.ellipse((x-5,y-5,x+5,y+5), fill=MARKER['FIRE']+(190,), outline=(120,0,0,230), width=1)

    # Major markers and labels.
    label_positions=[]
    for a in clear:
        cat=a['category']
        if cat in {'DOOR','FIRE'}:
            continue
        x,y=tx(a['x']),ty(a['z'])
        col=MARKER[cat]
        r=13 if cat in {'START','VICTIM'} else 10
        if cat=='START':
            draw.polygon([(x,y-r-4),(x+r+4,y),(x,y+r+4),(x-r-4,y)], fill=col+(255,), outline=(0,0,0,255))
        elif cat=='EXIT':
            draw.polygon([(x,y-r-3),(x+r+4,y+r+3),(x-r-4,y+r+3)], fill=col+(255,), outline=(0,0,0,255))
        else:
            draw.ellipse((x-r,y-r,x+r,y+r), fill=col+(245,), outline=(0,0,0,255), width=2)
        if cat in LABEL_CATS:
            # Keep map labels compact; the side panel and CSV carry full names.
            ox=16 if x < panel_x/2 else -16
            anchor='lm' if ox>0 else 'rm'
            label=a['label'] if compact_labels else f"{a['label']} {a['name']}"
            draw.line((x,y,x+ox,y-18), fill=(40,40,40,110), width=2)
            draw_label(draw,(x+ox,y-18),label,col,anchor,size=22 if compact_labels else 24)

    # Panel
    draw.rectangle((panel_x,0,W,H), fill=(255,255,255,245), outline=(160,160,160,255), width=2)
    title_f=font(34,True); f=font(22,False); fb=font(22,True)
    y=44
    draw.text((panel_x+22,y), f"{name}", font=title_f, fill=(20,24,28)); y+=42
    draw.text((panel_x+22,y), "Simplified stage diagram", font=f, fill=(70,70,70)); y+=44
    for cat,label in [('START','Start'),('VICTIM','Survivor base'),('ALT','RNG alt spot'),('HOSE','Hose'),('VALVE','Valve'),('EXIT','Exit'),('FIRE','Fire dot'),('DOOR','Door square')]:
        col=MARKER[cat]
        draw.rectangle((panel_x+24,y-14,panel_x+42,y+4), fill=col+(255,), outline=(0,0,0,255))
        count=sum(1 for a in clear if a['category']==cat)
        draw.text((panel_x+54,y-18), f"{label}: {count}", font=f, fill=(20,24,28)); y+=31
    y+=16
    draw.text((panel_x+22,y), "Notes", font=fb, fill=(20,24,28)); y+=30
    notes=["Walls are simplified BSP vertical faces.","Doors are unnumbered brown squares.","Fire labels are hidden to reduce clutter.","Survivor alt spots indicate RNG options.","See important_entities.csv for coordinates."]
    for n in notes:
        draw.text((panel_x+22,y), n, font=font(18), fill=(65,65,65)); y+=25

    out_dir.mkdir(parents=True,exist_ok=True)
    img=img.convert('RGB')
    img.save(out_dir/filename, quality=95)
    print(json.dumps({'map':name,'out':str(out_dir/filename),'size':[W,H],'floors':len(floors),'wall_segments':len(wall_segments),'markers':len(clear)},indent=2))


def render(name, bsp, out_dir):
    gameplay = {"START", "VICTIM", "ALT", "FIRE", "HOSE", "VALVE", "EXIT", "DOOR"}
    survivor = {"START", "VICTIM", "ALT", "EXIT", "DOOR"}
    render_one(name, bsp, out_dir, "stage_diagram_simple.png", gameplay, compact_labels=False)
    render_one(name, bsp, out_dir, "stage_diagram_gameplay_compact.png", gameplay, compact_labels=True)
    render_one(name, bsp, out_dir, "stage_diagram_survivors.png", survivor, compact_labels=True)

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('name'); ap.add_argument('bsp',type=Path); ap.add_argument('out_dir',type=Path)
    a=ap.parse_args(); render(a.name,a.bsp,a.out_dir)
if __name__=='__main__': main()
