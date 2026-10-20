# BModel throw update route and drop-event data audit (2026-10-20)

## Pinned inputs and method

- Retail executable: `FDNYFirefighter.exe`, SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Local source manifest: `inputs/original/manifest.csv`, SHA-256
  `f0066868de28f4d91f3271bffb0f5122a067e4b15fac8f5ad2298febd40069a6`.
- Ghidra 12.0.4 read-only inspection of the saved `FDNYPort` project with
  `DecompileTargetAndRefs.java` and `FindFunctionCallers.java`; no auto-analysis
  and no runtime capture.
- Authored-data audit: all 21 EVS files in `inputs/original/Data`, plus the
  exported entity records for all 17 original retail BSPs. Each export's BSP
  SHA-256 matches its `inputs/original/manifest.csv` entry. Across those
  exports there are 4,323 entities, 329 `RCS_BModel` records, and 9,266
  nonempty `S_Script*` values.

## Callback and trajectory call

`0043DC60` is the embedded BModel ScriptActor's slot-zero update. Its initial
model-count guard returns for a BModel with no child model. At `0043DDC6`,
the callback directly calls `0043D330` when the ScriptActor field at `+0x234`
is nonzero. Because this ScriptActor is owner `-0x24`, that gate is BModel root
`+0x258` (`useMode`). The raw listing and Ghidra caller query identify
`0043DDC6` as the only direct code call to `0043D330`:

```text
0043DDA3  cmp dword ptr [esi + 0x234], ebx
0043DDA9  je  0x0043DDCB
0043DDC4  mov ecx, ebp
0043DDC6  call 0x0043D330
```

The local route into slot zero remains root slot `+0x14` → thunk `0043AFE0` →
shared updater `00470740` → embedded ScriptActor callback `0043DC60`. No
recurring manager route into that root slot has been proven. Therefore the
callback cadence and ordering against contact and rendering remain unknown.
The timed-command loop is later in the same `0043DC60` callback.

`0043D330` updates bit `0x10` in the first child model's flags at `+0xCC`: it
clears the bit when BModel state `+0x25C` is zero and sets it for nonzero state.
The shipped Genesis collision traces identify the effect: both swept-hull
`FUN_100372A0` and point/ray `FUN_10038470` skip a model when its `+0xCC` flags
contain `0x10` (`genesis_collision_functions.c`, Genesis SHA-256
`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`). This
proves collision exclusion for those two query paths; it does not establish a
renderer effect for this bit.
Only state 2 enters the throw trajectory branch. That branch derives elapsed
time from the wrapped millisecond clock and stored start tick at `+0x208`,
converts it to seconds, interpolates X/Z and ballistic Y, and applies the
position through `0043BAA0`. On the strict `elapsed > duration` completion
path, it sets state to zero, applies the selected endpoint, flips the selector
byte at `+0x220`, dispatches ScriptActor event `drop` at root `+0x24`, then
requests playback on the sound instance at root `+0x124` and returns. This
call sequence is recorded in the fresh [Ghidra decompilation](0043d330_bmodel_throw_fresh_2026-10-20.c).

The trajectory function contains no direct Player held-object mutation and no
call to `0043C260`. Event dispatch goes through `004705B0`, which looks up the
event and routes its stored commands in array order through `00470150`; when
lookup finds no event, it dispatches no commands. The later sound request is
independent of that event lookup.

## Authored `drop` binding search

A case-insensitive, word-bounded search of all 21 preserved EVS source files
found no literal `drop` event command and no `removealltimecmds` command. A
search of every property name and value in the 17 retail BSP entity exports
also found no such literal; this includes all 9,266 nonempty `S_Script*`
values and all 329 BModels. The exports are pinned to their original BSP
hashes through their `report.json` source hashes and the source manifest.

This search did not identify a map-authored `drop` handler or timed-list
cleanup. The retail call still performs the event lookup; dynamic registration
or behavior outside these preserved authored sources is not excluded. Do not
equate throw completion with `removealltimecmds` or add guessed cleanup to the
host.

