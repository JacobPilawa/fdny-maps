# RCS_MiscEmitter child-particle path

Snapshot: 2026-10-12. Read-only Ghidra 12.0.4 queries used the hash-pinned FDNYFirefighter.exe (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). The preserved project copy at `/private/tmp/fdny-ash-transform-analysis/fdny` was queried with `-noanalysis -readOnly`. Entry 00444D00 was defined only in that isolated scratch copy after confirming its vtable reference at 004E1458 and method prologue. The preserved Ghidra project was not changed.

## Factory and particle children

Factory 00442900 parses the RCS_MiscEmitter data and selects a child effect by its type word. Its error string names TE_MiscEmitter and `sharedents\MiscEmitterEnt.cpp`. Types 5 and 6 allocate the 0xA8-byte emitter object with constructor 00445600 and set mode 2 or 1. Startup class registration includes RCS_MiscEmitter (`src/startup.h`); this trace does not establish that the Training BSP contains a live instance.

The 00445600 object installs vtable 004E1438, initializes a child pointer list at +0x4C with count at +0x50, and stores the mode at +0x64. Its mode-specific child constructor 00445720 allocates four particles with resource-data address 00502D40 for mode 2 or three with 00502D78 for mode 1. Each child goes through 0047F850, then the emitter sets its +0xA4 word to 0xA0, byte +0xAD to 1, and word +0xB0 to 4 before appending it to the list.

## Update and shared particle routine

Vtable entry 004E1458 maps to 00444D00. The method checks emitter state, resolves its own transform through 004977B0, updates the child cached-geometry and scalar fields for modes 1 or 2, then walks every child in the +0x4C/+0x50 list and calls 0047F960(child, renderer). In mode 2 the method writes a float to each child at +0x150; that is not the particle transform-parent field.

The shared 0047F960 particle routine is reached from several unrelated callers, including the Player ash list updater 004467E0 and this miscellaneous-emitter callback. These callers do not imply one shared schedule or identical particle parameters. The miscellaneous-emitter vtable callback's recurring owner/cadence and its reachability from Training remain open.

## Transform-parent offset check

The ash particle's transform begins at particle +0x34. Its parent pointer is transform +0x40, therefore particle +0x74. The particle constructor calls 0042D2D0(particle+0x34, 0), and helper 0047F500 explicitly writes zero to particle +0x74. The Player ash updater 004467E0 calls 00446510 and then 0047F960 for each listed particle; neither that updater nor the cached geometry routine writes +0x74. A whole-listing FieldStoreAudit at offset 0x74 returns many unrelated object/stack matches; the only particle-specific direct store identified here is the constructor's zeroing in 0047F500. No nonzero ash-parent setter was found by this bounded static trace, but pointer arithmetic or indirect services outside the inspected path are not ruled out.

The original ash geometry oracle seeds the transform at particle +0x34 with parent pointer +0x74 null, then executes 0047F5A0 through the real 004977B0 resolver and 00495D20 vertex transform. Thus null-parent resolver-plus-geometry behavior is already compared together. A non-null parent chain through geometry and polygon-list insertion is not covered by that fixture; generic non-null transform chains are compared separately.

## Ghidra queries

- `AddressXrefs.java 00445600 00444D00 0047F960 004467E0 0047F850`
- `AddressXrefs.java 0042D2D0 0047F500 0047F960 004467E0`
- `DataValue.java 004E1438 004E1458`
- `DecompileFunctions.java 00442900 00445720 00444D00 0047F850 0047F500 004467E0 004977B0 0042D2D0`
- `FieldStoreAudit.java 0x150` classified the emitter's float write; `FieldStoreAudit.java 0x74` bounded the transform-parent scan. The script also reports stack locals and unrelated class fields at the same offset, so its output is not a class-wide no-write proof.

Raw query output remains outside the repository. This note preserves the shared effect path for later full-game work without claiming Training runtime behavior.
