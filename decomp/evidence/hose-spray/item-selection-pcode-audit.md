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

The Axe gate's initialized value is source-established. Factory `0044D080` constructs it with `00460A50`, then calls its vtable `+0x08` initializer `0045F970`. That initializer dispatches `+0x08` on both Axe subobjects; their `004E27F0+0x08` entry is `00460250`, which sets the receiving subobject's `+0x368` to zero. For the child at Axe `+0x648`, that is Axe `+0x9B0`, so `0045F930 → 004E27F0+0x38 / 0045FA50` initially allows switching. The field is stateful: updater `0045FAD0` clears `DAT_00502E04`, dispatches child `+0x18` updates, then writes `Axe+0x9B0=3` if a child raises that flag. The successful hit path in `00460B90` raises it; handler `00461120` reaches that path when the child state at `+0x368` is 1. Child animation/update paths later change and clear that child state. Native's default-true adapter matches the initialized idle state, not all in-play states; Training update cadence and complete axe action behavior remain open.

The Axe activation callback is also resolved. Vtable `004E2868+0x3C` points to `0045F9B0`; it calls `0045F620(this,active)`, then conditionally forwards `+0x3C(active)` to the `+0x200` child and always forwards to the `+0x648` child. `0045F620` writes the Axe active byte at `+0x158`, calls the item animation service, and on activation reattaches its follower to Player frame `+0x34A4`. Both child `+0x3C` entries reach `0045FA30`; its shared activation logic sets the child active state, and deactivation also clears Player byte `+0x345C`. The animation service internals and original frame scheduling remain separate work.

## Source-mapped attachment frame commit

After successful Hose Spray selection, `0044C840` resolves the valve root's local frame at `valve+0xD0` through `004977B0`. The resolver follows parent pointers at frame `+0x40`, composes each parent with `00497300`, and finishes with frame copy `0042D0A0`. The owner extracts the resolved translation with `00446EE0` and negates its three components with `0042D2B0`.

The owner then calls the valve root's vtable `+0x18` to obtain a frame-like object and copies its `+0x28` three-word position into Hose Spray `+0xC70..+0xC78`. It copies the negated resolved valve translation to Hose Spray `+0xC7C..+0xC84`, stores the valve-root pointer at Hose Spray `+0xC6C`, and switches to mode 3 with `00463370`. Finally, it reads the first generated stream-pose node through the list head at Hose Spray `+0xCB8`, caches that node pointer at `+0xCD0`, and copies the first three position words from `+0xC70..+0xC78` into node `+8/+0xC/+0x10`. The owner then hides the item UI at Player `+0x4C0`.

`004977B0`'s parent-first transform arithmetic already has an independent four-case original-P-code comparison in `evidence/scripts/training-setashdensity/ash-transform-chain-pcode-oracle.json`. The Hose owner fixture still deliberately stops before this path: no P-code fixture currently joins the valve object's real Training frame to the owner commit, and the native host has not implemented these attachment vectors, pointer stores, or stream-pose-node writes.

## Native integration and remaining scope

`src/hose_item_selection.h` now models the five Training slots, selected index, mode change, slot-selection callbacks and detach rescan. The runtime creates indexed Axe/Hose Spray state from `equip axe` / `touchhose 25`, and Space/Shift uses the indexed transaction for attach/detach before updating the existing local visual/tool state. Item `+0x3C` bodies, axe gating state, Hose Spray follower/frame parenting, valve-frame transform, real UI effects, Hose Spray creation helpers, cached-target schedule, actor stream and suppression are still separate work. No live-game inventory capture has run.

Executable SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
