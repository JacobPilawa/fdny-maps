# Authored rescue HoseSpray trigger audit (2026-10-20)

The retail executable used for the command-path traces is
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
I scanned all 17 preserved `inputs/original/Data/Maps/*.bsp` files and all 21
preserved `*.evs` files with `strings -a`, then checked each literal match in
its entity-string context. The scans found 16 `touchhose` commands in 12 BSPs
and none in the EVS files. Hashes below match
[`level_map_inventory.csv`](../../level_map_inventory.csv).

Each occurrence is an `S_Script01` binding on an `RCS_PropModel`, with the
event command `touch: tell player (touchhose N)`. In `mbase.bsp`, for example,
the string sequence includes `A_Name hose1`, `classname RCS_PropModel`,
`F_TouchRadius`, and `touch: tell player (touchhose 25)`. The four dedicated
PlayerHose maps contain no `touchhose` command; they instantiate
`RCS_PlayerHose` rather than `RCS_PlayerRescue`.

| BSP | Commands | Argument values | SHA-256 |
| --- | ---: | --- | --- |
| `bar_r.bsp` | 1 | 30 | `1a796f4dfbf0d04bedf7e35ebdcf82e2e726940bf69b1df7ce2ea7428e018a98` |
| `boat_a.bsp` | 2 | 50, 50 | `e1685b45ef29aa075c688985617b593079a6aad8a6b493bbae327356bd45ad44` |
| `boat_b.bsp` | 2 | 50, 50 | `4ad7de96916b5c265a4cdd6c477d96a0b6f9cc6e8151c96ff2e1ef8a31313a50` |
| `gas_r.bsp` | 3 | 25, 25, 25 | `f6822f8d598d2888896165bbee8b554351230cd76ad6331abf0d735a03b54cbc` |
| `mbase.bsp` | 1 | 25 | `4ff3b18dd809c619a6734673a7f90bd2e98d9fc9518ee7bf94ece4b6ff90b266` |
| `oil_rig_A.bsp` | 1 | 50 | `c63ff7e6a7e41a6df15f572bc1c327cbc6ecbe934a17cf1c5a136c2a7d7e36c1` |
| `oil_rig_B.bsp` | 1 | 50 | `bfc16ba4e151a814ba2f80d84a3aadb096d9468c83262daab900c48e5043c701` |
| `oil_rig_C.bsp` | 1 | 50 | `786aeb2a56929f1ac8270cd3be16608aec5a6ae9677c477209dbd2cc0721f499` |
| `oil_rig_D.bsp` | 1 | 50 | `f74675df4da49c2723473b6c9d13faca698824d9bc104262f788394d7f86914d` |
| `subway_a.bsp` | 1 | 15 | `b33b67a01e0a2c144033f1da5c9c627b197d0a413b3b13190dd8531b63881f7e` |
| `subway_b.bsp` | 1 | 20 | `279ef316b08c0eae5d2ae33811402cf754d7e1aef95c2af199a83b87ff99c9e4` |
| `training.bsp` | 1 | 25 | `32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963` |

## Static event and command route

The authored `touch:` event enters the PropModel contact path documented in
[`retail_propmodel_contact_trace.md`](../scenario-loading/retail_propmodel_contact_trace.md):
the PropModel event drain `00444050` submits the pair changes to shared event
dispatch `00470840`, which maps them to ScriptActor `touch` events. The nested
`tell player` uses the shared named-actor route documented in
[`named_script_actor_dispatch.md`](../scenario-loading/named_script_actor_dispatch.md):
`00475560` looks up the registered actor name and passes the command to its
ScriptActor. Rescue `PlayerRescue` registers the name `Player`, and handler
`00451870` sends `touchhose N` to `0044CAB0`. That command allocates and attaches
HoseSpray in Player inventory slot 4, then selects/starts its hose mode.

The static route is therefore authored and reachable from rescue maps. The
current shared viewer already produces PropModel touch events and interprets
nested `tell` commands, but its Player command host does not implement
`touchhose`; it does not yet run the rescue HoseSpray item or its finalized
FireRescue contact call. PropModel update ordering, live contact latency, and
retail parity remain unresolved.

## `touchhose N` argument

The hash-pinned retail `0044CAB0` command path passes N to `00482780` on both
the new-item path and the existing-item refresh path. `00482780` multiplies N
by 24, divides by the float at attachment-state `+0x40` (HoseSpray owner
`+0xCB0`), converts the result toward zero through the reviewed `004B9400`
helper, then passes the integer to `00485D20`. That list helper rebuilds the
intrusive list rooted at attachment-state `+0x48` with the requested number of
0x30-byte records. The global at `004E1858` is binary64 `1/24` (bytes verified
from the pinned EXE). The existing-item comparison computes
`(+0xCB0 * +0xCC0) / 24`; it requests a rebuild when that value is below N.
Modes 2 and 3 reject the already-existing item with “Already have hose.”

The default HoseSpray component setup supplies divisor 20.0, which makes the
new-item model's record count `trunc(N * 24 / 20)`. Existing items compare
their current count equivalent against N before rebuilding. This shows N
controls an attachment-list record count through a 24-scaled formula. The
records' runtime purpose and the authoring units for N remain unknown. The
authored values 15–50 are not a hose mode, and there is no trace that makes
them physical reach. The individual call-site and helper notes are
in [`touchhose_attachment_list_trace_2026-10-20.md`](touchhose_attachment_list_trace_2026-10-20.md).
