# Equipment actor render path audit

Retail binary: copied `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The initial pass used saved Ghidra outputs plus local instruction
disassembly. A follow-up used Ghidra headless on a temporary copy of the saved
project because no Ghidra MCP instance was available. No repository or saved
Ghidra project files were changed.

## Verified render chain

- ActorInfo creation routine `00492560` calls the Genesis `geActor_Create`
  import at IAT `0x4DC180`, stores the returned actor at `ActorInfo+0x60`, and
  calls `00492340` to derive render extents.
- When ActorInfo special-mode byte `+0x74` selects the attached/context path,
  creation calls `geActor_RenderPrep` (IAT `0x4DC178`) for the inner context
  world. The ordinary path calls `geWorld_AddActor` (IAT `0x4DC17C`) with
  actor flags `0x400`.
- ActorInfo vtable render slot `004926E0` calls `geActor_Render` (IAT
  `0x4DC18C`) with actor `+0x60`, context world `+0x58`, camera, and flags 2.
  This is the visible attached-actor render callback, outside the ordinary
  world actor-list path.
- Transform consumer `00492450` passes the ActorInfo matrix at `+0x10` to
  `geActor_SetPose` (or `geActor_ClearPose`) and supplies scale from ActorInfo
  `+0x40..+0x48`. `00492410` is the corresponding pose-only adapter.
- Base constructor `00492220` initializes matrix `+0x10` to identity;
  specialized constructor `004922A0` calls that base constructor. This is only
  the initial value, not the live held-item pose.

## Verified held-item matrix writer

- Equipment helper `0045F660` calls `00497E90` on its embedded transform
  component (`this+0x58`) in mode 2, with its equipment slot and attached-actor
  setup arguments. `00497E90` stores mode 2 and parent/source fields, calls
  `00494500`, and stores the resulting ActorInfo pointer at component `+0x94`.
- Component updater `00497F70` builds the hierarchical transform through
  `0042D0A0` and `004977B0`. Its mode-2 branch composes 16 matrix values and
  calls `00492110` with the ActorInfo pointer at component `+0x94`.
- `00492110` writes the 4x4 matrix into ActorInfo `+0x10..+0x48`, then
  dispatches ActorInfo vtable slot `+0x18`. In vtable `004E3BD4`, that slot is
  `00492450`, which applies the matrix with `geActor_SetPose` and supplies the
  ActorInfo scale. Vtable slot `+0x10` is the render callback `004926E0`.

This corrects the earlier conclusion that `00497F70` only updated actor
flags. Retail updates the held actor pose at runtime and consumes it in the
attached render path.

## Remaining gap

The writer/consumer chain is now closed statically, but the component's live
parent/source frame and exact per-item matrix values have not been mapped back
to camera coordinates. ActorInfo scale is read from its own fields; whether
those fields encode the final visible scale for each equipment actor has not
been runtime-compared. The decoded Axe and extinguisher ACTs show different
faces under candidate roots (Axe half-turn; extinguisher direct basis), but
this only characterizes the port assets. Exact retail hand offset, per-item
orientation, scale, and depth behavior still need setup constants and/or a
retail runtime capture.
