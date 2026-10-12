# Training Hose valve interaction scenario

Date: 2026-10-12
Executable SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`

## Connected native path

`src/training_hose_interaction.h` composes the existing source-derived owners in the live `main.cpp` USE path:

1. `UseManager::select` filters the authored `makevalve` candidates and chooses action 1001 (attach) or 1002 (detach).
2. `HoseValveCallback::invoke` gates on the PropModel state. Action 1001 selects Hose Spray through `HoseItemSelection::attach`; action 1002 calls detach, sets mode 2 and rescans available inventory.
3. On attach, `HoseAttachmentFrame::commit` resolves the model frame, stores the authored valve getter position and host identity, changes mode 2→3, and identifies the first stream-pose node.
4. `HoseSprayPoseList::attach` writes that position into the frame and cached list head.
5. The result carries the callback's script-event rule back to `main.cpp`: attach forwards `use` after the owner transaction; detach suppresses it.

The frame input comes from Training's `D_Model=5`, authored heading and location. Evidence identifies model 5 as `valve.act`, with an identity root attachment and no motions. The original angle path converts Valve3 heading 9° to −360° and Valve5 heading 3° to −180°. The supplied Training frame oracle runs the original rotation calls and recursive resolver, but Ghidra's emulator substitutes Java Math for x87 `FCOS`/`FSIN`; its small residual basis values therefore remain provisional.

## Scenario coverage

`tools/training_hose_interaction_check.cpp` reads the preserved `assets/training/entities.json` and uses its two `makevalve` entries in order (Valve3 entity 159, Valve5 entity 164). It initializes the authored Axe and Hose Spray slots and the `touchhose 25` list, then checks:

- Valve3 target selection, attach action, inventory switch and mode 3.
- A source-gated busy Axe (`Axe+0x9B0=3`) prevents the Hose selection/frame commit; after the Axe gate returns to zero, the same Valve3 attach succeeds.
- Saved valve identity, exact authored valve position in Hose and pose-list head, and resolved axis offset within the documented approximate frame basis.
- Suppression of a different-valve target while attached.
- Same-valve detach, mode 2, Axe reselection and no forwarded detach `use` event.
- Valve5 selection and second attach, with its own authored position and resolved frame offset.

This is a native composition regression scenario. It does not compare live target ordering, update cadence, or script latency with an original Training run; nor does it render the attached inventory item as a hose follower or implement the separate `RCS_PlayerHose` actor/stream.

The Axe switch check uses the original gate inputs, not the old generic boolean: `0045F930` conditionally checks Axe `+0x568` when byte `+0x1FC` is set and always checks `+0x9B0`; `0045FA50` permits the switch only for zero. The owner oracle uses an explicitly source-mapped gate adapter; it does not execute these two leaf bodies. Its original owner P-code comparison checks callback order and selection behavior around the supplied gate result. The native scenario supplies the source-written busy value 3 to exercise the composed refusal, then returns to the constructor-established idle state. Axe hit/update scheduling that produces the value is still unimplemented.

## Verification

- `cmake -S decomp -B decomp/build` succeeded.
- `cmake --build decomp/build --target training_hose_interaction-check fdny-training -j 2` succeeded.
- Focused CTest selection for this scenario plus the callback, item-selection, frame-commit and pose-list owner checks passed 5/5.
- The full configured suite passed 40/40 CTests after the integration.

The full Player/world-interaction module remains open: axe/chopping, BModel lift/use/carry/throw, door/ladder parity, dynamic candidate ordering and original callback cadence still need connected Training scenarios.
