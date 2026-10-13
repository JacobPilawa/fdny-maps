#!/usr/bin/env python3
"""Join Subway A's EVS event bindings to their authored BSP actors and models."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path

from audit_bmodel_reach_assets import _parse_motion_end_times
from gbsp_v15 import parse_entities, read_gbsp

SCRIPT_PROPERTY_RE = re.compile(r"^S_Script\d+$")
CHILD_MODEL_RE = re.compile(r"^C_AnimModel(\d+)$")
SCRIPT_DEFINITION_RE = re.compile(r"^set\s+(\w+)\s+(.+?)\s*$")
SOUND_COMMAND_RE = re.compile(r"\bplaysnd\s+([A-Za-z0-9_]+)\b", re.IGNORECASE)
SOUND_DEFINITION_RE = re.compile(
    r'^\s*(ROC_SOUND_[A-Za-z0-9_]+)\s+([^\s;]+)'
    r'(?:\s+([^\s;]+)\s+([^\s;]+))?\s+"([^"]+)"\s*;',
    re.IGNORECASE,
)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _sound_definitions(path: Path) -> tuple[bytes, dict[str, dict]]:
    raw = path.read_bytes()
    definitions: dict[str, dict] = {}
    for line_number, line in enumerate(raw.decode("latin1").splitlines(), 1):
        match = SOUND_DEFINITION_RE.match(line)
        if not match:
            continue
        kind, name, arg1, arg2, source_path = match.groups()
        definitions.setdefault(name.casefold(), {
            "definition_file": path.as_posix(),
            "line": line_number,
            "resource_type": kind,
            "name": name,
            "parameters": [value for value in (arg1, arg2) if value is not None],
            "source_path": source_path,
        })
    return raw, definitions


def _resolve_data_path(data_root: Path, authored_path: str) -> Path | None:
    parts = authored_path.replace("\\", "/").split("/")
    if parts and parts[0].casefold() == "data":
        parts = parts[1:]
    current = data_root
    for part in parts:
        if not current.is_dir():
            return None
        match = next((item for item in current.iterdir() if item.name.casefold() == part.casefold()), None)
        if match is None:
            return None
        current = match
    return current if current.is_file() else None


def analyze(bsp_path: Path, evs_path: Path, sound_defs_path: Path) -> dict:
    bsp_bytes, _chunks, payloads = read_gbsp(bsp_path)
    entities = parse_entities(payloads[16])
    motion_count, motion_ends = _parse_motion_end_times(payloads.get(24, b""))

    model_by_name: dict[str, list[dict]] = {}
    for entity_index, epairs in enumerate(entities):
        props = dict(epairs)
        if props.get("classname") != "%Model%":
            continue
        model_text = props.get("Model", "")
        if not model_text.isdigit():
            continue
        model_id = int(model_text)
        model_by_name.setdefault(props.get("%name%", ""), []).append({
            "entity_index": entity_index,
            "model_index": model_id,
            "motion_record_present": model_id in motion_ends,
            "motion_max_end_time": motion_ends.get(model_id),
        })

    evs_bytes = evs_path.read_bytes()
    evs_text = evs_bytes.decode("latin1")
    sound_defs_bytes, sound_definitions = _sound_definitions(sound_defs_path)
    event_definitions = {}
    for line_number, line in enumerate(evs_text.splitlines(), 1):
        match = SCRIPT_DEFINITION_RE.match(line.strip())
        if match and match.group(1).lower() in {"subdoor", "brokendoor"}:
            event_definitions[match.group(1)] = {
                "line": line_number,
                "text": line.strip(),
            }

    used_sound_names = sorted({
        sound_name
        for definition in event_definitions.values()
        for sound_name in SOUND_COMMAND_RE.findall(definition["text"])
    }, key=str.casefold)
    sound_resources = []
    for sound_name in used_sound_names:
        definition = sound_definitions.get(sound_name.casefold())
        row = {"command_name": sound_name, "definition": definition}
        if definition is not None:
            source_file = _resolve_data_path(sound_defs_path.parent.parent, definition["source_path"])
            row["source_file"] = source_file.as_posix() if source_file else None
            row["source_sha256"] = sha256(source_file.read_bytes()) if source_file else None
        sound_resources.append(row)

    bindings = []
    unresolved = []
    for entity_index, epairs in enumerate(entities):
        props = dict(epairs)
        class_name = props.get("classname", "")
        if class_name not in {"RCS_BModel", "RCS_BoundsTrigger"}:
            continue
        script_bindings = []
        for property_name, value in epairs:
            if not SCRIPT_PROPERTY_RE.fullmatch(property_name):
                continue
            script_name = value[1:] if value.startswith("*") else value
            if script_name.lower() in {"subdoor", "brokendoor"}:
                script_bindings.append({
                    "property": property_name,
                    "value": value,
                    "event_name": script_name,
                    "event_definition": event_definitions.get(script_name),
                })
        if not script_bindings:
            continue

        row = {
            "entity_index": entity_index,
            "classname": class_name,
            "actor_name": props.get("A_Name", ""),
            "entity_name": props.get("%name%", ""),
            "reach": float(props["E_Reach"]) if "E_Reach" in props else None,
            "location": props.get("ZZ_location", ""),
            "script_bindings": script_bindings,
        }
        if class_name == "RCS_BModel":
            row["hit_points"] = float(props["F_HitPoints"]) if "F_HitPoints" in props else None
            row["liftable"] = float(props["G_Liftable"]) if "G_Liftable" in props else None
            children = []
            for property_name, model_name in epairs:
                match = CHILD_MODEL_RE.fullmatch(property_name)
                if not match:
                    continue
                matches = model_by_name.get(model_name, [])
                child = {
                    "slot": int(match.group(1)),
                    "model_name": model_name,
                    "model_entities": matches,
                }
                children.append(child)
                if not matches:
                    unresolved.append({
                        "owner_entity_index": entity_index,
                        "property": property_name,
                        "model_name": model_name,
                    })
            row["animation_models"] = sorted(children, key=lambda item: item["slot"])
        else:
            model_name = props.get("C_Model", "")
            matches = model_by_name.get(model_name, [])
            row["collision_model_name"] = model_name
            row["collision_model_entities"] = matches
            if not matches:
                unresolved.append({
                    "owner_entity_index": entity_index,
                    "property": "C_Model",
                    "model_name": model_name,
                })
        bindings.append(row)

    bmodels = [row for row in bindings if row["classname"] == "RCS_BModel"]
    triggers = [row for row in bindings if row["classname"] == "RCS_BoundsTrigger"]
    child_refs = [child for row in bmodels for child in row["animation_models"]]
    trigger_refs = [row for row in triggers]
    return {
        "schema_version": 1,
        "inputs": {
            "bsp_file": bsp_path.name,
            "bsp_sha256": sha256(bsp_bytes),
            "evs_file": evs_path.name,
            "evs_sha256": sha256(evs_bytes),
            "sound_definitions_file": sound_defs_path.name,
            "sound_definitions_sha256": sha256(sound_defs_bytes),
            "motion_record_count": motion_count,
        },
        "event_definitions": event_definitions,
        "sound_resources": sound_resources,
        "summary": {
            "subdoor_bmodel_entities": len(bmodels),
            "subdoor_animation_model_references": len(child_refs),
            "subdoor_animation_model_references_with_motion": sum(
                any(model["motion_record_present"] for model in child["model_entities"])
                for child in child_refs
            ),
            "brokendoor_bounds_trigger_entities": len(triggers),
            "brokendoor_collision_model_links_resolved": sum(
                bool(row["collision_model_entities"]) for row in trigger_refs
            ),
            "script_sound_commands": len(sound_resources),
            "sound_resource_definitions_resolved": sum(
                row["definition"] is not None for row in sound_resources
            ),
            "sound_3d_buffer_definitions": sum(
                row["definition"] is not None
                and row["definition"]["resource_type"].casefold() == "roc_sound_buffer3d"
                for row in sound_resources
            ),
            "sound_source_files_resolved": sum(
                row.get("source_file") is not None for row in sound_resources
            ),
            "unresolved_model_links": len(unresolved),
        },
        "bindings": bindings,
        "unresolved_model_links": unresolved,
        "interpretation": (
            "This is a static join of EVS variable bindings to same-map BSP actors and %Model% "
            "records. Runtime contacts, sound playback, animation appearance, and event timing "
            "are not observed by this inventory."
        ),
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--bsp", type=Path, default=Path("inputs/original/Data/Maps/subway_a.bsp"))
    parser.add_argument("--evs", type=Path, default=Path("inputs/original/Data/Maps/subway_a.evs"))
    parser.add_argument(
        "--sound-defs", type=Path,
        default=Path("inputs/original/Data/ResDefs/sound.rrf"),
    )
    parser.add_argument(
        "--out",
        type=Path,
        default=Path("tracker/evidence/scenario-loading/subway_a_interaction_inventory.json"),
    )
    args = parser.parse_args()
    report = analyze(args.bsp, args.evs, args.sound_defs)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print(json.dumps({"output": str(args.out), **report["summary"]}, sort_keys=True))


if __name__ == "__main__":
    main()
