# Player hint widget render trace

## Retail evidence

The hash-pinned retail executable is `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The trace used Ghidra's headless analyzer against the existing
`analysis/ghidra/FDNYPort` project; it did not use or alter a live Ghidra
instance. Existing endpoint exports are
[`0044ee50_player_hint_route_fresh.c`](0044ee50_player_hint_route_fresh.c) and
[`004564c0_hint_window_constructor_fresh.c`](004564c0_hint_window_constructor_fresh.c).

`0044EE50` forwards the hint duration and string to the subobject at `this+0x5F4`
through `004564C0`. The constructor stores the duration at `+0x6B4`, updates
duration-scaled child values, resets the prior component at `+0x7C4` with
`0047D300`, then constructs a generic text widget at `+0x6D8` through
`0048C780`. The input rectangle values encode opposite corners
`(120, 40)..(520, 440)`, plus two mode values `2, 2`. This is a 400×400
source-space rectangle centered at `(320, 240)` in 640×480.

The `0048C780` constructor parses `^(` … `)` markup into comma-separated color
components and line/text segments. Another caller, `004567D0`, supplies
`^(228,0,0)TOO HOT!\nCOOL OFF FAST!`, supporting that the markup controls text
color. The same constructor's reference list includes the player hint
constructor and unrelated briefing/UI widgets, so it is a shared text-widget
path. A separate call from `00456F70` passes a five-second message through
`004564C0`; the Player ScriptActor path forwards its authored duration.

`rescue.rrf` defines alpha-texture resources 1019 as
`sprites/sfont1.bmp` + `sfont1_a.bmp`, and 1020 as `sprites/font1.bmp` +
`font1_a.bmp`.

## Follow-up: pinned font and draw path

The direct font link is now established. In `FUN_004576B0`, the Player
constructor creates three child records at `Player+0x6BC` with
`FUN_0048AB20(1)` and wires them to the text widget at `Player+0x6D8` through
`FUN_0048AC60(..., 1, ..., 1)` (`.local/ghidra/oxygen-status-draw2.log`, lines
388–416). `FUN_0048AB20` maps selector 1 to font resource 1020. `FUN_0048ABB0`
sets the widget descriptor pointer to `0x00503104` (table base `0x005030F0`,
entry size `0x14`) and reads its resource ID from descriptor offset `+0x0C`.

The static initializer `FUN_004DA720` sets selector 0 to resource 1019 with
8×12 glyph cells and selector 1 to resource 1020 with 16×24 glyph cells; both
entries reference the alphabet string at `0x004E39C8`. That NUL-terminated
string is:

```text
ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+|`-=\{}:"<>?[];',./
```

The Player hint therefore uses the retail `font1.bmp` / `font1_a.bmp` pair
from resource 1020. Both pinned BMPs are 256×256, 24-bit images. The text
object constructor installs vtable `0x004E399C`; thunk `0x0048C110` jumps to
its draw callback `FUN_0048B7C0`. That callback resolves the bitmap and
descriptor, derives normalized glyph-cell steps from descriptor dimensions
and bitmap dimensions, skips spaces, and delegates individual glyph quads to
`FUN_0048AE20`. The glyph helper searches the descriptor alphabet, forms a
four-vertex quad, checks it against the active viewport, and submits it with
`geEngine_RenderPoly` (Genesis import slot `0x004DC0F8`). The `0x0048B7C0` and
`0x0048AE20` decomp exports are under
`reference/previous-work/decomp/evidence/item-pose/live-extinguisher/message-routing/`;
the retail function inventory ties both addresses to the pinned executable
hash above.

Color handling is also partly mapped. `FUN_0048B7C0` initializes fallback RGB
channels at offsets `+0xB8/+0xBC/+0xC0` to 255 and alpha byte `+0xB5` to 255
when no alpha override is active; an available sprite handle can modulate RGB.
`FUN_0048C780` parses `^(`…`)` text color markup into RGB triplets and passes
them to `FUN_004848C0`. The unrelated caller `FUN_004567D0` uses
`^(228,0,0)TOO HOT!\nCOOL OFF FAST!`, confirming the markup syntax. The
sprite-child tint/effect records attached to the hint have not been fully
decoded, so the runtime tint for unmarked hints remains less certain than the
fallback values.

The examined hint constructor and text draw callback contain no explicit
panel/background fill or backing-panel resource submission. The three child
records are all created using font resource 1020. This supports removing the
current synthetic panel from any claim of retail fidelity; it does not prove
that no separate layer elsewhere in the full UI can draw behind hints.

## Port comparison and open points

The shared viewer now loads the pinned resource-1020 color and alpha bitmaps,
uses the recovered glyph order and 16×24 cells, and draws text glyph quads with
no backing panel. It parses `^(R,G,B)` markup and defaults to white tint.
The host lays lines out within the source rectangle `(120,40)..(520,440)` and
centers the block; this is provisional because the meaning of mode `(2,2)`,
exact wrapping policy, and clipping behavior are not fully decoded. A separate
5×7 pixel font remains in the mission chooser only; that chooser is not the
Player hint widget.

The packaged `--hint-render-check` renders the source markup example
`^(228,0,0)TOO HOT!\nCOOL OFF FAST!` with 19 atlas glyphs and reports 15,751
changed pixels. Visual inspection shows the red retail glyphs without the
earlier opaque panel. The check also exercises strict visibility at start, one
tick after start, one tick before expiry, and exactly at expiry. These are host
checks of the recovered assets and timing rule, not a retail frame comparison.

The update callback `FUN_00458400` reads duration from `Player+0x6B4` and
start tick from `Player+0x6B8`. The constructor sets the start tick to
`DAT_00502AFC`; the update makes `Player+0x6D8` visible only when signed
`start < now` and `start + duration*1000.0f > normalizedNow`, otherwise it
hides it through `FUN_0040CB80`. `0x004E0ED4` is float 1000.0. Thus duration is
in seconds against a millisecond tick, with strict comparisons at both ends.
When the current tick is negative, the update adds its wrap constant before
the deadline comparison; exact rollover behavior is not a safe simplification
target.

No retail backing-panel texture, fill call, popup opacity, or transition curve
was established. The default text tint in the callback has a white fallback,
but the hint's attached sprite tint is not fully decoded.

The shared port uses the source start tick and duration in milliseconds for
ordinary no-wrap sessions, hiding the text at both strict boundaries. Retail's
negative-tick wrap normalization has not been ported. The render-check sample
uses the source rectangle but keeps line centering and wrapping provisional.

## Plain-language explanation

The game's hint text is drawn from its own bitmap font: each character occupies
a 16×24 cell in the `font1` atlas. Script hints default to white and can switch
to an authored RGB color with `^(R,G,B)`. The Player widget appears only after
its start tick and stays visible until the duration deadline, with both
comparisons strict. The inspected render path draws letters but no panel; the
viewer now follows those recovered parts and labels text placement as
provisional where the mode values are still unknown.
