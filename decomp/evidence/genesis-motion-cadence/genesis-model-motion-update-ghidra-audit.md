# Genesis model motion update audit

Read-only Ghidra 12.0.4 audit of scratch copies of the preserved executable and Genesis DLL projects, 2026-10-11. Inputs: `FDNYFirefighter.exe` SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`; `Genesis.dll` SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`. No live gameplay was used. The preserved Ghidra databases were not modified.

## BModel world-model motions

The BModel ScriptActor callback at `0043DC60` contains the game-side time advance and Genesis path sampling for its world models. Its recovered function body is retained in the [BModel throw runtime audit](../training-liftable-audit/throw-runtime-ghidra-audit.md) and its local raw Ghidra extract. There, `ESI` is the ScriptActor subobject at BModel root `+0x24`:

- BModel root `+0x70` selects motion behavior. State 5 advances the time at root `+0x1D8` by `DAT_00502B04 * root[+0x1DC]` and wraps it across the stored path interval. State 3 advances forward and clamps at the upper endpoint, then clears state; state 4 subtracts the scaled step and clamps at the lower endpoint, then clears state. The code also corrects out-of-range starting times. Other states leave the time unchanged.
- For each model ID in the BModel array (root `+0x1C0`, count `+0x1C4`), it calls `geWorld_ModelGetMotion`, obtains path 0 with `geMotion_GetPath`, then samples the current time. At `0043E126–0043E133`, the instructions load subobject `+0x1B4` (root `+0x1D8`), pass that float to imported `gePath_Sample` at `0043E133`, and then call imported `geWorld_SetModelXForm` at `0043E160`. Thus the time value is advanced first, then supplied explicitly to the path sampler; even a stationary state still resamples/applies the current pose when the method reaches this loop.
- The shipped Genesis DLL's named `gePath_Sample` implementation is at `1000E380`. It forwards its explicit time argument to `FUN_1000E260` for the rotation and translation samples and assembles the returned transform. This confirms that the DLL evaluates the requested time; it does not establish when FDNY invokes the BModel callback.

Training's `settime`/`settimescale` script writes map to root `+0x1D8`/`+0x1DC`; the shared Training Door macro sets the scale to `3.0`. These command and authored-motion details are recorded in the [Training door motion audit](../training-door-motion-audit.md). The throw updater `0043D330` has a second explicit `gePath_Sample` call at `0043D536`, after its own throw interpolation; that is a separate use of the same API, not the normal door-time step.

## Indirect dispatch and cadence boundary

For BModels, root primary-vtable slot `+0x14` at `004E0E0C` contains thunk `0043AFE0`. The thunk adjusts `this` to root `+0x24` and jumps to shared updater `00470740`; that updater can dispatch subobject vtable slot 0 to `0043DC60` unless its ScriptActor state gate suppresses the callback. The constructor installs the subobject vtable at `004E0DF0`, whose slot 0 is `0043DC60`. This local route is verified in the [BModel dispatch audit](../training-liftable-audit/bmodel-scriptactor-dispatch-ghidra-audit.md).

The regular owner path inspected there does not establish that it calls slot `+0x14`: its category-1 pass calls BModel slot `+0x48` (`0043B620`), while staging and rendering use different slots. Therefore the executable evidence currently establishes **what one BModel update invocation does**, but not that it runs once per rendered frame, where it sits relative to door/player/contact callbacks, or whether a given Training door receives every update. `DAT_00502B04` is used as the per-invocation time step here; the [fire update](../fire-update/training-think-behavior-audit.md) and [hose update](../hose-spray/player-hose-stream-ghidra-audit.md) audits identify the same global as frame delta, but this audit did not identify its writer or independently prove its update cadence.

## Separate Genesis actor-pose route

Actor ACT poses use another app-side path. `00480D50` calls imported `geActor_SetPose`; when its actor/motion fields select timed animation, its time argument is computed from `(DAT_00502AFC - start) % duration`, multiplied by the stored scale and `0.001`, then offset. Its direct caller in the victim path is `0045BEA0` at `0045BEE9`, after the state-dependent victim work (state 5 returns before this call). Genesis DLL wrapper `geActor_SetPose` is at `10007180` and transfers to `FUN_10010650`. `00492410` is a second app pose helper with additional `geActor_SetPose`/`geActor_ClearPose` calls; its owner and cadence were not traced here. The victim updater's schedule remains unresolved as documented in the [victim child-update audit](../victim-ai-audit/victim-child-update-scheduling-audit.md).

These actor poses are not the Training BSP `gePath` motion path above. Both app paths provide a time or pose to Genesis during a callback, but this audit does not prove either callback executes once per runtime/render frame. No runtime player scheduling is inferred from vtable entries or missing direct xrefs.

## Ghidra extracts

The local, Git-ignored extracts are in [`ghidra/`](ghidra/): EXE vtable/import/global xrefs, BModel and throw `gePath_Sample` call contexts, Genesis DLL sampling implementation, and the separate actor-pose helper. The read-only project copies were under `/private/tmp/fdny-motion-cadence-audit-20261011/{exe,genesis}`. `0043DC60` has no function boundary in the preserved DB; its method body was defined only in the separate analysis scratch copy documented in the linked throw-runtime audit.
