#!/usr/bin/env python3
"""Join authored RCS_BModel reach gates to GBSP named models and MOTIONS records.

This reports asset evidence needed by the executable's conditional BModel reach
expansion. A serialized motion record is only a prerequisite: the retail helper
tests the greatest child-motion end time, so rows marked
``extent_decode_required`` are not declared expanded by this inventory.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from pathlib import Path

from gbsp_v15 import parse_entities, read_gbsp

CHILD_PROP_RE = re.compile(r"^C_AnimModel(\d+)$")
MODEL_LINE_RE = re.compile(r"^ModelNum\s+(\d+)\s*$")
MOTION_COUNT_LINE_RE = re.compile(r"^NumMotions\s+(\d+)\s*$")
PATH_ARRAY_LINE_RE = re.compile(r"^PathArray\s+(\d+)\s*$")


def _float32(value: str | float) -> float:
    """Round an authored key time to the geFloat storage used by Genesis3D."""
    return struct.unpack("<f", struct.pack("<f", float(value)))[0]


def _parse_path_time_extent(lines: list[str], index: int, model_id: int) -> tuple[int, float | None]:
    if index >= len(lines) or lines[index].strip() != "PATH 0.F2":
        got = lines[index].strip() if index < len(lines) else "<eof>"
        raise ValueError(f"ModelNum {model_id}: expected PATH 0.F2, got {got!r}")
    index += 1
    channel_extents: list[tuple[float, float]] = []

    for expected_channel in ("Rotation", "Translation"):
        if index >= len(lines):
            raise ValueError(f"ModelNum {model_id}: truncated {expected_channel} channel")
        header = lines[index].split()
        index += 1
        if len(header) < 2 or header[0] != expected_channel:
            raise ValueError(
                f"ModelNum {model_id}: expected {expected_channel} channel, got {header!r}"
            )
        if int(header[1]) == 0:
            continue

        if index >= len(lines):
            raise ValueError(f"ModelNum {model_id}: missing {expected_channel} Keys record")
        key_header = lines[index].split()
        index += 1
        if len(key_header) < 4 or key_header[0] != "Keys":
            raise ValueError(f"ModelNum {model_id}: malformed {expected_channel} key header {key_header!r}")
        key_count = int(key_header[1])
        time_flags = int(key_header[3])
        if key_count < 0:
            raise ValueError(f"ModelNum {model_id}: negative {expected_channel} key count")

        if time_flags & 2:
            if index >= len(lines):
                raise ValueError(f"ModelNum {model_id}: missing compressed {expected_channel} time range")
            time_values = lines[index].split()
            index += 1
            if len(time_values) < 2:
                raise ValueError(f"ModelNum {model_id}: malformed compressed {expected_channel} time range")
            start, step = (_float32(time_values[0]), _float32(time_values[1]))
            if key_count:
                # The retail QKFrame/VKFrame readers store each generated key time as geFloat.
                times = [_float32(key_index * step + start) for key_index in range(key_count)]
                channel_extents.append((times[0], times[-1]))
            index += key_count
        else:
            times = []
            for _ in range(key_count):
                if index >= len(lines):
                    raise ValueError(f"ModelNum {model_id}: truncated {expected_channel} keys")
                key_values = lines[index].split()
                index += 1
                if not key_values:
                    raise ValueError(f"ModelNum {model_id}: empty {expected_channel} key")
                times.append(_float32(key_values[0]))
            if times:
                channel_extents.append((times[0], times[-1]))

    if not channel_extents:
        return index, None
    return index, max(end for _, end in channel_extents)


def _parse_motion_end_times(payload: bytes) -> tuple[int, dict[int, float | None]]:
    if not payload:
        return 0, {}
    lines = payload.decode("latin1").splitlines()
    if len(lines) < 2 or not lines[0].startswith("Genesis_Motion_File "):
        raise ValueError("MOTIONS payload does not begin with Genesis_Motion_File")
    count_match = MOTION_COUNT_LINE_RE.fullmatch(lines[1].strip())
    if count_match is None:
        raise ValueError("MOTIONS payload has no valid NumMotions header")
    declared_count = int(count_match.group(1))
    records: dict[int, float | None] = {}
    index = 2

    while index < len(lines):
        match = MODEL_LINE_RE.fullmatch(lines[index].strip())
        if match is None:
            if lines[index].strip():
                raise ValueError(f"Unexpected MOTIONS record text at line {index + 1}: {lines[index]!r}")
            index += 1
            continue
        model_id = int(match.group(1))
        if model_id in records:
            raise ValueError(f"Duplicate ModelNum {model_id}")
        index += 1

        if index >= len(lines) or lines[index].strip() != "MOTN 0.F0":
            got = lines[index].strip() if index < len(lines) else "<eof>"
            raise ValueError(f"ModelNum {model_id}: expected retail MOTN 0.F0, got {got!r}")
        index += 1
        if index < len(lines) and lines[index].startswith("NameID "):
            index += 1

        while index < len(lines):
            path_array = PATH_ARRAY_LINE_RE.fullmatch(lines[index].strip())
            if path_array:
                break
            index += 1
        if index >= len(lines):
            raise ValueError(f"ModelNum {model_id}: PathArray header not found")
        path_count = int(path_array.group(1))
        index += 1

        path_ends: list[float] = []
        for _ in range(path_count):
            index, path_end = _parse_path_time_extent(lines, index, model_id)
            if path_end is not None:
                path_ends.append(path_end)
        records[model_id] = max(path_ends) if path_ends else None

    if len(records) != declared_count:
        raise ValueError(f"MOTIONS declares {declared_count} records but parsed {len(records)}")
    return declared_count, records


def analyze_bsp(path: Path) -> dict:
    raw, chunks, payloads = read_gbsp(path)
    entities = parse_entities(payloads[16])
    motion_payload = payloads.get(24, b"")
    declared_motion_count, motion_end_by_model = _parse_motion_end_times(motion_payload)
    motion_model_set = set(motion_end_by_model)
    model_by_name: dict[str, list[dict]] = {}
    for index, epairs in enumerate(entities):
        props = dict(epairs)
        if props.get("classname") != "%Model%":
            continue
        model_text = props.get("Model")
        if not model_text or not model_text.isdigit():
            continue
        name = props.get("%name%", "")
        model_by_name.setdefault(name, []).append({
            "model_entity_index": index,
            "model_index": int(model_text),
            "motion_record_present": int(model_text) in motion_model_set,
            "motion_max_end_time": motion_end_by_model.get(int(model_text)),
        })

    rows = []
    for index, epairs in enumerate(entities):
        props = dict(epairs)
        if props.get("classname") != "RCS_BModel":
            continue
        child_refs = []
        for key, name in epairs:
            match = CHILD_PROP_RE.match(key)
            if match and name:
                child_refs.append((int(match.group(1)), name))
        child_refs.sort()
        children = []
        for slot, name in child_refs:
            matches = model_by_name.get(name, [])
            children.append({
                "slot": slot,
                "name": name,
                "matches": matches,
            })

        def parse_float(key: str) -> float | None:
            try:
                return float(props[key])
            except (KeyError, ValueError):
                return None

        reach = parse_float("E_Reach")
        hit_points = parse_float("F_HitPoints")
        liftable = parse_float("G_Liftable")
        authored_gates_pass = (
            reach is not None and hit_points is not None and liftable is not None
            and hit_points > 0 and liftable == 0 and reach < 48
        )
        linked_children = [child for child in children if child["matches"]]
        any_child_motion = any(
            match["motion_record_present"]
            for child in linked_children for match in child["matches"]
        )
        child_motion_ends = [
            match["motion_max_end_time"]
            for child in linked_children for match in child["matches"]
            if match["motion_max_end_time"] is not None
        ]
        # InitializeBModelEntity starts root +0x1EC at 0 and replaces it only
        # with a greater end time returned by geMotion_GetTimeExtents.
        greatest_child_end = max([0.0, *child_motion_ends])
        reach_predicate_outside_zero = greatest_child_end <= -1e-28 or greatest_child_end >= 1e-28
        reach_raised = authored_gates_pass and reach_predicate_outside_zero
        if not authored_gates_pass:
            status = "authored_gates_do_not_request_raise"
        elif len(linked_children) != len(children):
            status = "child_model_link_unresolved"
        elif not any_child_motion:
            status = "predicate_false_no_child_motion_records"
        elif not reach_predicate_outside_zero:
            status = "predicate_false_zero_child_motion_extent"
        else:
            status = "reach_raised_to_48"

        rows.append({
            "entity_index": index,
            "name": props.get("%name%", props.get("name", "")),
            "reach": reach,
            "hit_points": hit_points,
            "liftable": liftable,
            "authored_gates_pass": authored_gates_pass,
            "child_models": children,
            "any_child_motion_record": any_child_motion,
            "greatest_child_motion_end_time": greatest_child_end,
            "reach_expansion_helper_true": reach_raised,
            "effective_reach": 48.0 if reach_raised and reach < 48 else reach,
            "reach_gate_asset_status": status,
        })

    chunk = next((item for item in chunks if item.type == 24), None)
    counts: dict[str, int] = {}
    for row in rows:
        status = row["reach_gate_asset_status"]
        counts[status] = counts.get(status, 0) + 1
    return {
        "input": path.name,
        "input_sha256": hashlib.sha256(raw).hexdigest(),
        "motion_chunk_sha256": chunk.sha256 if chunk else hashlib.sha256(b"").hexdigest(),
        "declared_motion_count": declared_motion_count,
        "parsed_motion_count": len(motion_end_by_model),
        "bmodel_entity_count": len(rows),
        "reach_gate_status_counts": counts,
        "bmodels": rows,
        "motion_model_end_times": [
            {"model_index": model_id, "max_path_end_time": end_time}
            for model_id, end_time in sorted(motion_end_by_model.items())
        ],
        "interpretation": (
            "Per-model geMotion extents are decoded from the retail MOTN 0.F0 / PATH 0.F2 text "
            "records. The retail DLL extent routines take the last key time in each channel/path; "
            "the executable initializes BModel root +0x1EC to zero and keeps the greatest child "
            "motion end. reach_raised_to_48 follows the executable's hit-point, non-liftable, "
            "strictly-outside-1e-28 gate."
        ),
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bsp", nargs="+", type=Path, help="one or more GBSP v15 BSP files")
    parser.add_argument("--out", type=Path, help="write combined JSON here instead of stdout")
    args = parser.parse_args()
    results = [analyze_bsp(path) for path in args.bsp]
    output = json.dumps({"maps": results}, indent=2, allow_nan=False) + "\n"
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(output)
    else:
        print(output, end="")


if __name__ == "__main__":
    main()
