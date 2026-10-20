#!/usr/bin/env python3
"""Bundle PlayerHose resource 3's retail color and alpha bitmaps locally."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EXPECTED_ENTRY = (
    r'ROC_ALPHA_TEXTURE 3 "data\sprites\hdstream.bmp" '
    r'"data\sprites\hdstream_a.bmp"'
)


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
                        default=ROOT / ".local/actor_exports/playerhose-stream")
    args = parser.parse_args()

    definitions = args.inputs / "Data/ResDefs/hose.rrf"
    source_root = args.inputs / "Data/sprites"
    match = re.search(
        r'^\s*ROC_ALPHA_TEXTURE\s+3\s+"([^\"]+)"\s+"([^\"]+)"\s*;',
        definitions.read_text(encoding="utf-8", errors="replace"), re.IGNORECASE | re.MULTILINE)
    if not match:
        raise SystemExit(f"Cannot find the paired resource-3 declaration in {definitions}")

    resource_paths = match.groups()
    normalized = tuple(path.replace("\\", "/").casefold() for path in resource_paths)
    expected = ("data/sprites/hdstream.bmp", "data/sprites/hdstream_a.bmp")
    if normalized != expected:
        raise SystemExit(
            "hose.rrf resource 3 changed; refusing to export an unverified texture pair: " +
            " / ".join(resource_paths))

    names = ("hdstream.bmp", "hdstream_a.bmp")
    sources = [source_root / name for name in names]
    missing = [str(path) for path in [definitions, *sources] if not path.is_file()]
    if missing:
        raise SystemExit("Missing PlayerHose resource-3 input: " + ", ".join(missing))

    args.output.mkdir(parents=True, exist_ok=True)
    records = []
    for name, source in zip(names, sources):
        destination = args.output / name
        shutil.copyfile(source, destination)
        records.append({
            "file": name,
            "source": source.relative_to(ROOT).as_posix()
            if source.is_relative_to(ROOT) else source.name,
            "source_sha256": sha256(source),
            "bundled_sha256": sha256(destination),
            "bytes": source.stat().st_size,
        })

    manifest = {
        "format": "fdny-playerhose-stream-sprite-bundle-v1",
        "resource_id": 3,
        "resource_definition": definitions.relative_to(ROOT).as_posix()
        if definitions.is_relative_to(ROOT) else definitions.name,
        "resource_definition_sha256": sha256(definitions),
        "resource_definition_entry": EXPECTED_ENTRY,
        "mapping_status": (
            "hose.rrf declares the pair for resource 3; the component constructor "
            "resolves numeric ID 3 through the manager lookup"
        ),
        "host_conversion_note": (
            "The color bitmap is 64x64 indexed BMP and its alpha bitmap is 128x64 "
            "24-bit BMP. The host maps alpha by normalized coordinates to the color "
            "bitmap; Genesis sampling/filter behavior for this size mismatch remains "
            "unverified."
        ),
        "sprites": records,
    }
    (args.output / "manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Exported PlayerHose resource-3 textures to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
