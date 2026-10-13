#!/usr/bin/env python3
"""List the named GBSP v15 models that have serialized geMotion records.

Genesis3D's GBSP MOTIONS chunk is a byte stream containing a text header,
``NumMotions``, then ``ModelNum`` records followed by serialized geMotion data.
The loader starts every GFX_Model.Motion pointer as null and assigns motion
objects only for listed model numbers. This utility preserves that association
without trying to decode path keyframes.

The format behavior is cross-checked against the public Genesis3D 1.1 sources:
World/Gbspfile.c and Actor/motion.c in
https://github.com/RealityFactory/Genesis3D.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path

from gbsp_v15 import parse_entities, read_gbsp

MOTION_COUNT_RE = re.compile(rb"(?m)^NumMotions\s+(\d+)\r?$")
MODEL_NUM_RE = re.compile(rb"(?m)^ModelNum\s+(\d+)\r?$")


def list_motion_models(path: Path) -> dict:
    raw, chunks, payloads = read_gbsp(path)
    payload = payloads.get(24, b"")
    if payload:
        if not payload.startswith(b"Genesis_Motion_File "):
            raise ValueError("MOTIONS chunk does not start with Genesis_Motion_File")
        count_match = MOTION_COUNT_RE.search(payload)
        if count_match is None:
            raise ValueError("MOTIONS chunk has no NumMotions header")
        declared_count = int(count_match.group(1))
        model_indices = [int(value) for value in MODEL_NUM_RE.findall(payload)]
        if len(model_indices) != declared_count:
            raise ValueError(
                f"MOTIONS declares {declared_count} records but contains "
                f"{len(model_indices)} ModelNum records"
            )
        if len(set(model_indices)) != len(model_indices):
            raise ValueError("MOTIONS chunk contains duplicate ModelNum records")
    else:
        declared_count = 0
        model_indices = []

    motion_set = set(model_indices)
    named_models = []
    entity_payload = payloads.get(16)
    if entity_payload is not None:
        for entity_index, epairs in enumerate(parse_entities(entity_payload)):
            props = dict(epairs)
            if props.get("classname") != "%Model%":
                continue
            model_text = props.get("Model")
            if model_text is None or not model_text.isdigit():
                continue
            model_index = int(model_text)
            named_models.append({
                "entity_index": entity_index,
                "name": props.get("%name%", ""),
                "model_index": model_index,
                "motion_record_present": model_index in motion_set,
                "origin": props.get("origin"),
            })

    chunk = next((item for item in chunks if item.type == 24), None)
    return {
        "input": path.name,
        "input_sha256": hashlib.sha256(raw).hexdigest(),
        "motion_chunk_sha256": chunk.sha256 if chunk else hashlib.sha256(b"").hexdigest(),
        "motion_chunk_bytes": chunk.byte_length if chunk else 0,
        "declared_motion_count": declared_count,
        "motion_model_indices": model_indices,
        "named_models": named_models,
        "interpretation": (
            "motion_record_present=false means the GBSP loader leaves this model's "
            "Motion pointer null"
        ),
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bsp", type=Path, help="Genesis3D GBSP v15 BSP file")
    parser.add_argument("--out", type=Path, help="write JSON to this file instead of stdout")
    args = parser.parse_args()
    result = list_motion_models(args.bsp)
    text = json.dumps(result, indent=2) + "\n"
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(text)
    else:
        print(text, end="")


if __name__ == "__main__":
    main()
