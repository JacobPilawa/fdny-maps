# Training victim render transform

The playable scene now draws the two selected Training victims through `src/actor_renderer.h`. Their wrapper transform follows preserved source and EXE evidence; it is not an original render comparison.

`0045D5B0` resets the victim movement frame, copies the selected spot into its position, sets heading to `heading * 30 + 180`, then calls `0049DA00` to place the ±24 hull on the floor. `0045B690` initializes the child actor frame with unit scale, sets actor scale `(2,2,2)`, subtracts 25 from its local Y, rotates its local basis 90 degrees, and points its parent at the victim movement frame (`entity+0x24`). For both Training victims and all six candidates, authored heading is 6 and the helper input is 360 degrees.

`004977B0` recursively composes the parent before the child. `00497300` multiplies their bases, scales and translation; `00495D20` confirms a point is scaled, transformed by the basis, then translated. The native scene therefore composes the floor-adjusted entity frame with the local actor frame and applies the actor's uniform scale to posed vertices. The actor's own `-25` local offset is not multiplied by its child scale.

The local first-three ACT motions are each one frame long. Rendering starts their recovered loop at the native scene clock's initial value. The original motion-selection draw and speed are retained, but the original global timer epoch across level loading is not established here.

## Limit

This traces the outer wrapper transform and connects it to the existing native ACT pose/material renderer. The preserved Genesis source covers pose hierarchy composition, but the original body-render/scaling path and final skinned vertex positions have not been captured or compared against Genesis.dll. The heading trig uses native math, so its last bits may differ from the original x87 helper. The six victim floor contacts also remain unverified against Genesis.dll. Do not treat visible placement as actor-render or whole-Training parity.

Sources: `startup-initializers/0045b690_FUN_0045b690.c`, `victim-startup/0045d5b0_FUN_0045d5b0.c`, `victim-render/root-transform-assembly.txt`, `player-tool-pose/frame-functions/004977b0_FUN_004977b0.c`, `player-tool-pose/view-methods/00497300_FUN_00497300.c`, `fire-placement/00495d20_FUN_00495d20.c`, and `genesis-actor-source/pose.c`.
