#!/usr/bin/env python3
"""Run the C++ SceneData loader in inspect mode and write its per-map coverage CSV."""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
from datetime import datetime, timezone
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_VIEWER = ROOT / "port/spikes/training-native-prototype/.local/build-scene-viewer/fdny-scene-viewer"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--viewer", type=Path, default=DEFAULT_VIEWER)
    parser.add_argument("--exports", type=Path, default=ROOT / ".local/exports")
    parser.add_argument("--output", type=Path, default=ROOT / "tracker/scene_load_coverage.csv")
    args = parser.parse_args()
    viewer = args.viewer.resolve()
    exports = args.exports.resolve()
    if not viewer.is_file():
        parser.error(f"scene viewer binary not found: {viewer}; build the fdny-scene-viewer target first")
    if not exports.is_dir():
        parser.error(f"scene export directory not found: {exports}")

    fields = [
        "checked_utc", "scene_key", "source_file", "source_sha256", "entity_records",
        "mesh_groups", "collision_models", "collision_nodes", "collision_leaves",
        "bsp_faces", "render_triangles", "unresolved_mesh_textures", "player_spawn",
        "cpp_loader_status", "coverage_note",
    ]
    checked = datetime.now(timezone.utc).isoformat(timespec="seconds").replace("+00:00", "Z")
    rows = []
    for package in sorted((path for path in exports.iterdir() if path.is_dir()), key=lambda path: path.name.casefold()):
        result = subprocess.run(
            [str(viewer), str(package), "--inspect"],
            check=True, capture_output=True, text=True,
        )
        output = result.stdout
        scene = re.search(r"^Scene: (.*?) \((.*?)\)$", output, re.M)
        digest = re.search(r"^BSP SHA-256: ([0-9a-f]{64})$", output, re.M)
        counts = re.search(
            r"^Entities: (\d+), mesh groups: (\d+), collision models: (\d+), nodes: (\d+), leaves: (\d+)$",
            output, re.M,
        )
        rendering = re.search(
            r"^BSP faces: (\d+), render triangles: (\d+), unresolved mesh textures: (\d+)$",
            output, re.M,
        )
        spawn = re.search(r"^Spawn: (.+)$", output, re.M)
        if not all((scene, digest, counts, rendering, spawn)):
            raise RuntimeError(f"Scene viewer returned an unexpected inventory for {package.name}:\n{output}")
        rows.append({
            "checked_utc": checked,
            "scene_key": scene.group(1),
            "source_file": scene.group(2),
            "source_sha256": digest.group(1),
            "entity_records": counts.group(1),
            "mesh_groups": counts.group(2),
            "collision_models": counts.group(3),
            "collision_nodes": counts.group(4),
            "collision_leaves": counts.group(5),
            "bsp_faces": rendering.group(1),
            "render_triangles": rendering.group(2),
            "unresolved_mesh_textures": rendering.group(3),
            "player_spawn": spawn.group(1),
            "cpp_loader_status": "loaded",
            "coverage_note": "SceneData package load and case-aware texture-path resolution; inspect only, no window or gameplay parity",
        })

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    print(f"Wrote {len(rows)} C++ scene-package load records to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
