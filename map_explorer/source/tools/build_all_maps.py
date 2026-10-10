#!/usr/bin/env python3
from __future__ import annotations

import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAP_DIR = ROOT / "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps"
EXTRACTED = ROOT / "extracted"
VIEWER_DATA = ROOT / "viewer/data"

COMMANDS = []

def run(cmd):
    print("$", " ".join(str(c) for c in cmd), flush=True)
    subprocess.run([str(c) for c in cmd], cwd=ROOT, check=True)


def display_name(stem: str) -> str:
    return stem.replace("_", " ").title().replace("Oil Rig A", "Oil Rig A").replace("City H", "City H")


def main():
    bsps = sorted(MAP_DIR.glob("*.bsp"), key=lambda p: p.stem.lower())
    manifest = []
    VIEWER_DATA.mkdir(parents=True, exist_ok=True)
    for bsp in bsps:
        stem = bsp.stem
        out_dir = EXTRACTED / stem
        viewer_key = stem.lower()
        viewer_out = VIEWER_DATA / f"{viewer_key}.json"
        name = display_name(stem)
        print(f"\n=== {stem} ===", flush=True)
        run(["python3", "tools/extract_gbsp.py", bsp, "--out", out_dir])
        run(["python3", "tools/render_annotated_maps.py", name, bsp, out_dir])
        run(["python3", "tools/render_simple_maps.py", name, bsp, out_dir])
        run(["python3", "tools/render_map_table_pdf.py", name, bsp, out_dir / "annotated_entities.csv", out_dir / f"{stem}_topdown_stage_clear_with_table.pdf"])
        run(["python3", "tools/render_map_table_pdf.py", name, bsp, out_dir / "annotated_entities.csv", out_dir / f"{stem}_topdown_annotated_with_table.pdf", "--mode", "full"])
        run(["python3", "tools/export_viewer_data.py", name, bsp, viewer_out])
        manifest.append({"key": viewer_key, "stem": stem, "name": name, "bsp": str(bsp.relative_to(ROOT)), "out_dir": str(out_dir.relative_to(ROOT)), "viewer_data": str(viewer_out.relative_to(ROOT))})
    (VIEWER_DATA / "manifest.json").write_text(json.dumps(manifest, indent=2), encoding="utf-8")
    print(f"\nWrote {VIEWER_DATA / 'manifest.json'} with {len(manifest)} maps")

if __name__ == "__main__":
    main()
