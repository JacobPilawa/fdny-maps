# Hose Spray item-selection transaction audit

Date: 2026-10-12

## Training inventory facts

The item pointer array begins at `Player+0x3564`; its count is `Player+0x3568`, and its selected zero-based index is `Player+0x357C`. Training entity 115's `equip axe` command flows through EVS handler `00451870` to `0044D080`. Name lookup `0044CA50` maps axe to kind 1; the factory allocates `0xA94` bytes, invokes constructor `00460A50`, stores the item at slot 1, then calls `0044C2A0(Player,1,false)`. The axe constructor installs vtable `004E2868`. The selection helper activates slot 1 when the prior selection is zero.

Training's `touchhose 25` command reaches `0044CAB0`; Hose Spray is stored at slot 4 and starts in mode 2 at item `+0xC68`. The command does not select it. This proves the Training slots used by the new native indexed inventory adapter: axe 1 and hose 4. It does not establish axe's full action behavior or any other game's inventory layout.

## Owner transaction behavior

`0044C840(Player,valveRoot,action)` first checks the expected five-slot array and reads Hose Spray from slot 4. Action 1002 calls `00463370(hose,2)`, then `0044C360`, then hides the item UI at `Player+0x4C0`. Other action codes take the attach branch. With a non-null Hose Spray, attachment selection behaves as follows:

- Current slot 4: keep selection and proceed to frame construction.
- Current slot 0: write selected index 4, call hose `+0x3C(1)`, then refresh HUD.
- Any other selection: call its vtable `+0x38()`; false hides UI at `Player+0x508` and returns; true calls old item `+0x3C(0)`, writes slot 4, calls hose `+0x3C(1)`, and refreshes HUD.

`0044C360` scans populated slots from zero upward and delegates to `0044C2A0(Player,index,true)`. Forced reselection bypasses the old item's `+0x38` gate. The first populated slot succeeds; if selection changes, old `+0x3C(0)` runs before the selected-index write/new `+0x3C(1)`, followed by HUD mode 5. If none are populated, it writes selected index zero. The final owner path hides UI at `Player+0x4C0`.

Mode setter `00463370` writes `item+0xC68` only when changed. Transitions out of 0/1 into 2/3 call HUD mode 6 with parameter 4; other changes call mode 5 with a null parameter. Detach from mode 3 to 2 therefore emits HUD mode 5 before the first-available rescan. Detach does not clear the saved valve pointer `item+0xC6C`; mode 2 makes that stale pointer irrelevant to candidate action selection until a later attachment overwrites it.

## Original execution comparison

`tools/ghidra/HoseItemSelectionPcodeOracle.java` runs the hash-pinned PE32 through `0044C840`, `0044C360`, `0044C2A0`, and the real `00463370` setter. The actual array helpers and scan logic execute. Only item `+0x38/+0x3C`, HUD `004571A0`, and hide `004A11A0` are controlled service boundaries. Attach samples stop at `004977B0`, after selection and HUD refresh but before valve-frame math or the pointer/mode-3 stores.

The eight owner cases cover slot-0 direct switch, successful/blocked Training axe switch, already-selected Hose Spray, missing Hose Spray, first populated detach target, first target already selected, and mode-2 detach. A separate `0044C360` case checks the no-candidate selected-index clear. The fixture compares selected index, inventory slots, hose mode, saved-pointer state and the ordered boundary-event sequence. `hose_item_selection_pcode-check` reports zero mismatches across these original cases.

The controlled axe gate result is an input to the owner oracle, not proof of a live axe state. Vtable `004E2868+0x38` resolves to `0045F930`, which reads the axe's constructed subobject state through `004E27F0+0x38` (`0045FA50`: returns whether child `+0x368` / axe `+0x9B0` is zero). The field's value after all item initialization and during normal Training has not been captured. Native uses a default-true adapter result so standard Training attaches; exact stateful gate behavior remains open.

## Native integration and remaining scope

`src/hose_item_selection.h` now models the five Training slots, selected index, mode change, slot-selection callbacks and detach rescan. The runtime creates indexed Axe/Hose Spray state from `equip axe` / `touchhose 25`, and Space/Shift uses the indexed transaction for attach/detach before updating the existing local visual/tool state. Item `+0x3C` bodies, axe gating state, Hose Spray follower/frame parenting, valve-frame transform, real UI effects, Hose Spray creation helpers, cached-target schedule, actor stream and suppression are still separate work. No live-game inventory capture has run.

Executable SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
