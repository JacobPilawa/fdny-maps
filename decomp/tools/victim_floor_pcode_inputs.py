"""Pack Training's BSP data and six authored victim positions for Ghidra P-code emulation."""
import json
import struct
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
STATE = 0x20000000
STATE_SIZE = 0x1000000


def f32(value):
    return struct.unpack("<f", struct.pack("<f", float(value)))[0]


def put(memory, address, payload):
    offset = address - STATE
    if offset < 0 or offset + len(payload) > len(memory):
        raise ValueError(f"emulator address outside state block: {address:#x}")
    memory[offset:offset + len(payload)] = payload


def build_state(data):
    memory = bytearray(STATE_SIZE)
    bsp = STATE + 0x1000
    put(memory, STATE + 24, struct.pack("<I", bsp))
    next_address = STATE + 0x20000

    def alloc(payload):
        nonlocal next_address
        address = next_address
        put(memory, address, payload)
        next_address = (address + len(payload) + 15) & ~15
        return address

    models = alloc(b"".join(
        struct.pack(
            "<ii18f", model["root"], -1, *model["mins"], *model["maxs"],
            *model.get("origin", [0, 0, 0]), 0, 0, 0, 0, 0, 0, 0, 0, 0,
        )
        for model in data["models"]
    ))
    nodes = alloc(b"".join(
        struct.pack(
            "<5i6f", *node["children"], node.get("num_faces", 0),
            node.get("first_face", 0), node["plane"], *node.get("mins", [0, 0, 0]),
            *node.get("maxs", [0, 0, 0]),
        )
        for node in data["nodes"]
    ))
    leafs = alloc(b"".join(
        struct.pack(
            "<i6f8i", leaf["contents"], *leaf.get("mins", [0, 0, 0]),
            *leaf.get("maxs", [0, 0, 0]), leaf.get("first_face", 0),
            leaf.get("num_faces", 0), leaf.get("first_portal", 0),
            leaf.get("num_portals", 0), leaf.get("cluster", 0),
            leaf.get("area", 0), leaf["first_side"], leaf["num_sides"],
        )
        for leaf in data["leafs"]
    ))
    sides = alloc(b"".join(
        struct.pack("<ii", side["plane"], side["side"])
        for side in data["leaf_sides"]
    ))
    planes = alloc(b"".join(
        struct.pack("<4fi", *plane["normal"], plane["distance"], plane["type"])
        for plane in data["planes"]
    ))

    bsp_data = bsp + 200
    for offset, pointer in ((72, models), (76, nodes), (88, leafs), (104, planes), (116, sides)):
        put(memory, bsp_data + offset, struct.pack("<I", pointer))
    counts = [
        len(data["models"]), len(data["nodes"]), 0, len(data["leafs"]), 0, 0, 0,
        len(data["planes"]), 0, 0, len(data["leaf_sides"]),
    ] + [0] * 11
    put(memory, bsp_data + 164, struct.pack("<22i", *counts))

    runtime_models = alloc(bytes(212 * len(data["models"])))
    put(memory, bsp + 464, struct.pack("<I", runtime_models))
    identity = [1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0]
    for index, model in enumerate(data["models"]):
        origin = model.get("origin", [0, 0, 0])
        center = [(model["mins"][axis] + model["maxs"][axis]) * 0.5 for axis in range(3)]
        payload = bytearray(212)
        struct.pack_into(
            "<i12f18f", payload, 64, index, *identity, *model["mins"], *model["maxs"],
            *model["mins"], *model["maxs"], *origin, *center,
        )
        struct.pack_into("<I", payload, 196, models + index * 80)
        struct.pack_into("<I", payload, 204, 8)
        put(memory, runtime_models + index * 212, payload)
    return memory


def training_candidates(entities):
    victims = {}
    for entity in entities:
        if entity.get("class") != "RCS_VictimRescue":
            continue
        properties = entity["properties"]
        name = properties["%name%"]
        position = [f32(part) for part in properties["location"].split()]
        victims[name] = {"victim_entity": entity["index"], "samples": [(entity["index"], position)]}
    for entity in entities:
        if entity.get("class") != "RCS_VictimAltSpot":
            continue
        properties = entity["properties"]
        victims[properties["A_Victim"]]["samples"].append((
            entity["index"], [f32(part) for part in properties["ZZ_location"].split()],
        ))
    return [
        (name, value["victim_entity"], candidate, position)
        for name, value in victims.items()
        for candidate, position in value["samples"]
    ]


def main():
    if len(sys.argv) != 2:
        raise SystemExit("usage: python3 tools/victim_floor_pcode_inputs.py <output-directory>")
    output = Path(sys.argv[1])
    output.mkdir(parents=True, exist_ok=True)
    collision = json.loads((ROOT / "assets/training/collision.json").read_text())
    entities = json.loads((ROOT / "assets/training/entities.json").read_text())
    (output / "state.bin").write_bytes(build_state(collision))
    with (output / "candidates.tsv").open("w") as handle:
        for name, victim, candidate, position in training_candidates(entities):
            handle.write("\t".join([name, str(victim), str(candidate), *(repr(x) for x in position)]) + "\n")
    print(f"Wrote local P-code inputs for {len(training_candidates(entities))} candidates to {output}")


if __name__ == "__main__":
    main()
