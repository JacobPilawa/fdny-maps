#!/usr/bin/env python3
"""Bundle a map's authored EVS sources and referenced WAV sounds."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
from pathlib import Path


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def resolve_casefold(root: Path, relative: str) -> Path:
    current = root
    for component in Path(relative.replace("\\", "/")).parts:
        if component in ("", "."):
            continue
        if component == "..":
            current = current.parent
            continue
        if not current.is_dir():
            raise FileNotFoundError(f"Missing source directory while resolving {relative!r}")
        matches = [entry for entry in current.iterdir() if entry.name.casefold() == component.casefold()]
        if len(matches) != 1:
            raise FileNotFoundError(f"Expected one case-insensitive match for {relative!r} at {current}")
        current = matches[0]
    if not current.is_file():
        raise FileNotFoundError(f"Source is not a file: {current}")
    return current


def resolve_exec(name: str, including: Path, original_root: Path) -> Path:
    candidates = [including.parent / name.replace("\\", "/")]
    candidates.extend((original_root / base / name.replace("\\", "/") for base in ("Data/Maps", "Data/Scripts", "Data")))
    for candidate in candidates:
        try:
            return resolve_casefold(candidate.parent, candidate.name)
        except FileNotFoundError:
            pass
    raise FileNotFoundError(f"EVS exec target {name!r} included from {including} was not found")


def collect_sources(entry: Path, original_root: Path) -> list[dict]:
    ordered: list[dict] = []
    visited: set[Path] = set()
    by_basename: dict[str, bytes] = {}
    exec_pattern = re.compile(r"^\s*exec\s+([^\s;]+)", re.IGNORECASE)

    def visit(path: Path) -> None:
        resolved = path.resolve()
        if resolved in visited:
            return
        visited.add(resolved)
        raw = path.read_bytes()
        name = path.name.casefold()
        previous = by_basename.get(name)
        if previous is not None and previous != raw:
            raise ValueError(f"EVS files with the same basename differ: {path.name}")
        by_basename[name] = raw
        text = raw.decode("latin-1")
        try:
            relative = path.resolve().relative_to(original_root.resolve()).as_posix()
        except ValueError:
            relative = path.name
        ordered.append({"path": relative, "text": text, "sha256": sha256(raw)})
        for line in text.splitlines():
            match = exec_pattern.match(line)
            if match:
                visit(resolve_exec(match.group(1), path, original_root))

    visit(entry)
    return ordered


def sound_names(sources: list[dict], entities_path: Path) -> set[str]:
    pattern = re.compile(r"\b(?:playsnd|loopsnd|stopsnd|ambientsnd)\s+([A-Za-z0-9_#.-]+)", re.IGNORECASE)
    texts = [source["text"] for source in sources]
    if entities_path.is_file():
        entities = json.loads(entities_path.read_text(encoding="utf-8"))
        for entity in entities:
            texts.extend(str(value) for key, value in entity.get("properties", {}).items()
                         if key.startswith("S_Script"))
    return {match.group(1) for text in texts for match in pattern.finditer(text)}


def parse_sound_definitions(path: Path) -> dict[str, dict]:
    definitions: dict[str, dict] = {}
    pattern = re.compile(
        r'^\s*ROC_SOUND_BUFFER(?P<kind>2D|3D)\s+(?P<name>\S+)\s+'
        r'(?:(?P<minimum>-?\d+(?:\.\d+)?)\s+(?P<maximum>-?\d+(?:\.\d+)?)\s+)?'
        r'"(?P<file>[^"]+)"', re.IGNORECASE)
    for line_number, line in enumerate(path.read_text(encoding="latin-1").splitlines(), 1):
        match = pattern.match(line)
        if not match:
            continue
        row = match.groupdict()
        row["kind"] = row["kind"].upper()
        row["minimum"] = float(row["minimum"]) if row["minimum"] else 0.0
        row["maximum"] = float(row["maximum"]) if row["maximum"] else 0.0
        row["definition_line"] = line_number
        definitions[row["name"].casefold()] = row
    return definitions


def export(map_key: str, original_root: Path, scene_package: Path) -> dict:
    map_evs = resolve_casefold(original_root, f"Data/Maps/{map_key}.evs")
    sources = collect_sources(map_evs, original_root)
    sound_defs_path = resolve_casefold(original_root, "Data/ResDefs/sound.rrf")
    definitions = parse_sound_definitions(sound_defs_path)
    wanted = sound_names(sources, scene_package / "entities.json")
    # Player fire exposure (0x7531 -> 00451640) plays these retail chatter
    # sets directly, outside EVS sound-command references.
    wanted.update(f"RC_Pain{i}" for i in range(4))
    wanted.update(f"RC_PassOut{i}" for i in range(3))
    # Player status update 00458400 dispatches RC_OxygenLow once below 0.25;
    # its one-entry chatter set is advanced and played by 00489110.
    wanted.add("RC_OxygenLow0")
    # The Player sound tables configure these shared 2D resources outside EVS:
    # 00451560 plays Breathe/OxyRelease around oxygen depletion/recovery, while
    # 00455AB0/00455F60 play the visor transition and lock events.
    wanted.update(("Breathe", "OxyRelease", "VisorUpDown", "VisorLock"))
    # FireRescue initializer 00448D60 selects one of these three named 3D loop
    # sounds for each authored fire; the choice is made outside EVS.
    wanted.update(("FireBurn1", "FireBurn2", "FireBurn3"))
    sounds_dir = scene_package / "sounds"
    sounds_dir.mkdir(parents=True, exist_ok=True)
    sound_rows = []
    for name in sorted(wanted, key=str.casefold):
        definition = definitions.get(name.casefold())
        if definition is None:
            raise ValueError(f"No sound.rrf definition found for EVS sound {name!r}")
        wav_path = resolve_casefold(original_root, definition["file"])
        output_name = re.sub(r"[^A-Za-z0-9._-]", "_", definition["name"]) + ".wav"
        relative = f"sounds/{output_name}"
        shutil.copyfile(wav_path, scene_package / relative)
        sound_rows.append({
            "name": definition["name"],
            "kind": definition["kind"],
            "minimum_distance": definition["minimum"],
            "maximum_distance": definition["maximum"],
            "file": relative,
            "source_file": wav_path.resolve().relative_to(original_root.resolve()).as_posix(),
            "source_sha256": sha256(wav_path.read_bytes()),
            "definition_file": sound_defs_path.resolve().relative_to(original_root.resolve()).as_posix(),
            "definition_sha256": sha256(sound_defs_path.read_bytes()),
            "definition_line": definition["definition_line"],
        })
    bundle = {
        "format": "fdny-evs-sound-bundle-v1",
        "map_key": map_key,
        "sources": sources,
        "sound_resources": sound_rows,
    }
    (scene_package / "scripts.json").write_text(json.dumps({
        "format": bundle["format"], "map_key": map_key, "sources": sources,
    }, indent=2) + "\n", encoding="utf-8")
    (scene_package / "sounds.json").write_text(json.dumps({
        "format": bundle["format"], "map_key": map_key,
        "resources": sound_rows,
    }, indent=2) + "\n", encoding="utf-8")
    return bundle


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--map-key", required=True, help="same-key EVS filename stem")
    parser.add_argument("--original-root", type=Path, required=True, help="copied inputs/original directory")
    parser.add_argument("--scene-package", type=Path, required=True, help="exported scene package directory")
    args = parser.parse_args()
    bundle = export(args.map_key, args.original_root, args.scene_package)
    print(json.dumps({
        "map_key": bundle["map_key"],
        "script_sources": len(bundle["sources"]),
        "sound_resources": len(bundle["sound_resources"]),
        "source_files": [source["path"] for source in bundle["sources"]],
        "sound_names": [sound["name"] for sound in bundle["sound_resources"]],
    }, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
