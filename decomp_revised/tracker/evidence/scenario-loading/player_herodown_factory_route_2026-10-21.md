# Mode-2 HeroDown spawn and VictimRescue factory boundary

## Provenance

The pinned `FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 imported the binary into the disposable project
`/private/tmp/fdny_carried_trace/FDNYCarriedTrace`; a read-only, no-analysis
headless pass freshly decompiled `0044FF10`, `0049D130`, `0049D4C0`, and
`0045A010`. The Player tail is preserved in
[`0044ff10_herodown_spawn_tail_fresh_2026-10-21.c`](0044ff10_herodown_spawn_tail_fresh_2026-10-21.c).
The queue-to-actor follow-up used a separate fresh auto-analyzed import at
`/private/tmp/fdny_hero_factory_trace/FDNYHeroFactory`; its read-only
decompilation logs are `/private/tmp/fdny_hero_factory_trace_2026-10-21.log`
and `/private/tmp/fdny_hero_factory_init_trace_2026-10-21.log`. They cover
`0042B1A0`, `0045DF80`, `0045DDD0`, `0045B690`, `0045D5B0`, `0049D4C0`, and
`0049D160` in addition to the initial tail trace. No retail runtime or
rendered actor capture was made.

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

## Fresh queue-to-actor follow-up

A separate read-only Ghidra 12.0.4 import of the same hash-pinned executable
decompiled `0042B1A0`, `0045DF80`, `0045DDD0`, `0045B690`, `0045D5B0`, and
`0049D4C0`. It confirms that `0049D4C0` calls the entity-factory service's
primary vtable slot `+4`; the service table maps that slot to
`SetEntityFactoryRequestPending` (`0042B1A0`). That function appends the
request and flag to the shared linked queue. The normal queue pump
`0049C670`, dispatched by `00429520`, later invokes the registered class
descriptor's create slot, registers each returned root through `00429490`,
then runs a full root `+0x20` pass followed by a full root `+0x24` pass.
The queue and pass order are also documented in
[`entity_factory_queue_update_order.md`](entity_factory_queue_update_order.md)
and [`world_factory_resolution.md`](world_factory_resolution.md).

The `RCS_VictimRescue` create slot `0045DF80` allocates a `0x22c`-byte actor
and calls `0045DDD0`. Its initializer attaches the supplied `0x34`-byte user
data pointer at VictimRescue `+0x150` through `0049D160`; that helper validates
the registered entity user-data size rather than copying the bytes. The
initializer copies request position words 1 through 3 into the actor origin
and registers the initial position using request word 6 as the heading. The
ordinary first-pass initializer
`0045B690` resolves the VictimRescue type; the second-pass spawn callback is
`0045D5B0`. Thus the dynamic request enters the same two-pass VictimRescue
path used by BSP-authored actors.

The request's word 4 is the entity `A_Name` (`POFighter` plus its counter
suffix); word 5 is `D_Type`, `herodown`. The hash-pinned source data resolves
`herodown` to Model 30 in `Data/ResDefs/victims.srf`, and Model 30 to
`data\\acts\\vic_HeroDown.act` in `Data/ResDefs/rescue.rrf`. This confirms
the authored ACT selected by the normal victim type/model path. It does not
establish exact motion choice, rendered pose, callback cadence, or live retail
appearance.

## Shared-port boundary and next trace

The shared viewer's rescue mode now defaults to fresh-profile difficulty 1;
it does not load existing profile difficulty. Mode 2 therefore is not
currently selected, and the `spawnHeroDown` adapter still fails explicitly
if the mode-2 path is reached. The source-shaped tail helper captures the
request fields, but currently names word 5 `scriptName`; the retail path uses
word 5 as VictimRescue `D_Type`, while word 4 is `A_Name`. The static host
entity read also currently supplies the authored entity name where this
counter needs the victim type. Correct that distinction before using the
helper with dynamic HeroDown instances.

The shared port still needs to allocate a unique entity index, create a
HeroDown `PreviewVictim` and runtime actor from the resolved ACT, and append
the instance to `victims`, `rescueState.actors`, and `rescueState.entities`.
Its `A_Name` must remain separate from `D_Type` for name routing and the
HeroDown cap. The port's proximity contact loop will include the instance
through the victim list; exact retail contact-manager residency, floor
placement for dynamic spawns, ACT motion/default details, renderer setup, and
cleanup ownership remain to be reconciled before enabling mode 2. This is
static evidence only, not retail parity.

Related notes: [profile mode default and shared port](rescue_profile_difficulty_mode_port_2026-10-21.md),
[Player passout recovery](player_passout_recovery_port_2026-10-14.md),
and [the ordinary VictimRescue factory path](world_factory_resolution.md).
