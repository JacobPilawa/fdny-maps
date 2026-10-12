#!/usr/bin/env python3
"""Export the ACT actors declared for rescue victims into local review assets.

Decoded geometry and textures stay under .local/ and are not intended for Git.
The committed CSV is a compact, hash-pinned inventory of the source coverage.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(Path(__file__).resolve().parent))
import extract_act as act  # noqa: E402


def parse_victim_definitions(path: Path) -> dict[str, int]:
    result: dict[str, int] = {}
    current: str | None = None
    for line in path.read_text(errors="replace").splitlines():
        victim = re.match(r"\s*Victim:\s*(\S+)\s*$", line, re.IGNORECASE)
        model = re.match(r"\s*Model:\s*(\d+)\s*$", line, re.IGNORECASE)
        if victim:
            current = victim.group(1).lower()
        elif model and current:
            result[current] = int(model.group(1))
            current = None
    return result


def parse_model_resources(path: Path) -> dict[int, str]:
    return {
        int(model): resource.replace("\\", "/")
        for model, resource in re.findall(
            r'ROC_MODEL\s+(\d+)\s+"([^"]+\.act)"',
            path.read_text(errors="replace"),
            re.IGNORECASE,
        )
    }


def resolve_case_insensitive(root: Path, relative: str) -> Path | None:
    current = root
    for component in relative.replace("\\", "/").split("/"):
        if not component or component == ".":
            continue
        if not current.is_dir():
            return None
        candidate = next((p for p in current.iterdir() if p.name.lower() == component.lower()), None)
        if candidate is None:
            return None
        current = candidate
    return current if current.is_file() else None


def interpolation_modes(motions: list[dict], channel: str) -> set[int]:
    return {
        int(path[channel]["interpolation"])
        for motion in motions
        for path in motion["paths"]
        if path.get(channel) is not None
    }


def write_csv(path: Path, rows: list[dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    columns = [
        "victim_type", "model_id", "resource_act_path", "source_sha256",
        "source_file_exists", "map_entity_count", "map_file_count", "used_maps",
        "geometry_vertices", "lod_count", "texture_count", "motion_count",
        "material_slots", "bitmap_entry_names", "motion_names", "translation_interpolation_modes",
        "rotation_interpolation_modes", "unsupported_native_actor_pose_modes",
        "export_status",
    ]
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=columns)
        writer.writeheader()
        writer.writerows(rows)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, default=ROOT / "inputs/original")
    parser.add_argument("--output", type=Path, default=ROOT / ".local/actor_exports")
    parser.add_argument(
        "--summary", type=Path, default=ROOT / "tracker/actor_asset_inventory.csv"
    )
    args = parser.parse_args()

    data = args.inputs / "Data"
    victim_defs = parse_victim_definitions(data / "ResDefs/victims.srf")
    model_resources = parse_model_resources(data / "ResDefs/rescue.rrf")
    with (ROOT / "tracker/victim_resource_crosswalk.csv").open(newline="") as stream:
        crosswalk = {row["victim_type"].lower(): row for row in csv.DictReader(stream)}

    args.output.mkdir(parents=True, exist_ok=True)
    rows: list[dict[str, str]] = []
    for victim_type, cross in sorted(crosswalk.items()):
        model_id = victim_defs.get(victim_type)
        resource_path = model_resources.get(model_id) if model_id is not None else None
        source = resolve_case_insensitive(args.inputs, resource_path or "")
        row = {
            "victim_type": victim_type,
            "model_id": str(model_id) if model_id is not None else "",
            "resource_act_path": resource_path or "",
            "source_sha256": "",
            "source_file_exists": "no",
            "map_entity_count": cross["map_entity_count"],
            "map_file_count": cross["map_file_count"],
            "used_maps": cross["map_keys"],
            "geometry_vertices": "",
            "lod_count": "",
            "texture_count": "",
            "motion_count": "",
            "material_slots": "",
            "bitmap_entry_names": "",
            "motion_names": "",
            "translation_interpolation_modes": "",
            "rotation_interpolation_modes": "",
            "unsupported_native_actor_pose_modes": "",
            "export_status": "unresolved_resource_definition",
        }
        if source is not None and model_id is not None:
            source_bytes = source.read_bytes()
            outer = act.read_vfs(source_bytes)
            body_vfs = act.read_vfs(outer.read("Body"))
            body = act.read_body(body_vfs.read("Geometry"))
            motion_entries = sorted(outer.find("Motions").children, key=lambda entry: int(entry.name))
            motions = [
                act.read_motion(outer.data[entry.offset:entry.offset + entry.size])
                for entry in motion_entries
            ]
            payload = {
                "source": source.relative_to(ROOT).as_posix()
                if source.is_relative_to(ROOT)
                else source.name,
                "outerVfs": act.entry_report(outer.entries),
                "bodyVfs": act.entry_report(body_vfs.entries),
                "motions": motions,
                **body,
            }
            stem = source.stem.lower()
            (args.output / f"{stem}.json").write_text(json.dumps(payload, separators=(",", ":")))
            act.write_obj(body, args.output / f"{stem}.obj")
            bitmap_directory = body_vfs.find("Bitmaps")
            for entry in bitmap_directory.children:
                bitmap = body_vfs.data[entry.offset:entry.offset + entry.size]
                width, height, rgba = act.decode_gebm(bitmap)
                act.write_png(args.output / f"{stem}_{entry.name}.png", width, height, rgba)

            translation = interpolation_modes(motions, "translation")
            rotation = interpolation_modes(motions, "rotation")
            unsupported = sorted(
                [f"translation:{mode}" for mode in translation if mode not in {0, 1, 2}]
                + [f"rotation:{mode}" for mode in rotation if mode != 1]
            )
            row.update({
                "source_sha256": hashlib.sha256(source_bytes).hexdigest(),
                "source_file_exists": "yes",
                "geometry_vertices": str(len(body["vertices"])),
                "lod_count": str(len(body["lods"])),
                "texture_count": str(len(bitmap_directory.children)),
                "motion_count": str(len(motions)),
                "material_slots": ";".join(
                    f"{index}:{material['name']}:{material['bitmapId']}"
                    for index, material in enumerate(body["materials"])
                ),
                "bitmap_entry_names": ";".join(
                    entry.name for entry in sorted(bitmap_directory.children, key=lambda item: int(item.name))
                ),
                "motion_names": ";".join(motion["name"] for motion in motions),
                "translation_interpolation_modes": ";".join(map(str, sorted(translation))),
                "rotation_interpolation_modes": ";".join(map(str, sorted(rotation))),
                "unsupported_native_actor_pose_modes": ";".join(unsupported),
                "export_status": "exported_local",
            })
            print(
                f"{victim_type}: model {model_id}, {len(body['vertices'])} vertices, "
                f"{len(motions)} motions, {len(bitmap_directory.children)} textures"
            )
        elif not model_resources.get(model_id or -1):
            row["export_status"] = "unresolved_actor_type"
        rows.append(row)

    write_csv(args.summary, rows)
    (args.output / "manifest.json").write_text(json.dumps(rows, indent=2) + "\n")
    print(f"Inventory: {args.summary}")
    print(f"Decoded review assets: {args.output}")
    return 0 if all(row["export_status"] == "exported_local" for row in rows if row["victim_type"] != "boy1") else 1


if __name__ == "__main__":
    raise SystemExit(main())
