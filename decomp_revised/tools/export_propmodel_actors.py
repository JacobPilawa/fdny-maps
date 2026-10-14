#!/usr/bin/env python3
"""Export ACT resources referenced by authored RCS_PropModel entities.

The scene viewer uses these decoded assets for world-space PropModel instances.
Exports stay under .local/actor_exports/props and are rebuilt from copied retail
inputs and the typed scene packages, rather than from the map-explorer reference.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(Path(__file__).resolve().parent))

from export_equipment_actors import export_model  # noqa: E402
from export_victim_actors import parse_model_resources, resolve_case_insensitive  # noqa: E402


def referenced_models(exports: Path) -> dict[int, list[dict[str, str]]]:
    records: dict[int, list[dict[str, str]]] = {}
    for entity_file in sorted(exports.glob("*/entities.json")):
        scene_key = entity_file.parent.name
        entities = json.loads(entity_file.read_text())
        for entity in entities:
            if entity.get("class") != "RCS_PropModel":
                continue
            properties = entity.get("properties", {})
            raw_model_id = properties.get("D_Model")
            try:
                model_id = int(raw_model_id)
            except (TypeError, ValueError):
                continue
            records.setdefault(model_id, []).append({
                "scene": scene_key,
                "entity": str(entity.get("index", "")),
                "name": str(properties.get("A_Name", "")),
            })
    return records


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, default=ROOT / "inputs/original")
    parser.add_argument("--exports", type=Path, default=ROOT / ".local/exports")
    parser.add_argument("--output", type=Path, default=ROOT / ".local/actor_exports/props")
    args = parser.parse_args()

    rrf_path = args.inputs / "Data/ResDefs/rescue.rrf"
    model_resources = parse_model_resources(rrf_path)
    references = referenced_models(args.exports)
    if not references:
        raise FileNotFoundError(f"No exported RCS_PropModel records under {args.exports}")

    manifest_models = []
    for model_id, instances in sorted(references.items()):
        resource = model_resources.get(model_id)
        if resource is None:
            raise ValueError(f"rescue.rrf has no ACT mapping for PropModel resource {model_id}")
        source = resolve_case_insensitive(args.inputs, resource)
        if source is None:
            raise FileNotFoundError(f"PropModel resource {model_id} is missing: {resource}")
        stem = f"model_{model_id}"
        model = export_model(args.inputs, args.output, stem, model_id, resource,
                             asset_kind="world-prop")
        manifest_models.append({
            **model,
            "resourceActPath": resource,
            "instances": instances,
        })

    manifest = {
        "format": "fdny-local-propmodel-act-export-v1",
        "sourceRrf": rrf_path.relative_to(ROOT).as_posix()
        if rrf_path.is_relative_to(ROOT)
        else rrf_path.name,
        "sceneExports": sorted(path.parent.name for path in args.exports.glob("*/entities.json")),
        "models": manifest_models,
        "presentationLimit": "bind-pose world actors; PropModel transforms come from authored setup and script commands",
    }
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / "props_manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    for model in manifest_models:
        print(
            f"PropModel resource {model['resourceModelId']}: {model['resourceActPath']}, "
            f"{model['vertices']} vertices, {model['lod0Faces']} LOD0 faces, "
            f"{len(model['bitmaps'])} bitmap entries, {len(model['instances'])} references"
        )
    print(f"Wrote local PropModel ACT assets to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
