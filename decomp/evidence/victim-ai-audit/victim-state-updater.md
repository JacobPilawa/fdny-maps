# Victim child updater and state-7 movement

This note records the source-backed work reached from the victim child subobject. It does not claim the subobject's runtime schedule: the constructor places its vptr at victim `+0xd4`, but the recurring caller/insertion path for `0045BEA0` is still unidentified.

The read-only Ghidra project analyzes `FDNYFirefighter.exe` SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` with Ghidra 12.0.4. Ghidra's function names and inferred types remain provisional; the offsets, branches and calls below are the evidence being preserved.

## Child updater `0045BEA0`

The victim constructor `0045DDD0` installs child vtable `004E25B0` at `victim+0xd4`; slot 0 is `0045BEA0`. The child updater reads victim state at child `+0x4c` (victim `+0x120`) and behaves as follows:

- State 5 returns immediately. It skips state work, sound dispatch and actor animation update.
- State 6 calls `004599B0(victim)`, then skips the `+0x154` sound call and proceeds to the actor animation updater.
- State 7 calls `0045B220(victim)`, then conditionally calls sound helper `0048D8F0` when victim `+0x154` is non-null, and proceeds to the actor animation updater.
- Other states skip the state-6/state-7 branch, conditionally call that sound helper, then run the actor animation updater.

The final animation call is `00480D50(child+0x9c)`, which is victim `+0x170`. Pickup handler `0045BEF0` sets state 5 and calls `00481140(victim+0x170,0)`; release helper `0045C000` sets state 6. These transitions are source-mapped, but their update timing remains unknown.

State 6's rescue initialization and timer fields are traced in `training-pickup-flow.md`; the recurring child-updater schedule remains unresolved.

## State 7 movement `0045B220 → 0045AE50`

`0045B220` may draw a debug line under a global debug flag, calls `0045AE50`, computes displacement between victim position `+0x4c..+0x54` and goal `+0x1c8..+0x1d0`, updates facing when squared displacement exceeds scalar `004E2338`, saves prior position to `+0x1d4..+0x1dc`, then calls `00483580` with movement speed at `+0x228` to produce the next position. Facing uses x87 `fpatan`, wrap/limit scalars, and frame helpers `0042D350`/`00496C30`.

`0045AE50` chooses or validates that goal. The recovered branches that consume shared RNG are:

- If byte `victim+0x1ec` is set, it draws `00498B30(6.0f,18.0f)` and then `00404AC0()`, uses `0045A190` to form a candidate around the current route vector, and checks it through `0045AB40`. An invalid candidate returns before the route flag is cleared; a valid one clears the flag and continues movement.
- In the other branch, after player/frame, distance (`00459BE0` with 432), visibility and direction checks, one path conditionally calls `00404B30(0x0f)`. The exact condition is preserved in raw Ghidra output; its purpose and Training reachability are not inferred.

The candidate checker `0045AB40` includes squared-distance checks, a hull trace through `00492CE0`, saved spawn-spot checks in the array at victim `+0x1c4`, and two offset probes around the candidate. It also has an optional debug trace. Its complete geometry composition is not ported or oracle-compared.

## State-7 entry helper `0045C7F0`

This routine stores its argument at victim `+0x224`, sets state 7, copies current position to goal `+0x1c8..+0x1d0`, sets speed `+0x228` to `36.0f`, and calls `0045C4D0`. It checks four diagonal `(±5,0,±5)` offsets with `0045AB40`. If every candidate fails, it logs `Victim <name> (<type>) is stuck when spawned!` and sets state 4. Otherwise it calls `00459A50(this,2)`.

Ghidra shows a script-command route into this helper: victim event handler `0045CDB0` recognizes `roam`, parses its numeric argument, and calls `0045C7F0(victim, argument)` at `0045D116`. This establishes a general victim-script entry path, but not Training reachability. The preserved Training inputs are `evidence/original/training.evs` (which executes `rescue.evs`), the five sources listed in `assets/training/scripts.json`, and entity records in `assets/training/entities.json`. None contains `roam`, `playanim`, `forcespawnspot`, or `norescuebot`; Training victim entities 191 and 194 only define `pickup` script commands. Therefore state-7 behavior is source-mapped but has not been shown reachable in Training.

Training's known victim state path is state 4 at actor initialization (`0045B690`), state 5 when pickup begins (`0045BEF0`), then state 6 when rescue releases the victim (`0045C000`). No preserved Training input or identified code path enters state 7. Keep the state-7 routines documented for full-game work, but do not treat that movement branch as required Training behavior unless another entry path is found. The actor updater's schedule still matters for animation and the post-release state-6 timer.

## Open items

- Identify who calls or registers child updater `0045BEA0` and when relative to `0045A070`, Player movement and the owner message drain.
- Determine whether a non-authored or runtime-generated Training path enters the generic `roam` command; no preserved Training script/entity currently references it.
- Recover meanings of the route, visibility, candidate and timing fields without relying on Ghidra's provisional variable names.
- Port/compare state-7 geometry and conditional RNG against a controlled original run before wiring it into native updates.

The detailed Ghidra function exports and refs are retained locally under `evidence/victim-ai-audit/runtime-flow/` and `runtime-flow-refs.tsv`; raw decompiler exports are excluded from Git by `decomp/.gitignore`.
