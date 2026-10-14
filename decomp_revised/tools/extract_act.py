#!/usr/bin/env python3
"""Extract Genesis3D ACT body geometry without loading proprietary game code.

FDNY's .act files are nested VF00 virtual filesystems.  This reader follows
the public Genesis3D VFile/dirtree.c and Actor/body.c binary layouts and emits
auditable JSON plus a bind-pose OBJ.
"""

from __future__ import annotations

import argparse
import json
import math
import struct
import zlib
from dataclasses import dataclass, field
from pathlib import Path


VF00 = 0x30304656
DT01 = 0x31305444
BODY = 0x5E444F42
SBKB = 0x424B4253
DIRECTORY = 0x2


class Reader:
    def __init__(self, data: bytes, offset: int = 0):
        self.data = data
        self.offset = offset

    def take(self, fmt: str):
        values = struct.unpack_from("<" + fmt, self.data, self.offset)
        self.offset += struct.calcsize("<" + fmt)
        return values[0] if len(values) == 1 else values

    def bytes(self, count: int) -> bytes:
        result = self.data[self.offset:self.offset + count]
        if len(result) != count:
            raise ValueError(f"short read at 0x{self.offset:x}: wanted {count}")
        self.offset += count
        return result


@dataclass
class VfsEntry:
    name: str
    attributes: int
    size: int
    offset: int
    children: list["VfsEntry"] = field(default_factory=list)

    @property
    def directory(self) -> bool:
        return bool(self.attributes & DIRECTORY)


@dataclass
class Vfs:
    data: bytes
    entries: list[VfsEntry]

    def find(self, path: str) -> VfsEntry:
        parts = [part for part in path.replace("\\", "/").split("/") if part]
        entries = self.entries
        found = None
        for part in parts:
            found = next((entry for entry in entries if entry.name.lower() == part.lower()), None)
            if found is None:
                raise KeyError(path)
            entries = found.children
        if found is None:
            raise KeyError(path)
        return found

    def read(self, path: str) -> bytes:
        entry = self.find(path)
        if entry.directory:
            raise IsADirectoryError(path)
        return self.data[entry.offset:entry.offset + entry.size]


def read_vfs(data: bytes) -> Vfs:
    r = Reader(data)
    signature = r.take("I")
    version = r.take("H")
    r.take("H")  # Win32 alignment before geBoolean.
    dispersed, directory_offset, data_length, end_position = r.take("iiii")
    if signature != VF00 or version != 0 or dispersed:
        raise ValueError("unsupported Genesis3D VFS header")
    if not (24 <= directory_offset <= len(data)):
        raise ValueError(f"invalid VFS directory offset 0x{directory_offset:x}")
    directory = Reader(data, directory_offset)
    if directory.take("I") != DT01:
        raise ValueError("missing DT01 directory")
    directory_size = directory.take("i")

    def read_node() -> VfsEntry | None:
        terminator = directory.take("i")
        if terminator == -1:
            return None
        if terminator != 0:
            raise ValueError(f"invalid directory marker {terminator}")
        name_length = directory.take("i")
        name = directory.bytes(name_length).rstrip(b"\0").decode("latin-1")
        directory.take("II")  # geVFile_Time
        attributes, size, offset, hint_length = directory.take("Iiii")
        directory.bytes(hint_length)
        children: list[VfsEntry] = []
        while (child := read_node()) is not None:
            children.append(child)
            # ReadTree consumes the child's full sibling chain itself.
            break
        entry = VfsEntry(name, attributes, size, offset, children)
        sibling = read_node()
        if sibling is not None:
            # Keep the C recursive encoding but expose siblings as a list.
            setattr(entry, "_sibling", sibling)
        return entry

    def flatten_chain(node: VfsEntry | None) -> list[VfsEntry]:
        result: list[VfsEntry] = []
        while node is not None:
            sibling = getattr(node, "_sibling", None)
            if hasattr(node, "_sibling"):
                delattr(node, "_sibling")
            node.children = flatten_chain(node.children[0] if node.children else None)
            result.append(node)
            node = sibling
        return result

    root = read_node()
    if root is None:
        raise ValueError("empty VFS directory")
    roots = flatten_chain(root)
    if directory.offset > directory_offset + directory_size:
        raise ValueError("directory parser exceeded declared size")
    # Genesis writes an empty, directory-valued root node.
    entries = roots[0].children if len(roots) == 1 and roots[0].name == "" else roots
    if data_length > len(data) or end_position > len(data):
        raise ValueError("truncated VFS")
    return Vfs(data, entries)


