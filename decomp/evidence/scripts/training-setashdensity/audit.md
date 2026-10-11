# Training `setashdensity`

## Scope and evidence

Static analysis of the local read-only Ghidra 12.0.4 project for `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). The Ghidra decompilations and instructions named below were refreshed with `analyzeHeadless -noanalysis -readOnly`; local raw exports are ignored by the Git mirror.

Training's preserved entity records contain three commands: Player entity 115 starts with `setashdensity 0`; BoundsTrigger 170 sends `setashdensity 0.5` to Player on touch; BoundsTrigger 189 sends `setashdensity 0` on touch. These are authored command values, not observations from a live run.

## Original path

- Player's command handler is `00451870`. Its `setashdensity` branch reads one float argument with `0040CC50`; that helper copies the token and calls `sscanf` with `%f`, then `%g` if the first conversion fails. On success the handler calls `0044BC90` with the Player base (`this - 0x140`) and parsed float.
- `0044BC90` adds `0x55c` to the Player base and calls `00446350`. `00446350` stores at child offset `+0x54`, absolute Player offset `+0x5b0`. It clamps values above the scalar at `004DFC24` (`1.0f`) to 1 and values below `004DC50C` (`0.0f`) to 0. The raw x87 comparison sequence is retained in `assembly.txt`.
- Player constructor `00453D50` calls `004469F0` on Player `+0x55c`; this initializes the child but does not assign its `+0x54` field. Player primary vtable `004E1C48` slot 8 (`+0x20`) is `0044D550`, the Player initialization method. It calls `004465D0` on the same child; that method builds the 100-entry object list and assigns child `+0x54 = 1.0f`. Thus the verified post-initialization default is 1.0 before the Training Player startup command overwrites it with 0.
- The Player movement callback `00452570` is slot 0 of the secondary vtable `004E1C40` installed at Player `+0x140`. Its call at `00452926` passes `this + 0x41c` to `004467E0`; secondary base `+0x140` plus `+0x41c` is the same Player `+0x55c` child. `004467E0` updates the pool, then assembly at `004468A2–004468B3` loads pool count `+0x3c`, multiplies it by density `+0x54`, and converts the result through `004B9400` before scanning that many leading slots. `004465D0` constructs 100 slots, so the authored values produce scan limits of 0, 50, and 0. An inactive slot in the scanned prefix can spawn through `00446470`; this path has conditional shared-RNG calls. Pool construction, finite animation, and visibility/submission are now source-mapped; native pool wiring, ash-specific rendering, and the Player callback's complete frame schedule remain open.

## Pool construction and finite animation

`004469F0` constructs two adjacent `0x1c`-byte descriptor containers with `0047DBA0`; that constructor initializes their flags to `0x35`. On its one-time path, `004465D0` adds five seven-dword records to each container through `0047DB10`. The first-dword identifier is `200` in container 0 and `201` in container 1. The remaining six dwords, grouped as the three scalar arguments and the trailing three-word value, are:

| Entry | Three scalar words | Trailing three words |
|---|---|---|
| 0 | `3f000000, 3f800000, 3e4ccccd` | `3f666666, 3dcccccd, 00000000` |
| 1 | `3f000000, 3f800000, 3e4ccccd` | `3f666666, 3f000000, 00000000` |
| 2 | `3f000000, 3f800000, 3e4ccccd` | `3f000000, 3f000000, 3f000000` |
| 3 | `3f800000, 3f800000, 3e4ccccd` | `00000000, 00000000, 00000000` |
| 4 | `3f800000, 00000000, 3e4ccccd` | `00000000, 00000000, 00000000` |

The same five word groups are added to both containers. The initializer then changes each container's flags with `(flags & ~1) | 8`, producing `0x3c`. It allocates 100 objects of size `0x16c`; object index `i` is constructed from container `i & 1`, and its pointer is appended to the list at manager `+0x38`/count `+0x3c`. The manager's density field `+0x54` is set to `1.0f` at the end of this initialization. `004463F0` calls `0047F850`, whose `0047F500` setup writes active byte `+0x30 = 1`; the constructor then writes that object's deadline `+0x168 = 0`.

The descriptor flags also determine a finite animation lifecycle. `0047D2B0` uses bit 0 to choose looping; with bit 0 clear it clamps an index past the descriptor count to the final entry and reports completion. `0047D660` advances the index when its computed deadline is due and, on completion, clears active byte `+0x30` when bit 3 is set. For a newly constructed, otherwise untouched pool, the first `004467E0` call therefore enters `00446510`'s active/deadline branch for each of the 100 entries before the density scan, producing four direct RNG calls per entry (400 total). That count is conditional on no intervening writes to those entry fields; the exact first callback relative to Training startup remains unresolved. The Training `setashdensity 0` command changes the later prefix scan and does not itself clear these constructor fields.

## Direct RNG branches (partial call graph)

Ghidra `AddressXrefs.java 00498B30` lists direct calls from `004467E0`, `00446470`, and `00446510` at the callsites below. The `004467E0` source first walks all pool entries, calling `00446510` and then `0047F960` for each entry. It then scans the density-selected prefix. For each inactive entry in that prefix, the visible direct RNG order is:

1. `004467E0`: `00498B30(0, 6.2831855)` for an angle.
2. `004467E0`: `00498B30(10, 400)` for a distance scalar.
3. `00446470`: `00498B30(0.5, 1.5)`, stored at entry `+0x1c`.
4. `00446470`: `00498B30(0.5, 1.5)`, stored at entry `+0xa8`.
5. `00446470`: `00498B30(-80, -10)`, passed to the vector update after setting active byte `+0x30` and clearing deadline `+0x168`.

The separate update branch in `00446510` is taken only when active byte `+0x30` is nonzero and unsigned deadline `+0x168` is at most global time `[0x00502AFC]`. It first writes deadline `global time + 99,999,999`, then directly draws, in order, `(-40,40)`, `(0,10)`, `(-40,40)`, and `(-30,-5)`. This branch is distinct from spawning; a newly spawned entry is encountered by this update pass on a later call to `004467E0`.

These are only the direct `00498B30` calls visible in these three functions. Ghidra `CallGraphPath.java` found no statically resolved direct-call path from `0047F960` to `00498B30` after visiting its 177-function direct-call closure; it found no unresolved direct targets and did not hit its function limit. It did encounter 130 computed callsites that it could not follow, so indirect shared-RNG use remains unresolved. A positive-control search from `00446510` reaches `00498B30` at `00446548`. Thus the five-call activation path and four-call due-update path are not complete per-frame or whole-run RNG counts. The native port does not yet connect this pool or consume these draws. Ghidra xrefs, graph output, and instruction listings are locally retained in ignored files in this directory; the reusable tools are `tools/ghidra/AddressXrefs.java`, `ExportFunctionAssembly.java`, and `CallGraphPath.java`.

## Presentation path partially mapped

`0047F960` runs for each pool entry after `00446510`. When active byte `+0x30` is set, it selects an internal state from float field `+0xb0`, calls helper routines, and submits a polygon with `geWorld_AddPolyOnce` if its visibility path allows it. With byte flag `+0xac` set, it averages stored vectors and tests the result with `geWorld_GetLeaf` and `geWorld_MightSeeLeaf`; a false visibility result returns before submission. A second `geWorld_AddPolyOnce` is reachable when flag bit 1 at `+0x18` is set and the helper-derived state values differ. FireRescue's `0044B060` callback also calls `0047F960` for four sprite records after its own visibility/range gates; the native `FireRenderer` draws those Training fire visuals with recovered geometry, UV, and opacity components, while full parity remains open. The shared address does not establish that the Training ash-pool descriptor setup or its rendered output matches those four fire records. Helper field meanings, exact ash-pool vertex/texture values, transitive RNG calls, and native ash-pool visual parity remain open.

## Native status and limits

`Player::ashDensity` now starts at the source-backed 1.0 default. The native host uses `strtof`'s prefix conversion semantics to preserve the original `%f`/`%g` scan behavior, then stores the state and applies the same verified clamp. The existing `--script-check` runs startup and reports 0.0 after Player entity 115's command. The two BoundsTrigger commands are event bindings and are not exercised by this startup-only check.

This ports the command state only; the native pool, its source-driven update/count rule, RNG calls, and visible particles are not yet wired. No original live capture or visual parity result is claimed. The refreshed local raw Ghidra exports are in `functions/`, with the emitter/update instructions in `effect-assembly.txt` and Player vtable words in `player-vtable.tsv`; all raw exports remain ignored by the Git mirror. The `004B9400` conversion's decompilation and instructions are also preserved under `../rng/004b9400_FUN_004b9400.c` and `../fire-update/player-exposure-timer-followup/random-conversion-assembly.txt`.
