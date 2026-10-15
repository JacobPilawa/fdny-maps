# Genesis BSP face lightmap draw handoff (2026-10-15)

## Provenance

- `inputs/original/binaries/Genesis.dll`, SHA-256
  `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- `inputs/original/binaries/D3DDrv.dll`, SHA-256
  `7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7`.
- Read-only Ghidra 12.0.4 headless analysis used an isolated project under
  `/private/tmp/fdny-genesis-render-audit/renderer25`; the saved project under
  `analysis/ghidra` was not opened or changed. The D3DDrv table target at
  `0x10007BC0` was initially present only as a data reference, so the analysis
  manually disassembled and created a function there before decompiling it.
- The later row-copy trace used a separate isolated headless project at
  `/private/tmp/fdny-lm-d3d/d3d`. Its temporary logs were not committed; the
  function and instruction addresses needed to reproduce the row mapping are
  recorded below.
- This is static binary evidence. It does not establish runtime mode selection,
  frame cadence, or pixel parity.

## Plain-language explanation

Genesis reuses each face vertex's TEXINFO projection for its base texture and
lightmap. It snaps the lightmap origin down to a 16-unit sample grid, then adds
a half-sample offset when it builds lightmap coordinates. It stores each face
lightmap in one padded power-of-two surface. The D3D driver can draw the base
first and multiply the face lightmap over it. This traces the data and draw
state. Retail V and the viewer's uploaded RGB both progress through rows in
the same order, though final pixel parity is still unverified.

## Face record reaches the driver draw callback

Genesis builds 136-byte runtime face records in `FUN_10035110`. The existing
lightdata trace maps width/height at `+0/+2`, minima at `+4/+8`, source RGB
pointer at `+12`, face index at `+20`, and the cached per-face bitmap at `+24`.
`FUN_10025770` creates and caches the light bitmap through driver table `+0x38`
and stores the returned handle at face record `+24`.

For visible geometry, `FUN_1003DA90` selects the record for the face. It passes
the record pointer as `psVar17` when the TEXINFO high-byte sign check allows
light data; otherwise it passes null. The same routine assembles a 36-byte
per-vertex stream and calls `FUN_100233B0`. In the callsite, `ECX` points to
that stream and `EDX` is its vertex count. The wrapper either calls the
selected driver table's `+0x80` method immediately when its low flag bit is
set, or queues the descriptor pointer and vertex stream through
`FUN_100232D0`. Queue type 0 is later sent to `+0x80` by `FUN_100231E0`.

In the hash-pinned D3DDrv, the table's `+0x80` target is `0x10007BC0`.
`FUN_10007BC0` receives the stream in `ECX` and vertex count in `EDX`. Its
stack arguments are the pointer returned by `FUN_10017250` from its input
object's `+0xAC` field, a pointer to four draw-context values, the face
descriptor, and raw flags. The descriptor and context are copied into the
driver's per-draw state before texture setup. Genesis initializes D3DDrv table
`+0xB0` with callback
`FUN_10033D60` at `0x10025FA4`; the driver calls that callback with the face
descriptor and a pointer for its update result.

`FUN_10033D60` reads the descriptor width, height, and face index, builds the
style-scaled RGB lightdata buffer, stores its pointer at descriptor `+12`, and
writes an update flag through the second argument. D3DDrv's `FUN_100071C0` and
`FUN_10007450` paths both call the installed `+0xB0` callback and pass that
update flag to `FUN_10006FA0` while preparing a texture stage. The `+0x80`
callback also has a raw flag bit `0x08` path that clears the descriptor before
one setup mode. The bit's public name and runtime use are not established.
In the `FUN_10007450` path, another branch tests descriptor `+0x10` and follows
descriptor `+0x18` (the cached bitmap handle) into nested texture state before
making an additional setup call; those nested fields are left unnamed.

This joins Genesis' per-face bitmap creation, draw descriptor, dynamic RGB
conversion callback, and D3DDrv textured draw setup. It does not prove that all
faces use the descriptor at runtime or establish all driver state semantics.

## Vertex coordinates and lightmap UV calculation

`FUN_10034650` caches two unshifted TEXINFO projections for each face vertex
in a 24-byte-stride record. On the ordinary vertex-builder path,
`FUN_10030230` transforms the vertex position and copies the first two cached
values into the 36-byte stream at offsets `+12/+16`. Its alternate
`FUN_100302A0` path copies five values from the per-vertex record into stream
offsets `+12..+28`. In `FUN_10007BC0`, the ordinary pair at stream offsets
`+12/+16` is copied as two floats into 12-byte per-vertex scratch records that
also hold the packed color. The first pair is at `0x10122A0C`; this is exactly
the state base `0x1011D9E8` plus `0x5024`, which the D3D helpers use as their
source-coordinate array. This resolves the secondary-coordinate source: it is
the same raw TEXINFO projection pair, not a separate Genesis-generated UV
stream.

The base-texture helper `FUN_100070B0` applies the face's two scale/offset
projections and the base bitmap mip shift to those raw values. The lightmap
helper `FUN_10007120` applies the descriptor minima and lightmap bitmap mip
shift. Its exact per-vertex calculation is:

```text
lightU = (projectedU - descriptorMinU + 8) / (16 * 2^mipShift)
lightV = (projectedV - descriptorMinV + 8) / (16 * 2^mipShift)
```

`FUN_10034750` defines each descriptor minimum as
`floor(minProjected / 16) * 16` (`0x10034bf0–0x10034c2a` floors the projection
minimum scaled by `1/16`; `0x10034c4b–0x10034c69` scales it back by 16 and
stores it). The helper at `0x100440a8` converts that grid-aligned value to an
integer. The formula above therefore uses the retail-snapped face origin, not
the unsnapped polygon minimum.

`FUN_10025770` creates the face light bitmap by passing its width and height,
one mip level, and format slot 1 to `FUN_10024e90` (`0x100257e0–0x100257ee`).
Genesis requests format flags 4 through `FUN_10024BF0` and stores the matching
format pair in slot 1 at `engine+0x38A30/+0x38A34` (`FUN_10024CB0`); it does
not request D3D format code 4. `FUN_10025770` uses that slot. D3DDrv maps the
request to format code 3 or 5 and uses the record selected at
`0x10030860` (`FUN_10008240` and `FUN_10008990`), then takes the `flags & 4`
path to `FUN_10008850`. That path allocates one square surface of side
`P = nextPowerOfTwo(max(width, height))`, not a mip chain. The handle stores
`log2(P)` as its shift. For a tightly packed OpenGL face texture of `W × H`,
the exporter divides the sample-center coordinates by `W` and `H`; this
preserves the same sample indices as retail's square-surface UVs divided by
`P`. The row sequence and positive V progression are now confirmed below.

Ghidra memory bytes confirm the addend is the float `8.0`; the denominator is
the lightmap handle's mip shift times 16. The addend is half of the 16-unit
lightmap texel interval used by this conversion. `FUN_10007380` combines the
base and lightmap calculations in one setup path. With a face descriptor,
the default setup calls `FUN_10007120`; the alternate setup calls
`FUN_10007380`, which performs the same lightmap calculation alongside the
base-texture conversion.

This closes the source-coordinate and UV-formula gap, but not rendered parity.
The alternate stream fields' meanings, raw flag meanings, host frame cadence,
and final pixel/channel conventions remain open. The ordinary lightmap draw
states are traced below; the optional face sub-branch and the device's
effective Z-enable state remain unresolved. Do not claim final rendered parity
from the verified UV formula and RGB callback alone.

## Native viewer data path

`tools/export_gbsp_scene.py` now emits the complete LIGHTDATA payload as
`lightdata.bin` even when optional raw chunk export is disabled. It also emits
one normalized OpenGL lightmap UV pair per fan-triangulated vertex, using
float32 raw TEXINFO projections, the grid-snapped origin above, and the
half-sample offset. `scene_viewer.cpp` loads the sidecar (or the legacy raw
chunk), checks the `LightOfs + 1` marker and the byte span for every active
style plane, and retains the planes per face. It combines style 0 as a direct
Q8 RGB copy and nonzero styles with the traced Q8 scale/add/clamp path. EVS
`setlighttable` strings populate the 24-slot viewer style table; the viewer
advances each string once per world draw and refreshes changed atlas rectangles.
The GL renderer packs face lightmaps into a padded shared atlas and uses the
traced ZERO/SRCCOLOR multiply pass once per mesh group, keeping draw submission
bounded by groups rather than faces. On Training, Bank, and Bar R, the native
viewer loaded and uploaded 1,575, 9,474, and 7,063 lit faces respectively.
An export made with `--no-raw-chunks` loads the same Training counts from the
top-level sidecar. The atlas has duplicated edge samples to emulate per-face
clamp and a white texel for unlit faces sharing a draw. This is a working
renderer implementation, not proof of retail pixel parity. The source row
sequence and positive V-to-row association are now traced; exact style phase
during level startup and final image parity remain open.

## Lightmap row order

Genesis `FUN_10033D60` converts each source triplet in order and has no row
reversal. Its point-query helper `FUN_10033510` indexes rows as `y * width + x`.
D3DDrv `FUN_10006ED0` locks mip 0 at `0x10006F22`, reads face width, height,
and the Genesis RGB pointer at `0x10006ED8–0x10006EE1`, then converts source
triplets in order at `0x10006F40–0x10006F6F`. It advances the destination
through each row and skips the unused `P - width` padding pixels at
`0x10006F75`, where `P = nextPowerOfTwo(max(width,height))`. It does not
reverse rows. `FUN_10007120` computes positive V from projected V minus the
face minimum, plus the half-sample offset; it does not negate V. Thus
increasing retail V selects successive source rows. The viewer uploads those
rows in their original order, so its normalized lightmap V must also be
positive. The exporter's former `1 - V` inverted row selection and has been
removed.

The call chain is `FUN_10025770` → Genesis `FUN_10033D60` and
`FUN_10024E90` → D3DDrv `FUN_100071C0` → `FUN_10006FA0` → `FUN_10006ED0`.
Genesis stores the RGB pointer at descriptor `+12` and the returned texture
handle at `+24`. The draw path calls the Genesis update callback, then passes
the same face descriptor to `FUN_10006FA0`; that helper selects mip 0 and
calls `FUN_10006ED0` with the handle's shift from `+0x11`. The upload helper
reads descriptor `+12` and computes the square-side row stride from that
shift. D3DDrv `FUN_10008850` stores `log2(P)` at handle `+0x11` and allocates
the `P × P` surface, tying the row padding to the face texture handle.

The row loop's address sequence is: descriptor height, RGB pointer, and width
loads at `0x10006ED8–0x10006EE1`; `P - width` is calculated at
`0x10006EF3`; the surface is locked at `0x10006F22`; RGB source bytes are read
and advanced through `0x10006F40–0x10006F6B`; destination pixels are written
at `0x10006F6C–0x10006F6F`; then `0x10006F75` skips the row padding. The loop
repeats for the loaded height at `0x10006F79–0x10006F7A`. No negative stride,
row swap, or V negation appears in this path.

Each source byte is also converted through one of three 256-entry lookup
tables at `0x100309D4`, `0x10030DD4`, and `0x100311D4`. `FUN_10006ED0`
combines the three 16-bit table results with OR and writes one 16-bit
destination pixel (`0x10006F4D–0x10006F6F`). `FUN_10004520`'s texture-format
enumeration checks a 16-bit record at `0x1000470F–0x10004743`: red is either
`0xF800` or `0x7C00`, green is either `0x07E0` or `0x03E0`, and blue is
`0x001F`. It copies the matching record at `0x10004759–0x10004764`, then
calls `FUN_10002C90` at `0x10004784–0x10004789` to build the three tables.
`FUN_10002AB0` reads the selected record's channel masks and derives the
quantization shifts. Genesis's flags-4 lightmap request maps to D3D format
code 3 for 5:5:5 or code 5 for 5:6:5; `FUN_10008240` copies the selected
record into the face texture descriptor used by `FUN_10008850`. The shipped
path supports both formats, but the selected record in a particular retail
device session is not established here. The viewer currently keeps composed
samples in RGB8, so the exact packed-color quantization remains open along
with the runtime format choice.

This proves the sample-row association from projected V to LIGHTDATA bytes.
The raw row zero has no separately established world-space top/bottom label,
and rendered pixel parity remains unverified.

## D3D render state and lightmap composition

`FUN_10005070` initializes these render states after creating the D3D device:
ZFUNC state 23 to value 4, SPECULARENABLE state 29 to 0, DITHERENABLE state
26 to 1, SHADEMODE state 9 to 2, CULLMODE state 22 to 1, state 41 to 1, state
18 to 5, and state 17 to 2. Frame begin `FUN_10007F30` then sets FILLMODE
state 8 to 3 (solid), ZWRITEENABLE state 14 to 1, ZFUNC state 23 to 4
(LESSEQUAL), SRCBLEND state 19 to 5 (SRCALPHA), DESTBLEND state 20 to 6
(INVSRCALPHA), and ALPHABLENDENABLE state 27 to 1. The unknown initialization
state 41 and state 18 values are retained as numeric IDs rather than assigned
names here. The initialization and frame-begin calls inspected do not set
ZENABLE state 7; whether depth testing is enabled by another path or device
creation default remains open.

The initialization order also creates a Z-buffer surface in `FUN_10003BE0`,
attaches it to the selected render/back surface (`DAT_100307C4`), and then
passes that surface to Direct3D's `CreateDevice` call in `FUN_10003B10`. This
proves a depth surface is available, but not that Z testing is enabled: the
traced setup does not explicitly set render state 7, and the effective default
for this legacy device path is not established.

At frame begin, `FUN_10001890(2,2)` sets MINFILTER and MAGFILTER to LINEAR
(value 2) on stage 0 and, when `DAT_10042184` is nonzero, stage 1 too. The
lightmap setup clamps the participating stage's address, U, and V modes via
`FUN_100019B0(stage,0)`. A mip filter assignment was not found in these traced
paths.

The path selector `DAT_10042184` is initialized from whether the selected
device's `wMaxSimultaneousTextures` value is greater than one. `FUN_10002520`
enumerates devices with callback `0x10002230`; that callback copies the
0xFC-byte device descriptor into its per-device record, then copies its last
two 16-bit fields into record offsets `+0x130/+0x132`. In the Direct3D device
descriptor layout, those fields are `wMaxTextureBlendStages` and
`wMaxSimultaneousTextures`, respectively. Genesis selects using `+0x132`, so
the test is specifically for more than one simultaneously bound texture; it
does not test the separate maximum blend-stage count. Zero selects the
two-pass fallback in `FUN_100071C0`; nonzero selects the two-stage path in
`FUN_10007450`.

Both paths configure stage 0 with TEXCOORDINDEX 0 and MODULATE for color and
alpha, using TEXTURE and DIFFUSE as its arguments. In the fallback, D3DDrv
draws the base texture first. It then enables alpha blending, sets SRCBLEND to
ZERO (1) and DESTBLEND to SRCCOLOR (3), binds the lightmap at stage 0, replaces
the coordinate pair with the lightmap UVs, and draws again. Those blend factors
make the lightmap source color multiply the existing destination color, the
standard color-lightmap operation documented by Microsoft.

In the two-stage path, stage 1 uses TEXCOORDINDEX 1. Its color operation
MODULATEs TEXTURE by CURRENT, so the lightmap modulates the result from stage
0; its alpha operation MODULATEs DIFFUSE by CURRENT. D3DDrv clamps stage 1,
binds the lightmap there, calculates both UV sets, and draws once. This path
relies on the frame's SRCALPHA/INVSRCALPHA blending setup.
`FUN_10007450` has a separate branch when the face descriptor's `+0x10` field
is nonzero; it sets ONE/ONE blending and additional stage state. Its purpose
and state restoration are not established, so it is not included in the normal
two-stage composition description.

The numeric mappings above follow Microsoft's Direct3D enum references for
[render states](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3drenderstatetype)
and [depth comparison functions](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dcmpfunc),
[texture-stage states](https://learn.microsoft.com/en-us/previous-versions/windows/embedded/ms886612(v=msdn.10)),
[the Direct3D device descriptor](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dhal/ns-d3dhal-_d3ddevicedesc_v3),
[texture arguments](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dta),
[texture operations](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dtextureop),
[blend factors](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dblend),
[texture filters](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dtexturefiltertype),
and [texture addressing](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dtextureaddress).
The source/destination blend combination is also Microsoft's documented
[color-light-mapping setup](https://learn.microsoft.com/en-us/windows/win32/direct3d9/alpha-blending-state).
The binary establishes the state IDs and values; these references supply the
API names and operation meanings.

Together, these traces establish the ordinary fallback and two-stage lightmap
composition plus Z-write and Z-function settings. They do not establish
ZENABLE, mip filtering, the optional face branch, runtime path choice for the
actual device, host calls per second, or final pixel parity.

## Light-style sequence advancement

`geWorld_Create` reaches `FUN_10033990`, which clears the light-style runtime
block and calls `FUN_10032e30` once to initialize its 24 style slots.
`geEngine_RenderWorld` calls `FUN_1003ead0`, which calls `FUN_10034040` once;
`FUN_10034040` calls `FUN_10032e30` before updating dynamic-light contributions
for the render. For each style slot, `FUN_10032e30` advances its character
index by one, wraps to index zero when the next string byte is NUL, converts
the selected character (`byte - 96`) with the binary's `1/27` and `256`
constants into the Q8 coefficient consumed by `FUN_10033d60`, and records
whether the selected value changed. A one-character style wraps to index zero
on every call and therefore stays static.

This establishes one style-sequence step per `geEngine_RenderWorld` call, not
per elapsed-time unit in this function. The host's render-call rate and its
relationship to simulation time remain unknown; the initialization call also
means the sequence is advanced before the first world-render call.

The game EXE's `TScriptTrigger::SetLightTable` handler calls
`geWorld_SetLTypeTable` after validating IDs 1–19 and truncating input to 60
characters. The setter copies the string into that style slot and does not
reset its cursor. Bank sets style 1 to a 60-character prefix of its longer
source string; Bar R sets styles 1 and 2. The other supplied map EVS files do
not set face-light styles, and Oil Rig's style 1 sequence does not appear in
its BSP face-style records. The viewer's EVS host now handles this command.
Its per-draw update mirrors the traced increment-before-read operation; the
exact retail scheduling of the delayed EVS pass against the first visible
render remains provisional.

## Native viewer port and package checks

The exporter now writes each mesh corner's lightmap UV alongside the base UV
and stores the source LIGHTDATA bytes in `lightdata.bin`. The native viewer
validates each face's marker and tightly packed RGB style planes, composes the
Q8 style values, and packs the face images into a padded shared atlas. It
remaps each face's UVs into that atlas and draws one multiply pass per mesh
group; the host advances styles once per world render. The shared atlas avoids
issuing one draw call per face.

All 17 copied BSP packages passed the viewer's `--inspect` load path, including
texture decoding and face-lightdata span checks. The rescue-all app bundle was
rebuilt with 13 mission/stage packages and actor assets. Its packaged
`--render-check` passed for Training (1,575 face lightmaps; 24/24 base
textures), Bank (9,474; 105/105), and Bar R (7,063; 76/76). A Training frame
was visually inspected, but no retail image comparison has established pixel
parity. Bank's short active-versus-disabled capture produced identical frames;
it only covered the repeated early `a` value, so a visible animated change has
not yet been confirmed. The traced row association was applied by removing the
lightmap V flip while retaining the source row order; a retail image comparison
is still needed to establish full pixel parity.

After removing the lightmap V flip, all 17 packages were re-exported and each
passed `--inspect`. The rescue-all app was rebuilt, then the packaged
Training, Bank, and Bar R `--render-check` commands passed again with the same
face-lightmap and base-texture counts. The fresh Training frame differs from
its pre-correction capture at 933,909 of 1,024,000 pixels. This confirms the
coordinate correction changes rendered output; it does not measure retail
parity.

## Related evidence

- [Genesis lightdata decode](genesis_lightdata_rgb_decode_2026-10-15.md)
- [Shipped D3DDrv texture and draw path](d3ddrv_render_path.md)
- [Selected renderer load and table initialization](../driver-boundary/selected_driver_initialization_trace.md)