def read_string_block(r: Reader) -> list[str]:
    if r.take("I") != SBKB:
        raise ValueError(f"expected SBKB at 0x{r.offset - 4:x}")
    count, size = r.take("iI")
    block = r.bytes(size)
    offsets = struct.unpack_from("<" + "i" * count, block) if count else ()
    strings = []
    for offset in offsets:
        end = block.index(0, offset)
        strings.append(block[offset:end].decode("latin-1"))
    return strings


def matrix_transform(m: list[float], p: list[float]) -> list[float]:
    return [
        m[0] * p[0] + m[1] * p[1] + m[2] * p[2] + m[9],
        m[3] * p[0] + m[4] * p[1] + m[5] * p[2] + m[10],
        m[6] * p[0] + m[7] * p[1] + m[8] * p[2] + m[11],
    ]


def matrix_multiply(a: list[float], b: list[float]) -> list[float]:
    # Stored as three matrix rows followed by XYZ translation.
    result = [0.0] * 12
    for row in range(3):
        for col in range(3):
            result[row * 3 + col] = sum(a[row * 3 + k] * b[k * 3 + col] for k in range(3))
    transformed_translation = matrix_transform(a, b[9:12])
    result[9:12] = transformed_translation
    return result


def read_body(geometry: bytes) -> dict:
    r = Reader(geometry)
    body_type, version = r.take("II")
    if body_type != BODY or version != 0xF1:
        raise ValueError("unsupported body geometry")
    bounds_min = list(r.take("fff"))
    bounds_max = list(r.take("fff"))

    vertex_count = r.take("h")
    vertices = []
    for _ in range(vertex_count):
        x, y, z, u, v, lod, bone = r.take("fffffbxh")
        vertices.append({"position": [x, y, z], "uv": [u, v], "lodMask": lod, "bone": bone})

    normal_count = r.take("h")
    normals = []
    for _ in range(normal_count):
        x, y, z, lod, bone = r.take("fffbxh")
        normals.append({"normal": [x, y, z], "lodMask": lod, "bone": bone})

    bone_count = r.take("h")
    bones = []
    for _ in range(bone_count):
        minimum = list(r.take("fff"))
        maximum = list(r.take("fff"))
        attachment = list(r.take("ffffffffffff"))
        parent = r.take("h")
        r.take("H")  # structure tail alignment
        bones.append({"boundsMin": minimum, "boundsMax": maximum, "attachment": attachment, "parent": parent})
    bone_names = read_string_block(r)
    if len(bone_names) != bone_count:
        raise ValueError("bone-name count mismatch")
    for bone, name in zip(bones, bone_names):
        bone["name"] = name

    material_count = r.take("h")
    materials = []
    for _ in range(material_count):
        bitmap, red, green, blue = r.take("Ifff")
        materials.append({"bitmapId": bitmap, "textured": bitmap != 0, "color": [red, green, blue]})
    material_names = read_string_block(r)
    if len(material_names) != material_count:
        raise ValueError("material-name count mismatch")
    for material, name in zip(materials, material_names):
        material["name"] = name

    lod_count = r.take("i")
    lods = []
    for _ in range(lod_count):
        face_count = r.take("I")
        faces = []
        for _ in range(face_count):
            values = r.take("hhhhhhh")
            faces.append({"vertices": list(values[:3]), "normals": list(values[3:6]), "material": values[6]})
        lods.append(faces)

    # gePose_Clear: each bone's bind transform is parent * attachment.
    bind = []
    for bone in bones:
        parent = bone["parent"]
        bind.append(bone["attachment"] if parent < 0 else matrix_multiply(bind[parent], bone["attachment"]))
    for vertex in vertices:
        vertex["bindPosition"] = matrix_transform(bind[vertex["bone"]], vertex["position"])

    if r.offset != len(geometry):
        raise ValueError(f"unparsed geometry bytes: {len(geometry) - r.offset}")
    return {
        "format": "Genesis3D BODY 0xF1",
        "boundsMin": bounds_min,
        "boundsMax": bounds_max,
        "vertices": vertices,
        "normals": normals,
        "bones": bones,
        "materials": materials,
        "lods": lods,
    }


