#!/usr/bin/env python3
"""Shared, lossless reader for Genesis3D GBSP v15 chunk containers."""
from __future__ import annotations

import hashlib
import struct
from dataclasses import asdict, dataclass
from pathlib import Path

CHUNK_NAMES = {
    0: "HEADER", 1: "MODELS", 2: "NODES", 3: "BNODES", 4: "LEAFS",
    5: "CLUSTERS", 6: "AREAS", 7: "AREA_PORTALS", 8: "LEAF_SIDES",
    9: "PORTALS", 10: "PLANES", 11: "FACES", 12: "LEAF_FACES",
    13: "VERT_INDEX", 14: "VERTS", 15: "RGB_VERTS", 16: "ENTDATA",
    17: "TEXINFO", 18: "TEXTURES", 19: "TEXDATA", 20: "LIGHTDATA",
    21: "VISDATA", 22: "SKYDATA", 23: "PALETTES", 24: "MOTIONS",
    0xFFFF: "END",
}

# Element sizes from Genesis3D 1.1 World/Gbspfile.h. A size mismatch is a
# format/version warning, not permission to reinterpret the bytes silently.
ELEMENT_SIZES = {
    0: 28, 1: 80, 2: 44, 3: 12, 4: 60, 5: 4, 6: 8, 7: 8, 8: 8,
    9: 16, 10: 20, 11: 36, 12: 4, 13: 4, 14: 12, 15: 12, 16: 1,
    17: 64, 18: 52, 19: 1, 20: 1, 21: 1, 22: 44, 23: 768, 24: 1,
}


@dataclass(frozen=True)
class Chunk:
    type: int
    name: str
    element_size: int
    elements: int
    header_offset: int
    data_offset: int
    byte_length: int
    sha256: str


def read_gbsp(path: Path) -> tuple[bytes, list[Chunk], dict[int, bytes]]:
    """Read ordered chunks, checking bounds while retaining unknown payloads."""
    raw = path.read_bytes()
    pos = 0
    chunks: list[Chunk] = []
    payloads: dict[int, bytes] = {}
    seen: set[int] = set()
    terminated = False

    while pos < len(raw):
        if pos + 12 > len(raw):
            raise ValueError(f"truncated chunk header at 0x{pos:x}")
        header_offset = pos
        chunk_type, element_size, elements = struct.unpack_from("<iii", raw, pos)
        pos += 12
        name = CHUNK_NAMES.get(chunk_type, f"UNKNOWN_{chunk_type}")

        if chunk_type == 0xFFFF:
            chunks.append(Chunk(chunk_type, name, element_size, elements,
                                header_offset, pos, 0, hashlib.sha256(b"").hexdigest()))
            terminated = True
            if pos != len(raw):
                raise ValueError(f"unexpected {len(raw) - pos} trailing bytes after end marker")
            break

        if element_size < 0 or elements < 0:
            raise ValueError(f"negative size/count in {name} chunk at 0x{header_offset:x}")
        byte_length = element_size * elements
        if byte_length > len(raw) - pos:
            raise ValueError(f"{name} chunk at 0x{header_offset:x} runs past EOF")
        if chunk_type in seen:
            raise ValueError(f"duplicate {name} chunk; preserving multiple instances is not implemented")
        seen.add(chunk_type)

        expected = ELEMENT_SIZES.get(chunk_type)
        if expected is not None and element_size != expected:
            raise ValueError(
                f"{name} chunk element size is {element_size}, expected {expected} for GBSP v15"
            )
        data_offset = pos
        payload = raw[pos:pos + byte_length]
        payloads[chunk_type] = payload
        chunks.append(Chunk(chunk_type, name, element_size, elements, header_offset,
                            data_offset, byte_length, hashlib.sha256(payload).hexdigest()))
        pos += byte_length

    if not terminated:
        raise ValueError("GBSP stream has no 0xffff end marker")
    if 0 not in payloads:
        raise ValueError("GBSP stream has no HEADER chunk")
    if len(payloads[0]) != 28:
        raise ValueError(f"GBSP HEADER is {len(payloads[0])} bytes, expected 28")
    return raw, chunks, payloads


def parse_entities(payload: bytes) -> list[list[tuple[str, str]]]:
    """Decode ENTDATA while preserving authored epair order and duplicates."""
    if len(payload) < 4:
        raise ValueError("truncated ENTDATA chunk")
    entity_count = struct.unpack_from("<i", payload, 0)[0]
    if entity_count < 0:
        raise ValueError(f"invalid ENTDATA entity count {entity_count}")

    def cstring(offset: int) -> tuple[str, int]:
        if offset + 4 > len(payload):
            raise ValueError("truncated ENTDATA string length")
        length = struct.unpack_from("<i", payload, offset)[0]
        offset += 4
        if length < 0 or offset + length > len(payload):
            raise ValueError(f"invalid ENTDATA string length {length}")
        value = payload[offset:offset + length].split(b"\0", 1)[0].decode("latin1", "replace")
        return value, offset + length

    pos = 4
    entities: list[list[tuple[str, str]]] = []
    for entity_index in range(entity_count):
        if pos + 4 > len(payload):
            raise ValueError(f"truncated ENTDATA before entity {entity_index}")
        epair_count = struct.unpack_from("<i", payload, pos)[0]
        pos += 4
        if epair_count < 0:
            raise ValueError(f"negative epair count in entity {entity_index}")
        pairs = []
        for _ in range(epair_count):
            key, pos = cstring(pos)
            value, pos = cstring(pos)
            pairs.append((key, value))
        entities.append(pairs)

    if pos != len(payload):
        raise ValueError(f"ENTDATA decoded {pos} of {len(payload)} bytes")
    return entities


def chunk_dicts(chunks: list[Chunk]) -> list[dict]:
    return [asdict(chunk) for chunk in chunks]

