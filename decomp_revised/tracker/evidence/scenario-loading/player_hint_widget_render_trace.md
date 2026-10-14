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
`font1_a.bmp`. `00436C40` initializes sprite objects with resource IDs 1020 and
1019 and contrasting white/gray tints. These are concrete retail font-atlas
assets for a shared renderer, but a direct resource-ID/dataflow link from the
hint widget to either atlas is not established here.

## Port comparison and open points

[`hint_overlay.h`](../../../port/spikes/training-native-prototype/src/hint_overlay.h)
draws a dynamically sized opaque dark panel with a custom 5×7 glyph font. Its
center Y of 240/480 matches the retail source rectangle center, but its panel
dimensions, fill/border, glyph design, line layout, and text color are preview
choices; retail's two mode values are not decoded here. The retail duration is
forwarded and stored, but the precise timeout transition is not mapped.

The generic text object's per-frame update and actual draw callback, font atlas
selection for ScriptActor hints, wrapping/clipping behavior, popup background,
opacity, and timing/transition curve remain open. The trace did not establish
any retail texture-backed panel asset.
