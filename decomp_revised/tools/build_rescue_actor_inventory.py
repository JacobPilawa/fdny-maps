#!/usr/bin/env python3
"""Export hash-pinned rescue actor epairs for every local GBSP map."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
from collections import Counter
from pathlib import Path

from gbsp_v15 import parse_entities, read_gbsp

ROOT = Path(__file__).resolve().parents[1]
MAP_DIR = ROOT / "inputs/original/Data/Maps"
LEVEL_CSV = ROOT / "tracker/level_map_inventory.csv"
COVERAGE_CSV = ROOT / "tracker/rescue_mission_coverage.csv"
TARGET_CLASSES = {
    "RCS_PlayerRescue",
    "RCS_FireRescue",
    "RCS_VictimRescue",
    "RCS_VictimAltSpot",
    "RCS_PathPoint",
}
def is_script_key(key: str) -> bool:
    return key.startswith("S_Script")

FIELDS = [
    "map_key", "catalog_level_key", "rescue_scenario_key", "map_relation",
    "map_file", "map_sha256", "entity_index", "classname", "entity_name",
    "actor_name", "d_type", "location_key", "location", "spawn_message_key",
    "spawn_message", "fire_type", "height", "radius", "spray_time",
    "fire_alive_flag", "smoking_flag", "heading", "use_heading", "num_keys",
    "fire_keys_json", "script_epairs_json", "epairs_json",
]


def sha256(raw: bytes) -> str:
    return hashlib.sha256(raw).hexdigest()


def read_index(path: Path, key_field: str) -> dict[str, dict[str, str]]:
    if not path.exists():
        return {}
    with path.open(newline="", encoding="utf-8") as stream:
        rows = csv.DictReader(stream)
        return {row[key_field].casefold(): row for row in rows}


def first_value(props: dict[str, str], *keys: str) -> tuple[str, str]:
    for key in keys:
        if key in props:
            return key, props[key]
    return "", ""


def fire_keys(props: dict[str, str]) -> list[dict[str, str]]:
    result = []
    for number in range(1, 17):
        size_key = f"P_Key{number}Size"
        time_key = f"P_Key{number}TimeOffset"
        if size_key in props or time_key in props:
            result.append({
                "index": str(number),
                "size": props.get(size_key, ""),
                "time_offset": props.get(time_key, ""),
            })
    return result


def build() -> tuple[list[dict[str, str]], dict, str]:
    levels = read_index(LEVEL_CSV, "bsp_file")
    scenarios = read_index(COVERAGE_CSV, "bsp_file")
    inventory: list[dict[str, str]] = []
    map_summaries = []
    class_totals: Counter[str] = Counter()
    victim_types: Counter[str] = Counter()
    fire_types: Counter[str] = Counter()
    victim_script_values: Counter[str] = Counter()

    for bsp_path in sorted(MAP_DIR.glob("*.bsp"), key=lambda p: p.name.casefold()):
        raw, _chunks, payloads = read_gbsp(bsp_path)
        entities = parse_entities(payloads[16])
        victim_names = {
            dict(epairs).get("%name%", "")
            for epairs in entities
            if dict(epairs).get("classname") == "RCS_VictimRescue"
        }
        alt_spot_links = {"total": 0, "matched_local_victim": 0, "unresolved": []}
        for entity_index, epairs in enumerate(entities):
            props = dict(epairs)
            if props.get("classname") != "RCS_VictimAltSpot":
                continue
            alt_spot_links["total"] += 1
            target_name = props.get("A_Victim", "")
            if target_name in victim_names:
                alt_spot_links["matched_local_victim"] += 1
            else:
                alt_spot_links["unresolved"].append({
                    "entity_index": entity_index,
                    "entity_name": props.get("%name%", ""),
                    "a_victim": target_name,
                })
        key = bsp_path.stem.casefold()
        catalog_ref = f"Maps/{bsp_path.name}".casefold()
        level = levels.get(catalog_ref, {})
        scenario = scenarios.get(catalog_ref, {})
        counts: Counter[str] = Counter()
        map_victim_script_values: Counter[str] = Counter()

        for entity_index, epairs in enumerate(entities):
            props = dict(epairs)
            classname = props.get("classname", "")
            if classname not in TARGET_CLASSES:
                continue
            counts[classname] += 1
            class_totals[classname] += 1
            if classname == "RCS_VictimRescue":
                victim_types[props.get("D_Type", "<missing>")] += 1
                for script_key, script_value in epairs:
                    if is_script_key(script_key) and script_value not in ("", "<null>"):
                        map_victim_script_values[script_value] += 1
                        victim_script_values[script_value] += 1
            if classname == "RCS_FireRescue":
                fire_types[props.get("F_FireType", "<missing>")] += 1
            location_key, location = first_value(props, "ZZ_location", "location")
            spawn_key, spawn_message = first_value(props, "ZZ_spawn_message", "spawn_message")
            keys = fire_keys(props)
            scripts = [(name, value) for name, value in epairs if is_script_key(name)]
            row = {
                "map_key": key,
                "catalog_level_key": level.get("level_key", ""),
                "rescue_scenario_key": scenario.get("scenario_key", ""),
                "map_relation": level.get("map_relation", ""),
                "map_file": bsp_path.relative_to(ROOT / "inputs/original").as_posix(),
                "map_sha256": sha256(raw),
                "entity_index": str(entity_index),
                "classname": classname,
                "entity_name": props.get("%name%", ""),
                "actor_name": props.get("A_Name", ""),
                "d_type": props.get("D_Type", ""),
                "location_key": location_key,
                "location": location,
                "spawn_message_key": spawn_key,
                "spawn_message": spawn_message,
                "fire_type": props.get("F_FireType", ""),
                "height": props.get("H_Height", ""),
                "radius": props.get("H_Radius", props.get("J_Radius", "")),
                "spray_time": props.get("K_SprayTime", ""),
                "fire_alive_flag": props.get("N_IsAlive", ""),
                "smoking_flag": props.get("M_Smoking", ""),
                "heading": props.get("T_Heading", props.get("G_Heading", "")),
                "use_heading": props.get("T_UseHeading", ""),
                "num_keys": props.get("P_NumKeys", ""),
                "fire_keys_json": json.dumps(keys, separators=(",", ":")),
                "script_epairs_json": json.dumps(scripts, separators=(",", ":")),
                "epairs_json": json.dumps(epairs, separators=(",", ":")),
            }
            inventory.append(row)

        map_summaries.append({
            "map_key": key,
            "map_file": bsp_path.relative_to(ROOT / "inputs/original").as_posix(),
            "map_sha256": sha256(raw),
            "entity_count": len(entities),
            "target_class_counts": {name: counts.get(name, 0) for name in sorted(TARGET_CLASSES)},
            "victim_script_values": dict(sorted(map_victim_script_values.items())),
            "victim_alt_spot_links": alt_spot_links,
        })

    summary = {
        "schema_version": 2,
        "map_count": len(map_summaries),
        "target_actor_count": len(inventory),
        "target_classes": sorted(TARGET_CLASSES),
        "class_counts": dict(sorted(class_totals.items())),
        "victim_type_counts": dict(sorted(victim_types.items())),
        "victim_script_value_counts": dict(sorted(victim_script_values.items())),
        "fire_type_counts": dict(sorted(fire_types.items())),
        "maps": map_summaries,
        "interpretation": (
            "Authored BSP data only. Each row preserves ordered raw epairs and "
            "hash-pins the source BSP. It does not assert runtime spawn, behavior, or parity."
        ),
    }
    lines = [
        "# Rescue actor inventory",
        "",
        "This is a hash-pinned, entity-level index of player, fire, victim,",
        "victim alternate spot, and PathPoint records across every local BSP.",
        "It preserves raw ordered epairs; it is authored data, not runtime evidence.",
        "",
        f"- Maps: {summary['map_count']}",
        f"- Indexed actors: {summary['target_actor_count']}",
        "Target classes: " + ", ".join(f"`{k}`={v}" for k, v in sorted(class_totals.items())),
        "",
        "## Per-map class counts",
        "",
        "| Map | Player | Fire | Victims | Alt spots | PathPoints |",
        "|---|---:|---:|---:|---:|---:|",
    ]
    for item in map_summaries:
        c = item["target_class_counts"]
        lines.append(
            f"| `{item['map_key']}` | {c.get('RCS_PlayerRescue', 0)} | "
            f"{c.get('RCS_FireRescue', 0)} | {c.get('RCS_VictimRescue', 0)} | "
            f"{c.get('RCS_VictimAltSpot', 0)} | {c.get('RCS_PathPoint', 0)} |"
        )
    lines += [
        "",
        "## Victim and fire types",
        "",
        "Victim `D_Type`: " + ", ".join(f"`{k}`={v}" for k, v in sorted(victim_types.items())),
        "",
        "Fire `F_FireType`: " + ", ".join(f"`{k}`={v}" for k, v in sorted(fire_types.items())),
        "",
        "## Authored alternate victim-spot links",
        "",
        "These source-data references are checked against `RCS_VictimRescue` `%name%` values on the same BSP. The executable's alternate-spot handler resolves the same name field before registering position and heading; this join does not prove runtime factory admission.",
        "",
        "| Map | Alt spots | Local victim name matches | Unresolved |",
        "|---|---:|---:|---:|",
    ]
    for item in map_summaries:
        links = item["victim_alt_spot_links"]
        unresolved = len(links["unresolved"])
        lines.append(
            f"| `{item['map_key']}` | {links['total']} | "
            f"{links['matched_local_victim']} | {unresolved} |"
        )
    lines += [
        "",
        "## Authored VictimRescue script values",
        "",
        "Exact non-empty `S_Script*` epair values on victim records; these are literals, not interpreted command names.",
        "",
        "| Map | Script values |",
        "|---|---|",
    ]
    for item in map_summaries:
        parts = []
        for value, count in sorted(item["victim_script_values"].items()):
            safe_value = value.replace("|", "\\|")
            parts.append(f"`{safe_value}` × {count}")
        lines.append(f"| `{item['map_key']}` | {'; '.join(parts) or '—'} |")
    lines += [
        "",
        "The static route for `roam <number>` is documented in [victim_roam_behavior.md](victim_roam_behavior.md). Numeric-only values (for example Boat B `500` and Gas R `100`) remain unresolved, and Training's `pickup:` literals are retained as data without being treated as roam commands.",
        "",
        "The CSV contains one row per entity, explicit locations/resource types,",
        "keyframe size/time pairs, script fields, and the full ordered epair list.",
        "Compare its `map_sha256` values with `tracker/level_map_inventory.csv`",
        "before using rows to seed a port build.",
        "",
    ]
    return inventory, summary, "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--csv", type=Path, default=ROOT / "tracker/rescue_actor_inventory.csv")
    parser.add_argument("--summary", type=Path, default=ROOT / "tracker/evidence/scenario-loading/rescue_actor_inventory.json")
    parser.add_argument("--report", type=Path, default=ROOT / "tracker/evidence/scenario-loading/rescue_actor_inventory.md")
    args = parser.parse_args()
    rows, summary, report = build()
    for path in (args.csv, args.summary, args.report):
        path.parent.mkdir(parents=True, exist_ok=True)
    with args.csv.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=FIELDS, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    args.summary.write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    args.report.write_text(report, encoding="utf-8")
    print(json.dumps({"csv": str(args.csv), "summary": summary["class_counts"], "actors": len(rows), "maps": summary["map_count"]}, sort_keys=True))


if __name__ == "__main__":
    main()
