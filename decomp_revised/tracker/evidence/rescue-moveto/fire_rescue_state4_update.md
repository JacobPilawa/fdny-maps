# FireRescue state-4 update path

Checked 2026-10-13 against the pinned executable and the prior project audit.
This resolves the state-4 path from FireRescue's secondary-vtable callback into
path advancement. A 2026-10-20 follow-up also maps the ordinary owner-manager
dispatch path and establishes that FireRescue's root `+0x14` callback precedes
the root `+0x48` contact callback during a normal owner update. The exact
wall-clock/rendered-frame cadence remains unresolved; see the
[Think dispatch and ordering follow-up](../fire-spread/fire_update_dispatch_order_2026-10-20.md).

## Static call path

Binary: `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

- Data at `0x004E1774` points to `UpdateFireRescueActor` (`0x00449FE0`). The
  FireRescue construction path installs that secondary vtable at object
  `+0x24`.
- `UpdateFireRescueActor` returns for state 5. For an ordinary update outside
  state 4, it decrements a byte at `+0xF8` and can return early while updating
  the visual actor. For state 4 it bypasses that countdown gate and calls
  `AdvanceFireRescuePathMotion` (`0x00449340`) with the enclosing entity
  pointer (`receiver - 0x24`). It then calls `FUN_0044B060` on the embedded
  visual subobject at `root+0x12C`; when its key track is active, that routine
  calls `UpdateFireRescueTimedVectorTrack` (`0x0044AE80`). This helper consumes
  the current/next vector keys, interpolates against `DAT_00502AFC`, and updates
  the visual position. The path updater and the visual key interpolator are now
  mapped inside the actor callback; only the external recurring dispatcher and
  cadence are open.
- `AdvanceFireRescuePathMotion` advances the path-progress value using the
  entity rate and frame delta. Its target/completion branch resets state to 0
  and adjusts the current position. It also contains a conditional
  `RCS_FireRescue` factory path and timed motion-key setup.
- The earlier `moveto` mapping shows how `SetFireRescuePathPointMove`
  (`0x00448A90`) resolves the named point and enters state 4. MBase's command
  is `tell mortarfire1 (moveto mortar1 1 10 120)`.

Fresh hash-pinned pseudocode for the command, PathPoint registration, the
state-4 callback, and its timed-vector consumer is in this directory. The Ghidra disassembly at
`0x004497E3` and `0x00449832` shows the trailing command values being scaled
by `1000.0f`, added to existing integer key times, rounded by
`FUN_004B9400`, and stored as two generated key timestamps. See
[`00449340_timing_key_offsets.asm`](00449340_timing_key_offsets.asm). The
current Ghidra C-like decompilation omits these x87 operations, so the assembly
listing is the direct evidence for this detail. `UpdateFireRescueActor` was initially
missing as a function-manager entry, so it was manually bounded at the
vtable-referenced start `0x00449FE0` and saved to the local Ghidra project.
The two trailing command values at `+0xB0/+0xB4` are proven inputs to generated
key timestamps. Their author-facing terms and exact runtime effect remain open.

## Reuse of prior work

The previous project already contained three pseudocode exports for
`0x00449FE0` and seven for `0x00449340`. Selected copies are preserved under
[`reference/legacy-evidence/rescue-moveto/`](../../../reference/legacy-evidence/rescue-moveto/). The old function pseudocode was used as corroborating reference material, not
original source; the direct disassembly also exposed operand reads omitted by
the C-like decompiler. The focused prior audit at
[`scheduler-registration-audit.md`](../../../reference/legacy-evidence/fire-update/scheduler-registration-audit.md)
and the follow-up
[`training-fire-dispatch-order-audit.md`](../../../reference/legacy-evidence/fire-update/training-fire-dispatch-order-audit.md)
already searched owner, app-callback, and candidate dispatch paths. They
conclude that the recurring caller/order of this secondary Think callback is
still unresolved. The later owner-manager follow-up now supplies the static
normal-update route for FireRescue root slot `+0x14`; those earlier audits
remain useful for the limits they record and are not evidence of a missing
static caller.

## Remaining work

- Establish the callback interval and order relative to Player, extinguisher,
  message-drain, and outer scene/render callbacks. The manager order inside a
  normal owner update is now statically mapped, but frame cadence is not.
- Compare the MBase timing values (`10`, `120`) with gas_r’s (`99999`, `99999`)
  and capture runtime behavior to distinguish delay, duration, or phase semantics.
