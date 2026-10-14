#!/usr/bin/env python3
"""Copy the retail FireRescue sprite bitmaps into the local actor asset bundle."""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
from pathlib import Path


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--project-root", type=Path,
                        default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    project_root = args.project_root.resolve()
    source_root = project_root / "inputs" / "original" / "Data" / "sprites"
    output_root = project_root / ".local" / "actor_exports" / "fire-sprites"
    names = ([f"lflame-{frame:02d}.bmp" for frame in range(19)] +
             [f"fflame{frame:04d}.bmp" for frame in range(19)])

    missing = [name for name in names if not (source_root / name).is_file()]
    if missing:
        raise SystemExit("Missing retail FireRescue sprites: " + ", ".join(missing))

    output_root.mkdir(parents=True, exist_ok=True)
    records = []
    for name in names:
        source = source_root / name
        destination = output_root / name
        shutil.copy2(source, destination)
        records.append({
            "file": name,
            "source": str(source.relative_to(project_root)),
            "sha256": sha256(source),
        })
    manifest = {
        "format": "fdny-fire-sprite-bundle-v1",
        "source_root": str(source_root.relative_to(project_root)),
        "sprites": records,
    }
    (output_root / "manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Exported {len(records)} retail FireRescue sprites to {output_root}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
