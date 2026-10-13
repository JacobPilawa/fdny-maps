#!/usr/bin/env python3
"""Compare rescue.rrf base bitmap stems with exported rescue BSP texture names.

Reads the local copied game input and hash-pinned GBSP export reports. It emits
only metadata and overlap names; copyrighted bitmap/BSP payloads are not copied.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
from pathlib import Path, PureWindowsPath

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_RRF = ROOT / "inputs/original/Data/ResDefs/rescue.rrf"
DEFAULT_EXPORTS = ROOT / ".local/exports"
DEFAULT_OUT = ROOT / "tracker/evidence/runtime/rescue_rrf_bsp_texture_name_audit.csv"
MAPS = ("mbase", "bank", "bar_r", "gas_r", "boat_a", "subway_a", "oil_rig_a")
RECORD = re.compile(
    r'^\s*ROC_ALPHA_TEXTURE\s+(-?\d+)\s+"([^"]+)"(?:\s+"([^"]*)")?\s*;',
    re.IGNORECASE | re.MULTILINE,
)


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rrf", type=Path, default=DEFAULT_RRF)
    parser.add_argument("--exports", type=Path, default=DEFAULT_EXPORTS)
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT)
    args = parser.parse_args()

    raw = args.rrf.read_bytes()
    text = raw.decode("latin1")
    records = RECORD.findall(text)
    if not records:
        raise SystemExit(f"No ROC_ALPHA_TEXTURE records found in {args.rrf}")
    rrf_hash = hashlib.sha256(raw).hexdigest()
    rows = []
    for map_name in MAPS:
        report_path = args.exports / map_name / "report.json"
        geometry_path = args.exports / map_name / "geometry.json"
        report = json.loads(report_path.read_text())
        geometry = json.loads(geometry_path.read_text())
        names = {entry["name"].casefold() for entry in geometry["textures"]}
        overlaps = sorted({
            PureWindowsPath(base_path).stem.casefold()
            for _, base_path, _ in records
            if PureWindowsPath(base_path).stem.casefold() in names
        })
        rows.append({
            "map": map_name,
            "bsp_sha256": report["source_sha256"],
            "bsp_texture_records": len(geometry["textures"]),
            "rescue_rrf_sha256": rrf_hash,
            "alpha_texture_records": len(records),
            "unique_numeric_keys": len({key for key, _, _ in records}),
            "unique_base_paths": len({path.casefold() for _, path, _ in records}),
            "optional_alpha_records": sum(bool(alpha) for _, _, alpha in records),
            "base_name_overlap_count": len(overlaps),
            "overlap_names": ";".join(overlaps),
        })

    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    for row in rows:
        print(f"{row['map']}: {row['bsp_texture_records']} BSP texture records; "
              f"RRF base-name overlaps {row['base_name_overlap_count']}")
    print(f"RRF records: {len(records)}; unique keys: {rows[0]['unique_numeric_keys']}; "
          f"unique base paths: {rows[0]['unique_base_paths']}; "
          f"optional alpha paths: {rows[0]['optional_alpha_records']}")
    print(f"Wrote {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