def read_keyframes(r: Reader, quaternion: bool) -> dict:
    block_size = r.take("I")
    block = Reader(r.bytes(block_size))
    frame_flags, count = block.take("II")
    interpolation = (frame_flags >> 16) & 0xFF
    compression = (frame_flags >> 8) & 0xFF
    looping = bool(frame_flags & 1)
    if compression & 2:
        first, step = block.take("ff")
        times = [first + index * step for index in range(count)]
    else:
        times = [block.take("f") for _ in range(count)]

    values: list[list[float]] = []
    if quaternion and compression & 1:
        axis = block.take("fff")
        for _ in range(count):
            half_angle = block.take("f") * 0.5
            sine = math.sin(half_angle)
            values.append([axis[0] * sine, axis[1] * sine, axis[2] * sine, math.cos(half_angle)])
    else:
        width = 4 if quaternion else 3
        values = [list(block.take("f" * width)) for _ in range(count)]
    if block.offset != len(block.data):
        raise ValueError(f"unparsed keyframe bytes: {len(block.data) - block.offset}")
    return {
        "interpolation": interpolation,
        "compression": compression,
        "looping": looping,
        "times": times,
        "values": values,
    }


def read_path(r: Reader) -> dict:
    header = r.take("I")
    if header >> 16 != 0x1001:
        raise ValueError(f"unsupported path header 0x{header:08x}")
    result = {
        "looping": False,
        "translationInterpolation": (header >> 9) & 0x7F,
        "rotationInterpolation": (header >> 2) & 0x7F,
        "translation": None,
        "rotation": None,
    }
    if header & 2:
        result["translation"] = read_keyframes(r, False)
        result["looping"] = result["translation"]["looping"]
    if header & 1:
        result["rotation"] = read_keyframes(r, True)
        result["looping"] = result["looping"] or result["rotation"]["looping"]
    return result


def read_motion(data: bytes) -> dict:
    r = Reader(data)
    if r.take("I") != 0x424E544D or r.take("I") != 0xF0:
        raise ValueError("unsupported Genesis3D motion")
    descriptor = r.take("I")
    name_length = descriptor & 0xFFFF
    maintain_names = bool(descriptor & (1 << 16))
    node_type = descriptor >> 24
    name = r.bytes(name_length).rstrip(b"\0").decode("latin-1") if name_length else ""
    if node_type != 2:
        raise ValueError(f"unsupported motion node type {node_type}")
    path_count, checksum, flags = r.take("iiI")
    if flags & 1:
        raise ValueError("motion event blocks are not yet supported")
    path_names = read_string_block(r) if flags & 2 else [str(i) for i in range(path_count)]
    paths = [read_path(r) for _ in range(path_count)]
    if len(path_names) != path_count:
        raise ValueError("motion path-name count mismatch")
    for path, path_name in zip(paths, path_names):
        path["name"] = path_name
    if r.offset != len(data):
        raise ValueError(f"unparsed motion bytes: {len(data) - r.offset}")
    starts, ends = [], []
    for path in paths:
        for channel in (path["translation"], path["rotation"]):
            if channel and channel["times"]:
                starts.append(channel["times"][0])
                ends.append(channel["times"][-1])
    return {
        "name": name,
        "maintainNames": maintain_names,
        "nameChecksum": checksum,
        "start": min(starts) if starts else 0,
        "end": max(ends) if ends else 0,
        "paths": paths,
    }


