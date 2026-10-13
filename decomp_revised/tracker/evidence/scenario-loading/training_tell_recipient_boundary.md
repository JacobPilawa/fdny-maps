# Training `tell player escape` recipient boundary

## Pinned inputs

Static analysis uses `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. No live game event was captured.

## Confirmed command path

Training's authored BoundsTrigger touch text includes `tell player escape`; the BoundsTrigger contact/event route reaches the shared ScriptActor event dispatcher, which parses and dispatches that command text. Branch attribution is now corrected: `0047A305 -> 004752A0` is in the `help` branch, selected by comparison with `help` at string VA `004E3514`. The `tell` string is at `004E3504`; its branch calls `00475560` at `0047A5D4`.

## Recipient dispatch path

Raw disassembly of `00472270` (`00472270–004727C7`) contains direct calls to string, parser/container, logging, and cleanup helpers. It contains no direct call to actor-name lookup `00470C50`, no direct call to Player command handler `00451870`, and no indirect call instruction. The direct call/jump listing is preserved in [`00472270_direct_calls.txt`](00472270_direct_calls.txt). This bounds what the body itself proves; it does not rule out effects inside its callees or a later consumer of data it prepares.

At the `tell` branch, the loop at `0047A4F4–0047A634` obtains two values from `00473D20` using selectors 0 and 1, retains their reference-counted values, and forwards them to `00475560` at `0047A5D4`. The first value is the case-insensitive ScriptActor name key passed to `00470C50`; the second is the command passed to `00470150` on the first matching registry actor. The role boundary and Training's authored PlayerRescue class-to-handler route are established; live load execution, registry insertion order, and event timing remain open. The shared route and Bank BModel example are documented in [named_script_actor_dispatch.md](named_script_actor_dispatch.md) and [bank_vault_open_dispatch.md](bank_vault_open_dispatch.md).

A separate saved helper, `00477740`, maps `<console>` context to the name `player`, looks that name up through `00470C50`, and dispatches through `0049B7E0`. It remains a distinct console-command path; it is not used as proof of Training's `tell` recipient.

Fresh analysis of `InitializePlayerRescueEntity` (`0044D550`) proves that the `RCS_PlayerRescue` embedded ScriptActor at entity+0x140 is registered under the literal name `Player`. Its vtable slot +4 resolves to `00451870`, whose `escape` branch reaches rescue completion through `004513A0` and `00451160`. The separate `PlayerHose` initializer is another literal-name registration, but the hash-checked Training BSP contains one `RCS_PlayerRescue`, no `RCS_PlayerHose`, and no exact `Player` epair values. This resolves Training's authored class-to-handler identity statically. Factory admission and runtime registry insertion/event order remain to be captured live.

## Evidence and next work

- Fresh same-hash dispatcher decompilation: [`0047a1f0_script_command_dispatch.c`](0047a1f0_script_command_dispatch.c).
- Fresh same-hash PlayerRescue initializer and identity registration: [`0044d550_player_rescue_entity_init_fresh.c`](0044d550_player_rescue_entity_init_fresh.c).
- The saved same-hash exports for `004752A0` and `00477740` are retained in the local previous-work reference but are not mirrored into the revised folder; their limited role and the remaining argument/object edge are summarized above.
- The archived [Training command audit](https://github.com/JacobPilawa/fdny-maps/blob/main/decomp/evidence/training-script-command-audit.md) provides prior context for the parser route.
- The active trigger contact-to-event map is [`training_touch_dispatch_boundary.md`](training_touch_dispatch_boundary.md).

Next, capture a live map load and registry/event timing, then exercise the shared rescue-completion effect across all seven campaign rescues plus Training.

## Focused raw-instruction follow-up (2026-10-12)

A fresh raw disassembly of the pinned EXE confirms the branch mapping: `0047A229` and `0047A267` compare against `004E3514` (`help`), and the matching path reaches `0047A305 -> 004752A0`. The `tell` comparison is at `0047A4DC` against `004E3504`; its matching path reaches `0047A5D4 -> 00475560`. The `004752A0` wrapper's call to `00472270` therefore belongs to help handling, not the Training `tell` path.

The body range `00472270–004727C7` was checked again against raw x86 disassembly. Its calls are direct calls to string/container, formatting/logging, allocation, and cleanup helpers; it has no direct call to `00470C50` (actor-name lookup), `00451870` (Player command handler), or any indirect call instruction. The call at `004725CA` is an ordered-tree iterator increment, not a parsed-value materializer: it passes the address of a local iterator to `00432A60`, which updates its current-node field and returns the iterator address. This iterator evidence pertains to the help path and does not establish or negate the `tell` dispatch path.

Branch-map provenance: raw `objdump -d` of the hash-pinned EXE at `0047A1F0–0047A634` and `00475560–004756E9`; `strings -a -t x` plus the PE `.text` mapping locates `help`, `action`, and `tell` at VAs `004E3514`, `004E350C`, and `004E3504`. The executable hash remains the pinned SHA-256 above.

Evidence for this bounded trace: raw `objdump -d` of the hash-pinned EXE at `00432A60–00432B49`, `00472270–00472310`, and `004725C1–004725D4`; SHA-256 rechecked as `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The saved same-hash Ghidra decompilation of adjacent `004727D0` independently shows the same iterator layout and use (`FUN_00471C90`, node/current checks, `FUN_00432A60` advancement); that adjacent routine supports the iterator interpretation but does not establish `00472270`'s semantics.

**Conclusion:** static evidence establishes the Training `tell` argument order and lookup route, then identifies its authored `player` actor as `RCS_PlayerRescue`; the embedded vtable reaches `00451870`, and `escape` reaches the shared rescue-completion route. Live load execution, registry order, event delivery, and visible parity remain open.
