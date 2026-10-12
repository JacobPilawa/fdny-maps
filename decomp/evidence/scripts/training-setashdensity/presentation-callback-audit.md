# Training ash-pool presentation callback (`0047F960`)

Read-only Ghidra 12.0.4 audit on 2026-10-12 of `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Queries used the preserved program copied to `/private/tmp/fdny-ash-presentation-audit/`
and opened with `-noanalysis -readOnly`; the repository Ghidra database was not
modified. `DecompileFunctions.java`, `ExportFunctionAssembly.java`,
`AddressXrefs.java`, and `DataValue.java` were run against `0047F960` and its
presentation helpers. The existing direct-call closure result is retained in
`rng-callgraph-ash-render.txt` (177 functions visited, no direct path to
`00498B30`, 130 computed callsites not followed); this audit narrows its
interpretation rather than repeating that scan.

## Confirmed callback order and state writes

`004467E0` calls `00446510` and then `0047F960` for every one of the 100 pool
records. `0047F960` first checks byte `+0xAD` and may call animation update
`0047D660`, then checks byte `+0xA0` and may call `0047F2A0`. It then checks
active byte `+0x30`; inactive records return without presentation. An active
record chooses a state from float bits at `+0xB0` (values 0 through 4): state
0 copies the base position at `+0x5C..+0x64` to `+0xD8..+0xE0`, state 1 clears
lazy-cache byte `+0x78` if set, and state 2 lazily initializes cached
geometry through `0047F5A0`. States 3 and 4 preserve the state value for the
animation/vector path. The source has no default assignment to its local frame
count for an unrecognized state, so a port must preserve the state invariant
rather than invent a fallback.

When culling byte `+0xAC` is set, the routine averages the first `N` stored
vertex positions beginning at `+0xD8` (stride `0x24`), computes a world leaf using the
render context selected from `param_1`, and calls `geWorld_MightSeeLeaf`. A
negative visibility result returns before color selection and polygon
submission. The stride and fields are directly visible in the Ghidra
decompilation; this alone does not establish which ash descriptors set `+0xAC`.

Visible active records call `0047D6D0` for two frame-derived byte values and
`0047D420` for the current vertex source. If `+0xCC` is set, position is taken
from `+0xC0..+0xC8`; otherwise the helper result is used. If `+0xD4` is set,
one additional value is computed via `004B9400`. A scale at `+0xEC..+0xF4`
and selected byte value are then copied into repeated vertex/color records
covering `+0xEC..+0x15C`. The first `geWorld_AddPolyOnce` call is at `0047FCF9`.
When flag bit 1 at `+0x18` is set and frame indices returned by `0047D3E0` and
`0047D400` differ, a second vertex/color fill uses `0047D560`, followed by
`0047D860` / `0047D960` and a second `geWorld_AddPolyOnce` at `0047FE58`.

`0047F2A0` is a simulation step, not a random emitter: it integrates velocity
at `+0x88` into position `+0x7C..+0x84` using global `00502B04`, updates
orientation/position values at `+0x5C..+0x64` through vector helpers, and
clamps velocity components against zero. `0047F5A0` is lazy cached-geometry
construction: it transforms four local template vectors with the basis at
`+0x34`, writes results into `+0xD8`, `+0xFC`, `+0x120`, and `+0x144` regions,
then clears `+0x78`. Its static scratch/template block spans globals
`0050308C..005030B8`; one-time object construction for that block is guarded
by `005030BC`.


The isolated native component now reproduces the four cached position
vertices. The template is an XY square with corners `(-halfX,+halfY)`,
`(+halfX,+halfY)`, `(+halfX,-halfY)`, and `(-halfX,-halfY)`; every local Z is
zero. The transform scales local components, applies the resolved basis,
stores the linear result as float, then adds translation. Five original P-code
cases match every cached position bit and the byte clear. The comparison marks
the local template guard initialized and supplies no parent transform, so it
leaves `004977B0` parent-chain composition open.

## RNG interpretation

The apparent `00498B30` uncertainty is narrower than the earlier note suggests.
The only `004B9400` xref inside the `0047F960` path is `0047D6D0`,
`0047D660`, and the optional `0047F960` call at `0047FBFA`. Assembly shows
`0047D6D0` loads a descriptor float at `+0x8` or `+0x4`, multiplies it by
`255.0f` (`004E3524`), then calls `004B9400` to convert that x87 value into an
integer byte. `0047D660` similarly forms a frame deadline from descriptor
duration, animation scale, `1000.0f` (`004E0ED4`), and deadline `+0x14`, then
converts it before comparing against clock global `00502AFC`. The helper body
at `004B9400` only rounds the incoming x87 value; it does not read or advance
the shared RNG state. The optional call from `0047F960` follows the same
conversion helper and casts its result to a byte.

Therefore these known presentation/animation-helper paths do not consume the
shared RNG through `004B9400`; the four direct RNG draws in the active-particle
update `00446510` and five direct draws in activation `00446470` remain distinct
and are not canceled by a renderer draw. This does not close the 130 computed
call sites in the older direct-call closure: dynamic engine callbacks and any
other computed dispatch remain unproven. The ash render path calls through the
Genesis.dll IAT entry at `004DC0D0` from `0047FCF9` and `0047FE58`. The PE
import-by-name record and Ghidra external reference identify
`geWorld_AddPolyOnce`, whose list effects are mapped in
`resource-render-followup.md`. The other computed callsites remain open, so
whole-frame RNG parity is still unproven.

`00502B04` is zero in the static data snapshot but is read by `0047F2A0` and
other engine update routines; its runtime value is not established here.
Likewise, `00502AFC` is zero in the static image and acts as a runtime clock.
Those are initialization/runtime-state questions, not constants to bake into
the native component.

## Portability and open edges

The control flow for state gating, geometry cache writes, per-particle
visibility, frame/color selection, and the one/two polygon submission paths is
source-backed and safe to translate as a bounded native component. Both calls
resolve to the imported `Genesis.dll!geWorld_AddPolyOnce`; the wrapper's list
effects are documented, while live world/resource state remains open. Preserve
the original x87 rounding behavior and float operation order for parity.
Rendering cannot yet be claimed exact: the ash descriptor flags, five frame
durations, and collection-to-particle binding are now recovered in
`descriptor-lifetime-followup.md`; `resource-render-followup.md` maps IDs
200/201 to the authored `ash1.bmp`/`ash2.bmp` names and resolves descriptor
color/alpha operands. Exact bitmap copies are now present in the ignored local
asset extraction; runtime resource-table objects, computed render-context
helpers, global step-time setup, other indirect calls, and visible parity remain
unresolved. There is no live Training capture in this evidence.
Do not replace these gaps with placeholder geometry, material choices, or
callback defaults in a parity claim.

## Best next step

Use the source references (`0047F960` call from `004467E0` at `00446891` and
the FireRescue four-record loop in `0044B060`) to identify the actual Training
descriptor/resource setup and flags, then inspect candidate table values and
texture/material bindings. The polygon submission target is resolved; next
trace the remaining context/visibility helpers and connect the render payload
to native world-list state. A live Training capture is still needed to validate
the first callback timing and actual visible output.
