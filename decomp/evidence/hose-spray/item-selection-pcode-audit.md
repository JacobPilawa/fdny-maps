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

After successful Hose Spray selection, `0044C840` resolves the valve root's local frame at `valve+0xD0` through `004977B0`. The resolver follows parent pointers at frame `+0x40`, composes each parent with `00497300`, and finishes with frame copy `0042D0A0`. Correction from owner-level P-code: `00446EE0` copies the first basis vector at resolved-frame `+4..+0xC`, not the translation at `+0x28`; `0042D2B0` negates its three components. The resulting negated axis is stored at Hose Spray `+0xC7C..+0xC84`.

The owner then calls the valve root's vtable `+0x18` to obtain a frame-like object and copies its `+0x28` three-word position into Hose Spray `+0xC70..+0xC78`. It stores the valve-root pointer at Hose Spray `+0xC6C`, switches to mode 3 with `00463370`, then reads the first generated stream-pose node through the list head at Hose Spray `+0xCB8`, caches that node pointer at `+0xCD0`, and copies the position from `+0xC70..+0xC78` into node `+8/+0xC/+0x10`. The owner then hides the item UI at Player `+0x4C0`.

`tools/ghidra/HoseAttachmentFramePcodeOracle.java` now runs the original `0044C840` owner through `004977B0`, `00446EE0`, `0042D2B0`, the controlled valve `+0x18` getter, mode setter, and first pose-node write. Four frame trees (root-only, rotated parent, two-parent chain, and signed zero) compare the three Hose vectors bit-for-bit (36 words), mode, saved object identities, and ordered HUD/hide calls with zero mismatches. Frame-tree values, getter output, and object identities are controlled inputs; real Training valve frames and the runtime caller remain open. Native comparison: `src/hose_attachment_frame.h` and `tools/hose_attachment_frame_pcode_check.cpp`.

### Hose Spray stream-pose list

Constructor `004634F0` calls `00482850` on the pose subobject beginning at Hose `+0xC70`. The linked list's head, tail, and count are at Hose `+0xCB8`, `+0xCBC`, and `+0xCC0`; the current node cache is `+0xCD0`. Fresh construction creates 12 nodes through `00485D20` (the count setup uses `240/20`). The Training initializer path then resizes from the `touchhose 25` parameter through `00482780`, which computes `25*24/20` and truncates to 30 nodes. `00485D20` allocates 0x30-byte nodes and links them in order; the first node is reachable at the list head. Its position is at `+8/+0xC/+0x10`; two three-float vectors occupy `+0x14..+0x28`. The constructor does not visibly initialize node `+0x2C`, so preserve that as unknown rather than defaulting it. `004827B0` separately writes pose defaults and does not resize the list. These counts and links are deterministic and this traced path contains no RNG draw.

## Native integration and remaining scope

`src/hose_item_selection.h` models the five Training slots, selected index, mode change, slot-selection callbacks and detach rescan. `src/hose_attachment_frame.h` now models the controlled owner frame commit through the first pose-node position write. The runtime still has only indexed Axe/Hose Spray state from `equip axe` / `touchhose 25`; Space/Shift does not yet apply the source-derived attachment frame, initialize/resize the original pose list, reparent the Hose follower, or reproduce original UI/stream effects. Axe busy-state cadence, the real Training valve-frame/getter inputs, cached-target schedule, and suppression path remain separate work. No live-game inventory capture has run.

Executable SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
