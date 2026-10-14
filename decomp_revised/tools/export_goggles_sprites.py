#!/usr/bin/env python3
"""Copy the retail goggles alpha textures into the local actor asset bundle."""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPRITES = ("goggtop.bmp", "goggbott.bmp", "heatpulse.bmp", "heatpulse_a.bmp")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, default=ROOT / "inputs/original")
    parser.add_argument("--output", type=Path, default=ROOT / ".local/actor_exports/goggles")
    args = parser.parse_args()

    source_root = args.inputs / "Data/sprites"
    args.output.mkdir(parents=True, exist_ok=True)
    rows = []
    for name in SPRITES:
        source = source_root / name
        if not source.is_file():
            matches = [path for path in source_root.iterdir() if path.name.casefold() == name.casefold()]
            if len(matches) != 1:
                raise FileNotFoundError(f"Cannot resolve retail goggles sprite {name} under {source_root}")
            source = matches[0]
        raw = source.read_bytes()
        shutil.copyfile(source, args.output / name)
        rows.append({
            "name": name,
            "source": source.relative_to(ROOT).as_posix() if source.is_relative_to(ROOT) else source.name,
            "sha256": hashlib.sha256(raw).hexdigest(),
            "bytes": len(raw),
        })

    manifest = {
        "format": "fdny-local-goggles-sprite-export-v1",
        "resourceIds": {"goggtop.bmp": 300, "goggbott.bmp": 301,
                        "heatpulse.bmp": 703, "heatpulse_a.bmp": 703},
        "resources": rows,
        "presentationLimit": "retail goggles alpha textures copied verbatim; runtime tint and exact draw activation remain under review",
    }
    (args.output / "goggles_manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Copied {len(rows)} retail goggles textures to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
