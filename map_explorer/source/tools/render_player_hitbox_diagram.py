#!/usr/bin/env python3
from __future__ import annotations

from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "extracted" / "player_collision_extents.png"


def font(size: int, bold: bool = False):
    candidates = [
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Verdana Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Verdana.ttf",
    ]
    for candidate in candidates:
        try:
            return ImageFont.truetype(candidate, size)
        except Exception:
            pass
    return ImageFont.load_default()


def draw_box(draw, origin, size, label, dims, fill, outline):
    x, y = origin
    w, h = size
    depth = min(w, h) * 0.28
    front = [(x, y + depth), (x + w, y + depth), (x + w, y + h + depth), (x, y + h + depth)]
    top = [(x, y + depth), (x + depth, y), (x + w + depth, y), (x + w, y + depth)]
    side = [(x + w, y + depth), (x + w + depth, y), (x + w + depth, y + h), (x + w, y + h + depth)]
    draw.polygon(top, fill=fill, outline=outline)
    draw.polygon(side, fill=tuple(max(0, c - 22) for c in fill), outline=outline)
    draw.polygon(front, fill=tuple(min(255, c + 10) for c in fill), outline=outline)
    draw.text((x, y + h + depth + 18), label, font=font(23, True), fill=(25, 25, 25))
    draw.text((x, y + h + depth + 48), dims, font=font(18), fill=(70, 70, 70))


def main():
    img = Image.new("RGB", (1700, 1050), (248, 248, 244))
    draw = ImageDraw.Draw(img)
    title = font(42, True)
    body = font(22)
    small = font(18)
    draw.text((60, 50), "Player Collision / Actor Extent Evidence", font=title, fill=(20, 20, 20))
    draw.text((60, 112), "Render hint = graphics/visibility bounds. Swept collision = movement test through the world.", font=body, fill=(55, 55, 55))

    draw_box(
        draw,
        (100, 220),
        (260, 260),
        "Render hint box (graphics)",
        "min=(-24,-24,-24), max=(24,24,24)",
        (184, 211, 235),
        (34, 78, 120),
    )
    draw_box(
        draw,
        (520, 310),
        (110, 110),
        "Swept collision box (gameplay clue)",
        "radius arg 15.0 -> extents +/-7.5",
        (238, 179, 108),
        (136, 75, 20),
    )

    # Orthographic views of the stronger swept-collision box.
    bx, by = 940, 230
    draw.text((bx, by), "Swept collision box views", font=font(28, True), fill=(20, 20, 20))
    views = [("Top X/Z", 0), ("Front X/Y", 1), ("Side Z/Y", 2)]
    for i, (name, _) in enumerate(views):
        x = bx + i * 220
        y = by + 70
        draw.rectangle((x, y, x + 140, y + 140), fill=(246, 211, 167), outline=(136, 75, 20), width=3)
        draw.line((x + 70, y - 18, x + 70, y + 158), fill=(90, 90, 90), width=1)
        draw.line((x - 18, y + 70, x + 158, y + 70), fill=(90, 90, 90), width=1)
        draw.text((x, y + 160), name, font=font(20, True), fill=(30, 30, 30))
        draw.text((x, y + 188), "15u wide", font=small, fill=(70, 70, 70))

    notes_x, notes_y = 90, 690
    draw.text((notes_x, notes_y), "Evidence", font=font(28, True), fill=(20, 20, 20))
    notes = [
        "FUN_00492340 writes -24.0 and +24.0 then calls geActor_SetRenderHintExtBox.",
        "That render hint helps Genesis3D render/cull the actor; it is not direct movement collision proof.",
        "The wrapper at 0x00482740 pushes 0x41700000 (15.0) into FUN_00490000.",
        "FUN_00490000 multiplies that value by constant 0.5 at 0x004de6f8, creating +/-7.5 extents.",
        "Because this is passed into geWorld_Collision as an extbox, it behaves like a small swept AABB/cube.",
        "Caveat: more tracing is still needed to prove every player movement branch uses only this 15.0 sweep.",
    ]
    y = notes_y + 42
    for note in notes:
        draw.text((notes_x, y), note, font=body, fill=(45, 45, 45))
        y += 38

    OUT.parent.mkdir(parents=True, exist_ok=True)
    img.save(OUT, quality=95)
    print(OUT)


if __name__ == "__main__":
    main()
