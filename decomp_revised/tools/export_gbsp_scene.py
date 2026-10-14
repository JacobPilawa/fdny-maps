#!/usr/bin/env python3
"""Export decoded, provenance-pinned GBSP v15 scene data for reverse engineering.

This exporter preserves every chunk payload and emits typed geometry,
collision, entity, and texture tables. It decodes base-level indexed textures
and projects world-mesh UVs. The retail Genesis MOTN/PATH 0.F2 records are
decoded to motions.json while their original bytes remain preserved.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from collections import Counter
from pathlib import Path

from gbsp_v15 import chunk_dicts, parse_entities, read_gbsp
from extract_gbsp_typedefs import extract_typedefs
from decode_gbsp_motions import decode_gbsp_motions


def records(payload: bytes, fmt: str, label: str) -> list[tuple]:
    size = struct.calcsize(fmt)
    if len(payload) % size:
        raise ValueError(f"{label} payload length {len(payload)} is not divisible by {size}")
    return list(struct.iter_unpack(fmt, payload))


def decode_header(payload: bytes) -> dict:
    tag, version, *system_time = struct.unpack("<5s3xi8H", payload)
    return {
        "tag": tag.split(b"\0", 1)[0].decode("ascii", "replace"),
        "version": version,
        "system_time_words": system_time,
    }


def decode_models(payload: bytes) -> list[dict]:
    result = []
    for v in records(payload, "<ii9f8iI", "MODELS"):
        result.append({
            "root_node": [v[0], v[1]],
            "mins": list(v[2:5]),
            "maxs": list(v[5:8]),
            "origin": list(v[8:11]),
            "first_face": v[11], "num_faces": v[12],
            "first_leaf": v[13], "num_leafs": v[14],
            "first_cluster": v[15], "num_clusters": v[16],
            "areas": [v[17], v[18]],
            "motion_pointer_or_offset": v[19],
        })
    return result


def decode_nodes(payload: bytes) -> list[dict]:
    result = []
    for v in records(payload, "<5i6f", "NODES"):
        result.append({
            "children": [v[0], v[1]], "num_faces": v[2],
            "first_face": v[3], "plane": v[4],
            "mins": list(v[5:8]), "maxs": list(v[8:11]),
        })
    return result


def decode_leafs(payload: bytes) -> list[dict]:
    result = []
    for v in records(payload, "<i6f8i", "LEAFS"):
        result.append({
            "contents": v[0], "mins": list(v[1:4]), "maxs": list(v[4:7]),
            "first_face": v[7], "num_faces": v[8],
            "first_portal": v[9], "num_portals": v[10],
            "cluster": v[11], "area": v[12],
            "first_side": v[13], "num_sides": v[14],
        })
    return result


def decode_collision(models: list[dict], nodes: list[dict], bnodes: list[tuple],
                     leafs: list[dict], leaf_sides: list[tuple],
                     planes: list[tuple]) -> dict:
    return {
        # The port's collision implementation uses this mask; retain it as a
        # runtime choice with provenance, separate from raw BSP contents.
        "contents_mask": 0x43,
        "models": [
            {"index": i, "root": m["root_node"][0], "root_node": m["root_node"],
             "mins": m["mins"], "maxs": m["maxs"], "origin": m["origin"],
             "first_leaf": m["first_leaf"], "num_leafs": m["num_leafs"]}
            for i, m in enumerate(models)
        ],
        "nodes": nodes,
        "bnodes": [{"children": [v[0], v[1]], "plane": v[2]} for v in bnodes],
        "leafs": leafs,
        "leaf_sides": [{"plane": v[0], "side": v[1]} for v in leaf_sides],
        "planes": [
            {"normal": list(v[:3]), "distance": v[3], "type": v[4]}
            for v in planes
        ],
    }


def decode_geometry(payloads: dict[int, bytes], models: list[dict]) -> dict:
    faces = []
    for v in records(payloads.get(11, b""), "<8i4B", "FACES"):
        faces.append({
            "first_vert": v[0], "num_verts": v[1],
            "plane_num": v[2], "plane_side": v[3], "texinfo": v[4],
            "light_ofs": v[5], "light_width": v[6], "light_height": v[7],
            "light_types": list(v[8:12]),
        })

    texinfo = []
    # GFX_TexInfo is 10 floats, an int32 flags field, 4 floats, and an int32
    # texture index. Treating all 15 leading words as floats loses Flags.
    for v in records(payloads.get(17, b""), "<10fi4fi", "TEXINFO"):
        texinfo.append({
            "u_vec": list(v[0:3]), "v_vec": list(v[3:6]),
            "shift": list(v[6:8]), "draw_scale": list(v[8:10]),
            "flags": v[10], "face_light": v[11],
            "reflective_scale": v[12], "alpha": v[13],
            "mip_map_bias": v[14], "texture": v[15],
        })

    textures = []
    for v in records(payloads.get(18, b""), "<32sIiiii", "TEXTURES"):
        textures.append({
            "name": v[0].split(b"\0", 1)[0].decode("latin1", "replace"),
            "flags": v[1], "width": v[2], "height": v[3],
            "offset": v[4], "palette_index": v[5],
        })

    sky_data = None
    if 22 in payloads:
        v = records(payloads[22], "<4f6if", "SKYDATA")[0]
        sky_data = {
            "axis": list(v[0:3]), "degrees_per_minute": v[3],
            "textures": list(v[4:10]), "draw_scale": v[10],
        }

    def i32s(chunk: int, label: str) -> list[int]:
        return [v[0] for v in records(payloads.get(chunk, b""), "<i", label)]

    def vec3s(chunk: int, label: str) -> list[list[float]]:
        return [list(v) for v in records(payloads.get(chunk, b""), "<3f", label)]

    return {
        "models": models,
        "nodes": decode_nodes(payloads.get(2, b"")),
        "planes": [list(v) for v in records(payloads.get(10, b""), "<4fi", "PLANES")],
        "faces": faces,
        "leaf_faces": i32s(12, "LEAF_FACES"),
        "vertex_indices": i32s(13, "VERT_INDEX"),
        "vertices": vec3s(14, "VERTS"),
        "rgb_vertices": vec3s(15, "RGB_VERTS"),
        "texinfo": texinfo,
        "textures": textures,
        "sky": sky_data,
        "format_notes": [
            "Texture UV projection inputs are retained in TEXINFO; projected UVs are not yet emitted.",
            "Lightmap, embedded texture, palette, visibility, and motion payloads are preserved as raw chunks.",
        ],
    }


def entity_records(epairs: list[list[tuple[str, str]]]) -> list[dict]:
    entities = []
    for index, pairs in enumerate(epairs):
        properties: dict[str, str] = {}
        for key, value in pairs:
            properties[key] = value
        entities.append({
            "index": index,
            "class": properties.get("classname", "<untyped>"),
            "name": properties.get("%name%", properties.get("name", "")),
            "properties": properties,
            "epairs": [{"key": k, "value": v} for k, v in pairs],
        })
    return entities


def safe_texture_files(textures: list[dict]) -> list[str]:
    """Return deterministic, case-insensitively unique relative BMP paths."""
    files: list[str] = []
    used: set[str] = set()
    for index, texture in enumerate(textures):
        name = texture["name"] or f"texture_{index:03d}"
        stem = re.sub(r"[^A-Za-z0-9._-]", "_", name) or f"texture_{index:03d}"
        candidate = stem
        suffix = 0
        while f"textures/{candidate}.bmp".casefold() in used:
            suffix += 1
            candidate = f"{stem}__tex_{index:03d}" + (f"_{suffix}" if suffix > 1 else "")
        relpath = f"textures/{candidate}.bmp"
        used.add(relpath.casefold())
        files.append(relpath)
    return files


def world_mesh(geometry: dict, texture_files: list[str]) -> dict:
    """Build triangle groups from GBSP faces with stable material identity."""
    grouped: dict[tuple[int, int], dict] = {}
    faces, models = geometry["faces"], geometry["models"]
    texinfo, textures = geometry["texinfo"], geometry["textures"]
    vertices, indices = geometry["vertices"], geometry["vertex_indices"]

    for model_id, model in enumerate(models):
        first = model["first_face"]
        end = first + model["num_faces"]
        if first < 0 or end > len(faces):
            raise ValueError(f"model {model_id} has out-of-range face span [{first},{end})")
        for face_id in range(first, end):
            face = faces[face_id]
            info = texinfo[face["texinfo"]]
            texture_id = info["texture"]
            if texture_id < 0 or texture_id >= len(textures):
                raise ValueError(f"texinfo {face['texinfo']} has invalid texture index {texture_id}")
            texture = textures[texture_id]
            if texture["width"] <= 0 or texture["height"] <= 0:
                raise ValueError(f"texture {texture_id} has invalid dimensions")
            scale_u, scale_v = info["draw_scale"]
            if scale_u == 0 or scale_v == 0:
                raise ValueError(f"texinfo {face['texinfo']} has a zero draw scale")

            key = (model_id, texture_id)
            group = grouped.setdefault(key, {
                "model": model_id, "texture": texture["name"],
                "texture_index": texture_id, "texture_file": texture_files[texture_id],
                "positions": [], "uvs": [], "face_spans": [],
            })
            start, count = face["first_vert"], face["num_verts"]
            polygon = [indices[i] for i in range(start, start + count)]
            first_triangle = len(group["positions"]) // 9
            triangle_count = max(0, len(polygon) - 2)
            group["face_spans"].append({
                "face_id": face_id,
                "first_triangle": first_triangle,
                "triangle_count": triangle_count,
                "texinfo_id": face["texinfo"],
                "texinfo_flags": info["flags"],
                "texinfo_alpha": info["alpha"],
                "face_light": info["face_light"],
                "reflective_scale": info["reflective_scale"],
                "mip_map_bias": info["mip_map_bias"],
                "light_ofs": face["light_ofs"],
                "light_width": face["light_width"],
                "light_height": face["light_height"],
                "light_types": face["light_types"],
            })
            # Genesis BSP faces are convex polygons; the Training prototype's
            # asset builder triangulated each polygon as a fan from vertex 0.
            for corner in range(1, len(polygon) - 1):
                for vertex_id in (polygon[0], polygon[corner], polygon[corner + 1]):
                    point = vertices[vertex_id]
                    group["positions"].extend(point)
                    raw_u = sum(point[i] * info["u_vec"][i] for i in range(3))
                    raw_v = sum(point[i] * info["v_vec"][i] for i in range(3))
                    # Public Genesis Surface.c uses dot products, inverse
                    # DrawScale, Shift and texture dimensions. V is flipped
                    # for the SDL/OpenGL bitmap convention used by the port.
                    u = (raw_u / scale_u + info["shift"][0]) / texture["width"]
                    v = 1.0 - (raw_v / scale_v + info["shift"][1]) / texture["height"]
                    group["uvs"].extend((u, v))

    return {"groups": list(grouped.values())}


def validate_world_mesh(mesh: dict, geometry: dict) -> dict[str, int]:
    """Check that face spans preserve the source faces and fan-triangle layout."""
    faces, models = geometry["faces"], geometry["models"]
    texinfo = geometry["texinfo"]
    seen_faces: list[tuple[int, int]] = []
    triangle_count = 0
    for group_index, group in enumerate(mesh["groups"]):
        model_id = group["model"]
        texture_id = group["texture_index"]
        if model_id < 0 or model_id >= len(models):
            raise ValueError(f"world mesh group {group_index} has invalid model {model_id}")
        model = models[model_id]
        expected_faces = [
            face_id for face_id in range(model["first_face"], model["first_face"] + model["num_faces"])
            if texinfo[faces[face_id]["texinfo"]]["texture"] == texture_id
        ]
        spans = group["face_spans"]
        if [span["face_id"] for span in spans] != expected_faces:
            raise ValueError(f"world mesh group {group_index} face identity/order does not match model source")

        expected_first_triangle = 0
        for span in spans:
            face_id = span["face_id"]
            face = faces[face_id]
            info = texinfo[face["texinfo"]]
            expected_triangles = max(0, face["num_verts"] - 2)
            expected_fields = {
                "first_triangle": expected_first_triangle,
                "triangle_count": expected_triangles,
                "texinfo_id": face["texinfo"],
                "texinfo_flags": info["flags"],
                "texinfo_alpha": info["alpha"],
                "face_light": info["face_light"],
                "reflective_scale": info["reflective_scale"],
                "mip_map_bias": info["mip_map_bias"],
                "light_ofs": face["light_ofs"],
                "light_width": face["light_width"],
                "light_height": face["light_height"],
                "light_types": face["light_types"],
            }
            for key, expected in expected_fields.items():
                if span[key] != expected:
                    raise ValueError(f"face {face_id} span field {key} does not match source geometry")
            expected_first_triangle += expected_triangles
            triangle_count += expected_triangles
            seen_faces.append((model_id, face_id))

        if len(group["positions"]) != expected_first_triangle * 9:
            raise ValueError(f"world mesh group {group_index} positions disagree with its face spans")
        if len(group["uvs"]) != expected_first_triangle * 6:
            raise ValueError(f"world mesh group {group_index} UVs disagree with its face spans")

    expected_model_faces = [
        (model_id, face_id)
        for model_id, model in enumerate(models)
        for face_id in range(model["first_face"], model["first_face"] + model["num_faces"])
    ]
    if sorted(seen_faces) != sorted(expected_model_faces):
        raise ValueError("world mesh face spans do not cover the model face ranges exactly once")
    return {"face_spans": len(seen_faces), "triangles": triangle_count}


def write_bmp24(path: Path, width: int, height: int, indices: bytes, palette: bytes) -> None:
    """Write a palette-indexed GBSP texture as a 24-bit Windows BMP."""
    if len(indices) != width * height:
        raise ValueError(f"texture has {len(indices)} base pixels, expected {width * height}")
    if len(palette) != 768:
        raise ValueError(f"palette has {len(palette)} bytes, expected 768")
    row_bytes = width * 3
    stride = (row_bytes + 3) & ~3
    image_size = stride * height
    offset = 14 + 40
    file_size = offset + image_size
    file_header = struct.pack("<2sIHHI", b"BM", file_size, 0, 0, offset)
    info_header = struct.pack("<IiiHHIIiiII", 40, width, height, 1, 24,
                              0, image_size, 0, 0, 0, 0)
    body = bytearray()
    padding = b"\0" * (stride - row_bytes)
    # Positive BMP height stores rows bottom-up. Palette entries are RGB in
    # GBSP; pixel triples in a 24-bit BMP are BGR.
    for y in range(height - 1, -1, -1):
        row = indices[y * width:(y + 1) * width]
        for palette_index in row:
            base = palette_index * 3
            red, green, blue = palette[base:base + 3]
            body.extend((blue, green, red))
        body.extend(padding)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(file_header + info_header + body)


def export_textures(payloads: dict[int, bytes], textures: list[dict], texture_files: list[str], out_dir: Path) -> list[dict]:
    texdata, palettes = payloads.get(19, b""), payloads.get(23, b"")
    texture_dir = out_dir / "textures"
    texture_dir.mkdir(parents=True, exist_ok=True)
    manifest = []
    for index, texture in enumerate(textures):
        name = texture["name"] or f"texture_{index:03d}"
        relpath = texture_files[index]
        palette_index = texture["palette_index"]
        pal_start = palette_index * 768
        pal_end = pal_start + 768
        pixel_count = texture["width"] * texture["height"]
        data_start = texture["offset"]
        data_end = data_start + pixel_count
        if pal_start < 0 or pal_end > len(palettes):
            raise ValueError(f"texture {name} references invalid palette {palette_index}")
        if data_start < 0 or data_end > len(texdata):
            raise ValueError(f"texture {name} base pixels exceed TEXDATA")
        write_bmp24(out_dir / relpath, texture["width"], texture["height"],
                    texdata[data_start:data_end], palettes[pal_start:pal_end])
        manifest.append({"index": index, **texture, "file": relpath})
    return manifest


def export(bsp_path: Path, out_dir: Path, keep_raw_chunks: bool = True) -> dict:
    raw, chunks, payloads = read_gbsp(bsp_path)
    source_sha256 = hashlib.sha256(raw).hexdigest()
    header = decode_header(payloads[0])
    if header["tag"] != "GBSP":
        raise ValueError(f"unexpected GBSP tag {header['tag']!r}")
    if header["version"] != 15:
        raise ValueError(f"GBSP version {header['version']} is not version 15")

    models = decode_models(payloads.get(1, b""))
    nodes = decode_nodes(payloads.get(2, b""))
    bnodes = records(payloads.get(3, b""), "<3i", "BNODES")
    leafs = decode_leafs(payloads.get(4, b""))
    leaf_sides = records(payloads.get(8, b""), "<2i", "LEAF_SIDES")
    planes = records(payloads.get(10, b""), "<4fi", "PLANES")
    epairs = parse_entities(payloads[16])
    entities = entity_records(epairs)
    classes = Counter(e["class"] for e in entities)
    motions = decode_gbsp_motions(payloads.get(24, b""), source_sha256)
    motion_paths = sum(len(record["paths"]) for record in motions["records"])
    motion_channels = sum(
        int(path["rotation"]["present"]) + int(path["translation"]["present"])
        for record in motions["records"] for path in record["paths"]
    )

    # Bounds and indices are checked before these arrays feed a renderer or
    # collision runtime; unknown/opaque payloads remain available in chunks/.
    geometry = decode_geometry(payloads, models)
    faces, vertices = geometry["faces"], geometry["vertices"]
    indices, texinfo = geometry["vertex_indices"], geometry["texinfo"]
    for face_index, face in enumerate(faces):
        start, count = face["first_vert"], face["num_verts"]
        if start < 0 or count < 0 or start + count > len(indices):
            raise ValueError(f"face {face_index} has out-of-range vertex-index span")
        if face["texinfo"] < 0 or face["texinfo"] >= len(texinfo):
            raise ValueError(f"face {face_index} has invalid texinfo index {face['texinfo']}")
        for vertex_index in indices[start:start + count]:
            if vertex_index < 0 or vertex_index >= len(vertices):
                raise ValueError(f"face {face_index} references invalid vertex {vertex_index}")

    out_dir.mkdir(parents=True, exist_ok=True)
    raw_dir = out_dir / "chunks"
    if keep_raw_chunks:
        raw_dir.mkdir(parents=True, exist_ok=True)
        for chunk in chunks:
            if chunk.type == 0xFFFF:
                continue
            payload = payloads[chunk.type]
            (raw_dir / f"{chunk.type:02d}_{chunk.name.lower()}.bin").write_bytes(payload)

    collision = decode_collision(models, nodes, bnodes, leafs, leaf_sides, planes)
    texture_files = safe_texture_files(geometry["textures"])
    mesh = world_mesh(geometry, texture_files)
    mesh_counts = validate_world_mesh(mesh, geometry)
    texture_manifest = export_textures(payloads, geometry["textures"], texture_files, out_dir)
    geometry["format_notes"] = [
        "Texinfo and lightmap references are retained for parity work.",
        "MOTN 0.F0 and PATH 0.F2 key values are decoded in motions.json from the retail Genesis.dll reader behavior; chunk 24 is also preserved byte-for-byte.",
        "Other PATH encodings and opaque MOTN event/name-array structures remain undecoded.",
        "World mesh UVs use the current Genesis3D Surface.c projection and were compared against all 121 Training mesh groups.",
    ]
    (out_dir / "world_mesh.json").write_text(json.dumps(mesh, indent=2) + "\n", encoding="utf-8")
    (out_dir / "geometry.json").write_text(json.dumps(geometry, indent=2) + "\n", encoding="utf-8")
    (out_dir / "collision.json").write_text(json.dumps(collision, indent=2) + "\n", encoding="utf-8")
    (out_dir / "entities.json").write_text(json.dumps(entities, indent=2) + "\n", encoding="utf-8")
    (out_dir / "motions.json").write_text(json.dumps(motions, indent=2) + "\n", encoding="utf-8")
    typedefs = extract_typedefs(epairs)
    (out_dir / "typedefs.json").write_text(json.dumps({"typedefs": typedefs}, indent=2) + "\n", encoding="utf-8")
    (out_dir / "texture_manifest.json").write_text(json.dumps(texture_manifest, indent=2) + "\n", encoding="utf-8")

    report = {
        "format": "fdny-gbsp-v15-scene-v2",
        "source_file": bsp_path.name,
        "source_sha256": source_sha256,
        "source_bytes": len(raw),
        "header": header,
        "chunks": chunk_dicts(chunks),
        "counts": {
            "models": len(models), "nodes": len(nodes), "bnodes": len(bnodes),
            "leafs": len(leafs), "leaf_sides": len(leaf_sides), "planes": len(planes),
            "faces": len(faces), "vertices": len(vertices), "vertex_indices": len(indices),
            "textures": len(geometry["textures"]), "mesh_groups": len(mesh["groups"]),
            "mesh_face_spans": mesh_counts["face_spans"],
            "mesh_triangles": mesh_counts["triangles"],
            "texinfo": len(texinfo),
            "entities": len(entities), "typedefs": len(typedefs),
            "motion_records": motions["declared_count"], "motion_paths": motion_paths,
            "motion_channels": motion_channels,
        },
        "entity_classes": dict(sorted(classes.items())),
        "outputs": ["geometry.json", "world_mesh.json", "collision.json", "entities.json",
                    "motions.json", "typedefs.json", "texture_manifest.json", "textures/"]
                   + (["chunks/"] if keep_raw_chunks else []),
        "open_decoders": ["texture mip-chain semantics", "LIGHTDATA", "VISDATA",
                           "non-F2 PATH variants", "MOTN event/name-array runtime use",
                           "render-time lightmap and face-material state"],
        "source_identity_note": "MOTN/PATH key decoding is pinned to the copied FDNY Genesis.dll SHA-256 recorded in motions.json; no public map-explorer implementation is used.",
    }
    (out_dir / "report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bsp", type=Path, help="GBSP v15 BSP file")
    parser.add_argument("--out", type=Path, required=True, help="output directory (prefer .local/exports/<map>)")
    parser.add_argument("--no-raw-chunks", action="store_true", help="omit byte-for-byte chunk payload copies")
    args = parser.parse_args()
    report = export(args.bsp, args.out, keep_raw_chunks=not args.no_raw_chunks)
    print(json.dumps({"out": str(args.out), "source_sha256": report["source_sha256"], "header": report["header"], "counts": report["counts"]}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
