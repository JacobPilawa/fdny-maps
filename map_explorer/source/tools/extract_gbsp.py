#!/usr/bin/env python3
"""Extract basic geometry/metadata from Genesis3D GBSP v15 BSP files."""
from __future__ import annotations

import argparse
import json
import math
import struct
from dataclasses import asdict, dataclass
from pathlib import Path

CHUNK_NAMES = {
    0: "HEADER",
    1: "MODELS",
    2: "NODES",
    3: "BNODES",
    4: "LEAFS",
    5: "CLUSTERS",
    6: "AREAS",
    7: "AREA_PORTALS",
    8: "LEAF_SIDES",
    9: "PORTALS",
    10: "PLANES",
    11: "FACES",
    12: "LEAF_FACES",
    13: "VERT_INDEX",
    14: "VERTS",
    15: "RGB_VERTS",
    16: "ENTDATA",
    17: "TEXINFO",
    18: "TEXTURES",
    19: "TEXDATA",
    20: "LIGHTDATA",
    21: "VISDATA",
    22: "SKYDATA",
    23: "PALETTES",
    24: "MOTIONS",
    0xFFFF: "END",
}

@dataclass
class Chunk:
    type: int
    name: str
    size: int
    elements: int
    offset: int
    byte_length: int


def read_chunks(path: Path):
    data = path.read_bytes()
    pos = 0
    chunks: list[Chunk] = []
    payloads: dict[int, bytes] = {}
    while pos + 12 <= len(data):
        ctype, size, elements = struct.unpack_from("<iii", data, pos)
        pos += 12
        byte_length = size * elements
        name = CHUNK_NAMES.get(ctype, f"UNKNOWN_{ctype}")
        chunks.append(Chunk(ctype, name, size, elements, pos, byte_length))
        if ctype == 0xFFFF:
            break
        if pos + byte_length > len(data):
            raise ValueError(f"Chunk {name} runs past EOF")
        payloads[ctype] = data[pos:pos + byte_length]
        pos += byte_length
    return data, chunks, payloads


def vecs(payload: bytes):
    return [struct.unpack_from("<fff", payload, i) for i in range(0, len(payload), 12)]


def parse_models(payload: bytes):
    # GFX_Model is 80 bytes on 32-bit builds. Final geMotion* is serialized as a 4-byte pointer/placeholder.
    out = []
    fmt = "<ii" + "fff" * 3 + "iiiiiiii" + "I"
    for i in range(0, len(payload), 80):
        vals = struct.unpack_from(fmt, payload, i)
        out.append({
            "root_node": [vals[0], vals[1]],
            "mins": vals[2:5],
            "maxs": vals[5:8],
            "origin": vals[8:11],
            "first_face": vals[11],
            "num_faces": vals[12],
            "first_leaf": vals[13],
            "num_leafs": vals[14],
            "first_cluster": vals[15],
            "num_clusters": vals[16],
            "areas": [vals[17], vals[18]],
            "motion_ptr_or_offset": vals[19],
        })
    return out


def parse_faces(payload: bytes):
    out = []
    for i in range(0, len(payload), 36):
        vals = struct.unpack_from("<iiiiiiii4B", payload, i)
        out.append({
            "first_vert": vals[0],
            "num_verts": vals[1],
            "plane_num": vals[2],
            "plane_side": vals[3],
            "texinfo": vals[4],
            "light_ofs": vals[5],
            "light_width": vals[6],
            "light_height": vals[7],
            "light_types": vals[8:12],
        })
    return out


def parse_textures(payload: bytes):
    out = []
    for i in range(0, len(payload), 52):
        raw, flags, width, height, offset, palette = struct.unpack_from("<32sIiiii", payload, i)
        name = raw.split(b"\0", 1)[0].decode("latin1", "replace")
        out.append({"name": name, "flags": flags, "width": width, "height": height, "offset": offset, "palette": palette})
    return out


def parse_texinfo(payload: bytes):
    out = []
    for i in range(0, len(payload), 64):
        vals = struct.unpack_from("<" + "f" * 15 + "i", payload, i)
        out.append({
            "u_vec": vals[0:3],
            "v_vec": vals[3:6],
            "shift": vals[6:8],
            "draw_scale": vals[8:10],
            "flags": int(vals[10]),
            "face_light": vals[11],
            "reflective_scale": vals[12],
            "alpha": vals[13],
            "mip_map_bias": vals[14],
            "texture": vals[15],
        })
    return out


def parse_planes(payload: bytes):
    out = []
    for i in range(0, len(payload), 20):
        nx, ny, nz, dist, ptype = struct.unpack_from("<ffffi", payload, i)
        out.append({"normal": [nx, ny, nz], "dist": dist, "type": ptype})
    return out


def parse_cstring(raw: bytes, pos: int):
    if pos + 4 > len(raw):
        raise ValueError("truncated entity string length")
    size = struct.unpack_from("<i", raw, pos)[0]
    pos += 4
    if size < 0 or pos + size > len(raw):
        raise ValueError(f"invalid entity string size {size} at {pos - 4}")
    value = raw[pos:pos + size].split(b"\0", 1)[0].decode("latin1", "replace")
    return value, pos + size


def parse_entities(raw: bytes):
    if len(raw) < 4:
        return "", []
    pos = 0
    num_entities = struct.unpack_from("<i", raw, pos)[0]
    pos += 4
    entities = []
    for _ in range(num_entities):
        if pos + 4 > len(raw):
            break
        num_epairs = struct.unpack_from("<i", raw, pos)[0]
        pos += 4
        ent = {}
        for _ in range(num_epairs):
            key, pos = parse_cstring(raw, pos)
            value, pos = parse_cstring(raw, pos)
            ent[key] = value
        entities.append(ent)
    lines = [f"NumEntities {len(entities)}"]
    for i, ent in enumerate(entities):
        lines.append("")
        lines.append(f"entity {i}")
        for key, value in ent.items():
            lines.append(f"{key}: {value}")
    return "\n".join(lines) + "\n", entities


