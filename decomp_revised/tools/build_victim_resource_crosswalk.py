#!/usr/bin/env python3
"""Join BSP victim types to victims.srf, rescue.rrf, and local ACT files."""
from __future__ import annotations

import argparse
import csv
import json
import re
from collections import Counter, defaultdict
from pathlib import Path


def parse_victim_srf(path: Path) -> dict[str, int]:
    definitions: dict[str, int] = {}
    current: str | None = None
    for raw in path.read_text(errors="replace").splitlines():
        line = raw.strip()
        match = re.match(r"Victim:\s*(.+)$", line, re.IGNORECASE)
        if match:
            current = match.group(1).strip()
            continue
        if current:
            match = re.match(r"Model:\s*(\d+)\s*$", line, re.IGNORECASE)
            if match:
                definitions[current] = int(match.group(1))
    return definitions


def parse_rrf_models(path: Path) -> dict[int, str]:
    text = path.read_text(errors="replace")
    return {
        int(match.group(1)): match.group(2).replace("\\", "/")
        for match in re.finditer(r'ROC_MODEL\s+(\d+)\s+"([^"]+)"\s*;', text, re.IGNORECASE)
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", type=Path, default=Path("inputs/original/Data"))
    parser.add_argument("--inventory", type=Path, default=Path("tracker/level_map_inventory.csv"))
    parser.add_argument("--exports", type=Path, default=Path(".local/exports"))
    parser.add_argument("--output", type=Path, default=Path("tracker/victim_resource_crosswalk.csv"))
    args = parser.parse_args()

    data_root = args.data.resolve()
    definitions = parse_victim_srf(data_root / "ResDefs/victims.srf")
    default_type = next(iter(definitions), None)
    model_paths = parse_rrf_models(data_root / "ResDefs/rescue.rrf")
    files_by_casefold = {
        path.relative_to(data_root).as_posix().casefold(): path
        for path in data_root.rglob("*") if path.is_file()
    }
    observed: dict[str, Counter[str]] = defaultdict(Counter)
    totals: Counter[str] = Counter()

    with args.inventory.open(newline="", encoding="utf-8") as stream:
        maps = list(csv.DictReader(stream))
    for row in maps:
        key = row["level_key"]
        export_dir = args.exports / key.casefold()
        report = json.loads((export_dir / "report.json").read_text(encoding="utf-8"))
        if report.get("format") != "fdny-gbsp-v15-scene-v2" or report.get("source_sha256") != row.get("bsp_sha256"):
            parser.error(f"{key}: scene export is missing or stale")
        entities = json.loads((export_dir / "entities.json").read_text(encoding="utf-8"))
        for entity in entities:
            if entity.get("class") != "RCS_VictimRescue":
                continue
            victim_type = entity.get("properties", {}).get("D_Type", "<missing>")
            observed[victim_type][key] += 1
            totals[victim_type] += 1

    all_types = sorted(set(definitions) | set(observed), key=str.casefold)
    columns = [
        "victim_type", "resolved_victim_type", "map_entity_count", "map_file_count", "map_keys",
        "victims_srf_model", "runtime_actor_model_ids", "runtime_actor_model_selection",
        "rescue_rrf_act_path", "act_file_exists",
        "mapping_status",
    ]
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=columns, lineterminator="\n")
        writer.writeheader()
        for victim_type in all_types:
            resolved_type = victim_type if victim_type in definitions else ""
            model_id = definitions.get(victim_type)
            if model_id is None and victim_type in observed and default_type is not None:
                resolved_type = default_type
                model_id = definitions[default_type]
            runtime_model_ids = str(model_id) if model_id is not None else ""
            runtime_model_selection = "fixed" if model_id is not None else "unresolved"
            if model_id == 14:
                runtime_model_ids = "11;12;13"
                runtime_model_selection = "11 + random() % 3 (FUN_0045B690)"
            act_path = model_paths.get(model_id, "") if model_id is not None else ""
            local_ref = act_path.split("/", 1)[1] if act_path.casefold().startswith("data/") else act_path
            local_act = files_by_casefold.get(local_ref.casefold()) if local_ref else None
            if victim_type in observed and victim_type not in definitions and resolved_type:
                status = "runtime_fallback_first_srf_entry"
            elif victim_type in observed and model_id is None:
                status = "unresolved_in_victims_srf"
            elif model_id is None:
                status = "definition_without_model"
            elif not act_path:
                status = "model_missing_from_rescue_rrf"
            elif local_act is None:
                status = "act_file_missing"
            elif victim_type in observed:
                status = "mapped_by_srf_and_rrf"
            else:
                status = "defined_not_used_by_bsp_victim_entity"
            per_map = observed.get(victim_type, Counter())
            writer.writerow({
                "victim_type": victim_type,
                "resolved_victim_type": resolved_type,
                "map_entity_count": totals.get(victim_type, 0),
                "map_file_count": len(per_map),
                "map_keys": ";".join(sorted(per_map, key=str.casefold)),
                "victims_srf_model": model_id if model_id is not None else "",
                "runtime_actor_model_ids": runtime_model_ids,
                "runtime_actor_model_selection": runtime_model_selection,
                "rescue_rrf_act_path": act_path,
                "act_file_exists": "yes" if local_act else "no" if act_path else "",
                "mapping_status": status,
            })
    print(f"Wrote {len(all_types)} actor-resource rows across {len(maps)} map files to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
