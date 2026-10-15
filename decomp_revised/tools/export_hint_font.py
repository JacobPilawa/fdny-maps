#!/usr/bin/env python3
"""Copy the retail Player hint font atlas and alpha map into local assets."""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
RESOURCES = ("font1.bmp", "font1_a.bmp")
GLYPH_ORDER = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+|`-=\\{}:\"<>?[];',./"


def resolve(source_root: Path, name: str) -> Path:
    direct = source_root / name
    if direct.is_file():
        return direct
    matches = [path for path in source_root.iterdir() if path.name.casefold() == name.casefold()]
    if len(matches) != 1:
        raise FileNotFoundError(f"Cannot resolve retail hint font {name} under {source_root}")
    return matches[0]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, default=ROOT / "inputs/original")
    parser.add_argument("--output", type=Path, default=ROOT / ".local/actor_exports/hints")
    args = parser.parse_args()

    source_root = args.inputs / "Data/sprites"
    args.output.mkdir(parents=True, exist_ok=True)
    records = []
    for name in RESOURCES:
        source = resolve(source_root, name)
        raw = source.read_bytes()
        shutil.copyfile(source, args.output / name)
        records.append({
            "name": name,
            "source": source.relative_to(ROOT).as_posix() if source.is_relative_to(ROOT) else source.name,
            "sha256": hashlib.sha256(raw).hexdigest(),
            "bytes": len(raw),
        })

    manifest = {
        "format": "fdny-local-hint-font-export-v1",
        "resourceId": 1020,
        "sourceExecutableSha256": "4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e",
        "cell": {"width": 16, "height": 24},
        "glyphOrder": GLYPH_ORDER,
        "resources": records,
        "presentationLimit": "resource and glyph grid are source-mapped; hint layout modes, tint layers, and overflow remain provisional",
    }
    (args.output / "hint_font_manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Copied retail hint font resources to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
