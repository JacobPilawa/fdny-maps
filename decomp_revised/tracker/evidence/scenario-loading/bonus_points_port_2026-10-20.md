# Bonus Points command trace and host integration

## Retail evidence

Static analysis used Ghidra 12.0.4 against
`inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The authored sources were also hashed:

| File | SHA-256 |
| --- | --- |
| `Data/Scripts/bonusitems.evs` | `6615a42fd7e21bd60d54c9b06b51dc31d2c3dcc8fe0c21f9c8c63902338964e6` |
| `Data/Scripts/rescue.evs` | `4b6d986fb9eb04f593956d7dcf3b8290ada0858a777dfaa29e204f1f4c0195c7` |
| `Data/Maps/gas_r.evs` | `1f83f48debdbfe916c0b5e818f35e0dbf6cb9046a996dfecd75882e27e3fa8c0` |

`00451870` recognizes `addbonus` case-insensitively, parses each argument
with the retail integer parser `0040CD50`, and calls Player method `0044EF10`.
That method adds the signed 32-bit amount to the global `"Bonus Points"`
statistic through `00486980`, then dispatches Player event 7 through
`004571A0`. The stat manager adds to the statistic's 32-bit value word
(record `+0x18`; the decompiler temporary base is offset by `0xC`); addition
wraps in 32 bits. The complementary getter `0044EBE0` reads that same word
from the same named statistic, while `00424710` registers
Bonus Points with the result-stat table.

The Player update `00458400` reads the statistic. When it is zero, it hides the
widget at status-subobject `+0x480` (absolute Player `+0xA74`); when nonzero it
refreshes it through `004574C0`. The refresh formats the integer with `%i` and
submits it to the generic text component at widget `+0x130` (absolute Player
`+0xBA4`) through `0048C2A0`. Event 7 resets the widget's animation clock and
zero-valued interpolation state via `00455F10`.

## Fresh score-widget trace

The Player constructor `0044D550` passes status subobject `Player+0x5F4` to
`004576B0`; that constructor creates the score sprite at subobject `+0x480`.
The widget uses resource `0x1F6` (502), mapped in `Data/ResDefs/rescue.rrf`
to color `Data/sprites/tools.bmp` and alpha `Data/sprites/rescbon_a.bmp`.
Both source bitmaps are 64x64. Their SHA-256 values are
`40bca3c159f2824349b545294548e44021442b7319a36b4e87e808e42a8a6856` and
`c2eb0c761de02bea34f6c568875c1a65d1ea0899d7347533e7ffb8807df30755`,
respectively. The widget setup at `00455DE0` applies the vertical texture band
from `28/63` through `56/63`; the matching alpha crop visibly contains the
`+BONUS+` title.

`00455DE0` installs four source coordinate vectors in its timed track:

| Seconds | Four source coordinates |
| ---: | --- |
| 0.0 | `(320, 320, 180, 180)` |
| 0.1 | `(256, 384, 176, 304)` |
| 1.0 | `(256, 384, 176, 304)` |
| 1.1 | `(5, 69, 83, 111)` |

Event 7 calls `00455F10` on this same object, restarts its clock, and samples
the track at time zero. The Player update calls `00455C00` and refreshes the
formatted total through `004574C0`. `00455DE0` sets text child style index 3
through `0048ABB0` and passes `0.65` through `0048AB40`. `004574C0` obtains the
parent rectangle through `0047DCE0` and gives the child an expanded rectangle
and `%i` total through `0048C2A0`.

The style-3 text profile is initialized by `004DA720` through `0048B210` from
the full printable ASCII charset. Profile index 3 points to resource 64000:
`Data/sprites/confont.bmp` and `confont_a.bmp`, both 128x128 8-bit bitmaps.
`0048B7C0` / `0048AE20` draw 8x14 glyph cells in a 16-column atlas; digits
`0`-`9` are cells 52-61 (columns 4-13, row 3), and minus is cell 77 (column
13, row 4). Space is skipped. The text child has unit scales at `+0xF8/+0xFC`
and glyph origin fields `+0x24/+0x28`; initial vertex RGB and alpha are 255.
The bonus caller sets text modes `+0xE0=2`, `+0xE4=1`. `0048BD40` measures
line/glyph bounds and derives a Y origin from the lower edge minus scaled
glyph height; it does not center or reposition the text by itself. How the
measured bounds affect the final screen placement remains unresolved.

The four coordinate vectors and key times are exact Ghidra observations, but
their mapping to rendered corners, y-axis conversion at non-640x480 viewports,
the `0.65`/`+0x120` render-state meaning, actual alignment, and runtime pixels
remain uncertain. This is enough to identify the authored sprite/font and
glyph metrics, but not to claim visual parity.

Retail `bonusitems.evs` defines touch rewards from 100 to 500 points and routes
them through `tell player (addbonus N)`. Gas R has its own EVS include of
`bonusitems.evs`; the general rescue script also includes it. The duplicate
`BONUS63` definition in the authored file is preserved as-is; no missing
`BONUS64` is inferred.

## Shared host HUD integration

The shared scene viewer accepts Player `addbonus` commands and accumulates their
signed amounts in its `Bonus Points` result statistic using the existing
32-bit wrapping statistic helper. Each nonzero result restarts the widget
animation clock; a wrapped zero hides the widget, following the Player update's
zero/nonzero visibility branch.

`BonusPointsHudRenderer` draws the retail color/alpha crop, follows the four
source rectangles with truncating linear interpolation, and samples the
resource-64000 8x14 cells for decimal digits and minus. It loads the copied
assets produced by `tools/export_bonus_points_sprites.py`, whose manifest stores
source and bundled hashes for the resource-502 and resource-64000 files. The
builder includes this export in the macOS app's `actor_exports` bundle. The HUD
is integrated into the shared host used by the rescue missions and dedicated
PlayerHose scenes.

The host uses the traced parent-relative `+0x24` X anchor and derives its text
row from lower edge minus the 14-pixel glyph height. Exact text alignment,
coordinate-vector-to-render-corner mapping, viewport scaling parity, alpha
channel interpretation, filtering, and the meaning of the `0.65`/`+0x120`
render parameters remain provisional. OpenGL output is not a retail pixel
comparison. Result-screen rendering and terminal mission handoff remain
unimplemented.

No live retail pickup, host interaction, or parity check was performed for this
slice. The macOS build is a compile/package check only; the source map proves
static command routing, not which authored touch references become reachable
during play.
