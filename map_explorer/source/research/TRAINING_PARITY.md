# Training parity status

> **2026-09-06 correction:** The earlier claim that the player has no step-up or
> sliding was wrong: it used the wrong vtable base. The live movement path now
> uses the recovered 16-unit step and slide response. Earlier floor-normalization
> and hard-stop descriptions below are historical, not the current runtime.
> See `COLLISION_CORRECTION.md` for the evidence and passage regressions.

This is the evidence boundary for the current build. “Matched” means the value
or behavior is taken from the shipped Training data, the executable, or public
Genesis3D source and has a repeatable check. It does **not** mean the complete
Training level geometry and several native gameplay subsystems are reconstructed, but full retail replay parity is still in progress.

| Area | Status | Evidence / remaining gap |
| --- | --- | --- |
| Static map geometry | Matched data | Original v15 GBSP: 6,905 vertices, 6,389 faces, 89 brush models |
| Static map textures | Matched pixels | All 29 embedded palettes/textures decoded from `training.bsp` |
| Collision topology and fresh sweeps | Matched | Original BSP nodes/leaves/sides/planes plus shipped recursive traversal; 611 `Genesis.dll` fresh contacts agree within `7.45e-9` unit |
| Player hull, gravity, jump, drag, acceleration | Matched implementation | Ghidra-derived constants/order and deterministic tests; see `PHYSICS_PARITY.md` |
| Wall behavior | Matched core | Retail 16-unit step, plane slide, five-contact retry/fallback; 1,000 native slide cases exact |
| Floor start-solid edge case | Partial | Shipped DLL retained-global behavior is proven and tagged, but process-wide prior-contact state is not reproduced |
| Spawn location and heading | Matched | `(-1268,-172,4692)`, heading conversion `3*30+180 = 270°` |
| Simulation pacing | Recovered rules, unverified runtime parity | Minimum 17 integer ms, actual elapsed dt capped at 100 ms; rendering shares the throttled update loop; see NATIVE_CODE_PROOF.md |
| Mouse scale and pitch limits | Matched constants | 0.10 degrees/count; 20/40/70-degree difficulty limits |
| Original movement/tool key bindings | Recovered active defaults | Constructor `0x004395C0`: WASD/arrows, C/Ctrl crouch, Space/Shift use, left mouse action, right mouse jump, 1/2/3 tools. This supersedes the old `input.rrf` interpretation; E/G are aliases |
| Victim candidate positions | Matched data | Two base spots plus four alternates from the original entities |
| Victim hull and floor snap | Matched implementation | ±24-unit hull, +48/-1000 sweep, +1-unit result, plus native four-corner escape probes for solid spawns; all six authored candidates settle at Y `-230.9` |
| Victim random algorithm | Matched implementation | Retail 624-word MT, low-23-bit float, x87 nearest-even choice |
| Victim RNG draw ordinal | Range rule matched; startup ordinal unproven | Victim spot selection now uses the executable's `count - 0.01` upper bound at `0x004DFC78`, followed by x87 nearest-even conversion. The global RNG can still be consumed before victim construction; fresh-game call order needs runtime instrumentation |
| Axe mesh and texture | Matched asset | Retail `axe.act`: 59 vertices, 110 faces, three bones, original 256² texture |
| Male victim mesh and textures | Matched asset | Retail `vic_man.act`: 303 vertices, 456 faces, 26 bones, three original 256² textures, executable-confirmed scale 2 |
| Axe animation | Matched clip data, partial playback | Retail `swing` keys, named bone paths, Hermite translations, quaternion slerp, and 2.99997-second extent are decoded and sampled; gameplay hit crossing at 0.275 s is now recovered separately; visual playback alignment remains incomplete |
| Axe view placement | Partial | Retail skinned geometry is rendered, but its exact camera-space placement transform is still being recovered |
| Victim animation/AI | Downed state matched; autonomous movement absent in Training | Training survivors are spawned by the native spot-selection path, floor-snapped with the ±24-unit hull, enter state 4 (`downed`) with the `onback` clip, and remain stationary until pickup transitions state 5 (`carried`). The native four-corner 5-unit escape probe order is implemented and tested for blocked spawns; state-7 movement dispatch and full AI timing remain incomplete |
| Lighting | Not matched | Original texture pixels are used, but Genesis3D lightmaps/blend pipeline is not reproduced yet |
| Axe mechanics | Recovered core, partial integration | 0.275 s hit crossing, 1 s swing cycle, repeat on hold, 300 ms stuck hold, 20-unit movement release, original swept probe and brush damage. Moving/rotated brush eligibility and camera transform remain incomplete |
| Extinguisher supply | Recovered | Normalized charge drains 0.1/s while spraying, including misses; spray contact/strength and depletion transitions remain incomplete |
| Fire suppression | Partial | Native nonlinear health loss, zero-time small fires, four-second authored scale, strict 0.1 douse threshold, and the original particle ellipsoid/falloff (`0x0044A7A0`) are implemented; particle emission/contact wiring remains incomplete |
| Smoke, hose, HUD | Not matched | Functional inspection stand-ins and simplified state logic remain |
| Training doors | Matched core, partial touch proof | All 20 extracted quarter-turn tracks use their authored hinges and stay collidable while moving; both-side floor passage passes; exact tracker boundary and quaternion rounding remain |
| Moving/breakable brushes | Partial | Cleared inspection objects now remove all associated brush collision as well as rendering; retail carrying, throw, damage timing, animation and debris remain incomplete |
| Tutorial scripts and completion | Partial | Exact EVS macro text, ordered conditions/variables, one-time actions, queued hints, player-hull overlap, native `touch`/`touching`/`untouch` dispatch, player commands, authored ash-density commands, and presentation command consumption are covered; full VM, global startup/event registration and retail completion remain incomplete |
| Audio | Not matched | Original files identified but not wired to events |

The next physics-proof step is a retail runtime trace that records timestamped
input, body position, velocity, grounded state, collision plane/fraction, and
global RNG counter, then replays the same trace in the browser for per-frame
diffing. On this ARM macOS host the original 32-bit Windows executable cannot be
run natively, so that comparison requires a Windows/Wine-compatible runtime or
a captured trace from one.

See `MECHANICS_PARITY.md` for original-unit verification and the F8/F9 trace
recorder. Exporting a reconstruction trace and replaying it is an internal
consistency check; it does not establish retail parity.
