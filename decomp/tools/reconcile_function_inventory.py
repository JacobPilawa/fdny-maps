"""Exact-address reconciliation of the Ghidra inventory and existing notes.

This reports links between current records. It does not judge whether a
function is correct, complete, reachable, or behaviorally reproduced.
"""
from __future__ import annotations

import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def read_json(relative_path: str) -> tuple[dict, bytes]:
    raw = (ROOT / relative_path).read_bytes()
    return json.loads(raw), raw


def normalize_address(value: str) -> str:
    address = value.strip().upper().removeprefix("0X")
    if len(address) > 8 or not address or any(c not in "0123456789ABCDEF" for c in address):
        raise ValueError(f"Invalid 32-bit address: {value!r}")
    return address.zfill(8)


inventory, inventory_raw = read_json("evidence/original-function-inventory.json")
function_index, function_index_raw = read_json("evidence/function-index.json")
mechanics, mechanics_raw = read_json("mechanics.json")

defined: dict[str, dict] = {}
for function in inventory["functions"]:
    address = normalize_address(function["address"])
    if address in defined:
        raise ValueError(f"Duplicate Ghidra function entry: {address}")
    defined[address] = function

indexed_exports: dict[str, dict] = {}
for entry in function_index["functions"]:
    address = normalize_address(entry["address"])
    if address in indexed_exports:
        raise ValueError(f"Duplicate function-index address: {address}")
    indexed_exports[address] = entry

mechanics_by_system: dict[str, list[str]] = {}
all_mechanics_references: list[str] = []
for system in mechanics["systems"]:
    references = [normalize_address(address) for address in system.get("original_entries", [])]
    mechanics_by_system[system["name"]] = references
    all_mechanics_references.extend(references)

export_addresses = set(indexed_exports)
mechanics_addresses = set(all_mechanics_references)
artifact_addresses = export_addresses | mechanics_addresses
export_matches = export_addresses & set(defined)
mechanics_matches = mechanics_addresses & set(defined)
artifact_matches = artifact_addresses & set(defined)

export_addresses_without_defined_entry = [
    {
        "address": address,
        "systems": indexed_exports[address].get("systems", []),
        "exports": [path for variant in indexed_exports[address].get("variants", [])
                    for path in variant.get("exports", [])],
    }
    for address in sorted(export_addresses - set(defined))
]
mechanics_addresses_without_defined_entry = {
    name: sorted(set(references) - set(defined))
    for name, references in mechanics_by_system.items()
    if set(references) - set(defined)
}

output = {
    "scope": "Exact-address joins among current Ghidra function definitions, preserved decompiler export index, and mechanics ledger references.",
    "limitation": "Address links only. The export index does not carry an executable identity for every source file, so unmatched addresses require provenance review before calling them missing boundaries. This does not assess correctness, function-boundary completeness, runtime reachability, implementation quality, or game completion.",
    "inputSha256": {
        "evidence/original-function-inventory.json": hashlib.sha256(inventory_raw).hexdigest(),
        "evidence/function-index.json": hashlib.sha256(function_index_raw).hexdigest(),
        "mechanics.json": hashlib.sha256(mechanics_raw).hexdigest(),
    },
    "summary": {
        "ghidraDefinedFunctions": len(defined),
        "ghidraEntriesMarkedOtherThanUnclassified": sum(
            function.get("status") != "unclassified" for function in defined.values()
        ),
        "preservedExportAddresses": len(export_addresses),
        "preservedExportAddressesWithExactGhidraEntry": len(export_matches),
        "preservedExportAddressesWithoutExactGhidraEntry": len(export_addresses - set(defined)),
        "mechanicsOriginalEntryReferences": len(all_mechanics_references),
        "mechanicsUniqueAddresses": len(mechanics_addresses),
        "mechanicsAddressesWithExactGhidraEntry": len(mechanics_matches),
        "mechanicsUniqueAddressesWithoutExactGhidraEntry": len(mechanics_addresses - set(defined)),
        "uniqueAddressesFromEitherArtifactWithExactGhidraEntry": len(artifact_matches),
        "definedGhidraEntriesWithoutExactAddressInEitherArtifact": len(set(defined) - artifact_addresses),
    },
    "mechanicsSystems": {
        name: {
            "references": references,
            "exactGhidraEntryMatches": sorted(set(references) & set(defined)),
            "referencesWithoutExactGhidraEntry": sorted(set(references) - set(defined)),
        }
        for name, references in mechanics_by_system.items()
    },
    "preservedExportAddressesWithoutExactGhidraEntry": export_addresses_without_defined_entry,
    "mechanicsAddressesWithoutExactGhidraEntry": mechanics_addresses_without_defined_entry,
}

output_path = ROOT / "evidence/function-inventory-reconciliation.json"
output_path.write_text(json.dumps(output, indent=2) + "\n")
summary = output["summary"]
print(
    f"Wrote {output_path.relative_to(ROOT)}: "
    f"{summary['ghidraDefinedFunctions']} defined entries, "
    f"{summary['preservedExportAddressesWithExactGhidraEntry']}/"
    f"{summary['preservedExportAddresses']} exact export-index matches, "
    f"{summary['mechanicsAddressesWithExactGhidraEntry']}/"
    f"{summary['mechanicsUniqueAddresses']} exact mechanics matches."
)
