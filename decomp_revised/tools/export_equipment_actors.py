#!/usr/bin/env python3
"""Export the rescue ACT assets used by the axe, its left-hand variant, and extinguisher to .local.

The export includes an ACT JSON document, bind-pose OBJ, inspection images,
and an RGBA payload that preserves GeBm color-key transparency. These are
local render inputs, not proof of retail pose or equipment attachment.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(Path(__file__).resolve().parent))

import extract_act as act  # noqa: E402
from export_victim_actors import (  # noqa: E402
    parse_model_resources,
    resolve_case_insensitive,
    write_bmp,
)


EQUIPMENT_MODELS = {
    "axe": 1,
    "axel": 6,
    "ext": 2,
}


def export_model(inputs: Path, output: Path, name: str, model_id: int,
                 resource_path: str, asset_kind: str = "equipment") -> dict:
    source = resolve_case_insensitive(inputs, resource_path)
    if source is None:
        raise FileNotFoundError(f"Model {model_id} ({name}) is missing: {resource_path}")

    source_bytes = source.read_bytes()
    outer = act.read_vfs(source_bytes)
    body_vfs = act.read_vfs(outer.read("Body"))
    body = act.read_body(body_vfs.read("Geometry"))
    motion_directory = outer.find("Motions")
    motion_entries = sorted(motion_directory.children, key=lambda entry: int(entry.name))
    motions = [
        act.read_motion(outer.data[entry.offset:entry.offset + entry.size])
        for entry in motion_entries
    ]
    bitmaps = body_vfs.find("Bitmaps")

    payload = {
        "source": source.relative_to(ROOT).as_posix()
        if source.is_relative_to(ROOT)
        else source.name,
        "assetKind": asset_kind,
        "assetName": name,
        "resourceModelId": model_id,
        "sourceSha256": hashlib.sha256(source_bytes).hexdigest(),
        "outerVfs": act.entry_report(outer.entries),
        "bodyVfs": act.entry_report(body_vfs.entries),
        "motions": motions,
        **body,
    }
    output.mkdir(parents=True, exist_ok=True)
    (output / f"{name}.json").write_text(json.dumps(payload, separators=(",", ":")) + "\n")
    act.write_obj(body, output / f"{name}.obj")

    bitmap_rows = []
    for entry in bitmaps.children:
        bitmap_data = body_vfs.data[entry.offset:entry.offset + entry.size]
        width, height, rgba = act.decode_gebm(bitmap_data)
        # Keep the existing actor export's filename convention: use the
        # numeric bitmap-directory entry name as the suffix.
        stem = f"{name}_{entry.name}"
        act.write_png(output / f"{stem}.png", width, height, rgba)
        write_bmp(output / f"{stem}.bmp", width, height, rgba)
        (output / f"{stem}.rgba").write_bytes(struct.pack("<II", width, height) + rgba)
        bitmap_rows.append({"id": entry.name, "width": width, "height": height})

    return {
        "name": name,
        "resourceModelId": model_id,
        "resourceActPath": resource_path,
        "source": payload["source"],
        "sourceSha256": payload["sourceSha256"],
        "vertices": len(body["vertices"]),
        "bones": len(body["bones"]),
        "lods": len(body["lods"]),
        "lod0Faces": len(body["lods"][0]) if body["lods"] else 0,
        "materials": len(body["materials"]),
        "motions": len(motions),
        "bitmaps": bitmap_rows,
        "renderPose": "bind pose only; equipment attachment and action timing are not inferred here",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, default=ROOT / "inputs/original")
    parser.add_argument("--output", type=Path, default=ROOT / ".local/actor_exports/equipment")
    args = parser.parse_args()

    rrf_path = args.inputs / "Data/ResDefs/rescue.rrf"
    model_resources = parse_model_resources(rrf_path)
    manifest_models = []
    for name, model_id in EQUIPMENT_MODELS.items():
        resource = model_resources.get(model_id)
        if resource is None or Path(resource).stem.lower() != name:
            raise ValueError(
                f"rescue.rrf model {model_id} does not resolve to {name}.act; found {resource!r}"
            )
        manifest_models.append(
            export_model(args.inputs, args.output, name, model_id, resource)
        )

    manifest = {
        "format": "fdny-local-equipment-act-export-v1",
        "sourceRrf": rrf_path.relative_to(ROOT).as_posix()
        if rrf_path.is_relative_to(ROOT)
        else rrf_path.name,
        "models": manifest_models,
        "presentationLimit": "asset extraction only; root placement, view attachment, swing motion, and retail draw flags are not asserted",
    }
    (args.output / "equipment_manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n"
    )
    for model in manifest_models:
        print(
            f"{model['name']}: RRF model {model['resourceModelId']}, "
            f"{model['vertices']} vertices, {model['lod0Faces']} LOD0 faces, "
            f"{len(model['bitmaps'])} bitmap entries"
        )
    print(f"Wrote local equipment ACT assets to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
