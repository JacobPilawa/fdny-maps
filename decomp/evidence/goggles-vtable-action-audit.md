# Goggles vtable and ACTION callback audit

## Result

The original goggles equipment object and its left-button `ACTION` callback are statically identified. The constructor at `00462960` installs vtable `004E2938`; its `+0x14` entry is `00462E80`. Player update `0044F340` calls the selected equipment object's vtable `+0x14` when `ACTION` is pressed. `00462E80` rebuilds a list of qualifying nearby actors and associated effect/overlay records. This establishes the scan callback route, but not the exact runtime input binding, Training startup inventory, camera target convention, or what the player sees in a live frame.

## Proven static edges

1. **Equipment name to item kind.** `0044CA50` compares the equipment name against `axe`, `extinguisher`, and `@goggles`, returning kind codes 1, 2, and 3 respectively (otherwise 0). It is called by equipment operations `0044D080`, `0044D270`, and `0044ECD0`.
2. **Kind 3 allocation and insertion.** `0044D080` allocates `0x600` bytes for kind 3, calls constructor `00462960`, then invokes the new object's vtable `+8` method with the Player as argument. It stores the object in the Player item array at index 3. The constructor `00462960` installs `&PTR_FUN_004E2938` into the object. Initialization method `00462A60` calls common item setup and registers a UI/resource named `Goggles`; its strings also include `@goggles` at `004E1890`.
3. **Confirmed goggles vtable ACTION slots.** The preserved Ghidra data values at `004E2938` onward are:

   | Vtable byte offset | Target | Evidence-based interpretation |
   | --- | --- | --- |
   | `+0x14` | `00462E80` | ACTION pressed handler; see dispatch below |
   | `+0x18` | `0043F660` | Empty function body in preserved listing |
   | `+0x1C` | `0043F660` | Same empty function body |
   | `+0x20` | `00462730` | Per-frame method candidate; not fully audited here |

   Values were read with `DataValue.java`; the table address is referenced by constructor `00462960` and the sibling routine `00462D60`.
4. **Player ACTION dispatch.** In `0044F340`, the selected item index is read from Player offset `+0x357C`. When the Player action gate permits dispatch, `ACTION` pressed is queried through `00438550`; if true the selected object's vtable `+0x14` is called. Held and released states use `00438480` / `00438620` and slots `+0x18` / `+0x1C`. Thus the goggles object's press path reaches `00462E80`, while its held/release entries resolve to the empty `0043F660` routine.
5. **Scan candidate filter and result writes in `00462E80`.** The callback obtains the Player owner from item offset `+0x54`, gets the owner's position through virtual slot `+0x18`, and scans the engine actor list. It considers actor records whose kind at `+0x18` is 6 or 2, computes squared distance from the Player, and rejects values above the float constant at `004E1108`. For kind 2 only, actor state `+0x120` equal to 5 or 6 is excluded. Accepted records are appended to the goggles object's list at `+0x5C0`; count is `+0x5D8`, capacity `+0x5C4`. Each record stores the actor pointer, selects a transform at goggles offsets `+0x1FC` (kind 6) or `+0x218` (kind 2), stores an active byte, and stores square-root distance. On completion the callback resets/starts associated effects, sets object byte `+0x364`, and records current game time at `+0x250`.

The scan routine's control flow and fields are visible in the already-preserved decompilation [00462E80](item-pose/live-extinguisher/message-routing/00462e80_FUN_00462e80.c). The constructor is in [00462960](item-pose/live-extinguisher/callback-refs/00462960_FUN_00462960.c). An equivalent export is also present under `startup-factories`; neither file assigns semantic names to actor kinds, transforms, or effect fields.

## Inference and limits

- **Strongly supported inference:** `00462E80` is the goggles scan because it is the `+0x14` entry of the object class created for `@goggles`, is reached on ACTION press, filters actor targets by distance/state, and builds per-target records and timed effect state. No scan-specific string is needed for this route.
- **Not established:** whether any Training path equips kind 3 at startup; whether the `SELECT_GOGGLES` control can select it in the shipped Training configuration; actor kind meanings and the state meanings 5/6; the units/meaning of the squared-distance threshold; whether the effect records render target boxes, text, or another cue; exact camera transform convention; and whether Training exposes targets of these classes.
- The string-reference query for `Goggles`, `goggles`, `scan`, and `SELECT_GOGGLES` found the control string, `@goggles`, and UI label references. It does not prove the binary has no other scan-related text because it only searches defined strings containing those exact substrings.
- `00462730` appears at vtable `+0x20` and performs per-frame work over the object's scan records, but this audit does not reconstruct its rendering or update semantics. Keep this as a follow-up full-game dependency.

## Live Training trace still needed

Yes. Static analysis identifies the behavior, but a controlled live trace is needed to establish Training reachability and visible results. At the main menu, capture a baseline through level load, then record:

1. Player equipment indices/pointers and selected index after Training startup, including whether kind 3 is present.
2. Control-state transitions for `SELECT_GOGGLES`, `ACTION` pressed, held, and released; resolve the default physical input for the selection control.
3. On each ACTION press, log `00462E80` entry, actor-list candidates (kind, `+0x120` state where applicable, positions, and computed squared distance), accepted list count, and writes to item offsets `+0x5C0`, `+0x5D8`, `+0x364`, and `+0x250`.
4. Capture the resulting HUD/render/effect changes while facing a Training victim, facing away, and with no target nearby; then note whether target state changes when a victim is rescued or otherwise changes state.

These events will distinguish the proven static candidate filter from its live Training inputs, state transitions, and actual presentation. No native behavior is proposed in this audit.
