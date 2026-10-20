# HoseSpray contact frame gap (2026-10-20)

Status: **the HoseSpray contact gates, rescue-player `touchhose` allocation,
dedicated PlayerHose input path, and PlayerHose-to-WindowFire contact-message
route are confirmed; the HoseSpray world-space contact writer and the
WindowFire douse-state completion semantics remain open.** This is static
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
on a hit, stores the hit point, radius, and active gate in the douse helper at
`PlayerHose+0x520`. The registered `RCS_DouseHose` runtime class uses callback
`00464AA0` (runtime vtable slot `+0x48`): when that active gate is set, it gets a
candidate's bounds through virtual slot `+0x0C`, checks AABB overlap against
the current helper volume, and calls `00481A60` with the candidate, hit point,
and helper intensity at `+0x3B0`.

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
increment of the shared `Fires Doused` statistic. The flag and rate writers,
their authoring meaning, and the exact hose-stage completion/result condition
are still unresolved. The four dedicated hose maps also have no authored
FireRescue actors in the current inventory. Do not treat a successful WindowFire
message as proof that a FireRescue actor has been extinguished or that a stage
is complete. Static tracing only; no live hose run or retail parity check.

The dedicated maps therefore need their own gameplay trace and shared-host
integration, not an assumption that they use the rescue Player's slot-4
HoseSpray tool.

## Contact-point producer remains open

The follow-up helper `0046D1B0` runs after `ApplyHoseFireContact`; its bounded
decompilation updates the attached component/animation frame at item-relative
`+0x608`. Its first vector write is relative `+0x15C` within that component
(absolute HoseSpray `+0x764`); it is not a write to the contact vector at
HoseSpray `+0x614`. The vtable attach callback `00463460`, activation callback
`00463400`, action-start callback `004632D0`, setup `00463700`, and constructor
`004634F0` were checked. The action start sets `+0xC58` and a deadline at
`+0x3B0`, but none of these functions writes the XYZ contact fields.

A fresh full-program decoded-instruction scan for literals `+0x614`, `+0x618`,
`+0x61C`, `+0x620`, and `+0x624` returned 16 instructions. The direct stores at
`0046176F..00461787` are in the separate Extinguisher constructor
`00461690`; the stores at `00462039..00462051` belong to its action path
`00461F80`. HoseSpray's `00463EF0` contributes reads at `00463F0C`, `00463F1C`,
and `00463F28`. No direct decoded instruction writer for HoseSpray contact XYZ
appears in this scan. Computed aliases, bulk copies, writes through an engine
helper, or a missed calling-context alias remain possible, so the contact point
is still unresolved.

The shared viewer has no retail rescue-player `touchhose`/slot-4 route, no
PlayerHose mission adapter, and no HoseSpray fire-contact call. The separate
Training host has a local hose action/pose path, but it does not establish
either retail contact route. No contact coordinate, padding value, update
cadence, or retail action timing is being inferred for the viewer.

## Next trace

Trace the indirect or engine-assisted producer of HoseSpray `+0x614..+0x61C`
and join it to the item attachment transform consumed by `00463EF0`. Separately
find writers for WindowFire linked state `+0x8`, `+0xC`, `+0xD`, `+0xE`, and
`+0x10`, establish the authored meaning of the WindowFire timer/flags, then
trace how the dedicated hose-stage objective reports completion. Implement the
two source-backed host routes with separate states and preserve extinguisher
`finalize=0`. Do not assign a guessed contact point or merge the paths.

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
The pre-existing HoseSpray contact and suppression exports are listed in
[`README.md`](README.md). The temporary Ghidra project/logs and raw offset-scan
output are outside the repository and are not part of the evidence pack.
