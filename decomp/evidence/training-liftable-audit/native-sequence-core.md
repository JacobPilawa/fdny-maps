# Native Training liftable sequence core

`src/training_liftable_sequence.h` composes the source-backed liftable transaction
from the existing action and scalar helpers. It is intentionally an explicit
call-driven component: the production Player/BModel scheduler has not been
recovered, so these functions do not tick themselves or claim live timing.

## Modeled order

1. `pickupAction` delegates to `BModelUse::candidate`, preserving the
   `0044C520` gate for use mode, Hose Spray slot 4, Player byte `+0x12C`, held
   pointer, and BModel object state.
2. `pickUp` applies the `0044BC40` writes and calls the ordered
   `004571A0(Player+0x5F4, 3, nullptr)` boundary through the existing callback;
   it then emits the BModel ScriptActor `use` event, matching `0043D2B0` order.
3. `updateCarryAndMaybeRelease` calls the host's held-pose/correction service,
   reads the host-supplied angle, and uses the strict `<10°` branch. It runs the
   existing `BModelThrowScalar::setup` before status 4, clears the Player held
   pointer, restores state 3, and increments Objects Thrown.
4. `updateThrow` runs one explicit BModel update using the source scalar helper.
   The host transform callback receives each active point. Completion clears
   mode, applies the selected endpoint, flips the endpoint byte, dispatches
   `drop`, and clears the timer, in the order traced from `0043D330`.

## Deliberate boundaries

This is not connected to the live SDL Player or BModel scene loop. The callers
must still provide cached selection/list order, action dispatch, carry pose and
angle construction, model transforms/bounds, action/stat services, and timer
storage. Training contact pair membership, contact-to-event latency, recurring
Player/BModel callback cadence, and their relative order remain unresolved.
The scalar interpolation callback also does not reproduce Genesis model
transform side effects by itself. Axis-box query parity retains the documented
signed-zero exclusions and far-negative-zero question. No full Training
interaction or live original run is claimed.

The source basis and bounded fixtures are documented in
[`training-liftable-audit.md`](../training-liftable-audit.md),
[`bmodel-use-component.md`](bmodel-use-component.md),
[`throw-path-ghidra-audit.md`](throw-path-ghidra-audit.md),
[`throw-runtime-ghidra-audit.md`](throw-runtime-ghidra-audit.md), and
[`throw-scalar-native-check.md`](throw-scalar-native-check.md).
