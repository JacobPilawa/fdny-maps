#!/usr/bin/env python3
"""Inventory authored rescue and stage-exit BoundsTriggers in every BSP."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from gbsp_v15 import parse_entities, read_gbsp  # noqa: E402

EXIT_WORDS = ("tell player escape", "tell player finalexit", "finalexitloadmap")


def write_csv(path: Path, rows: list[dict], fields: list[str]) -> None:
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--maps", type=Path, default=ROOT / "inputs/original/Data/Maps")
    parser.add_argument("--out", type=Path, default=ROOT / "tracker")
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)

    rescue_scenarios = set()
    rescue_coverage = ROOT / "tracker/rescue_mission_coverage.csv"
    with rescue_coverage.open(newline="") as stream:
        for row in csv.DictReader(stream):
            if row["coverage_class"] in {"training_catalog_required", "campaign_rescue_required"}:
                rescue_scenarios.add(row["scenario_key"].lower())

    expected_hashes = {}
    inventory_path = ROOT / "tracker/scene_load_coverage.csv"
    if inventory_path.exists():
        with inventory_path.open(newline="") as stream:
            for row in csv.DictReader(stream):
                expected_hashes[row["scene_key"].lower()] = row["source_sha256"]

    trigger_rows: list[dict] = []
    map_rows: list[dict] = []
    for bsp_path in sorted(args.maps.glob("*.bsp"), key=lambda path: path.stem.lower()):
        raw, _, payloads = read_gbsp(bsp_path)
        map_key = bsp_path.stem.lower()
        digest = hashlib.sha256(raw).hexdigest()
        expected = expected_hashes.get(map_key)
        if expected and expected != digest:
            raise SystemExit(f"Hash mismatch for {bsp_path.name}: {digest} != {expected}")

        entities = parse_entities(payloads[16])
        classname_counts = Counter(
            dict(epairs).get("classname", "") for epairs in entities
        )
        exact_player_epair_count = sum(
            value.casefold() == "player"
            for epairs in entities
            for _, value in epairs
        )
        model_entities: dict[str, list[dict]] = defaultdict(list)
        for model_entity_index, model_epairs in enumerate(entities):
            model_props = {key: value for key, value in model_epairs}
            if model_props.get("classname") == "%Model%":
                model_entities[model_props.get("%name%", "")].append({
                    "entity_index": model_entity_index,
                    "model_index": model_props.get("Model", ""),
                    "origin": model_props.get("origin", ""),
                })
        selected = []
        for entity_index, epairs in enumerate(entities):
            props = {key: value for key, value in epairs}
            if props.get("classname") != "RCS_BoundsTrigger":
                continue
            scripts = [(key, value) for key, value in epairs
                       if key.startswith("S_Script")
                       and any(word in value.lower() for word in EXIT_WORDS)]
            if not scripts:
                continue

            text = " ".join(value.lower() for _, value in scripts)
            actions = []
            if "tell player escape" in text:
                actions.append("rescue_exit")
            if re.search(r"tell player\s*\(?finalexit\)?(?:\s|$)", text):
                actions.append("final_exit_check")
            destinations = sorted(set(re.findall(r"finalexitloadmap\s+([a-z0-9_]+)", text)))
            if destinations:
                actions.append("level_transition")
            model_joins = model_entities.get(props.get("C_Model", ""), [])

            row = {
                "map_key": map_key,
                "catalog_rescue_scenario": "yes" if map_key in rescue_scenarios else "no",
                "bsp_sha256": digest,
                "entity_index": entity_index,
                "classname": props.get("classname", ""),
                "entity_name": props.get("%name%", ""),
                "actor_name": props.get("A_Name", ""),
                "collision_model": props.get("C_Model", ""),
                "collision_model_joins_json": json.dumps(model_joins, separators=(",", ":")),
                "authored_location": props.get("ZZ_location", ""),
                "reach": props.get("E_Reach", ""),
                "actions_json": json.dumps(actions, separators=(",", ":")),
                "transition_targets_json": json.dumps(destinations, separators=(",", ":")),
                "script_slots_json": json.dumps(scripts, separators=(",", ":")),
                "ordered_epairs_json": json.dumps(epairs, separators=(",", ":")),
            }
            trigger_rows.append(row)
            selected.append(row)

        action_counts = Counter(
            action
            for row in selected
            for action in json.loads(row["actions_json"])
        )
        targets = sorted({
            target
            for row in selected
            for target in json.loads(row["transition_targets_json"])
        })
        map_rows.append({
            "map_key": map_key,
            "catalog_rescue_scenario": "yes" if map_key in rescue_scenarios else "no",
            "bsp_sha256": digest,
            "entity_count": len(entities),
            "player_rescue_entity_count": classname_counts["RCS_PlayerRescue"],
            "player_hose_entity_count": classname_counts["RCS_PlayerHose"],
            "exact_player_epair_value_count": exact_player_epair_count,
            "exit_trigger_count": len(selected),
            "rescue_exit_binding_count": action_counts["rescue_exit"],
            "final_exit_check_binding_count": action_counts["final_exit_check"],
            "level_transition_binding_count": action_counts["level_transition"],
            "collision_model_join_count": sum(bool(json.loads(row["collision_model_joins_json"])) for row in selected),
            "collision_model_unresolved_count": sum(not json.loads(row["collision_model_joins_json"]) for row in selected),
            "transition_targets_json": json.dumps(targets, separators=(",", ":")),
        })

    if len(map_rows) != 17:
        raise SystemExit(f"Expected 17 BSP maps, found {len(map_rows)}")
    if expected_hashes and set(expected_hashes) != {row["map_key"] for row in map_rows}:
        raise SystemExit("The BSP set does not match level_map_inventory.csv")

    write_csv(args.out / "rescue_exit_trigger_inventory.csv", trigger_rows, list(trigger_rows[0]))
    write_csv(args.out / "rescue_exit_trigger_map_coverage.csv", map_rows, list(map_rows[0]))
    action_totals = Counter(
        action
        for row in trigger_rows
        for action in json.loads(row["actions_json"])
    )
    no_trigger = [row["map_key"] for row in map_rows if row["exit_trigger_count"] == 0]
    model_join_total = sum(row["collision_model_join_count"] for row in map_rows)
    model_unresolved_total = sum(row["collision_model_unresolved_count"] for row in map_rows)
    report = [
        "# Authored rescue and stage-exit trigger inventory",
        "",
        "Generated from all 17 copied retail GBSP v15 BSPs. Input hashes are checked against `scene_load_coverage.csv`.",
        "This is authored-data evidence: it maps BoundsTrigger scripts and model names, but does not prove runtime trigger admission, overlap cadence, command completion, or mission parity.",
        "",
        f"- BSP files: {len(map_rows)}",
        f"- BoundsTriggers with escape/final-exit script bindings: {len(trigger_rows)}",
        f"- Rescue-exit script bindings: {action_totals['rescue_exit']}",
        f"- Final-exit check bindings: {action_totals['final_exit_check']}",
        f"- Stage-transition bindings: {action_totals['level_transition']}",
        f"- Triggers joined to named `%Model%` entities: {model_join_total}",
        f"- Trigger `C_Model` references without a named model join: {model_unresolved_total}",
        f"- BSPs with no matching exit script: {', '.join(no_trigger)}",
        f"- `RCS_PlayerRescue` entities: {sum(row['player_rescue_entity_count'] for row in map_rows)}",
        f"- `RCS_PlayerHose` entities: {sum(row['player_hose_entity_count'] for row in map_rows)}",
        f"- Exact `Player` epair values: {sum(row['exact_player_epair_value_count'] for row in map_rows)}",
        "",
        f"The CSV flags all {len(rescue_scenarios)} required catalog rescue scenarios from `rescue_mission_coverage.csv`. The asset-level inventory also includes Boat B, Subway B, and Oil Rig B/C/D transition stages.",
        "",
        "| BSP | Rescue scenario | PlayerRescue | PlayerHose | exact Player epairs | Triggers | `escape` | `finalexit` | map transition | model joins | destinations |",
        "|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|",
    ]
    for row in map_rows:
        report.append(
            f"| `{row['map_key']}` | {row['catalog_rescue_scenario']} | {row['player_rescue_entity_count']} | "
            f"{row['player_hose_entity_count']} | {row['exact_player_epair_value_count']} | {row['exit_trigger_count']} | "
            f"{row['rescue_exit_binding_count']} | {row['final_exit_check_binding_count']} | "
            f"{row['level_transition_binding_count']} | {row['collision_model_join_count']}/{row['exit_trigger_count']} | "
            f"{', '.join(json.loads(row['transition_targets_json'])) or '—'} |"
        )
    report.extend([
        "",
        "The per-entity CSV keeps the original ordered epairs and exact script slots. Each `C_Model` is joined by exact same-BSP name to `%Model%` records, with model index/origin retained; `ZZ_location` and `E_Reach` are preserved separately. This asset join does not prove runtime factory admission or contact timing.",
        "",
        "The executable route is documented in [rescue_completion_and_exit_path.md](evidence/scenario-loading/rescue_completion_and_exit_path.md), with the preserved release/exit component in [the archived decomp](../../decomp/evidence/victim-ai-audit/rescue-deadline-exit-component.md).",
    ])
    (args.out / "rescue_exit_trigger_inventory.md").write_text("\n".join(report) + "\n")
    print(f"Wrote {len(trigger_rows)} exit-trigger records across {len(map_rows)} BSPs to {args.out}")


if __name__ == "__main__":
    main()
