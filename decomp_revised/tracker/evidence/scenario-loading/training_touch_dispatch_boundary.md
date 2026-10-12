# Training BoundsTrigger contact and touch dispatch

## Scope and pinned input

This static trace uses `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Raw x86 instructions and vtable bytes were checked against the pinned raw-copy executable. Shared dispatch behavior is cross-referenced to same-hash, read-only Ghidra audits. The `00440CE0` and `00440DC0` function bodies were inspected from raw instructions in this pass; fresh Ghidra pseudocode exports for those two bodies are not saved in this workspace. No runtime capture or game modification was used.

## BoundsTrigger vtables and contact list

The BoundsTrigger root vtable at `004E1090` maps:

- slot `+0x14` to thunk `0043AFE0`, which adjusts `ECX` by `+0x24` to the embedded ScriptActor and jumps to shared updater `00470740`;
- slot `+0x20` to setup method `00441040`;
- slot `+0x48` to BoundsTrigger contact producer `00440CE0`.

The embedded ScriptActor vtable at `004E1088` maps slot 0 to `00440DC0` and slot `+4` to `00441A60`. Constructors `00441BA0` and `00441C60` install the root table and the embedded table. `00441A60` is derived command fallback and handles `makezone`; it is separate from the touch event path.

`00440CE0`, the root `+0x48` method, reads the trigger's model/user-data and bounds, obtains candidate geometry through virtual slot `+0x0C`, and checks overlap using `004817A0`. On a positive match, it forms a pair key through `0049D350` and appends the pair to the BoundsTrigger contact list at root `+0x100` using `004901E0`. Constructor `00441C60` initializes that list.

## Contact list to event dispatch

`00440DC0`, the embedded ScriptActor slot 0, checks the trigger model and child state, then calls `00470840` with the script event context and contact list at root `+0x100`. It then rotates the current/previous contact state through `00490430` and clears the current list through `0043AED0`.

The shared `00470840` path enumerates contacts, resolves the associated ScriptActor with `00470B50`, and dispatches actions through `004705B0`. Event strings in the pinned image are `touch` at `004E1008`, `touching` at `004E2F70`, and `untouch` at `004E2F68`. This establishes the static BoundsTrigger overlap → pair list → ScriptActor event-dispatch route.

The root constructor sets entity type `8` at `00441D2F` and enables category bit 1 through `0049D330(this,1)` at `00441D21`. The shared entity base initializes the registration mask at root `+0x14` to `0xFFEB`; virtual slot `+0x1C` reads that mask. Registration routine `00429490` checks it against manager filter `0x20` and dispatches accepted entities through manager `+0xE4/+4`. Manager `0049BF10` at owner `+0xE0` has nine type lists; helper `0049BC90` chooses the list by root `+0x18` and records the active member. Since `0xFFEB` includes `0x20` and BoundsTrigger type is 8, a BoundsTrigger taking this registration path is eligible for list 8. The normal manager pass at `0049BD20` dispatches member slot `+0x48`, which maps to producer `00440CE0`. The exact registration path and list membership of each Training instance have not been verified, so this establishes class eligibility and the manager route, not per-instance occurrence.

The root `+0x14` thunk reaches the child updater locally, but the recurring caller of that root slot and its ordering relative to the contact producer remain unresolved. A bounded follow-up found data references to thunk `0043AFE0` at vtable words `004E0E0C` (BModel), `004E10A4` (BoundsTrigger), and `004E1794` (FireRescue), but no direct code references to the thunk. Direct references to shared updater `00470740` are the known class-wrapper sites; none identifies a BoundsTrigger root-slot caller. Fixed-offset candidate `0046269D` is not evidence for this scheduler: raw bytes show `call dword ptr [eax+0x14]`, and a same-hash scratch Ghidra audit of `00462660` found a conditional self-vtable call outside its child-array loop, with no constructor, receiver type, or runtime edge connecting it to ScriptActor or BoundsTrigger. See [the candidate disposition](fixed_slot_candidate_0046269d_rejection.md). The fixed-slot scan does not cover computed-slot dispatch. Do not state same-pass delivery or frame latency: the missing evidence is a loop or callback that selects a BoundsTrigger instance and invokes root slot `+0x14` (or an equivalent adjusted child update).

## Separate `+0x164` model-flag path

BoundsTrigger setup at `00441040` checks whether the authored `touch` list contains `tell player escape` and stores the boolean at root `+0x164`. A direct-call sweep found two callers of `00440F50`, which consumes this flag:

- `0044FD4E` in carry/reset pass `0044FCE0`;
- `00450984` in final-exit processing `004504C0`.

The helper writes a mode to `+0x165` and may update Genesis model flags. These are later model-flag consumers; they are separate from the contact list and event-dispatch path above. The flag alone does not prove that a touch action has fired.

## Provenance and next work

- The pinned raw-copy EXE supplied the instruction and vtable checks for `00440CE0`, `00440DC0`, constructors, and table entries in this pass.
- The archived same-hash [Training named-zone audit](https://github.com/JacobPilawa/fdny-maps/blob/main/decomp/evidence/training-zones-runtime-audit.md) records BoundsTrigger setup, `+0x164` initialization, embedded vtable fallback, and `makezone` handling.
- The archived same-hash [carry entity mode audit](https://github.com/JacobPilawa/fdny-maps/blob/main/decomp/evidence/victim-ai-audit/carry-entity-mode-component.md) records `0044FCE0` and `00440F50` behavior.
- The archived same-hash [BModel contact/event order audit](https://github.com/JacobPilawa/fdny-maps/blob/main/decomp/evidence/training-door-contact-event-order-audit.md) records shared `00470840` event conversion and scheduler limitations; its BModel pair producer is a distinct system.

Next, identify the dynamic caller(s) of root slot `+0x14`. The manager-side producer is dispatched during the normal owner manager pass, but the embedded updater's scheduling and order relative to contact production remain unknown. Do not infer same-pass delivery or latency.
