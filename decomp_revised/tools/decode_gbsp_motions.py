#!/usr/bin/env python3
"""Decode the retail Genesis MOTN/PATH 0.F2 tracks stored in GBSP chunk 24.

This parser follows the hash-pinned FDNY Genesis.dll readers documented in
tracker/evidence/scenario-loading/bmodel_door_preview_gap.md. It intentionally
rejects other PATH encodings instead of guessing from a reference viewer.
"""
from __future__ import annotations

import hashlib
import re
import struct


GENESIS_DLL_SHA256 = "5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643"
MOTION_COUNT_RE = re.compile(r"^NumMotions\s+(\d+)\s*$")
MODEL_NUM_RE = re.compile(r"^ModelNum\s+(\d+)\s*$")
PATH_ARRAY_RE = re.compile(r"^PathArray\s+(\d+)\s*$")


def _f32(value: str | float) -> float:
    return struct.unpack("<f", struct.pack("<f", float(value)))[0]


def _numbers(line: str, count: int, label: str) -> list[float]:
    fields = line.split()
    # geVFile_GetS feeds sscanf in the retail reader: numeric prefixes are
    # accepted and any trailing authoring comment is ignored by that call.
    if len(fields) < count:
        raise ValueError(f"{label}: expected at least {count} numeric fields, got {line!r}")
    try:
        return [_f32(field) for field in fields[:count]]
    except ValueError as exc:
        raise ValueError(f"{label}: invalid numeric fields in {line!r}") from exc


def _read_track(lines: list[str], index: int, channel: str) -> tuple[dict, int]:
    if index >= len(lines):
        raise ValueError(f"PATH: missing {channel} channel header")
    header = lines[index].strip().split()
    index += 1
    if len(header) != 3 or header[0] != channel:
        raise ValueError(f"PATH: expected '{channel} present mode', got {header!r}")
    present, header_mode = int(header[1]), int(header[2])
    track = {
        "present": bool(present),
        "header_mode": header_mode,
        "key_mode": None,
        "sampler_index": None,
        "flags": 0,
        "extra": 0,
        "time_encoding": None,
        "encoding": None,
        "axis": None,
        "keys": [],
    }
    if present == 0:
        return track, index
    if present != 1:
        raise ValueError(f"PATH: unsupported {channel} present value {present}")
    if index >= len(lines):
        raise ValueError(f"PATH: missing {channel} Keys header")

    key_header = lines[index].strip().split()
    index += 1
    if len(key_header) != 5 or key_header[0] != "Keys":
        raise ValueError(f"PATH: malformed {channel} Keys header {key_header!r}")
    key_count, key_mode, flags, extra = map(int, key_header[1:])
    if key_count < 0:
        raise ValueError(f"PATH: negative {channel} key count {key_count}")
    if key_mode not in (0, 1, 2):
        raise ValueError(f"PATH: unsupported {channel} key mode {key_mode}")

    compressed_times = bool(flags & 2)
    track.update({
        "key_mode": key_mode,
        "sampler_index": ({0: 3, 1: 4, 2: 5}[key_mode]
                          if channel == "Rotation" else key_mode),
        "flags": flags,
        "extra": extra,
        "time_encoding": "start_plus_step" if compressed_times else "explicit",
    })

    if channel == "Rotation":
        rotation_format = flags & ~2
        if rotation_format == 0:
            value_fields = 4
            track["encoding"] = "quaternion_wxyz"
        elif rotation_format == 1:
            value_fields = 1
            track["encoding"] = "axis_angle_radians"
            if index >= len(lines):
                raise ValueError("PATH: missing rotation axis")
            track["axis"] = _numbers(lines[index], 3, "Rotation axis")
            index += 1
        else:
            raise ValueError(f"PATH: unsupported Rotation key flags {flags}")
    else:
        value_fields = 3
        track["encoding"] = "xyz"

    if compressed_times:
        if index >= len(lines):
            raise ValueError(f"PATH: missing compressed {channel} start/step")
        start, step = _numbers(lines[index], 2, f"{channel} start/step")
        start_line = lines[index].strip()
        index += 1
    else:
        start, step, start_line = 0.0, 0.0, None

    for key_index in range(key_count):
        if index >= len(lines):
            raise ValueError(f"PATH: truncated {channel} key list")
        raw_line = lines[index].strip()
        index += 1
        fields = _numbers(raw_line, value_fields + (0 if compressed_times else 1),
                          f"{channel} key {key_index}")
        if compressed_times:
            time = _f32(key_index * step + start)
            values = fields
        else:
            time, values = fields[0], fields[1:]
        key = {"time": time, "value": values, "source_line": raw_line}
        track["keys"].append(key)

    track["key_count"] = key_count
    track["compressed_time_header"] = start_line
    track["loops"] = bool(extra)
    return track, index


