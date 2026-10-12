#!/usr/bin/env python3
"""Extract PE header/import/export facts from the pinned Windows binaries."""

from __future__ import annotations

import csv
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BIN_DIR = ROOT / "inputs" / "original" / "binaries"
TRACKER = ROOT / "tracker"

ROLES = {
    "fdnyfirefighter.exe": ("Game executable", "First-party game entry and gameplay logic"),
    "genesis.dll": ("Engine/runtime", "Large exported ge* API; likely core engine boundary"),
    "d3ddrv.dll": ("Graphics driver", "Imports DirectDraw and exports DriverHook"),
    "fmod.dll": ("Audio middleware", "UPX-packed in the supplied copy; analyze after a derived unpacked copy is made"),
}


def parse_one(path: Path, hashes: dict[str, str]) -> tuple[dict[str, object], list[dict[str, object]], list[dict[str, object]], list[dict[str, object]]]:
    text = subprocess.run(["objdump", "-p", str(path)], check=True, capture_output=True, text=True).stdout
    image_base = re.search(r"^ImageBase\s+([0-9a-fA-F]+)", text, re.M)
    entry = re.search(r"^AddressOfEntryPoint\s+([0-9a-fA-F]+)", text, re.M)
    image_size = re.search(r"^SizeOfImage\s+([0-9a-fA-F]+)", text, re.M)
    linker = re.search(r"^MajorLinkerVersion\s+(\d+)\s*\nMinorLinkerVersion\s+(\d+)", text, re.M)
    timestamp = re.search(r"^Time/Date\s+(.+)$", text, re.M)
    section_text = subprocess.run(["objdump", "-h", str(path)], check=True, capture_output=True, text=True).stdout
    section_rows = []
    for match in re.finditer(r"^\s*\d+\s+(\S+)\s+([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+(\S+)", section_text, re.M):
        section_rows.append({
            "binary": path.name,
            "section": match.group(1),
            "size_hex": match.group(2),
            "vma": match.group(3),
            "type": match.group(4),
        })

    imports: list[dict[str, object]] = []
    import_block = text.split("The Import Tables:", 1)[1].split("Export Table:", 1)[0] if "The Import Tables:" in text else ""
    current_dll = None
    for line in import_block.splitlines():
        dll = re.match(r"\s*DLL Name:\s*(.+)", line)
        if dll:
            current_dll = dll.group(1).strip()
            continue
        sym = re.match(r"\s*(\d+)\s+(\S+)\s*$", line)
        if current_dll and sym:
            imports.append({"binary": path.name, "library": current_dll, "hint_or_ordinal": sym.group(1), "symbol": sym.group(2)})

    exports: list[dict[str, object]] = []
    export_block = text.split("Export Table:", 1)[1] if "Export Table:" in text else ""
    for match in re.finditer(r"^\s*(\d+)\s+(0x[0-9a-fA-F]+)(?:\s+(.*?))?\s*$", export_block, re.M):
        exports.append({"binary": path.name, "ordinal": match.group(1), "rva": match.group(2), "symbol": (match.group(3) or "").strip()})

    file_info = subprocess.run(["file", str(path)], check=True, capture_output=True, text=True).stdout.strip()
    role, note = ROLES.get(path.name.lower(), ("Unclassified", ""))
    packed = "UPX compressed" in file_info
    row: dict[str, object] = {
        "binary": path.name,
        "sha256": hashes.get(path.name.lower(), ""),
        "file_bytes": path.stat().st_size,
        "format": "PE32 Intel 80386" if "PE32" in file_info else file_info,
        "file_identification": file_info,
        "image_base": f"0x{int(image_base.group(1), 16):08X}" if image_base else "",
        "entry_rva": f"0x{int(entry.group(1), 16):08X}" if entry else "",
        "entry_va": f"0x{int(image_base.group(1), 16) + int(entry.group(1), 16):08X}" if image_base and entry else "",
        "image_size_bytes": int(image_size.group(1), 16) if image_size else "",
        "linker_version": f"{linker.group(1)}.{linker.group(2)}" if linker else "",
        "timestamp": timestamp.group(1).strip() if timestamp else "",
        "imports": len(imports),
        "imported_libraries": len({str(x["library"]).lower() for x in imports}),
        "exports": len(exports),
        "packed": "yes" if packed else "no",
        "role": role,
        "classification_note": note,
    }
    return row, imports, exports, section_rows


def main() -> None:
    manifest = {}
    with (ROOT / "inputs" / "original" / "manifest.csv").open(newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(stream):
            if row["path"].startswith("inputs/original/binaries/"):
                manifest[Path(row["path"]).name.lower()] = row["sha256"]
    rows, imports, exports, sections = [], [], [], []
    for path in sorted(BIN_DIR.iterdir()):
        if path.is_file() and path.suffix.lower() in {".exe", ".dll"}:
            row, file_imports, file_exports, file_sections = parse_one(path, manifest)
            rows.append(row)
            imports.extend(file_imports)
            exports.extend(file_exports)
            sections.extend(file_sections)
    outputs = {
        "binaries.csv": (rows, list(rows[0]) if rows else []),
        "imports.csv": (imports, ["binary", "library", "hint_or_ordinal", "symbol"]),
        "exports.csv": (exports, ["binary", "ordinal", "rva", "symbol"]),
        "sections.csv": (sections, ["binary", "section", "size_hex", "vma", "type"]),
    }
    for name, (records, fields) in outputs.items():
        with (TRACKER / name).open("w", newline="", encoding="utf-8") as stream:
            writer = csv.DictWriter(stream, fieldnames=fields)
            writer.writeheader()
            writer.writerows(records)
    print(f"Wrote {len(rows)} binaries, {len(imports)} import symbols, {len(exports)} export rows, and {len(sections)} section rows.")


if __name__ == "__main__":
    main()
