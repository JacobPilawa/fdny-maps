#!/usr/bin/env python3
"""Inventory level-catalog entries against local map files without copying data."""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

from extract_gbsp_typedefs import read_entities


def sha256(path: Path | None) -> str:
    if path is None:
        return ""
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def parse_catalog(path: Path) -> list[dict[str, str]]:
    records: list[dict[str, str]] = []
    current: dict[str, str] | None = None
    field_pattern = re.compile(r"^(level|title|rrf|music|time|mapcoords)\s*:\s*(.*?)\s*$", re.I)
    for raw_line in path.read_text(errors="replace").splitlines():
        line = raw_line.strip()
        match = field_pattern.match(line)
        if not match:
            continue
        key, value = match.group(1).lower(), match.group(2)
        value = value.strip().strip('"')
        if key == "level":
            if current:
                records.append(current)
            current = {"level_key": value}
        elif current is not None:
            current[key] = value
    if current:
        records.append(current)
    return records


def file_lookup(directory: Path, stem: str, suffix: str) -> Path | None:
    candidates = [
        path for path in directory.iterdir()
        if path.is_file() and path.suffix.casefold() == suffix.casefold()
        and path.stem.casefold() == stem.casefold()
    ]
    if not candidates:
        return None
    # Prefer exact spelling; otherwise choose deterministically and preserve
    # the actual filename so a case-sensitive port can add an explicit alias.
    candidates.sort(key=lambda path: (path.stem != stem, path.name.casefold(), path.name))
    return candidates[0]


def rel(path: Path | None, root: Path) -> str:
    return path.relative_to(root).as_posix() if path is not None else ""


def scene_export_status(key: str, bsp: Path | None, source_hash: str, project_root: Path) -> str:
    if bsp is None:
        return "not_exported"
    report = project_root / ".local" / "exports" / key.casefold() / "report.json"
    if not report.is_file():
        return "not_exported"
    try:
        data = json.loads(report.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return "stale_or_invalid_local_export"
    if data.get("source_sha256") != source_hash:
        return "stale_or_invalid_local_export"
    if data.get("format") != "fdny-gbsp-v15-scene-v2":
        return "stale_or_invalid_local_export"
    required = {"geometry.json", "world_mesh.json", "collision.json", "entities.json"}
    if not required.issubset(set(data.get("outputs", []))):
        return "partial_local_export"
    return "typed_scene_export_local"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", type=Path, default=Path("inputs/original/Data"))
    parser.add_argument("--output", type=Path, default=Path("tracker/level_map_inventory.csv"))
    args = parser.parse_args()
    data_root = args.data.resolve()
    project_root = Path(__file__).resolve().parents[1]
    catalog_path = data_root / "ResDefs" / "thegame.lev"
    map_dir = data_root / "Maps"
    if not catalog_path.is_file() or not map_dir.is_dir():
        parser.error(f"expected thegame.lev and Maps/ under {data_root}")

    catalog = parse_catalog(catalog_path)
    catalog_keys = {record["level_key"].casefold() for record in catalog}
    bsp_files = sorted(map_dir.glob("*.bsp"), key=lambda path: path.name.casefold())
    rows: list[dict[str, str]] = []
    fields = [
        "catalog_order", "level_key", "title", "mode", "time_minutes", "rrf",
        "briefing_file", "briefing_sha256", "bsp_file", "bsp_sha256",
        "same_key_evs_file", "same_key_evs_sha256", "map_relation",
        "bsp_entity_records", "authored_entities", "fire_rescue_count",
        "victim_rescue_count", "bounds_trigger_count", "entity_class_counts",
        "scene_export_status", "notes",
    ]

    for ordinal, record in enumerate(catalog, start=1):
        key = record["level_key"]
        briefing = file_lookup(map_dir, key, ".brf")
        bsp = file_lookup(map_dir, key, ".bsp")
        evs = file_lookup(map_dir, key, ".evs")
        bsp_digest = sha256(bsp)
        scene_status = scene_export_status(key, bsp, bsp_digest, project_root)
        classes = Counter()
        if bsp:
            _, entities = read_entities(bsp)
            classes.update(next((value for name, value in pairs if name == "classname"), "<none>") for pairs in entities)
        mode = "hose" if record.get("rrf", "").casefold() == "hose.rrf" else "rescue"
        notes: list[str] = []
        if bsp and bsp.name != f"{key}.bsp":
            notes.append("catalog path spelling differs from disk filename; add a case-insensitive alias")
        if not evs:
            notes.append("no same-key EVS file; resolve script/resource association")
        if not bsp:
            notes.append("no same-key BSP file found")
        rows.append({
            "catalog_order": str(ordinal),
            "level_key": key,
            "title": record.get("title", ""),
            "mode": mode,
            "time_minutes": record.get("time", ""),
            "rrf": record.get("rrf", ""),
            "briefing_file": rel(briefing, data_root),
            "briefing_sha256": sha256(briefing),
            "bsp_file": rel(bsp, data_root),
            "bsp_sha256": bsp_digest,
            "same_key_evs_file": rel(evs, data_root),
            "same_key_evs_sha256": sha256(evs),
            "map_relation": "catalogued" if bsp else "catalogued_without_bsp",
            "bsp_entity_records": str(sum(classes.values())) if bsp else "",
            "authored_entities": str(sum(classes.values()) - classes.get("%typedef%", 0)) if bsp else "",
            "fire_rescue_count": str(classes.get("RCS_FireRescue", 0)) if bsp else "",
            "victim_rescue_count": str(classes.get("RCS_VictimRescue", 0)) if bsp else "",
            "bounds_trigger_count": str(classes.get("RCS_BoundsTrigger", 0)) if bsp else "",
            "entity_class_counts": ";".join(f"{name}={count}" for name, count in sorted(classes.items()) if name.startswith("RCS_")) if bsp else "",
            "scene_export_status": scene_status,
            "notes": "; ".join(notes + (["typed scene bundle is local-only and has not been loaded by the port"] if scene_status == "typed_scene_export_local" else [])),
        })

    for bsp in bsp_files:
        if bsp.stem.casefold() in catalog_keys:
            continue
        evs = file_lookup(map_dir, bsp.stem, ".evs")
        bsp_digest = sha256(bsp)
        scene_status = scene_export_status(bsp.stem, bsp, bsp_digest, project_root)
        _, entities = read_entities(bsp)
        classes = Counter(next((value for name, value in pairs if name == "classname"), "<none>") for pairs in entities)
        rows.append({
            "catalog_order": "",
            "level_key": bsp.stem,
            "title": "",
            "mode": "",
            "time_minutes": "",
            "rrf": "",
            "briefing_file": "",
            "briefing_sha256": "",
            "bsp_file": rel(bsp, data_root),
            "bsp_sha256": bsp_digest,
            "same_key_evs_file": rel(evs, data_root),
            "same_key_evs_sha256": sha256(evs),
            "map_relation": "BSP_not_in_level_catalog",
            "bsp_entity_records": str(sum(classes.values())),
            "authored_entities": str(sum(classes.values()) - classes.get("%typedef%", 0)),
            "fire_rescue_count": str(classes.get("RCS_FireRescue", 0)),
            "victim_rescue_count": str(classes.get("RCS_VictimRescue", 0)),
            "bounds_trigger_count": str(classes.get("RCS_BoundsTrigger", 0)),
            "entity_class_counts": ";".join(f"{name}={count}" for name, count in sorted(classes.items()) if name.startswith("RCS_")),
            "scene_export_status": scene_status,
            "notes": "determine whether this is a variant, shared map, or unused asset",
        })

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    print(f"Wrote {len(rows)} rows to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
