# Ash-pool helper field recovery

## Provenance

Read-only queries against the preserved Ghidra 12.0.4 project for
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
`DecompileFunctions.java` and `ExportFunctionAssembly.java` were run with
`-noanalysis -readOnly`; the preserved database was not modified. Queried
entries: `0047f500`, `0047d330`, `0047d300`, `0042d280`, `0042d2d0`,
`0047f1a0`, `0047f1b0`, `0047f1c0`, `0047f1d0`, `0047d660`, `0047d2b0`,
`004463f0`, `0047f850`, `0047f5a0`, `0047db10`, and `0047d370`.

The constructor extension (2026-10-12) also queried `0047f850`, `0047f500`,
`0047f1d0`, `0042d2d0`, `0047f2a0`, `0047db10`, `0047f5a0`, `0047d330`,
`0047d370`, `0047d660`, and `0047d820` with `DecompileFunctions.java` on the
scratch copy `/private/tmp/fdny-ash-presentation-audit`, using
`-noanalysis -readOnly`. No Ghidra project was modified. This extension found
that the base transform constructor zeros `+0x5c..+0x64`, and that
`004463f0`'s final `0047f1d0` call sets dirty byte `+0xa0` after
`0047f500` initially clears it.

## Confirmed writes

- `0047f500` sets `+0x74=0`; calls `0047d330`; sets `+0xa8=1`; writes
  `(1,1,1)` to vector `+0xb4`, zeros vectors `+0x7c`, `+0x88`, and `+0x94`,
  clears `+0xa0`, `+0xa4`, `+0xcc`, and `+0xd4`, then sets bytes `+0x30`,
  `+0xad`, and `+0x78` to 1.
- `0047f850` calls `0047d370`, initializes the transform subobject at `+0x34`
  through `0042d2d0`, zeroes vectors `+0x7c..+0x9c` and `+0xb4..+0xc8`, then
  calls `0047f500`. The transform initializer writes the translation at
  `+0x5c..+0x64` to zero, so a newly constructed particle's position is the
  origin. The constructor also zeros state `+0xb0`, sets fields `+0x108`,
  `+0x12c`, `+0x130`, and `+0x154` to raw float bits `0x3f7d70a4`, clears
  `+0xe4`, `+0xe8`, `+0x10c`, and `+0x150`, and clears culling byte `+0xac`.
  Those render-field meanings are not inferred from the constants alone.
  `0047d370` records the descriptor pointer/byte and clears `+0x24..+0x2c`.
- The `0047d330` assembly stores descriptor flags from `[this+0x0c]+0x18` to
  `+0x18`, resets/validates animation indices through `0047d2b0`, stores global
  time at `+0x14`, sets `+0x1c=1.0`, byte `+0x30=1`, and `+0x20=1.0`. This
  confirms the initial activation scale used by `00446470` is 1.0.
- `004463f0` calls `0047f850` with the descriptor, then stores deadline
  `+0x168=0`; it calls `0047f1d0`, which sets dirty byte `+0xa0=1` and returns
  vector `+0x94`, then zeros that returned vector through `0042d280`. Thus the
  final constructed object's dirty byte is 1, even though `0047f500` first
  cleared it; vector `+0x94` is zero.
- `0042d280` stores its three scalar arguments at vector X/Y/Z offsets `0/4/8`
  and copies them to the optional output buffer.
- `0047f1a0` sets lazy-cache byte `+0x78=1` and returns `this+0x34` (spawn
  vector at `+0x5c`). `0047f1b0` sets vector-dirty byte `+0xa0=1` and returns
  `this+0x7c`;
  `0047f1c0` sets the same byte and returns `this+0x88`.
- `0047d660` advances finite animation state when its deadline is due and can
  clear active byte `+0x30` when the descriptor's bit 3 is set. `0047f960`
  calls it when byte `+0xad` is set, before its active-state presentation path.
- `0047d300` resets and validates animation indices and stores global time,
  but the C export contains unresolved `this`/`extraout_ECX` temporaries.
  Assembly confirms it stores indices 0 and 1 through `0047d2b0` and then
  stores clock `00502AFC` at `+0x14`. For the known five-entry Training
  descriptor both indices are in range; an original P-code activation fixture
  verifies the exact behavior.

## Consequence for Training

The first Player update callback still updates and presents all 100 entries,
even after Training's `setashdensity 0`; that command only reduces the later
spawn-prefix scan to zero. All 100 entries start active with deadline zero, so
the direct due-update branch contributes four draws each if it runs before
presentation deactivates entries. The presentation callback can advance the
finite animation and change which slots are inactive by the subsequent scan.
Exact callback timing and indirect engine services are still unverified in a
live Training run.

The component models the recovered vector writes and activation reset directly
and requires explicit callbacks for construction, spawn transforms, and
presentation. This avoids substituting default coordinates or no-ops for
unrecovered engine behavior. Callback placeholders in its focused check are
test fixtures only.

## Original P-code comparison (2026-10-12)

`AshParticleUpdatePcodeOracle.java` ran against the preserved Ghidra 12.0.4
project with `-noanalysis -readOnly`. It checked the original executable hash
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, seeded
the original RNG through `00404B50`, then executed `00446510` in the Ghidra
P-code emulator. The fixture also executes original vector wrappers and the
bounded random helper rather than replacing these with test implementations.

The five fixture rows cover a due update with zero vectors, a due update that
adds to nonzero prior Y, an active but not due record, an inactive record, and
unsigned clock wrap. The native `player_ash_pool_pcode-check` compares active
state, deadline, dirty flag, all output vector words, and the number of
`00498B30` calls; all five rows match exactly. The update function's local
branches are now original-P-code checked, while constructor callbacks, pool
schedule, presentation, rendering, and whole-run RNG order remain unverified.

`AshParticleActivatePcodeOracle.java` separately executes original `00446470`
with original `0047D300` and `0047D2B0`, after `00404B50` seeds the stream.
Its three cases vary seed, clock (including unsigned wrap), and input position.
The native activation component matches active/position/dirty bytes, animation
indices and timestamp, all spawn and velocity vector words, both scale values,
deadline, and three bounded calls. This covers the activation callee with its
input point supplied; the caller's angle/distance draws and Player/camera scene
transform remain outside scope.


`AshParticleGeometryPcodeOracle.java` separately executes original `0047F5A0`
with its template guard already initialized and no parent transform. Five
affine-frame cases compare cached positions at `+0xD8`, `+0xFC`, `+0x120`,
and `+0x144`, as well as clearing byte `+0x78`. The native component matches
all vertex words. `0047F5A0` builds an XY square using half of `+0xB4.x/y`
(Z is zero), and `00495D20` applies the resolved basis/scale and translation
with an intermediate float spill. The oracle does not establish parent-chain
composition, resource binding, polygon submission, or live rendering.