def png_chunk(name: bytes, payload: bytes) -> bytes:
    return struct.pack(">I", len(payload)) + name + payload + struct.pack(">I", zlib.crc32(name + payload))


def write_png(path: Path, width: int, height: int, rgba: bytes) -> None:
    rows = b"".join(b"\0" + rgba[y * width * 4:(y + 1) * width * 4] for y in range(height))
    ihdr = struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0)
    path.write_bytes(
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", ihdr)
        + png_chunk(b"IDAT", zlib.compress(rows, 9))
        + png_chunk(b"IEND", b"")
    )


def raw_color(pixel_format: int, pixel: bytes) -> tuple[int, int, int, int]:
    """Decode the Genesis formats used by ACT palettes (pixelformat.c masks)."""
    if pixel_format in (9, 11):
        return pixel[0], pixel[1], pixel[2], 255
    if pixel_format == 10:
        return pixel[2], pixel[1], pixel[0], 255
    if pixel_format in (12, 16):  # memory bytes X/B/G/R or A/B/G/R
        return pixel[3], pixel[2], pixel[1], pixel[0] if pixel_format == 16 else 255
    if pixel_format in (13, 17):  # memory bytes B/G/R/X or B/G/R/A
        return pixel[2], pixel[1], pixel[0], pixel[3] if pixel_format == 17 else 255
    if pixel_format in (14, 18):  # memory bytes X/R/G/B or A/R/G/B
        return pixel[1], pixel[2], pixel[3], pixel[0] if pixel_format == 18 else 255
    if pixel_format in (15, 19):  # memory bytes R/G/B/X or R/G/B/A
        return pixel[0], pixel[1], pixel[2], pixel[3] if pixel_format == 19 else 255
    raise ValueError(f"unsupported palette pixel format {pixel_format}")


def decode_gebm(data: bytes) -> tuple[int, int, bytes]:
    r = Reader(data)
    if r.take("I") != 0x6D426547:
        raise ValueError("bitmap is not GeBm")
    version = r.take("B")
    if version >> 4 != 4:
        raise ValueError(f"unsupported GeBm version 0x{version:02x}")
    flags, pixel_format, mip_info = r.take("BBB")
    maximum_mip = mip_info >> 4
    if flags & 1:
        dimensions = r.take("B")
        width, height = 1 << (dimensions >> 4), 1 << (dimensions & 0xF)
    elif flags & (1 << 5):
        width, height = r.take("BB")
    else:
        width, height = r.take("HH")
    if flags & 4:
        raise ValueError("GeBm alpha sidecars are not yet supported")

    color_key = None
    if flags & 2:
        if pixel_format != 10:
            raise ValueError(f"unsupported color-keyed GeBm pixel format {pixel_format}")
        color_key_bytes = r.bytes(3)
        color_key = raw_color(pixel_format, color_key_bytes)[:3]

    palette: list[tuple[int, int, int, int]] = []
    if flags & 8:
        if pixel_format != 1:
            raise ValueError(f"unsupported paletted GeBm pixel format {pixel_format}")
        palette_flags = r.take("B")
        palette_format = palette_flags & 0x1F
        palette_size = 256 if palette_flags & 0x20 else r.take("B")
        if palette_flags & 0x40:
            raise ValueError("compressed GeBm palette is not supported")
        if palette_format not in range(9, 20):
            raise ValueError(f"unsupported GeBm palette pixel format {palette_format}")
        bytes_per_palette_entry = 3 if palette_format in (9, 10, 11) else 4
        palette = [
            raw_color(palette_format, r.bytes(bytes_per_palette_entry))
            for _ in range(palette_size)
        ]
        bytes_per_pixel = 1
    elif pixel_format == 10:
        # Genesis pixel format 10 is 24-bit BGR in memory. Ghidra's shipped
        # gePixelFormat_BytesPerPel / gePixelFormat_GetPixel path reads a
        # three-byte color key using the same format when flags bit 1 is set.
        bytes_per_pixel = 3
    else:
        raise ValueError(f"unsupported GeBm pixel format {pixel_format}, flags 0x{flags:02x}")

    mip_flags = r.take("B")
    mip = mip_flags & 0xF
    if mip != 0 or mip > maximum_mip or mip_flags & 0x30:
        raise ValueError(f"unsupported GeBm mip flags 0x{mip_flags:02x}")
    if maximum_mip != 0:
        raise ValueError(f"multi-level GeBm mip chain is not supported (max mip {maximum_mip})")
    pixels = r.bytes(width * height * bytes_per_pixel)
    rgba = bytearray()
    if palette:
        for index in pixels:
            rgba.extend(palette[index] if index < len(palette) else (0, 0, 0, 0))
    else:
        for offset in range(0, len(pixels), 3):
            pixel = pixels[offset:offset + 3]
            color = raw_color(pixel_format, pixel)
            if color_key is not None and color[:3] == color_key:
                rgba.extend((*color[:3], 0))
            else:
                rgba.extend(color)

    # The retail reader consumes the next level marker after each mip. For
    # these single-level ACT images 0x0f terminates the chain (low nibble 15).
    end_marker = r.take("B")
    if end_marker != 0x0F or r.offset != len(data):
        raise ValueError(
            f"unsupported GeBm tail: marker 0x{end_marker:02x}, "
            f"{len(data) - r.offset} trailing bytes"
        )
    return width, height, bytes(rgba)