## Port status

The trajectory is a substep of the BModel ScriptActor update, not a separately
identified scheduler. The shared mission scene now wires the strict Player
angle-under-10 gate to the source-shaped throw setup, clears carry state, sets
Player state 3/action status 4, and increments `Objects Thrown`. The source
gate reads Player view follower root `+0x34A4`, refreshed by `0044F340` earlier
in the same Player update pass. The held-target writer instead reads Player
root `+0x24`; these are distinct frames. The host camera's third basis column
is a provisional substitute for the follower basis and the separate held-
target frame adapter. The gate runs on each held-object host update because
the source block has no input-edge check. The follower's numeric relationship
to the rendered Genesis camera and the outer Player update cadence remain
unknown.

The shared BModel update samples its ordinary PATH pose, advances state-2
trajectory, then processes timed commands. Throw completion applies the
selected endpoint, flips the selector, dispatches `drop`, and requests the
owner's `ThrowObjectEnd` sound through host services in the source order. The
outer recurring BModel callback route and cadence are not proven; this

per-host-frame adapter is provisional. Player action status 3 enables the Axe
subobject and samples its GUI rectangle/frame-style record at time zero through
`00455BC0`; status 4 disables it. The shared renderer mirrors enable/disable
visibility, but the GUI rectangle-style frame record has no mapped
Player-`+0xA94` draw consumer or Axe ACT mesh link. A fresh caller census maps
status-3 reactivation to a later mode-1
liftable pickup through `0043D2B0` → `0044BC40`; ordinary inventory selection
uses a separate path: `0044D080` re-enables Axe root `+0x1FC`, and `0044C2A0`
invokes the Axe activation callback then Player status 5. This path does not
call status 3 or touch Player `+0xA94`. Runtime frame/cadence and renderer
mapping remain open. See
[`axe_reactivation_followup_2026-10-20.md`](axe_reactivation_followup_2026-10-20.md).
The first authored child model is
preserved even when it has no PATH, and every nonzero `useObjectState` maps to a
separate collision-exclusion set for swept and point/ray queries. This bit is
not mapped to the host render flag. The shared first-person equipment renderer
represents the status-3 Axe enable and status-4 hide; it does not represent the
time-zero internal frame-record refresh, which has no mapped ACT-render effect.

A fresh raw Player-update slice at `0045387B..00453A68` shows that a non-null
held-BModel pointer proceeds directly through held-target update and the
angle-under-10 comparison on each invocation of that update; no button or
action-edge check appears in the block. The strict under-10 branch starts the
trajectory, calls Player action status 4, clears the held pointer, restores
Player state 3, increments `Objects Thrown`, and calls the common Player action
dispatcher. Fresh helper decompilation resolves the compared values to the
selected BModel endpoint minus Player movement position `+0x338C`, and the
third basis column at Player view follower root `+0x34A4`. The held-target
writer's root `+0x24` frame is separate. The Player update calls `0044F340`
immediately before its held-object branch; that helper refreshes the follower
with `00483720`. This establishes a same-pass source relationship, not numeric
identity with the Genesis camera or outer callback cadence. See
[`00452570_held_bmodel_throw_gate_fresh_2026-10-20.asm`](00452570_held_bmodel_throw_gate_fresh_2026-10-20.asm).
The helper outputs are in
[`bmodel_throw_gate_vectors_fresh_2026-10-20.c`](bmodel_throw_gate_vectors_fresh_2026-10-20.c).
The caller and refresh excerpt is in
[`player_throw_view_follower_refresh_2026-10-20.asm`](player_throw_view_follower_refresh_2026-10-20.asm).
No gameplay test, interactive run, or retail-parity claim is made here.

See [BModel update notes](bmodel_update.md), [throw-stat producer trace](mission_stat_jump_throw_producers_2026-10-20.md),
[event dispatcher decompilation](004705b0_script_actor_event_dispatch_fresh.c), and
[event dispatcher assembly](004705b0_script_actor_event_dispatch_fresh.asm).
