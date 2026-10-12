# Training `tell player escape` recipient boundary

## Pinned inputs

Static analysis uses `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. No live game event was captured.

## Confirmed command path

Training's authored BoundsTrigger touch text includes `tell player escape`; the BoundsTrigger contact/event route reaches the shared ScriptActor event dispatcher, which parses and dispatches that command text. In the script-command dispatcher `0047A1F0`, the `tell` branch calls helper `004752A0` at instruction `0047A305`. The saved `004752A0` decompilation calls `00472270` and returns success. This establishes a parser/helper path for `tell`.

## Recipient lookup remains unproven

Raw disassembly of `00472270` (`00472270–004727C7`) contains direct calls to string, parser/container, logging, and cleanup helpers. It contains no direct call to actor-name lookup `00470C50`, no direct call to Player command handler `00451870`, and no indirect call instruction. The direct call/jump listing is preserved in [`00472270_direct_calls.txt`](00472270_direct_calls.txt). This bounds what the body itself proves; it does not rule out effects inside its callees or a later consumer of data it prepares.

A separate saved helper, `00477740`, maps `<console>` context to the name `player`, looks that name up through `00470C50`, and dispatches through `0049B7E0`. That helper is a concrete console-command recipient path, but no call or dataflow edge connects it to `004752A0` or `00472270`. It cannot be used as proof of Training's `tell` recipient.

Player handler `00451870` recognizes `escape` and reaches rescue completion through `004513A0` and `00451160`. The path from Training's `tell player escape` command to that Player handler remains open. Do not implement a rescue completion solely from trigger proximity or the matching command text; resolve the recipient dispatch or capture the original game's event first.

## Evidence and next work

- Fresh same-hash dispatcher decompilation: [`0047a1f0_script_command_dispatch.c`](0047a1f0_script_command_dispatch.c).
- The saved same-hash exports for `004752A0` and `00477740` are retained in the local previous-work reference but are not mirrored into the revised folder; their limited role and the missing edge are summarized above.
- The archived [Training command audit](https://github.com/JacobPilawa/fdny-maps/blob/main/decomp/evidence/training-script-command-audit.md) provides prior context for the parser route.
- The active trigger contact-to-event map is [`training_touch_dispatch_boundary.md`](training_touch_dispatch_boundary.md).

Next, resolve the argument's role in `00472270` and trace the post-parse `tell` consumer/dispatch path. Keep this as a distinct open edge until a recipient lookup and resulting Player command-handler call are proven.

## Focused raw-instruction follow-up (2026-10-12)

A fresh raw disassembly of the pinned EXE confirms the exact argument path at the branch already identified: in `0047A1F0`, instruction `0047A301` loads the parsed value from the stack into `ECX`, and `0047A305` calls `004752A0`. The same-hash saved body for `004752A0` passes the address of its local argument to `00472270` and then returns success. Thus this call is a per-value parser/helper step; these instructions do not themselves establish recipient resolution.

The body range `00472270–004727C7` was checked again against raw x86 disassembly. Its calls are direct calls to string/container, formatting/logging, allocation, and cleanup helpers; it has no direct call to `00470C50` (actor-name lookup), `00451870` (Player command handler), or any indirect call instruction. The call at `004725CA` is now identified as an ordered-tree iterator increment, not a parsed-value materializer: it passes the address of a 12-byte local iterator (`ECX = ESP+0x14` at that site) to `00432A60`. That routine updates the iterator's current-node field at offset `+8` and returns the iterator address in `EAX`. On the backedge to `00472310`, the caller checks the iterator node, reads the node's key record and string, and compares it with the current key; this consumes iterator state, not a recipient object. The same function initializes this iterator from global `DAT_00503078` via `00471C90` near its entry. This explains the `00432A60` dataflow but does not identify the semantic role of `00472270`'s stack argument or connect any parsed recipient to actor lookup.

Evidence for this bounded trace: raw `objdump -d` of the hash-pinned EXE at `00432A60–00432B49`, `00472270–00472310`, and `004725C1–004725D4`; SHA-256 rechecked as `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The saved same-hash Ghidra decompilation of adjacent `004727D0` independently shows the same iterator layout and use (`FUN_00471C90`, node/current checks, `FUN_00432A60` advancement); that adjacent routine supports the iterator interpretation but does not establish `00472270`'s semantics.

**Conclusion:** static evidence still does not prove that Training's `tell player escape` reaches the Player handler. Do not infer that it does (or that it cannot) from the command text, handler's matching `escape` branch, or absence of a direct call in this parser. `004725CA` is no longer an open candidate for recipient-object materialization: it advances the local ordered-tree iterator and its state is consumed on the caller's loop backedge. The next focused static query is to resolve `00472270`'s stack argument and establish what this routine does when called from the `tell` branch, then follow the actual post-parse recipient dispatch. If that edge escapes static resolution, an original-runtime trace of this exact Training event is the evidence needed to close it.