def entry_report(entries: list[VfsEntry]) -> list[dict]:
    return [{
        "name": entry.name,
        "directory": entry.directory,
        "size": entry.size,
        "offset": entry.offset,
        "children": entry_report(entry.children),
    } for entry in entries]


def write_obj(body: dict, path: Path) -> None:
    lines = ["# Genesis3D ACT bind pose extracted from the retail asset"]
    for vertex in body["vertices"]:
        x, y, z = vertex["bindPosition"]
        lines.append(f"v {x:.7g} {y:.7g} {z:.7g}")
    for vertex in body["vertices"]:
        u, v = vertex["uv"]
        lines.append(f"vt {u:.7g} {1.0 - v:.7g}")
    current_material = None
    for face in body["lods"][0]:
        material = body["materials"][face["material"]]["name"]
        if material != current_material:
            lines.append(f"usemtl {material}")
            current_material = material
        indices = [index + 1 for index in face["vertices"]]
        lines.append("f " + " ".join(f"{index}/{index}" for index in indices))
    path.write_text("\n".join(lines) + "\n")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    outer = read_vfs(args.input.read_bytes())
    body_vfs = read_vfs(outer.read("Body"))
    body = read_body(body_vfs.read("Geometry"))
    motion_entries = sorted(outer.find("Motions").children, key=lambda entry: int(entry.name))
    motions = [read_motion(outer.data[entry.offset:entry.offset + entry.size]) for entry in motion_entries]
    payload = {"source": str(args.input), "outerVfs": entry_report(outer.entries), "bodyVfs": entry_report(body_vfs.entries), "motions": motions, **body}

    args.output.mkdir(parents=True, exist_ok=True)
    stem = args.input.stem.lower()
    (args.output / f"{stem}.json").write_text(json.dumps(payload, indent=2) + "\n")
    write_obj(body, args.output / f"{stem}.obj")
    bitmap_directory = body_vfs.find("Bitmaps")
    for entry in bitmap_directory.children:
        bitmap = body_vfs.data[entry.offset:entry.offset + entry.size]
        width, height, rgba = decode_gebm(bitmap)
        write_png(args.output / f"{stem}_{entry.name}.png", width, height, rgba)
    print(f"{args.input.name}: {len(body['vertices'])} vertices, {len(body['lods'][0])} faces, "
          f"{len(body['bones'])} bones, {len(body['materials'])} materials")


if __name__ == "__main__":
    main()
