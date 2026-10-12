# Ash-pool helper field recovery

## Provenance

Read-only queries against the preserved Ghidra 12.0.4 project for
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
`DecompileFunctions.java` and `ExportFunctionAssembly.java` were run with
`-noanalysis -readOnly`; the preserved database was not modified. Queried
entries: `0047f500`, `0047d330`, `0047d300`, `0042d280`, `0047f1a0`,
`0047f1b0`, `0047f1c0`, `0047d660`, `0047d2b0`, and `004463f0`.

## Confirmed writes

- `0047f500` sets `+0x74=0`; calls `0047d330`; sets `+0xa8=1`; writes
  `(1,1,1)` to vector `+0xb4`, zeros vectors `+0x7c`, `+0x88`, and `+0x94`,
  clears `+0xa0`, `+0xa4`, `+0xcc`, and `+0xd4`, then sets bytes `+0x30`,
  `+0xad`, and `+0x78` to 1.
- `0047f850` calls `0047d370`, zeroes vectors `+0x7c..+0x9c` and
  `+0xb4..+0xc8`, calls `0047f500`, and initializes additional animation and
  render fields. `0047d370` records the descriptor pointer/byte and clears
  `+0x24..+0x2c`; neither function has a preserved store to spawn vector
  `+0x5c`.
- The `0047d330` assembly stores descriptor flags from `[this+0x0c]+0x18` to
  `+0x18`, resets/validates animation indices through `0047d2b0`, stores global
  time at `+0x14`, sets `+0x1c=1.0`, byte `+0x30=1`, and `+0x20=1.0`. This
  confirms the initial activation scale used by `00446470` is 1.0.
- `004463f0` calls `0047f850` with the descriptor, then stores deadline
  `+0x168=0`; it calls `0047f1d0` and zeros that returned vector through
  `0042d280`.
- `0042d280` stores its three scalar arguments at vector X/Y/Z offsets `0/4/8`
  and copies them to the optional output buffer.
- `0047f1a0` sets byte `+0x78=1` and returns `this+0x34` (spawn vector at
  `+0x5c`). `0047f1b0` sets byte `+0xa0=1` and returns `this+0x7c`;
  `0047f1c0` sets the same byte and returns `this+0x88`.
- `0047d660` advances finite animation state when its deadline is due and can
  clear active byte `+0x30` when the descriptor's bit 3 is set. `0047f960`
  calls it when byte `+0xad` is set, before its active-state presentation path.
- `0047d300` resets and validates animation indices and stores global time, but
  the C export contains unresolved `this`/`extraout_ECX` temporaries. Its
  internal dataflow remains an explicit callback in the native component; the
  Ghidra assembly confirms its two index helper calls and timestamp store.

## Consequence for Training

The first Player update callback still updates and presents all 100 entries,
even after Training's `setashdensity 0`; that command only reduces the later
spawn-prefix scan to zero. All 100 entries start active with deadline zero, so
the direct due-update branch contributes four draws each if it runs before
presentation deactivates entries. The presentation callback can advance the
finite animation and change which slots are inactive by the subsequent scan.
Exact callback timing and indirect engine services are still unverified in a
live Training run.

The component now models the recovered vector writes directly and requires
explicit callbacks for construction, spawn transforms, animation reset, and
presentation. This avoids substituting default coordinates or no-ops for
unrecovered engine behavior. Callback placeholders in its focused check are
test fixtures only.
