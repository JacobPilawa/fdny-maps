# Dedicated PlayerHose stage lifecycle (2026-10-20)

## Scope and evidence quality

This is a static trace of `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, using
Ghidra 12.0.4, plus the current authored BSP entity exports. The exports were
checked against their source BSP hashes. This does not establish live retail
timing, route reachability, presentation, or parity. No port implementation
was changed from this trace.

Fresh pseudocode for the state and transition functions is in
[`playerhose_hotspot_state_machine_fresh_2026-10-20.c`](playerhose_hotspot_state_machine_fresh_2026-10-20.c).
The prior dedicated contact and WindowFire trace is in
[`playerhose_douse_windowfire_fresh_2026-10-20.c`](../fire-spread/playerhose_douse_windowfire_fresh_2026-10-20.c).

## PlayerHose stream component and DouseHose owner

In the Ghidra pseudocode, `00466D60` and `00466780` receive
`int *param_1`; therefore expressions `param_1 + 0x520` are dword-indexed and
refer to byte offset `PlayerHose+0x1480`. This is the embedded stream component
initialized by `0046D6B0`, passed through path setup, and supplied as the owner
payload for the registered `RCS_DouseHose` callback context. It is not a
DouseHose helper at byte `PlayerHose+0x520`.

`00466780` passes that stream component to `0046D1B0`; `0046D1B0` updates its
stream presentation and calls `0046C5A0(this)` with the same component. On a
world collision, `0046C5A0` writes hit XYZ and the active byte at component
offsets `+0x0C`, `+0x10`, `+0x14`, and `+0x1C`, which are PlayerHose byte offsets
`+0x148C`, `+0x1490`, `+0x1494`, and `+0x149C`. `00464AA0` obtains the owner
through the DouseHose callback context and reads its XYZ, radius `+0x18`, active
byte `+0x1C`, and intensity `+0x3B0`. It submits contacts through the
DouseHose callback object's separate queue at `+0x28` via `00481A60`. This
distinguishes the stream collision result from the callback context and its
queue, and from the unresolved rescue HoseSpray contact fields at `+0x614`.

## Dousing one WindowFire target

The per-path manager at `00465B90` reads the `ZF0..ZF8_FireModel` and paired
`ZF0..ZF8_SprayTime` fields in authored order. Each non-null model reference
creates an `RCS_WindowFire` child for that path-point context; the generated
child's linked state starts with the active, dousing, and selected gates
cleared. The runtime linked record has the following behavior established by
the current calls:

| Linked offset | Observed behavior |
| --- | --- |
| `+0x04` | Receives the referenced fire model when the WindowFire context is constructed. |
| `+0x08` | Receives the paired SprayTime value; `00468680` divides frame delta by this value while applying a contact. |
| `+0x0C` | Active gate set or cleared for all children by `00465940`. |
| `+0x0D` | Set by `00468850` when a selected target begins dousing; `00468680` clears it when its timer passes the global threshold. The cycle updater treats it as still in progress while set. |
| `+0x0E` | Selects the current target for the cycle; `00465A20` clears the previous target and sets this gate on the next one. |
| `+0x10` | Read by `00468870`; when set, its update calls `00468680`. Its writer and authored meaning are unresolved. |

`00468850` begins dousing by setting linked `+0x0D`, setting WindowFire ramp
byte `+0x25D`, zeroing its timer at `+0x258`, then recalculating geometry.
The current Ghidra export for `00468870` adds twice the frame delta to
`+0x258` while the ramp byte is set, caps the timer at `1.0`, and clears the
ramp byte once it passes one. On message `0x7530`, `00468680` requires linked
`+0x0C` and `+0x0E`, subtracts frame delta divided by linked `+0x08` from
`+0x258`, and clears `+0x0D` only when the result is strictly below
`004dc50c` (`0.0f`); it then resets the timer, updates geometry, and sets
linked `+0x0F`. The host now uses this zero-start ramp and contact arithmetic.
Message-versus-entity scheduling and frame-delta calibration remain
provisional. The current contact trace shows a local WindowFire timer and
geometry transition; it does not call shared FireRescue suppression or update
the shared Fires Doused statistic. The paired SprayTime is a divisor in the
observed operation, but its exact authored units and zero-value handling have
not been established.

The `00468870` export also calls the contact updater when linked `+0x10` is
set. The writer and authored meaning of `+0x10`, update cadence, and rendered
result remain open; message `0x7530` separately reaches `00468680` through
`00468A80`.

A fresh full-program instruction-text search for offsets `0x2B8` and `0x2A8`
returned 36 instructions. It found no direct object-relative `+0x2B8` field
access; the only `0x2B8` literal outside the `+0x2A8` pointer references was a
stack-local immediate argument at `0045B3B0`. This bounded scan does not rule out
computed aliases, bulk copies, helper writes, or a writer that reaches the
linked record through another pointer.

## Path cycle, time budget, and stage result

`00465B90` initializes a path context with the authored `Q_NumCycles` value at
manager `+0x20`, current target index `-1`, and children for that context's
non-null fire references. `00465A20` waits while the current target's linked
`+0x0D` remains set. Once cleared, it deselects that target, advances the
index, and on a full wrap resets the index to zero and decrements the remaining
cycle count. If the count remains positive it selects the next child and calls
`00468850`. `004658A0` is the completion check (`manager+0x20 < 1`).

The PlayerHose spline updater `004677D0` resolves the authored path anchors.
When it encounters a hotspot, it applies that record's authored fields,
updates current and next hotspot pointers, and enters PlayerHose state 5. The
first hotspot also emits `RC_HoseLvlStart` and clears its `M_IsFirstHotSpot`
marker. The fields copied or consumed in this path include:

The event comes from `00490BB0`: it returns the selected 32-byte control-record
address only when that address differs from the cached record, then updates
the cache. `00490510` initializes the cache to null, so the first sampled
record is reported. `004677D0` resolves that record's node at `+0x10` and
accepts it if it matches the expected `D_NextHotSpot` pointer or has the first
marker set. The timer budget is written at `+0x1A84`; `00465FB0` copies it to
the live countdown at `+0x1A7C` on state-4 entry. The exact scheduler-frame
distance between anchor processing and state-4 countdown remains unresolved.

| `RCS_HosePath` field | Observed use |
| --- | --- |
| `D_NextHotSpot` | Resolved to the next hotspot pointer for the PlayerHose state machine. |
| `M_IsFirstHotSpot` | First-hotspot branch gates `RC_HoseLvlStart`; the marker is then cleared in the runtime record. |
| `M_SpotTime` | Multiplied by the active session mode's timer scale and stored at PlayerHose `+0x1A84`; state entry copies it to countdown `+0x1A7C`. Exact numeric scales are in [`player_hose_mode_timer_2026-10-20.md`](player_hose_mode_timer_2026-10-20.md); mode labels and the selection path are open. |
| `M_MaxPressure` | `004677D0` copies the path record's `+0x2C` to PlayerHose `+0x1834`; a fresh exact-offset instruction-text scan finds only this write at `00467B8A`. No direct-immediate reader appears, but computed and aliased accesses remain possible. See [`player_hose_pressure_scan_2026-10-20.md`](player_hose_pressure_scan_2026-10-20.md). |
| `N_PitchSpread` | Copied from the path record's `+0x30` to PlayerHose `+0x238`; the aiming updater uses it as a symmetric pitch clamp in degrees. Its interaction with path-angle progression remains open. |
| `Q_NumCycles` | Initializes manager `+0x20`, the number of complete target passes remaining for this path context. |
| `ZF0..ZF8_FireModel` and paired `SprayTime` | Non-null model references create WindowFire children for the initialized context; each paired SprayTime is copied to its child's linked state. |

The per-frame updater `00466780` reads `MOUSE_LB`, `STEER_PITCH`, and
`STEER_YAW`. Its pitch accumulator at PlayerHose byte `+0x230` subtracts the
pitch input and is clamped to `[-PlayerHose+0x238, +PlayerHose+0x238]`; its yaw
accumulator at `+0x234` subtracts yaw input and is clamped to the symmetric
limit at `+0x23C`. Pitch is also constrained by the current view-angle limits.
Constructor `00466D60` initializes the pitch and yaw limits to 45 and 60
degrees respectively; hotspot entry replaces the pitch limit with that
hotspot's `N_PitchSpread`.
On hotspot entry, `004677D0` writes the path record's `+0x30` pitch-spread value
to `+0x238`, and its `+0x2C` max-pressure value to `+0x1834`. The resulting
orientation feeds nozzle aim helper `00465EF0` and stream presentation
`0046D1B0`. Fresh decompilation of rotation helper `00496DD0` confirms the
angle input is multiplied by `0.0174532924` and `0.5` before sine/cosine, so
the pitch and yaw accumulators use degrees and the helper forms half-angle
rotations. This establishes the authored pitch bound's units, but not the
rendered stream's cone width. `M_MaxPressure`'s downstream effect remains
unknown; the earlier `0x194` scan was not relevant, and the fresh exact
`0x1834` scan only found the assignment. It is distinct from stream-owner intensity at PlayerHose
`+0x1830` (stream component-relative `+0x3B0`), which the DouseHose callback
passes to the WindowFire contact.
The fresh rotation pseudocode is
[`player_hose_angle_rotation_fresh_2026-10-20.c`](player_hose_angle_rotation_fresh_2026-10-20.c)
(SHA-256 `30a8b4e2aa55a82592408fcdb8d316a242121f5bb1a6f7913bca6d985ca2bfec`).

`00465FB0` enters state 4, enables the linked active gates, and initializes
the current spot-time countdown. In state 4, `004674A0` checks cycle
completion. When all cycles are exhausted, it emits a `Hotspot %i`
notification. If there is no next hotspot it calls `00466C50` (stage win); if
there is one, it resets hose path/camera state and returns to path-following.
If the countdown crosses the global threshold first, it calls `00466CE0`
(stage loss). While neither condition is met, it updates pressure/input state
and advances the WindowFire cycle.

The win handler emits `RC_HoseLvlWin`, writes PlayerHose result field
`+0x198 = 1`, and enters state 6. The loss handler emits `RC_HoseLvlLose`,
writes `+0x198 = 2`, and also enters state 6. A bounded caller search found
no confirmed PlayerHose result-field consumer. The apparent `+0x198` consumer
in a previous widget export belongs to the generic rescue `PlayerStatus` path.
The exact result-screen presentation and any surrounding mission transition
are not established by these functions.

The PlayerHose functions above emit `RC_HoseLvlStart`, `RC_HoseLvlWin`, and
`RC_HoseLvlLose`; defined-string xrefs for all three keys also lead to
`004895C0`, the `TRadioChatter` initializer. `00489200` reads the configured
list at `DAT_004E3888`, creates a `TChatterSet` record per entry, and logs each
sound index and key. The runtime path is also resolved: `004897C0` searches
the registered sets by key, and the emitters pass byte `1`, which selects
`00489110`. That method advances through the set's shuffled sound entries and
requests the selected instance through `004A11A0` and sound manager
`004A0BE0`. The checked code establishes a sound request for these events, but
not audible playback. `Data/ResDefs/sound.rrf` maps `RC_HoseLvlStart0`,
`RC_HoseLvlWin0`, and `RC_HoseLvlLose0` to `start2_b.wav`, `hosewin2.wav`, and
`lose5_b.wav` under `Data/Snd/chatter/`; this mapping file has SHA-256
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`.
`hose.rrf` maps result selectors 71 and 72 to `paperwin-h.bmp` and
`paperlose-h.bmp` (alpha `paperwin_a.bmp`), but no PlayerHose result consumer
has been confirmed; these selector entries do not prove that this stage uses
them. Result-screen feedback and mission transition remain open. The
hash-pinned export is
[`hose_stage_message_registration_fresh_2026-10-20.c`](hose_stage_message_registration_fresh_2026-10-20.c).