def _read_path(lines: list[str], index: int, model_index: int, path_index: int) -> tuple[dict, int]:
    if index >= len(lines):
        raise ValueError(f"ModelNum {model_index}: missing PATH {path_index}")
    path_format = lines[index].strip()
    index += 1
    if path_format != "PATH 0.F2":
        raise ValueError(f"ModelNum {model_index}: unsupported retail path format {path_format!r}")
    rotation, index = _read_track(lines, index, "Rotation")
    translation, index = _read_track(lines, index, "Translation")
    return {
        "format": path_format,
        "looped": rotation["loops"] or translation["loops"],
        "rotation": rotation,
        "translation": translation,
    }, index


def decode_gbsp_motions(payload: bytes, source_sha256: str | None = None) -> dict:
    """Return typed paths while preserving every source key line for review."""
    if not payload:
        return {
            "format": "fdny-retail-motions-f2-v1",
            "genesis_dll_sha256": GENESIS_DLL_SHA256,
            "declared_count": 0,
            "records": [],
        }
    lines = payload.decode("latin1").splitlines()
    if len(lines) < 2 or not lines[0].startswith("Genesis_Motion_File "):
        raise ValueError("MOTIONS chunk does not begin with Genesis_Motion_File")
    count_match = MOTION_COUNT_RE.fullmatch(lines[1].strip())
    if count_match is None:
        raise ValueError("MOTIONS chunk has no valid NumMotions header")
    declared_count = int(count_match.group(1))

    model_positions = []
    for index, line in enumerate(lines[2:], start=2):
        match = MODEL_NUM_RE.fullmatch(line.strip())
        if match:
            model_positions.append((index, int(match.group(1))))
    if len(model_positions) != declared_count:
        raise ValueError(
            f"MOTIONS declares {declared_count} records but contains "
            f"{len(model_positions)} ModelNum records"
        )

    records = []
    seen_models = set()
    for record_index, (record_start, model_index) in enumerate(model_positions):
        if model_index in seen_models:
            raise ValueError(f"MOTIONS contains duplicate ModelNum {model_index}")
        seen_models.add(model_index)
        record_end = (model_positions[record_index + 1][0]
                      if record_index + 1 < len(model_positions) else len(lines))
        if record_start + 1 >= record_end or lines[record_start + 1].strip() != "MOTN 0.F0":
            got = lines[record_start + 1].strip() if record_start + 1 < record_end else "<eof>"
            raise ValueError(f"ModelNum {model_index}: unsupported MOTN header {got!r}")
        path_array = None
        for index in range(record_start + 2, record_end):
            match = PATH_ARRAY_RE.fullmatch(lines[index].strip())
            if match:
                path_array = (index, int(match.group(1)))
                break
        if path_array is None:
            raise ValueError(f"ModelNum {model_index}: PathArray header not found")
        path_start, path_count = path_array
        cursor = path_start + 1
        paths = []
        for path_index in range(path_count):
            path, cursor = _read_path(lines, cursor, model_index, path_index)
            paths.append(path)
        if cursor > record_end:
            raise ValueError(f"ModelNum {model_index}: decoded paths cross record boundary")
        records.append({"model_index": model_index, "path_count": path_count, "paths": paths})

    return {
        "format": "fdny-retail-motions-f2-v1",
        "source_chunk_sha256": hashlib.sha256(payload).hexdigest(),
        "source_bsp_sha256": source_sha256,
        "genesis_dll_sha256": GENESIS_DLL_SHA256,
        "reader_functions": {
            "motion": "Genesis.dll:1000D570",
            "path": "Genesis.dll:1000F580",
            "rotation_keys": "Genesis.dll:10005A00",
            "translation_keys": "Genesis.dll:10015FB0",
            "path_sample": "Genesis.dll:1000E380",
        },
        "declared_count": declared_count,
        "records": records,
    }
