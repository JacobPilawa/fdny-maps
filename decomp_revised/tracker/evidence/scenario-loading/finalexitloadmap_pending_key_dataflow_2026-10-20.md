# `finalexitloadmap` checked and queued key dataflow

## Provenance

Read-only raw x86 review of `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Instructions were read directly from the copied executable with `objdump` and
cross-checked against the saved Ghidra exports. No retail runtime was captured.

## Result

The stage key supplied to `finalexitloadmap` is the same string checked for a
BSP and stored in the pending level descriptor. There is no separate forwarded
map context on this path.

1. Player handler `00451870` extracts each `finalexitloadmap` argument through
   `00473D20` and calls `00450A70` with that parsed string. The callsite is
   `00452313`–`00452386`; the matching handler branch is also preserved in
   [`00451870_player_script_command_handler.c`](00451870_player_script_command_handler.c).
2. At `00450AC9`–`00450AD3`, `00450A70` passes the command-key reference and
   a temporary output string to `00422DE0`. That helper builds
   `data\\maps\\<key>.bsp`; `004097F0` checks the resulting path.
3. On success, `00450C35`–`00450C5B` constructs
   `data\\resdefs\\rescue.rrf` and calls `0042A260`. The `LEA ECX,[ESP+0x80]`
   at `00450C48` points to the original `00450A70` stack argument slot (after
   the resource-string pointer is pushed). The setter receives that reference
   as its first value; the disassembly of `0042A260` shows it copies that
   string into EverGame `+0x660`. Its second value is the rescue RRF path at
   `+0x668`.
4. The checked path string is temporary. The pending request stores the
   original key, and `0042AB00` uses that key when constructing the subsequent
   `.bsp` load request.

The apparent second “context” parameter in the older `00450A70` decompilation
is a stack-variable alias. Raw stack offsets resolve the identity directly.
The shared host already applies the same authored key when selecting a sibling
stage package, so its key choice matches this static retail path.

## Remaining limits

The trace establishes key identity and static request flow only. It does not
prove that each BSP exists in the retail installation, that the queued request
is consumed successfully in a live game, how the old world is unloaded, or the
timing and visual behavior of linked-stage transitions.
