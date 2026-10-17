#!/usr/bin/env python3
"""Copy the catalog-selected retail MP3 into a scene package with provenance."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
from pathlib import Path

from build_rescue_mission_catalog import parse_catalog


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def find_case_insensitive(directory: Path, name: str) -> Path:
    matches = [entry for entry in directory.iterdir()
               if entry.is_file() and entry.name.casefold() == name.casefold()]
    if len(matches) != 1:
        raise FileNotFoundError(f"Expected one {name} under {directory}, found {len(matches)}")
    return matches[0]


def export(level_file: Path, original_root: Path, scene_package: Path,
           map_key: str) -> dict | None:
    records, catalog_hash = parse_catalog(level_file)
    match = next((row for row in records if row["key"] == map_key.casefold()), None)
    if match is None or not match.get("music"):
        (scene_package / "music.json").unlink(missing_ok=True)
        return None

    track_name = match["music"].strip()
    if Path(track_name).name != track_name or not re.fullmatch(r"[A-Za-z0-9_.-]+\.mp3", track_name, re.I):
        raise ValueError(f"Unsafe catalog music resource for {map_key}: {track_name!r}")

    music_root = original_root / "Data" / "Snd" / "Music"
    source = find_case_insensitive(music_root, track_name)
    source_bytes = source.read_bytes()
    target_directory = scene_package / "music"
    target_directory.mkdir(parents=True, exist_ok=True)
    target = target_directory / track_name
    shutil.copyfile(source, target)
    manifest = {
        "format": "fdny-level-music-v1",
        "map_key": match["key"],
        "track": track_name,
        "file": f"music/{track_name}",
        "loop": True,
        "catalog_source_file": "Data/ResDefs/thegame.lev",
        "catalog_source_sha256": catalog_hash,
        "catalog_ordinal": match["catalog_ordinal"],
        "source_file": source.resolve().relative_to(original_root.resolve()).as_posix(),
        "source_sha256": sha256(source_bytes),
        "bundled_sha256": sha256(target.read_bytes()),
    }
    (scene_package / "music.json").write_text(json.dumps(manifest, indent=2) + "\n",
                                               encoding="utf-8")
    return manifest


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--map-key", required=True)
    parser.add_argument("--level-file", type=Path, required=True)
    parser.add_argument("--original-root", type=Path, required=True)
    parser.add_argument("--scene-package", type=Path, required=True)
    args = parser.parse_args()
    manifest = export(args.level_file, args.original_root, args.scene_package, args.map_key)
    if manifest is None:
        print(f"No catalog music track for {args.map_key}; scene package left without music.")
        return 0
    print(f"Bundled {manifest['track']} for {args.map_key} "
          f"(SHA-256 {manifest['source_sha256']})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
