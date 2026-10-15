#!/usr/bin/env python3
"""Copy retail oxygen HUD and visor-effect color/alpha textures into local assets."""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPRITES = (
    "oxygauge.bmp", "oxygauge_a.bmp", "oxyneedle.bmp", "oxyneedle_a.bmp",
    "shield.bmp", "shield_a.bmp",
)


def find_casefolded_child(parent: Path, name: str) -> Path:
    direct = parent / name
    if direct.is_dir():
        return direct
    if parent.is_dir():
        matches = [path for path in parent.iterdir() if path.name.casefold() == name.casefold()]
        if len(matches) == 1 and matches[0].is_dir():
            return matches[0]
    raise FileNotFoundError(f"Cannot resolve {name} below {parent}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, default=ROOT / "inputs/original")
    parser.add_argument("--output", type=Path, default=ROOT / ".local/actor_exports/oxygen")
    args = parser.parse_args()

    source_root = find_casefolded_child(find_casefolded_child(args.inputs, "Data"), "Sprites")
    args.output.mkdir(parents=True, exist_ok=True)
    rows = []
    for name in SPRITES:
        source = source_root / name
        if not source.is_file():
            matches = [path for path in source_root.iterdir() if path.name.casefold() == name.casefold()]
            if len(matches) != 1:
                raise FileNotFoundError(f"Cannot resolve retail oxygen sprite {name} under {source_root}")
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
        "format": "fdny-local-oxygen-sprite-export-v1",
        "resourceIds": {
            "12": ["oxygauge.bmp", "oxygauge_a.bmp"],
            "13": ["oxyneedle.bmp", "oxyneedle_a.bmp"],
            "702": ["shield.bmp", "shield_a.bmp"],
        },
        "resources": rows,
        "presentationLimit": "retail color/alpha pairs copied verbatim; oxygen visor effect geometry and timing are source-mapped, while filtering, blend state, and in-game parity remain under review",
    }
    (args.output / "oxygen_manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Copied retail oxygen HUD texture pairs to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
