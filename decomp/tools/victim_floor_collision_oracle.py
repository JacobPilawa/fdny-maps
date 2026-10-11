"""Capture Genesis.dll floor sweeps for every authored Training victim spot."""
import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from genesis_point_oracle import GenesisCollisionOracle


def f32(value):
    import struct
    return struct.unpack("<f", struct.pack("<f", value))[0]


def main():
    entities = json.loads((ROOT / "assets/training/entities.json").read_text())
    collision = json.loads((ROOT / "assets/training/collision.json").read_text())
    victims = {}
    for entity in entities:
        if entity.get("class") != "RCS_VictimRescue":
            continue
        props = entity["properties"]
        authored = [f32(float(part)) for part in props["location"].split()]
        victims[props["%name%"]] = [{"entity": entity["index"], "position": authored}]
    for entity in entities:
        if entity.get("class") != "RCS_VictimAltSpot":
            continue
        props = entity["properties"]
        authored = [f32(float(part)) for part in props["ZZ_location"].split()]
        victims[props["A_Victim"]].append({"entity": entity["index"], "position": authored})

    dll = ROOT / "evidence/original/Genesis.dll"
    oracle_hash = hashlib.sha256(dll.read_bytes()).hexdigest()
    samples = []
    for victim_name, candidates in victims.items():
        for candidate in candidates:
            x, y, z = candidate["position"]
            start = [x, f32(y + 48.0), z]
            oracle = GenesisCollisionOracle(collision)
            hit = oracle.trace(start, [0.0, -1000.0, 0.0], [-24.0] * 3, [24.0] * 3, 0x43)
            contact = hit["point"] if hit else [0.0, 0.0, 0.0]
            floor_position = [f32(contact[0]), f32(contact[1] + 1.0), f32(contact[2])]
            samples.append({
                "victim": victim_name,
                "victim_entity": candidates[0]["entity"],
                "candidate_entity": candidate["entity"],
                "authored_position": candidate["position"],
                "start": start,
                "delta": [0.0, -1000.0, 0.0],
                "mins": [-24.0, -24.0, -24.0],
                "maxs": [24.0, 24.0, 24.0],
                "expected_hit": hit is not None,
                "recorded_contact": bool(hit and hit["recordedContact"]),
                "contact": contact,
                "expected_position": floor_position,
            })

    out = {
        "provenance": {
            "dll_sha256": oracle_hash,
            "bsp_sha256": hashlib.sha256((ROOT / "assets/training/collision.json").read_bytes()).hexdigest(),
            "entry": "Genesis.dll geWorld_Collision 10001E30; wrapper 00492CE0; floor caller 0049DA00",
            "method": "Unmodified shipped DLL under Unicorn; contents 0x43, flags 2; isolated fresh-contact call per candidate",
            "scope": "Six authored Training victim positions with the source-derived (-24,-24,-24)/(24,24,24) hull; model floor helper adds Y=48, sweeps Y=-1000, stores contact plus Y=1",
        },
        "samples": samples,
    }
    path = ROOT / "evidence/victim-startup/victim-floor-collision-oracle.json"
    path.write_text(json.dumps(out, indent=2) + "\n")
    print(f"Saved {len(samples)} original Training victim hull sweeps to {path}")
    print(f"fresh hits={sum(s['expected_hit'] and s['recorded_contact'] for s in samples)}, misses-or-retained={sum(not s['expected_hit'] or not s['recorded_contact'] for s in samples)}")


if __name__ == "__main__":
    main()
