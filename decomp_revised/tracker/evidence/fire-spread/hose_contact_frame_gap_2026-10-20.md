# HoseSpray contact frame gap (2026-10-20)

Status: **the HoseSpray contact gates and contact-vector writer,
rescue-player `touchhose` allocation, dedicated PlayerHose input path,
PlayerHose-to-WindowFire contact-message route, dedicated hotspot
cycle/time/win/loss state machine, and configured stage chatter sound names
are confirmed; the natural rescue-player `touchhose` trigger/host route,
authored/live Construction branch entry, WindowFire `+0x10` authored role,
stage result consumer, and mission transition remain open. A bounded
decoded-code scan found only a WindowFire `+0x10` clear and no nonzero setter
in the examined class/manager methods.** This is static
analysis of the pinned retail `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` using
Ghidra 12.0.4 and bounded instruction/operand searches. It is not a live hose
capture or retail-parity check.

## Confirmed retail path

`UpdateHoseSpray` (`004640F0`, primary vtable slot `+0x24`) calls
`ApplyHoseFireContact` (`00463EF0`) while HoseSpray action byte `+0xC58` is
active. The contact helper scans FireRescue candidates and reads a contact
vector at item `+0x614..+0x61C`, padding at `+0x620`, and a gate at `+0x624`.
It evaluates the shared ellipsoid falloff, requires influence `>= 0.01`, and
calls `ApplyFireSuppressionContact` (`00448970`) with `finalize=1`. When the
resulting intensity crosses the terminal threshold, that shared routine
performs the native doused-state, statistic, loop-sound, and visual-disable
transition.

The extinguisher path is separate. FireRescue's `0x7530` message receiver
calls the same suppression routine with `finalize=0`. Do not make that
nonterminal receiver path terminal while implementing hose contact.

## Two retail hose entry paths

PlayerRescue's script handler `00451870` recognizes `touchhose` separately
from the ordinary `equip` command. Its `0044CAB0` route creates a `0xD54`-byte
HoseSpray object through constructor `004634F0`, attaches it through vtable
slot `+0x08`, stores it in Player inventory slot 4, and dispatches the hose
mode. The ordinary equipment name selector `0044CA50` still recognizes only
axe, extinguisher, and goggles. This establishes the rescue-player slot-4
creation path; `touchhose` does not occur in the preserved authored map or
script files, so the natural runtime trigger for the command is still unknown.

The four dedicated hose BSPs (`city_h`, `const_h`, `dock_h`, `oil_rig_h`) use
`RCS_PlayerHose` instead of `RCS_PlayerRescue`. Initializer `00466D60`
registers the ScriptActor as `Player` and sets up hose path/camera/douse
components. Update `00466780` reads `MOUSE_LB`, `STEER_PITCH`, and `STEER_YAW`,
uses `00465EF0` to calculate an aimed hose point, and updates its hose visual
through `0046D1B0`. `0046C5A0` performs the world collision along the hose and,
on a hit, stores hit XYZ and the active gate in the embedded stream component
at PlayerHose byte offset `+0x1480` (owner-relative `+0x0C..+0x14` and `+0x1C`).
The registered `RCS_DouseHose` runtime class uses callback `00464AA0` (runtime
vtable slot `+0x48`): when that active gate is set, it gets a candidate's
bounds through virtual slot `+0x0C`, checks AABB overlap against the stream
owner volume (radius at owner `+0x18`), and calls `00481A60` with the candidate,
hit point, and owner intensity at `+0x3B0`. The DouseHose actor has its own
contact queue at callback-object `+0x28`; the stream component and the
callback/queue object are distinct.

`00481A60` adds the XYZ plus intensity to its bounded 32-entry contact queue
and sends the candidate message `0x7530`. In the dedicated hose path,
`RCS_WindowFire` is the recipient: `RCS_HosePath` initialization at `00465B90`
creates a WindowFire actor for each non-null authored fire reference, and the
WindowFire message handler `00468A80` routes `0x7530` to `00468680`. That method
only processes when the linked state bytes at `+0xC` and `+0xE` are set. It
subtracts frame delta divided by the linked `+0x8` value from WindowFire's
timer at `+0x258`; below a global float threshold it clears linked state `+0xD`
and resets the timer, then recomputes its geometry through `00468540` and sets
state `+0xF`. The WindowFire update `00468870` reads these flags to run its
visual/update path.

