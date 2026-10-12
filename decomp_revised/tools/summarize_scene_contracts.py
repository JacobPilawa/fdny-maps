#!/usr/bin/env python3
"""Summarize authored entity-field coverage across exported BSP scenes."""
from __future__ import annotations

import argparse
import csv
import json
from collections import Counter, defaultdict
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", type=Path, default=Path("tracker/level_map_inventory.csv"))
    parser.add_argument("--exports", type=Path, default=Path(".local/exports"))
    parser.add_argument("--output", type=Path, default=Path("tracker/scene_property_matrix.csv"))
    args = parser.parse_args()

    maps: list[str] = []
    class_counts: Counter[str] = Counter()
    class_maps: dict[str, set[str]] = defaultdict(set)
    field_records: Counter[tuple[str, str]] = Counter()
    field_maps: dict[tuple[str, str], set[str]] = defaultdict(set)

    with args.inventory.open(newline="", encoding="utf-8") as stream:
        inventory = list(csv.DictReader(stream))
    for row in inventory:
        key = row["level_key"]
        export_dir = args.exports / key.casefold()
        report = json.loads((export_dir / "report.json").read_text(encoding="utf-8"))
        if report.get("format") != "fdny-gbsp-v15-scene-v2":
            parser.error(f"{key}: export report is not the current v2 schema")
        if report.get("source_sha256") != row.get("bsp_sha256"):
            parser.error(f"{key}: export report hash does not match the map inventory")
        entities = json.loads((export_dir / "entities.json").read_text(encoding="utf-8"))
        maps.append(key)
        for entity in entities:
            entity_class = entity.get("class", "<unknown>")
            if not (entity_class.startswith("RCS_") or entity_class in {"%Model%", "<untyped>"}):
                continue
            class_counts[entity_class] += 1
            class_maps[entity_class].add(key)
            properties = entity.get("properties", {})
            for property_name in properties:
                field_records[(entity_class, property_name)] += 1
                field_maps[(entity_class, property_name)].add(key)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    columns = [
        "entity_class", "property_name", "total_class_entities", "map_files_with_class",
        "map_files_with_property", "map_keys_with_property",
        "map_keys_with_class_without_property", "entities_with_property",
        "entities_missing_property",
    ]
    with args.output.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=columns, lineterminator="\n")
        writer.writeheader()
        for entity_class, property_name in sorted(field_records):
            present = field_records[(entity_class, property_name)]
            writer.writerow({
                "entity_class": entity_class,
                "property_name": property_name,
                "total_class_entities": class_counts[entity_class],
                "map_files_with_class": len(class_maps[entity_class]),
                "map_files_with_property": len(field_maps[(entity_class, property_name)]),
                "map_keys_with_property": ";".join(sorted(field_maps[(entity_class, property_name)], key=str.casefold)),
                "map_keys_with_class_without_property": ";".join(sorted(
                    class_maps[entity_class] - field_maps[(entity_class, property_name)], key=str.casefold
                )),
                "entities_with_property": present,
                "entities_missing_property": class_counts[entity_class] - present,
            })
    print(f"Wrote {len(field_records)} class/property rows across {len(maps)} map files to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
