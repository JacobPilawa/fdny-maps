# RescueBot enable-byte constructor audit — 2026-10-21

## Pinned image and method

`FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The saved `00453D50` Ghidra decompilation was checked against direct x86
disassembly of the factory, constructor, component constructor and sound-instance
initializer. The current body excerpts are
[`00453d50_player_rescue_object_constructor_fresh.c`](00453d50_player_rescue_object_constructor_fresh.c)
and [`00454080_create_player_rescue_entity_fresh.c`](00454080_create_player_rescue_entity_fresh.c).
No Ghidra process was running during this follow-up; this note records static
same-hash evidence plus direct disassembly, not a new Ghidra body export.

## Enable-byte path

The recurring callback receives `Player+0x32CC` and reads its enable gate at
component offset `+0x18`, i.e. `Player+0x32E4`. The only direct store to that
Player offset in the pinned image found by a full `objdump -d -Mintel` scan is
the `setrescuebot` command at `004524C6`; it writes the parsed nonzero boolean.

The normal factory allocates `0x3580` bytes at `00454080`, calls constructor
`00453D50`, then finalizes the object through `0049D450`. The constructor calls
`00481C40` with `this = Player+0x32CC`. That component constructor calls
`004A13A0` for sound-instance storage at component offsets `+0` and `+0x24`.
The direct instruction body of `004A13A0` initializes dwords `+4`, `+8`, and
`+0xC`, byte `+0x1C`, and the pointer at `+0x20`; it does not store component
`+0x18`. The direct instruction body of `00481C40` also contains no store to
component `+0x18`. The enclosing Player constructor and PlayerRescue init
contain no other direct `+0x32E4` store.

The factory uses the program's allocation wrapper. Its ordinary heap branch
reaches `HeapAlloc` with allocation flags zero, while a pool branch may also be
selected. Static evidence therefore does not establish a deterministic zero
or one default for `Player+0x32E4`; it establishes that the reviewed Player and
component constructors do not assign one. The lifecycle behavior on reused
pool memory is not resolved. Do not infer a retail default from Training's
explicit `setrescuebot 0` or from the absence of that command in other maps.

The host keeps the recurring RescueBot path behind an explicit script enable.
Default behavior, allocator/pool reuse, the relationship between the separate
`Player+0x340C` initializer context and recurring `+0x32CC` component, and live
callback cadence remain open. This audit changes no gameplay code and makes no
retail runtime or natural RescueBot parity claim.
