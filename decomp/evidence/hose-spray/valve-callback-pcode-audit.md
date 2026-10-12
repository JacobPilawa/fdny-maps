# PropModel valve callback P-code audit

Date: 2026-10-12

## Scope

Ran the hash-pinned original x86 PE32 routine at 00444120 with five controlled PropModel-state/action combinations. The executable SHA-256 is 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. The fixture records the owner receiver, valve-root argument, action code, string-constructor boundary, and script-event forwarding result.

Ghidra EmulatorHelper intercepts 0044C840, 00401D80, and 004705B0. The owner action is therefore observed as a call boundary; its item-array, selection, frame, and UI effects are not part of this oracle. The string and script helpers are also boundaries.

## Original behavior

The event method is reached on the PropModel subobject at root +0x150. If that subobject's state at +0x70 equals 1, 00444120 calls 0044C840 with the Player pointer from DAT_00503A78+0x30 as receiver, the PropModel root as argument 1, and the action code as argument 2. When that action is 1002, it returns immediately after 0044C840 and never constructs or forwards the "use" event.

All other tested paths construct and dispatch "use" through the script subobject at PropModel root +0xD4. Thus attach code 1001 performs the owner action and then forwards "use"; a non-valve state skips the owner action and still forwards "use", even for action 1002.

| PropModel state | Action | 0044C840 | "use" dispatch |
| ---: | ---: | ---: | ---: |
| 0 | 1001 | skipped | yes |
| 0 | 1002 | skipped | yes |
| 1 | 1001 | Player receiver, root argument | yes |
| 1 | 1002 | Player receiver, root argument | skipped by early return |
| 1 | 7 | Player receiver, root argument | yes |

## Native port

src/hose_valve_callback.h expresses this call/return decision. The Training valve candidates are built only from PropModels whose S_Script01 is makevalve, so the current native callback input state is 1. The input loop applies its local attach/detach state when the owner action is called and now omits the script event on detach code 1002.

The next bounded owner slice now has its own original-P-code comparison: `item-selection-pcode-audit.md` covers the slot array, selection gate callsite, slot-4 callback order, detach first-available scan, mode change and UI service arguments through the pre-frame boundary. The actual Axe gate state, item callback bodies, valve transform/frame math, pointer/mode-3 stores, and follower parenting remain open. The Player updater's cached dispatch-before-refresh timing and complete native view/candidate integration also remain open.

## Reproduction

The original cases are generated with tools/ghidra/HoseValveCallbackPcodeOracle.java and stored in valve-callback-pcode-oracle.json. The native check is hose_valve_callback_pcode-check and is registered as the hose_valve_callback_pcode CTest.
