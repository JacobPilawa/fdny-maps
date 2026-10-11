#!/usr/bin/env python3
"""Preserve ordered Genesis3D BSP entity typedef epairs and known field offsets."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

# These are fixed-width serialized/runtime fields for the 32-bit game. Unknown
# types are retained verbatim and stop offset calculation instead of guessing.
FIELD_SIZES = {"int": 4, "float": 4, "string": 4, "model": 4, "point": 12}
ENTDATA_CHUNK = 16


def read_cstring(data: bytes, offset: int) -> tuple[str, int]:
    if offset + 4 > len(data):
        raise ValueError("truncated BSP epair string length")
    length = struct.unpack_from("<i", data, offset)[0]
    offset += 4
    if length < 0 or offset + length > len(data):
        raise ValueError(f"invalid BSP epair string length {length}")
    value = data[offset:offset + length].split(b"\0", 1)[0].decode("latin1", "replace")
    return value, offset + length


def read_entities(path: Path) -> tuple[bytes, list[list[tuple[str, str]]]]:
    raw = path.read_bytes()
    chunks: dict[int, bytes] = {}
    offset = 0
    while offset + 12 <= len(raw):
        chunk_type, element_size, count = struct.unpack_from("<iii", raw, offset)
        offset += 12
        length = element_size * count
        if chunk_type == 0xFFFF:
            break
        if length < 0 or offset + length > len(raw):
            raise ValueError(f"invalid BSP chunk {chunk_type} length {length}")
        chunks[chunk_type] = raw[offset:offset + length]
        offset += length
    if ENTDATA_CHUNK not in chunks:
        raise ValueError("BSP has no ENTDATA chunk")

    data = chunks[ENTDATA_CHUNK]
    if len(data) < 4:
        raise ValueError("truncated ENTDATA chunk")
    entity_count = struct.unpack_from("<i", data, 0)[0]
    if entity_count < 0:
        raise ValueError(f"invalid ENTDATA entity count {entity_count}")
    pos = 4
    entities: list[list[tuple[str, str]]] = []
    for entity_index in range(entity_count):
        if pos + 4 > len(data):
            raise ValueError(f"truncated ENTDATA before entity {entity_index}")
        epair_count = struct.unpack_from("<i", data, pos)[0]
        pos += 4
        if epair_count < 0:
            raise ValueError(f"invalid epair count in entity {entity_index}")
        epairs = []
        for _ in range(epair_count):
            key, pos = read_cstring(data, pos)
            value, pos = read_cstring(data, pos)
            epairs.append((key, value))
        entities.append(epairs)
    return raw, entities


def extract_typedefs(entities: list[list[tuple[str, str]]]) -> list[dict]:
    result = []
    for entity_index, epairs in enumerate(entities):
        if ("classname", "%typedef%") not in epairs:
            continue
        typename = next((v for k, v in epairs if k == "%typename%"), None)
        if not typename:
            continue
        fields = []
        cursor = 0
        offsets_known = True
        pending_field = None
        for key, value in epairs:
            if key in {"classname", "%typename%"}:
                continue
            if key == "%defaultvalue%":
                if pending_field is not None:
                    pending_field["default_value"] = value
                    pending_field = None
                continue
            size = FIELD_SIZES.get(value)
            field = {
                "name": key,
                "type": value,
                "byte_offset": cursor if offsets_known else None,
                "byte_size": size,
                "default_value": None,
            }
            fields.append(field)
            pending_field = field
            if size is None:
                offsets_known = False
            elif offsets_known:
                cursor += size
        result.append({
            "entity_index": entity_index,
            "name": typename,
            "field_count": len(fields),
            "layout_size": cursor if offsets_known else None,
            "layout_offsets_complete": offsets_known,
            "fields": fields,
        })
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bsp", type=Path, help="Genesis3D BSP file")
    parser.add_argument("--class", dest="class_name", help="emit only this typedef name")
    parser.add_argument("--output", type=Path, help="write JSON to this path")
    args = parser.parse_args()

    raw, entities = read_entities(args.bsp)
    typedefs = extract_typedefs(entities)
    if args.class_name:
        typedefs = [t for t in typedefs if t["name"] == args.class_name]
    output = {
        "format": "fdny-gbsp-typedefs-v1",
        "source_file": args.bsp.name,
        "source_sha256": hashlib.sha256(raw).hexdigest(),
        "source_entity_count": len(entities),
        "typedef_count": len(typedefs),
        "known_field_sizes_bytes": FIELD_SIZES,
        "typedefs": typedefs,
    }
    rendered = json.dumps(output, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")


if __name__ == "__main__":
    main()
