#!/usr/bin/env python3
"""Inventory authored G_Liftable BModels and their first-child retail PATHs."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from decode_gbsp_motions import GENESIS_DLL_SHA256, decode_gbsp_motions
from gbsp_v15 import parse_entities, read_gbsp

RETAIL_EXE_SHA256 = "4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e"


def _integer(value: str | None) -> int | None:
    try:
        return int(float(value)) if value is not None else None
    except ValueError:
        return None


def audit_map(path: Path) -> dict:
    raw, chunks, payloads = read_gbsp(path)
    bsp_sha256 = hashlib.sha256(raw).hexdigest()
    motions = decode_gbsp_motions(payloads.get(24, b""), bsp_sha256)
    motion_by_model = {record["model_index"]: record for record in motions["records"]}

    entities = [dict(pairs) for pairs in parse_entities(payloads[16])]
    named_models: dict[str, list[int]] = {}
    for entity in entities:
        if entity.get("classname") != "%Model%":
            continue
        model_index = _integer(entity.get("Model"))
        name = entity.get("%name%", "")
        if name and model_index is not None:
            named_models.setdefault(name, []).append(model_index)

    liftable_bmodels = []
    for entity_index, entity in enumerate(entities):
        if entity.get("classname") != "RCS_BModel":
            continue
        try:
            liftable_value = float(entity.get("G_Liftable", "0"))
        except ValueError:
            continue
        if liftable_value == 0.0:
            continue

        children = []
        for slot in range(64):
            child_name = entity.get(f"C_AnimModel{slot}")
            if not child_name or child_name == "<null>":
                continue
            indices = named_models.get(child_name, [])
            model_index = indices[0] if len(indices) == 1 else None
            record = motion_by_model.get(model_index) if model_index is not None else None
            paths = record["paths"] if record else []
            translation_keys = paths[0]["translation"]["keys"] if paths else []
            rotation_keys = paths[0]["rotation"]["keys"] if paths else []
            children.append({
                "slot": slot,
                "name": child_name,
                "model_index": model_index,
                "ambiguous_model_name": len(indices) > 1,
                "motion_record_present": record is not None,
                "path_count": record["path_count"] if record else 0,
                "translation_key_times": [key["time"] for key in translation_keys],
                "rotation_key_times": [key["time"] for key in rotation_keys],
            })

        first = children[0] if children else None
        first_times = first["translation_key_times"] if first else []
        liftable_bmodels.append({
            "entity_index": entity_index,
            "entity_name": entity.get("%name%"),
            "authored_name": entity.get("A_Name"),
            "liftable_value": entity.get("G_Liftable"),
            "hit_points": entity.get("F_HitPoints"),
            "reach": entity.get("E_Reach"),
            "children": children,
            # 0043BE00 reads the first child's motion PATH 0 and requires two
            # translation keys accepted against source times 0 and 1.
            "first_child_static_initializer_candidate": bool(
                first
                and not first["ambiguous_model_name"]
                and first["motion_record_present"]
                and first["path_count"] >= 1
                and first_times == [0.0, 1.0]
            ),
        })

    motion_chunk = next((chunk for chunk in chunks if chunk.type == 24), None)
    return {
        "map": path.stem,
        "bsp_sha256": bsp_sha256,
        "motion_chunk_sha256": motion_chunk.sha256 if motion_chunk else hashlib.sha256(b"").hexdigest(),
        "liftable_bmodels": liftable_bmodels,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("maps_dir", type=Path, help="directory containing retail GBSP v15 map files")
    parser.add_argument("--out", type=Path, help="write JSON to this file instead of stdout")
    args = parser.parse_args()
    maps = [audit_map(path) for path in sorted(args.maps_dir.glob("*.bsp"))]
    report = {
        "retail_exe_sha256": RETAIL_EXE_SHA256,
        "genesis_dll_sha256": GENESIS_DLL_SHA256,
        "map_count": len(maps),
        "liftable_bmodel_count": sum(len(item["liftable_bmodels"]) for item in maps),
        "static_initializer_candidate_count": sum(
            candidate["first_child_static_initializer_candidate"]
            for item in maps for candidate in item["liftable_bmodels"]
        ),
        "maps": maps,
    }
    output = json.dumps(report, indent=2) + "\n"
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(output, encoding="utf-8")
    else:
        print(output, end="")


if __name__ == "__main__":
    main()
