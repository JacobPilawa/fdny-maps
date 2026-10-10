# Training reconstruction audit — 2026-09-05

> **2026-09-06 correction:** The earlier claim that the player has no step-up or
> sliding was wrong: it used the wrong vtable base. The live movement path now
> uses the recovered 16-unit step and slide response. Earlier floor-normalization
> and hard-stop descriptions below are historical, not the current runtime.
> See `COLLISION_CORRECTION.md` for the evidence and passage regressions.

The current TypeScript/Three.js build is a useful reconstruction and inspection
tool. It does not meet the 1:1 completion contract in `SCOPE.md`. Passing its
tests demonstrates specific recovered equations and data relationships, not
whole-game equivalence.

## Reviewed evidence

- Current application, extractors, tests, findings and parity ledgers.
- Original Training BSP entities, `Maps/training.evs`, `Scripts/tools.evs`,
  `Scripts/rescue.evs`, and `ResDefs/input.rrf`.
- Existing Ghidra decompilations and import/export logs.
- A fresh read-only headless Ghidra export: `analysis/audit-decompiled` and
  `analysis/logs/ghidra-audit.log`. Ghidra MCP is not exposed in this session;
  `/Applications/ghidra/support/analyzeHeadless` works with the existing project.

The import log contains cache permission errors, missing PDB warnings and FMOD
analysis conflicts. These did not prevent the executable decompilation export.
They should not be interpreted as a fully clean analysis of every imported DLL.
The fresh export also reported a cache-maintenance permission error, then
successfully exported 25 functions and discarded project changes as requested.

## Fixed defects

| Defect | Correction | Check |
| --- | --- | --- |
| Missing difficulty became zero through `Number(null)` | Explicit default medium; empty/invalid settings fall back | Settings regression |
| High-refresh browsers updated physics faster than the retail clock permits | Simulation timer separated from rendering; integer milliseconds, minimum 17 ms, maximum 100 ms | Fresh clock decompile, binary constants, clock tests |
| Cleared boxes/chairs remained invisible solids | Associated brush models disabled in both swept and overlap collision | Original Training model 7 regression |
| Axe damage applied on every update while held | Recovered swing state machine; one contact test crossing 0.275 s, with held-action repeats | Axe state regressions; see subsequent mechanics audit |
| Spray gained an arbitrary 0.2-second bonus on press | Spray advances only by clamped simulation dt | Code audit; remaining spray approximation below |
| Tutorial macros were paraphrased and durations discarded | Original EVS text bundled with source hashes; authored hint duration retained | Real Training script fixtures |
| Every trigger was permanently disabled after first visit | Entry/exit tracking permits return visits; explicit `once` maintains one-time state | Script revisit fixtures; contact detection remains approximate |
| Conditions were substring checks on inventory | Ordered script variables and `if`/`ifnot`; `gotvictim` persists independently of carrying | Failure, hose, and survivor fixtures |
| Queued goggle hint displayed immediately | Three-second simulation-time queue; equipment action runs once | Actual `hinttrigger16` fixture |
| Alternate spawn always used base victim heading | Read heading from selected candidate | Decompile `0x0045D5B0`; all Training headings happen to be equal |
| Escape/focus loss could leave movement/tool input active | Clear held input on release/blur; gate gameplay before loading/capture | Browser crouch/Escape check |
| Launcher silently switched to another port | `--strictPort` reports occupied ports | Vite launcher setting |

## Limits that still prevent a 1:1 recreation

1. **Physics:** JavaScript numbers are double precision; retail code combines
   float storage and x87 operations. Start-solid response is deliberately
   normalized, not copied. Slopes, ceilings, ladder contact and collision query
   ordering lack retail trace comparisons. Browser timers cannot reproduce
   Windows scheduling exactly. The 60 Hz jump fixture is synthetic; the actual
   retail minimum update is 17 ms.
2. **Spawns:** Candidate coordinates, MT algorithm and snap-to-floor match the
   available evidence. The initial global RNG draw ordinal remains unknown.
   The browser's direct choices `1,1` are not proven retail startup results.
3. **Tools and fire:** Axe timing/repetition and the swept contact probe are now
   recovered, but camera transforms, animated brush eligibility and full tool
   transitions remain incomplete. Spray range, cone and the 0.25-second minimum
   for zero-time fires are inspection choices. Extinguisher charge now uses the
   recovered normalized 0.1/s consumption, including misses.
   Large fires can still be targeted by the simplified extinguisher logic.
   There is no complete retail damage/fire propagation/particle simulation.
4. **Objects and survivors:** Clear-object interaction removes an object instead
   of reproducing carry/throw physics. Brush transforms, break animations,
   debris and victim AI/animation selection are incomplete. Picking up a victim
   and exit handling use simplified distance/zone logic.
5. **Scripts:** The new interpreter is an explicitly limited EVS implementation,
   not a decompiled VM port. It loads macro definitions from two files, not the
   complete `exec` graph. Global startup/event registration, exact touch volume
   rules, `escape`, audio and other player verbs remain unsupported or special
   cased. Unsupported dispatched commands are logged once with their source.
6. **Presentation:** Lighting/lightmaps, fog, HUD, weapon placement, prop models,
   particles, audio, goggles and success feedback are not reproduced exactly.
7. **Input:** The active binding constructor `0x004395C0` resolves the old
   binding-file ambiguity: Ctrl crouches and Space uses. E/G browser aliases
   remain inspection conveniences. Embedded browser fallback steering differs
   from native pointer lock and is labeled when activated.

## Verification and next evidence

Initial baseline: 16 TypeScript tests, five Python checks, and production build
passed. After cleanup: 26 TypeScript tests, plus Python extraction checks and
production build. Browser verification covers map/axe loading, floor settling,
crouch, and Escape in the embedded-browser fallback; it is **not** a full
Training playthrough or an original-game comparison.

For defensible 1:1 physics and spawn claims, capture an original-game session
with executable/map hashes, difficulty, frame dt, input counts/buttons, player
position/velocity/state, collision fraction/plane/model and global RNG draw
counter. Include startup, both spawn constructions, floor/wall/ceiling contact,
crouch/jump/climb, tools, pickups and extraction. No running Windows/Wine
environment or such complete trace was identified in this workspace audit.

Continue static recovery with the existing Ghidra project while obtaining that
runtime evidence. Prioritize the clock/input/collision chain, RNG startup
consumers, player tool verbs, fire update, EVS dispatch, then actor/brush and
rendering pipelines. Do not replace these gaps with unlabelled approximations.

Original installation files and previous analysis logs were retained. A source
snapshot from before this cleanup is in `/private/tmp/fdny-before-cleanup`;
temporary storage is not a substitute for version control.

## Subsequent mechanics audit

`MECHANICS_PARITY.md` records the later movement, axe, supply and unit changes.
Fresh read-only exports are in `analysis/mechanics-decompiled` and
`analysis/tool-decompiled`, with corresponding Ghidra logs. Those supersede
the earlier per-press axe assumption and coyote-jump description.
