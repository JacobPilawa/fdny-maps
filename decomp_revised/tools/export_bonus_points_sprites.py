#!/usr/bin/env python3
"""Bundle the retail Bonus Points sprite and bitmap font used by Player status."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
RESOURCE_PAIRS = {
    502: ("tools.bmp", "rescbon_a.bmp"),
    64000: ("confont.bmp", "confont_a.bmp"),
}


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, default=ROOT / "inputs/original")
    parser.add_argument("--output", type=Path,
                        default=ROOT / ".local/actor_exports/bonus-points")
    args = parser.parse_args()

    definitions = args.inputs / "Data/ResDefs/rescue.rrf"
    source_root = args.inputs / "Data/sprites"
    if not definitions.is_file():
        raise SystemExit(f"Missing retail rescue resource table: {definitions}")
    table = definitions.read_text(encoding="utf-8", errors="replace")
    for resource_id, expected_names in RESOURCE_PAIRS.items():
        match = re.search(
            rf'^\s*ROC_ALPHA_TEXTURE\s+{resource_id}\s+"([^"]+)"\s+"([^"]+)"\s*;',
            table, re.IGNORECASE | re.MULTILINE)
        if not match:
            raise SystemExit(f"Cannot find rescue.rrf alpha pair for resource {resource_id}")
        observed = tuple(Path(value.replace("\\", "/")).name for value in match.groups())
        if tuple(name.casefold() for name in observed) != tuple(
                name.casefold() for name in expected_names):
            raise SystemExit(
                f"rescue.rrf resource {resource_id} changed: " + " / ".join(match.groups()))

    args.output.mkdir(parents=True, exist_ok=True)
    records = []
    for resource_id, names in RESOURCE_PAIRS.items():
        for name in names:
            source = source_root / name
            if not source.is_file():
                raise SystemExit(f"Missing retail Bonus Points bitmap: {source}")
            destination = args.output / name
            shutil.copyfile(source, destination)
            records.append({
                "resource_id": resource_id,
                "file": name,
                "source": source.relative_to(ROOT).as_posix()
                if source.is_relative_to(ROOT) else source.name,
                "source_sha256": sha256(source),
                "bundled_sha256": sha256(destination),
                "bytes": source.stat().st_size,
            })

    manifest = {
        "format": "fdny-bonus-points-hud-sprite-bundle-v1",
        "resource_definition": definitions.relative_to(ROOT).as_posix()
        if definitions.is_relative_to(ROOT) else definitions.name,
        "resource_definition_sha256": sha256(definitions),
        "resource_pairs": {
            str(resource_id): list(names) for resource_id, names in RESOURCE_PAIRS.items()
        },
        "host_conversion_note": (
            "Retail color and alpha bitmaps are copied byte-for-byte. Host RGBA conversion, "
            "linear filtering, alpha channel choice, and widget placement remain provisional."
        ),
        "files": records,
    }
    (args.output / "manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Exported retail Bonus Points sprite and font assets to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
