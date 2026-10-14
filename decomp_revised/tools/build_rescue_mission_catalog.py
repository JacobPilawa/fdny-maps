#!/usr/bin/env python3
"""Build the rescue-only chooser manifest from retail thegame.lev metadata."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path


def scalar(value: str) -> str:
    value = value.strip()
    if len(value) >= 2 and value[0] == value[-1] == '"':
        return value[1:-1]
    return value


def parse_catalog(path: Path) -> tuple[list[dict], str]:
    raw = path.read_bytes()
    text = raw.decode("cp1252")
    records: list[dict] = []
    current: dict[str, str] | None = None
    field_pattern = re.compile(r"^\s*([A-Za-z][A-Za-z0-9_]*):\s*(.*?)\s*$")

    def finish() -> None:
        if current is None:
            return
        key = current.get("level", "").strip()
        if not key:
            raise ValueError("thegame.lev contains a level record without a key")
        record: dict = {
            "catalog_ordinal": len(records),
            "key": key.casefold(),
            "title": current.get("title", key),
            "rrf": current.get("rrf", ""),
        }
        if "time" in current:
            record["time"] = int(current["time"])
        if "mapcoords" in current:
            coordinates = [part.strip() for part in current["mapcoords"].split(",")]
            if len(coordinates) != 2:
                raise ValueError(f"Invalid mapcoords for {key}: {current['mapcoords']}")
            record["mapcoords"] = [int(value) for value in coordinates]
        records.append(record)

    for line in text.splitlines():
        stripped = line.strip()
        if not stripped or stripped.startswith(";"):
            continue
        match = field_pattern.match(line)
        if not match:
            continue
        name, value = match.groups()
        name = name.casefold()
        value = scalar(value)
        if name == "level":
            finish()
            current = {name: value}
        elif current is not None:
            current[name] = value
    finish()

    if not records:
        raise ValueError(f"No level records found in {path}")
    return records, hashlib.sha256(raw).hexdigest()


def build(level_file: Path, scenes_dir: Path) -> dict:
    records, catalog_hash = parse_catalog(level_file)
    rescue = [record for record in records if record["rrf"].casefold() == "rescue.rrf"]
    if len(rescue) != 8:
        raise ValueError(f"Expected eight retail rescue catalog rows, found {len(rescue)}")
    seen: set[str] = set()
    for record in rescue:
        key = record["key"]
        if key in seen:
            raise ValueError(f"Duplicate rescue catalog key: {key}")
        seen.add(key)
        if not re.fullmatch(r"[a-z0-9_]+", key):
            raise ValueError(f"Unsafe rescue level key: {key}")
        package = scenes_dir / key
        if not package.is_dir():
            raise FileNotFoundError(f"Missing bundled scene package for retail rescue row {key}: {package}")
        for required in ("report.json", "entities.json", "collision.json", "world_mesh.json"):
            if not (package / required).is_file():
                raise FileNotFoundError(f"Incomplete scene package for {key}: {package / required}")
    return {
        "format": "fdny-rescue-mission-catalog-v1",
        "source_file": "Data/ResDefs/thegame.lev",
        "source_sha256": catalog_hash,
        "entry_count": len(rescue),
        "entries": rescue,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--level-file", type=Path, required=True)
    parser.add_argument("--scenes-dir", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    manifest = build(args.level_file, args.scenes_dir)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {manifest['entry_count']} rescue choices from {manifest['source_file']} "
          f"(SHA-256 {manifest['source_sha256']})")
    for record in manifest["entries"]:
        print(f"  ordinal {record['catalog_ordinal']}: {record['key']} — {record['title']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