This is a WindowFire-local timer/geometry path. It contains no direct call to
`ApplyFireSuppressionContact`, no write to FireRescue intensity/mode, and no
increment of the shared `Fires Doused` statistic. The current static trace
resolves the dedicated hotspot cycle and spot-time result path: PlayerHose
emits `RC_HoseLvlStart`, then `RC_HoseLvlWin` or `RC_HoseLvlLose` and stores
result 1 or 2 at `+0x198`. `00468870` reads WindowFire linked `+0x10` and calls
`00468680` when it is set while `+0x0D` is active; the `0x7530` message handler
`00468A80` calls `00468680` directly. The bounded decoded-code scan found one
relevant store, `00468AC0` at `00468B01`, which clears `+0x10` along with
`+0x0D` and `+0x0F`; no nonzero setter surfaced in the examined class/manager
methods. This points to a separate recurring-update gate by control flow, but
its authored role and any indirect/external writer remain unresolved. The
Construction branch's runtime entry, configured sound names for the
registered radio-chatter keys, and downstream result presentation remain open. The four
dedicated hose maps also have no authored FireRescue actors in the current inventory. A
successful WindowFire message is not proof of shared
FireRescue suppression. Static tracing only; no live hose run or retail parity
check.

The hotspot manager and its authored data are detailed in
[`player_hose_stage_lifecycle_2026-10-20.md`](../scenario-loading/player_hose_stage_lifecycle_2026-10-20.md),
with fresh pseudocode in
[`playerhose_hotspot_state_machine_fresh_2026-10-20.c`](../scenario-loading/playerhose_hotspot_state_machine_fresh_2026-10-20.c).
The stage-key `TRadioChatter` registration and dispatch to a sound-manager
request are documented in
[`hose_stage_message_registration_fresh_2026-10-20.c`](../scenario-loading/hose_stage_message_registration_fresh_2026-10-20.c);
`Data/ResDefs/sound.rrf` (SHA-256
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`) maps the
three keys to `start2_b.wav`, `hosewin2.wav`, and `lose5_b.wav`. This establishes
configured resource names, not audible playback. `hose.rrf` (SHA-256
`93a1b2dae7d755fdd66fabaee89514de8e420cfb62f2f96132b341c0cce6fe45`) maps
selectors 71 and 72 to `paperwin-h.bmp` and `paperlose-h.bmp` with
`paperwin_a.bmp`, but a PlayerHose consumer for those selectors is not found.
The apparent `+0x198` consumer in the previous widget export belongs to the
generic rescue `PlayerStatus` path, not PlayerHose. Result presentation and
mission transition remain unknown.

The dedicated maps therefore need their own gameplay trace and shared-host
integration, not an assumption that they use the rescue Player's slot-4
HoseSpray tool.

## Contact-point producer traced through shared stream component

The follow-up helper `0046D1B0` runs after `ApplyHoseFireContact`; its bounded
decompilation updates the attached component/animation frame at item-relative
`+0x608`. Its first vector write is relative `+0x15C` within that component
(absolute HoseSpray `+0x764`); it is not a write to the contact vector at
HoseSpray `+0x614`. The vtable attach callback `00463460`, activation callback
`00463400`, action-start callback `004632D0`, setup `00463700`, and constructor
`004634F0` were checked. The action start sets `+0xC58` and a deadline at
`+0x3B0`, but none of these functions writes the XYZ contact fields.

The broader update trace confirms the same distinction. `004634F0` allocates
and initializes HoseSpray without explicit stores to `+0x614..+0x61C`;
`00463460` attaches the component through an engine virtual call and helper
`004859E0` using attachment component `+0xC70`. Setup `00463700` gets the
current model through a virtual getter and calls `0046D6B0` for the `+0x608`
attachment. Update `004640F0` computes a candidate world-space point from the
item transform into a stack temporary, but does not store it to the contact
fields. It passes that point to `0046D1B0`, which writes visual/animation state
inside the attachment (`+0x608 + 0x15C`, absolute item `+0x764`) and then calls
`0046C5A0`; the point is not evidence of the contact vector read by
`00463EF0`. The action callbacks `004632D0` and `00463300` set and clear
`+0xC58`; the start callback also sets deadline `+0x3B0` to frame plus 100.

A fresh full-program decoded-instruction scan for literals `+0x614`, `+0x618`,
`+0x61C`, `+0x620`, and `+0x624` returned 16 instructions. The direct stores at
`0046176F..00461787` are in the separate Extinguisher constructor
`00461690`; the stores at `00462039..00462051` belong to its action path
`00461F80`. HoseSpray's `00463EF0` contributes reads at `00463F0C`, `00463F1C`,
and `00463F28`. Vtable data resolves the ambiguous `00461F80` writer as slot 9
of the Extinguisher table at `004E28C0`; HoseSpray's separate table at
`004E2998` has `UpdateHoseSpray` at slot 9. Thus the exact-offset scan found no
HoseSpray writer.

A second full-program scan for nearby owner offsets `+0x608`, `+0x60C`,
`+0x610`, `+0x628`, `+0x62C`, `+0x630`, `+0x634`, `+0x640`, `+0x64C`, and
`+0x658` returned 46 instruction-text matches. HoseSpray callsites use owner
`+0x608` as a component pointer; the neighboring field references belong to
other classes. Inspecting the called component methods revealed why the hit
point did not appear as an owner-relative `+0x614` access: its writer uses
component-relative offsets from the pointer passed at `+0x608`.

The component alias resolves the producer. HoseSpray constructor
`004634F0` calls `0046DC80` on item `+0x608`; update `004640F0` passes that
same embedded pointer to `0046D1B0`. `0046D1B0` clears component `+0x1C` and
then calls `0046C5A0`. On a successful `geWorld_Collision` (`flags 0x43, 2`),
`0046C5A0` copies the collision point to component `+0x0C/+0x10/+0x14`, sets
`+0x1C`, and calculates the contact padding at `+0x18`. Relative to the
HoseSpray owner, these are precisely `+0x614/+0x618/+0x61C`, gate `+0x624`,
and padding `+0x620` read by `00463EF0`. This is the same component type
constructed at PlayerHose `+0x1480`; its collision result lands at owner
`+0x148C..+0x1494` with gate `+0x149C`.

Within `UpdateHoseSpray`, `00463EF0` runs before the later `0046D1B0` refresh.
It therefore consumes the hit result from the prior component collision
refresh; `0046D1B0` clears the gate before attempting the next collision. This
ordering is static call-order evidence, not a live scheduler or cadence
measurement. The 2026-10-20 follow-up used Ghidra 12.0.4 read-only scans and
vtable inspection against the pinned EXE hash above. `InspectInstructionWindow`
and `SearchPlayerOffsets` are retained under `tools/ghidra-scripts/` for
reproduction. Computed aliases outside this shared-component path and dynamic
retail parity remain untested.

The shared viewer has no retail rescue-player `touchhose`/slot-4 route, no
rescue-player HoseSpray fire-contact call, and no retail hose selector. The
separate Training host has a local hose action/pose path, but it does not
establish the rescue-player entry route. No natural authored trigger for
`touchhose` is present in the preserved maps/scripts. Live collision cadence,
action timing, and retail parity remain unverified.

## Remaining trace

Port the confirmed rescue-player contact component and `finalize=1` HoseSpray
transaction into the shared host when its source-backed item/command route is
available. First establish how retail reaches the `touchhose` command; the
preserved authored maps and scripts contain no such token. Keep the rescue
HoseSpray transaction separate from dedicated PlayerHose's WindowFire message
route and the extinguisher `finalize=0` path. Separately establish the authored
meaning of WindowFire linked `+0x10` and whether an indirect/external writer
sets it, then trace result presentation and mission transition for the
registered stage chatter keys. Resolve the Construction alternate path and
authored hose controls. A live collision/action capture remains useful to
verify cadence and parity, but the static contact writer is no longer open.

The unrelated recurring RescueBot and `runrescuebot` paths also remain
separate: the one-shot passes Player `+0x340C` to selector `00455230`, while
recurring update receives Player `+0x32CC`; context aliasing and one-shot
drop-off servicing are still open.

Fresh decompilations for the touchhose handler, HoseSpray constructor/setup,
action/update, PlayerHose initializer/update, nozzle point helper, and shared
attachment helpers are in
[`hose_contact_and_playerhose_fresh_2026-10-20.c`](hose_contact_and_playerhose_fresh_2026-10-20.c).
The dedicated douse callback, WindowFire message/state updates, and the
path-reference actor factory are in
[`playerhose_douse_windowfire_fresh_2026-10-20.c`](playerhose_douse_windowfire_fresh_2026-10-20.c).
The current linked-state export for `00465B90`, `004683F0`, `00468680`,
`004686E0`, `00468870`, `004688F0`, `00468A80`, `00468AC0`, and `0049D140` is
[`windowfire_link_state_fresh_2026-10-20.c`](windowfire_link_state_fresh_2026-10-20.c).
The hotspot cycle, PlayerHose state transitions, and win/loss handlers are in
[`playerhose_hotspot_state_machine_fresh_2026-10-20.c`](../scenario-loading/playerhose_hotspot_state_machine_fresh_2026-10-20.c).
The pre-existing HoseSpray contact and suppression exports are listed in
[`README.md`](README.md). The temporary Ghidra project/logs and raw offset-scan
output are outside the repository and are not part of the evidence pack.