def face_vertices(face, vert_indices, verts):
    ids = vert_indices[face["first_vert"]:face["first_vert"] + face["num_verts"]]
    return [verts[idx] for idx in ids if 0 <= idx < len(verts)]


def write_obj(path: Path, faces, verts, vert_indices, texinfo, textures):
    with path.open("w", encoding="utf-8") as f:
        f.write("# Extracted from Genesis3D GBSP. Coordinates are engine-native.\n")
        for x, y, z in verts:
            f.write(f"v {x:.6f} {y:.6f} {z:.6f}\n")
        last_mat = None
        for face in faces:
            ti = texinfo[face["texinfo"]] if 0 <= face["texinfo"] < len(texinfo) else None
            tex_name = "unknown"
            if ti and 0 <= ti["texture"] < len(textures):
                tex_name = textures[ti["texture"]]["name"] or "unnamed"
            if tex_name != last_mat:
                safe = ''.join(c if c.isalnum() or c in '._-' else '_' for c in tex_name)
                f.write(f"usemtl {safe}\n")
                last_mat = tex_name
            ids = vert_indices[face["first_vert"]:face["first_vert"] + face["num_verts"]]
            ids = [idx + 1 for idx in ids if 0 <= idx < len(verts)]
            if len(ids) >= 3:
                f.write("f " + " ".join(str(i) for i in ids) + "\n")


def write_svg(path: Path, faces, verts, vert_indices, planes, width=1400):
    polys = []
    for face in faces:
        pts = face_vertices(face, vert_indices, verts)
        if len(pts) < 3:
            continue
        plane = planes[face["plane_num"]] if 0 <= face["plane_num"] < len(planes) else None
        n = plane["normal"] if plane else [0, 0, 0]
        # Floor/ceiling-ish surfaces in Genesis3D maps are usually Y-up. Use X/Z for top-down.
        if abs(n[1]) < 0.65:
            continue
        polys.append((sum(p[1] for p in pts) / len(pts), [(p[0], p[2]) for p in pts]))
    if not polys:
        return
    xs = [x for _, poly in polys for x, _ in poly]
    zs = [z for _, poly in polys for _, z in poly]
    minx, maxx, minz, maxz = min(xs), max(xs), min(zs), max(zs)
    spanx, spanz = maxx - minx, maxz - minz
    pad = 30
    scale = (width - pad * 2) / max(spanx, spanz, 1)
    height = int(spanz * scale + pad * 2)
    def tx(x): return pad + (x - minx) * scale
    def ty(z): return pad + (maxz - z) * scale
    with path.open("w", encoding="utf-8") as f:
        f.write(f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">\n')
        f.write('<rect width="100%" height="100%" fill="#111"/>\n')
        ys = [y for y, _ in polys]
        ymin, ymax = min(ys), max(ys)
        for y, poly in sorted(polys):
            shade = 60 + int(150 * ((y - ymin) / (ymax - ymin or 1)))
            points = " ".join(f"{tx(x):.2f},{ty(z):.2f}" for x, z in poly)
            f.write(f'<polygon points="{points}" fill="rgb({shade},{shade},{shade})" stroke="#f05a28" stroke-width="0.7" fill-opacity="0.55"/>\n')
        f.write('</svg>\n')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("bsp", type=Path)
    ap.add_argument("--out", type=Path, default=Path("extracted"))
    args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)

    _, chunks, payloads = read_chunks(args.bsp)
    header = None
    if 0 in payloads:
        tag = payloads[0][:5].split(b"\0", 1)[0].decode("ascii", "replace")
        version = struct.unpack_from("<i", payloads[0], 8)[0]
        header = {"tag": tag, "version": version}

    models = parse_models(payloads.get(1, b""))
    faces = parse_faces(payloads.get(11, b""))
    verts = vecs(payloads.get(14, b""))
    vert_indices = list(struct.unpack("<" + "i" * (len(payloads.get(13, b"")) // 4), payloads.get(13, b""))) if 13 in payloads else []
    textures = parse_textures(payloads.get(18, b""))
    texinfo = parse_texinfo(payloads.get(17, b""))
    planes = parse_planes(payloads.get(10, b""))
    ent_text, entities = parse_entities(payloads.get(16, b""))

    summary = {
        "source": str(args.bsp),
        "header": header,
        "chunks": [asdict(c) for c in chunks],
        "models": models,
        "counts": {"faces": len(faces), "verts": len(verts), "vert_indices": len(vert_indices), "planes": len(planes), "textures": len(textures), "texinfo": len(texinfo), "entities": len(entities)},
        "textures": textures,
        "entities": entities,
    }
    (args.out / "summary.json").write_text(json.dumps(summary, indent=2), encoding="utf-8")
    (args.out / "entities.txt").write_text(ent_text, encoding="utf-8")
    (args.out / "entdata.bin").write_bytes(payloads.get(16, b""))
    write_obj(args.out / "geometry.obj", faces, verts, vert_indices, texinfo, textures)
    write_svg(args.out / "topdown.svg", faces, verts, vert_indices, planes)

    print(json.dumps({"out": str(args.out), "header": header, "counts": summary["counts"], "models": len(models)}, indent=2))

if __name__ == "__main__":
    main()