## Hose time-warning gate

The state-4 updater `004674A0` decrements the hotspot countdown and calls
`004653B0` with its current value. That caller passes the value and literal `5`
to `00481F40` on PlayerHose component `+0xBC0`. The helper's `004B98B0` calls
floor the current value and stored comparison marker at `+0x278`; the warning
branch requires current floor `5` and stored floor `6`. A nonzero returned
low byte makes `004653B0` request `RC_HoseTimeWarn` through `004897C0`. This
supports a request when the countdown crosses from six to five units. Whether
those units are seconds, which configured sound entry is selected, and whether
it is audible in a live stage remain unverified. A second PlayerStatus/widget
caller passes a null value and the same literal to the shared helper, so that
call site's timer semantics differ.

`Data/ResDefs/sound.rrf` maps `RC_HoseTimeWarn0` and `RC_HoseTimeWarn1` to
`time4_b.wav` and `time5_b.wav`; its SHA-256 is
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`.
These are configured resources and a source-level sound request, not proof of
audible output or which entry is heard in a live hose stage. Fresh pseudocode
is in [`hose_pressure_path_fresh_2026-10-20.c`](../fire-spread/hose_pressure_path_fresh_2026-10-20.c)
(SHA-256 `c61d19bfaa00d1f385a5384183c1d2f65d85351acb71f63445e2963a0354e980`)
and [`hose_time_warning_helper_fresh_2026-10-20.c`](../fire-spread/hose_time_warning_helper_fresh_2026-10-20.c)
(SHA-256 `3a266991ef999dfddafe6108202ba2f269ae88fdddd3f37dd7de1279c91121a6`),
plus the floor helper export
[`hose_time_floor_helper_fresh_2026-10-20.c`](../fire-spread/hose_time_floor_helper_fresh_2026-10-20.c)
(SHA-256 `e681e37206a278a2eb3716a479cea235dbc7e2480392124dd2d26309c37cb534`).

## Current authored data in the four dedicated hose maps

Each report SHA-256 below was matched to the corresponding
`inputs/original/Data/Maps/*.bsp` file before counting current exported
`RCS_HosePath` entities. Counts are authored data, not evidence that every
record is reachable in a live session. The fire-reference total counts
non-null `ZF*_FireModel` properties across all path nodes; it is not the
hotspot count.

| Map | BSP SHA-256 | Path nodes | `C_IsAHotSpot=1` | Chain from `M_IsFirstHotSpot` via `D_NextHotSpot` | Authored cycles on hotspots | `M_SpotTime` range | Non-null ZF fire refs |
| --- | --- | ---: | ---: | ---: | --- | ---: | ---: |
| `city_h` | `9bc788dcaceb477fea52efd49cfbb2fd8f86824907d73c819280710b04fcb27a` | 19 | 9 | 9 | all 2 | 30–40 | 29 |
| `const_h` | `f4dc99b6dd3b4465c1ba68cc29c09c075368f45f744a32a470b0663251862984` | 34 | 8 | 7 | six 2, two 3 | 13–30 | 28 |
| `dock_h` | `17427ebd2964f31b47317e53c0881af41a05e78d57a7c95ef9783d10697a6d7c` | 17 | 5 | 5 | all 2 | 40–45 | 22 |
| `oil_rig_h` | `6681a6ecb688f69b37bbbc25c6c0dea3f030f33880e7210c7cbed48a108bd4d0` | 13 | 6 | 6 | all 2 | 25–55 | 24 |
| **Total** | — | **83** | **28** | **27** | — | — | **103** |

Construction has one hotspot, `RCS_HosePath36`, outside both the first
hotspot's directed `A_NextPoint` loop and its `D_NextHotSpot` chain. A fresh
walk of the source-matched entity graph gives the forward A route
`12→13→27→28→33→34→14→35→37→15→40→41→16→43→17→18→45→26→25→12`;
the D chain is `12→13→14→15→16→18→26→null`. Node 36 has both A and D pointing
to node 15, so it is an authored inbound branch that joins the main A route at
15, not a node reached by forward traversal from 12. In the hash-pinned
`004677D0` path handler, the touched hotspot resolves its D pointer into the
PlayerHose next-hotspot field; `004674A0` uses that field to decide whether to
advance or finish. The available code does not show `E_NextHotSpotPathDir`
being read or establish that runtime starts at node 36 / traverses its A edge
backward. Runtime reachability therefore remains open. The graph result is
not evidence of a damaged record. The same BSP has
`RCS_HosePath45` with four fire-model references (`ScaffFire_01` through
`ScaffFire_04`) while `C_IsAHotSpot=0`. This confirms that ZF references alone
must not be used to classify a path node as an authored hotspot or to infer
which actors are active together.

## Open work

- `00468870` reads WindowFire linked state `+0x10` at `004688E7` and calls
  `00468680` when it is nonzero while `+0x0D` is set. Message `0x7530` reaches
  `00468680` directly through `00468A80`, bypassing that gate. The bounded
  decoded-code scan found only one relevant writer, `00468AC0` at `00468B01`,
  which clears `+0x10` alongside `+0x0D` and `+0x0F`; no nonzero setter was
  found in the examined class and manager paths. This supports a recurring
  update gate as a control-flow inference, but its authored role is unknown.
  Computed aliases, opaque helpers, bulk initialization, and external writes
  remain possible. A runtime write watch on the live descriptor byte is the
  strongest next check.
- Explain which spline/start conditions can enter Construction node 36, and
  find a source-backed read of `E_NextHotSpotPathDir`; the current first-hotspot
  A route and D progression chain both exclude 36.
- Trace direct or aliased consumers of `M_MaxPressure` at PlayerHose `+0x1834`
  and resolve its effect; trace the pitch clamp's interaction with path-angle
  progression and SprayTime units/edge cases before implementing those semantics.
- Confirm the PlayerHose countdown's time units and which `RC_HoseTimeWarn`
  sound entry is selected; the traced crossing is six-to-five countdown units,
  not yet verified wall-clock seconds or audible playback.
- Trace the separate stage transition, result HUD, and return flow. Treat the
  configured chatter filenames as resolved but audible playback as unverified;
  the generic rescue `PlayerStatus` result widget is not a PlayerHose consumer.
- Separately trace the rescue-player HoseSpray contact producer at `+0x614`;
  it is not the dedicated PlayerHose/WindowFire path.
- Implement the two host routes from source-backed state and data, then perform
  controlled checks. No runtime or retail-parity claim follows from this
  static analysis.
