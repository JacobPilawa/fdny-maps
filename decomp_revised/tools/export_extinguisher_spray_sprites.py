#!/usr/bin/env python3
"""Bundle the retail resource-14 extinguisher particle texture pair locally."""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def convert_indexed_bmp_to_24bit(source: Path, destination: Path) -> str:
    """Write a valid 24-bit BMP for SDL while retaining the source palette colors.

    Retail bsmoke.bmp declares a 254-entry palette but contains pixel indices
    254 and 255. SDL rejects that malformed-but-shipped BMP. The retail loader's
    handling of those two undeclared colors is unknown, so the host converter
    pads them with black and records that limitation in the manifest.
    """
    data = source.read_bytes()
    if len(data) < 54 or data[:2] != b"BM":
        raise ValueError(f"Not a supported Windows BMP: {source}")
    pixel_offset = struct.unpack_from("<I", data, 10)[0]
    dib_size = struct.unpack_from("<I", data, 14)[0]
    if dib_size < 40 or len(data) < 14 + dib_size:
        raise ValueError(f"Unsupported BMP header: {source}")
    width, signed_height = struct.unpack_from("<ii", data, 18)
    planes, bits_per_pixel = struct.unpack_from("<HH", data, 26)
    compression = struct.unpack_from("<I", data, 30)[0]
    colors_used = struct.unpack_from("<I", data, 46)[0]
    if width <= 0 or signed_height == 0 or planes != 1 or bits_per_pixel != 8 or compression != 0:
        raise ValueError(f"Expected an uncompressed 8-bit indexed BMP: {source}")

    height = abs(signed_height)
    palette_start = 14 + dib_size
    palette_count = colors_used or 256
    palette_end = palette_start + palette_count * 4
    if pixel_offset < palette_start or palette_end > pixel_offset or pixel_offset > len(data):
        raise ValueError(f"Invalid indexed BMP palette or pixel offset: {source}")
    palette = [tuple(data[offset:offset + 3][::-1])
               for offset in range(palette_start, palette_end, 4)]
    stride = ((width + 3) // 4) * 4
    if pixel_offset + stride * height > len(data):
        raise ValueError(f"Truncated indexed BMP pixels: {source}")

    rgba = bytearray(width * height * 4)
    missing_indices: set[int] = set()
    for file_row in range(height):
        y = height - 1 - file_row if signed_height > 0 else file_row
        row = data[pixel_offset + file_row * stride:pixel_offset + file_row * stride + width]
        for x, index in enumerate(row):
            if index < len(palette):
                red, green, blue = palette[index]
            else:
                red = green = blue = 0
                missing_indices.add(index)
            target = (y * width + x) * 4
            rgba[target:target + 4] = bytes((red, green, blue, 255))

    row_stride = (width * 3 + 3) & ~3
    pixels = bytearray()
    for y in range(height - 1, -1, -1):
        for x in range(width):
            offset = (y * width + x) * 4
            red, green, blue = rgba[offset:offset + 3]
            pixels.extend((blue, green, red))
        pixels.extend(b"\0" * (row_stride - width * 3))
    file_header = b"BM" + struct.pack("<IHHI", 54 + len(pixels), 0, 0, 54)
    dib_header = struct.pack("<IiiHHIIiiII", 40, width, height, 1, 24, 0,
                             len(pixels), 2835, 2835, 0, 0)
    destination.write_bytes(file_header + dib_header + pixels)
    if missing_indices:
        return "missing_palette_indices_padded_black=" + ",".join(
            str(index) for index in sorted(missing_indices))
    return "palette_complete"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--project-root", type=Path,
                        default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    project_root = args.project_root.resolve()
    source_root = project_root / "inputs" / "original" / "Data" / "sprites"
    definitions = project_root / "inputs" / "original" / "Data" / "ResDefs" / "rescue.rrf"
    output_root = project_root / ".local" / "actor_exports" / "extinguisher-spray"
    names = ["bsmoke.bmp", "sm_alpha.bmp"]

    missing = [str(path) for path in [definitions, *(source_root / name for name in names)]
               if not path.is_file()]
    if missing:
        raise SystemExit("Missing retail extinguisher resources: " + ", ".join(missing))

    output_root.mkdir(parents=True, exist_ok=True)
    records = []
    for name in names:
        source = source_root / name
        destination = output_root / name
        conversion = convert_indexed_bmp_to_24bit(source, destination)
        records.append({
            "file": name,
            "source": str(source.relative_to(project_root)),
            "source_sha256": sha256(source),
            "bundled_sha256": sha256(destination),
            "host_conversion": conversion,
        })
    manifest = {
        "format": "fdny-extinguisher-spray-bundle-v2",
        "resource_id": 14,
        "resource_definition": str(definitions.relative_to(project_root)),
        "resource_definition_sha256": sha256(definitions),
        "resource_definition_entry": (
            'ROC_ALPHA_TEXTURE 14 "data\\sprites\\bsmoke.bmp" '
            '"data\\sprites\\sm_alpha.bmp"'
        ),
        "mapping_status": (
            "rescue.rrf declares the pair for resource 14; the particle pool's "
            "runtime manager-to-definition link remains unverified"
        ),
        "host_conversion_note": (
            "Indexed source BMPs are converted to valid 24-bit BMPs for SDL. "
            "bsmoke.bmp uses undeclared palette indices 254/255; the host pads "
            "those colors with black. Retail loader behavior for those pixels is unknown."
        ),
        "sprites": records,
    }
    (output_root / "manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Exported {len(records)} extinguisher particle sprites to {output_root}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
