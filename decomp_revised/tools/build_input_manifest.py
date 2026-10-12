#!/usr/bin/env python3
"""Write a stable SHA-256 manifest for copied original game inputs."""

from __future__ import annotations

import csv
import hashlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
INPUT_ROOT = ROOT / "inputs" / "original"
OUT = INPUT_ROOT / "manifest.csv"


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> None:
    files = sorted(
        (path for path in INPUT_ROOT.rglob("*") if path.is_file() and path != OUT),
        key=lambda path: path.relative_to(ROOT).as_posix().casefold(),
    )
    with OUT.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.writer(stream)
        writer.writerow(("path", "size_bytes", "sha256"))
        for path in files:
            writer.writerow((path.relative_to(ROOT).as_posix(), path.stat().st_size, sha256(path)))
    print(f"Wrote {len(files)} records to {OUT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
