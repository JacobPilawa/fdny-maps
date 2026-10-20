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
widget rooted at Player `+0x480`; when nonzero it refreshes that widget through
`004574C0`. The refresh formats the integer with `%i` and submits it to the
generic text component at widget `+0x130` through `0048C2A0`. Event 7 resets the
widget's animation clock and zero-valued interpolation state via `00455F10`.
The widget's exact screen layout, font/material selection, and full animation
curve have not been resolved, so the port does not invent a score placement or
popup.

Retail `bonusitems.evs` defines touch rewards from 100 to 500 points and routes
them through `tell player (addbonus N)`. Gas R has its own EVS include of
`bonusitems.evs`; the general rescue script also includes it. The duplicate
`BONUS63` definition in the authored file is preserved as-is; no missing
`BONUS64` is inferred.

## Host change

The shared scene viewer now accepts Player `addbonus` commands and accumulates
their signed amounts in its `Bonus Points` result statistic using the existing
32-bit wrapping statistic helper. This lets existing authored pickup bindings
award score without changing the authored hints, sound, or hide commands.
The numeric HUD widget and result-screen rendering are still unimplemented;
the correct score total is available in the shared scene state for that work.

No live retail pickup, host interaction, or parity check was performed for this
slice. The source map proves static command routing, not which authored touch
references become reachable during play.
