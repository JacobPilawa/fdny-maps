# Mode-2 HeroDown spawn and VictimRescue factory boundary

## Provenance

The pinned `FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 imported the binary into the disposable project
`/private/tmp/fdny_carried_trace/FDNYCarriedTrace`; a read-only, no-analysis
headless pass freshly decompiled `0044FF10`, `0049D130`, `0049D4C0`, and
`0045A010`. The Player tail is preserved in
[`0044ff10_herodown_spawn_tail_fresh_2026-10-21.c`](0044ff10_herodown_spawn_tail_fresh_2026-10-21.c).
No retail runtime or rendered actor capture was made.

## Trigger and cap

`00451080` calls `0044FF10(player, 0)` when Player state 6 has lasted strictly
more than 1,000 ms. The HeroDown branch additionally requires selected
profile `DifficultyLevel` at EverGame `+0x6E4/+0x2C` to equal 2. The direct
fire/oxygen entry `00450FB0(player, 2)` instead calls `0044FF10(player, 1)`,
which skips the branch.

The function enumerates active type-2 entities. It compares their registered
script name case-insensitively with `herodown` and counts states 4 or 7. If
the count is below 3, it submits one dynamic spawn request. It then increments
Player `+0x3528`, writes the `Rescues` statistic, and logs the new
Firefighters-down count. The three-actor cap and statistic update are
source-backed; whether the resulting stat is ultimately committed to a
profile remains a separate mission-result question.

## Dynamic request fields and factory handoff

The request is a zero-initialized `0x34`-byte block. Its fields include:

- word 0: `1000`;
- words 1 through 3: the Player's current origin;
- word 4: generated `POFighter` name plus a decimal suffix derived from
  passout counter minus one;
- word 5: script name `herodown`;
- word 6: zero;
- word 7: float `20.0`.

The function looks up class `RCS_VictimRescue` with class id `100`, resolves
its factory record through `00429C00`, writes the request pointer through
`0049D130`, then submits it through `0049D4C0`. Fresh decompilation shows
`0049D130` is a one-word setter and `0049D4C0` dispatches the object through
the global factory manager at `+0x24`, vtable slot `+4`, with flag 1. The
`RCS_VictimRescue` class constructor `0045A010` registers that class name.

This establishes the factory handoff, but not the full dynamic actor lifetime.
The post-submit queue callback, runtime entity-index insertion, victim
resource selection, floor placement, ScriptActor registration, contact
membership, and renderer registration still need to be traced through the
generic staged-spawn path. `Data/Acts/vic_HeroDown.act` exists in the input
bundle, but this trace does not prove that the dynamic factory selects it or
which motion/default data it uses.

## Shared-port boundary and next trace

The shared viewer's rescue mode now defaults to fresh-profile difficulty 1;
it does not load existing profile difficulty. Mode 2 therefore is not
currently selected, and the `spawnHeroDown` adapter still fails explicitly
if the mode-2 path is reached. The existing source-shaped tail helper captures
the cap and request fields, but the viewer's entity inventory is static-map
only and cannot yet realize the dynamic factory request.

Next trace the manager callback reached by `0049D4C0` through staged factory
creation into the ordinary `RCS_VictimRescue` constructor/update/render
registration sequence. Then adapt that route so the new entity participates
in the live victim inventory, actor render state, ScriptActor registry,
contact manager, and cleanup. Keep mode-2 selection unavailable until that
path and its authored HeroDown resource/defaults are integrated. This is
static evidence only, not retail parity.

Related notes: [profile mode default and shared port](rescue_profile_difficulty_mode_port_2026-10-21.md),
[Player passout recovery](player_passout_recovery_port_2026-10-14.md),
and [the ordinary VictimRescue factory path](world_factory_resolution.md).
