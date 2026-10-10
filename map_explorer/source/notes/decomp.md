# FDNY Firefighter Decomp Notes

Date: 2026-05-12
Analyst: Codex (first-pass reconnaissance)
Target binary: `<game-install>/FDNYFirefighter.exe`
Ghidra project: `fdny_decomp`
Program name in Ghidra: `FDNYFirefighter.exe`

## 1) Scope and Goals
This document captures a broad initial reverse-engineering pass with special attention to:
- Stage/map identification
- High-level gameplay command system and likely scripting hooks
- "Physics"-adjacent controls and movement-related command pathways
- RNG clues relevant to victim spawn behavior
- What `rescuebot` appears to be and how it is triggered

This is a first pass, not final attribution of all systems/classes.

## 2) Binary Overview
Source: `ghidra://program/FDNYFirefighter.exe/info`

- Architecture: `x86:LE:32:default` (32-bit little-endian)
- File format: `Portable Executable (PE)`
- Image base: `0x00400000`
- Approx complexity:
  - `functionCount`: 6094
  - `symbolCount`: 19708
- Sections:
  - `.text` `0x00401000 - 0x004DBFFF`
  - `.rdata` `0x004DC000 - 0x004FEFFF`
  - `.data` `0x004FF000 - 0x00505437`

## 3) Engine / Middleware Fingerprint
Imports strongly indicate Genesis3D-based game runtime and FMOD audio stack:

- `GENESIS.DLL` functions include:
  - `geWorld_Collision`, `geWorld_GetContents`, `geWorld_AddActor`, `geActor_SetPose`, `geMotion_GetPath`, etc.
- `FMOD.DLL` functions include:
  - `_FSOUND_PlaySoundEx@16`, `_FSOUND_3D_SetAttributes@12`, `_FSOUND_3D_Update@0`, etc.
- DirectInput import: `DirectInput8Create`

Implication: world physics/collision is likely partly engine-level (Genesis3D), with game logic wrapping it.

## 4) Stage / Map Discovery

### 4.1 On-disk map files (ground truth)
Found in game assets:
- `Data/Maps/bank.bsp`
- `Data/Maps/training.bsp`
- `Data/Maps/mbase.bsp`
- `Data/Maps/bar_r.bsp`
- `Data/Maps/subway_a.bsp`
- `Data/Maps/subway_b.bsp`
- `Data/Maps/boat_a.bsp`
- `Data/Maps/boat_b.bsp`
- `Data/Maps/gas_r.bsp`
- `Data/Maps/const_h.bsp`
- `Data/Maps/dock_h.bsp`
- `Data/Maps/oil_rig_A.bsp`
- `Data/Maps/oil_rig_B.bsp`
- `Data/Maps/oil_rig_C.bsp`
- `Data/Maps/oil_rig_D.bsp`
- `Data/Maps/oil_rig_h.bsp`
- `Data/Maps/City_h.bsp`

Likely these are mission/stage map identifiers (including hose/training variants).

### 4.2 In-binary map pipeline clues
Relevant strings observed:
- `"Data\\Maps"`
- `"*.bsp"`
- `"Available maps:"`
- `"File not found: "`
- `"ERROR: Couldn't load level definitions file"`
- source path fragment: `"main\\LevelManager.cpp"`

This indicates map enumeration and load errors are surfaced through an internal script/console command layer.

### 4.3 Map listing command function
Function: `FUN_00479670`
- Emits `"Available maps:"`
- Enumerates files under `Data\\Maps` matching `*.bsp`
- Prints each map name

This is almost certainly the backend for command `listmaps`.

### 4.4 Map load command function
Function: `FUN_00478150`
- Validates map/resource target existence
- Emits `"File not found: ..."` if missing
- Calls into resource/system loader with explicit `"data\\resdefs\\rescue.rrf"`

Observed in dispatcher as handler for `loadmap` (and separate handler for `loadhosemap`).

## 5) Command/Scripting System (High-value Pivot)

### 5.1 Master command dispatcher
Function: `FUN_0047a1f0`
This is a large command dispatcher with `__stricmp` matching over many command names. It handles:
- flow-control/script-like commands: `if`, `ifnot`, `if<`, `if<=`, `if>`, `if>=`, `iffacing`, `ifclear`, etc.
- variable/control commands: `set`, `inc`, `dec`, `print`, `listvars`
- map/system commands: `listmaps`, `loadmap`, `loadhosemap`, `difficulty`, `quit`
- debug/AI commands: `debugvictims`, `runrescuebot`
- audio/effects commands: `playsnd`, `stopsnd`, `loopsnd`, `fogenable`, `fogdisable`, `hint`, `quake`, etc.

Interpretation: this command processor is central to game script execution and debug tooling, and is the best starting point for deeper system discovery.

### 5.2 Related strings around command table
In `.rdata`, near command table data:
- `setrescuebot`
- `runrescuebot`
- `debugvictims`
- `finalexitloadmap`, `finalexit`
- `setashdensity`
- `toolcmd`, `touchhose`

This cluster reinforces that much mission behavior is script-command-driven.

## 6) RescueBot Findings

### 6.1 Direct debug command / token evidence
Strings:
- `DBG_ACTIVATE_RESCUEBOT`
- `runrescuebot`
- `setrescuebot`
- `RESCUEBOT`

### 6.2 Key bindings / debug binding setup
Function: `FUN_004395c0`
- Initializes many named actions and debug commands (e.g., `DBG_TOGGLE_FRAMERATE`, `DBG_ACTIVATE_RESCUEBOT`, movement/select actions).
- Suggests `DBG_ACTIVATE_RESCUEBOT` is a registered action/command in input/command binding layer.

### 6.3 Runtime execution path
In dispatcher `FUN_0047a1f0`, command `runrescuebot` calls:
- `FUN_00475020`

`FUN_00475020` is small:
- Calls `FUN_00455230(*(int *)(DAT_00503a78 + 0x30) + 0x340c)`

Interpretation:
- `runrescuebot` toggles or invokes a behavior on a global/game-state object at offset `0x340c` from a manager/context struct.
- We do not yet know whether rescuebot is full AI autopilot, scripted helper, or a mission-state solver. But it is definitely a callable subsystem, not just a text flag.

### 6.4 Resource linkage
`FUN_00478150` (loadmap path) explicitly references:
- `data\\resdefs\\rescue.rrf`

This implies rescue-related systems are data-driven via `rescue.rrf` and likely orchestrated by script commands.

## 7) Victim System Findings

### 7.1 Direct debug toggle
`debugvictims` command in dispatcher calls:
- `FUN_00474f60`

`FUN_00474f60` behavior:
- Toggles boolean at `DAT_00502aac + 0x6a9`
- Logs `"Victim debugging is ON/OFF"`

Interpretation:
- Victim subsystem has an explicit debug mode bit in a global runtime state object.

### 7.2 Victim resources present
On disk:
- `Data/ResDefs/victims.srf`
- victim sprites: `victim1.bmp`, `victim2.bmp`, variants

Likely victim placement/spawn definitions exist in resource/script formats (`.srf`, `.rrf`, `.evs`) rather than hardcoded only.

## 8) RNG / Spawn Investigation (First Pass)

### 8.1 What we have
- Found command token `"randomize"`, but in context of `RCS_BlinkLight` command family (`follow`, `setcolor`, `pulse`, `radius`, `turnon/off`) in/near function `FUN_00440460`.
- This appears related to light behavior randomization, not victim spawning.

### 8.2 What we do NOT yet have
- No direct `victim spawn RNG` function identified yet.
- No immediate string hits for `seed` or obvious `srand` command symbols.

### 8.3 Hypothesis
Victim spawn RNG likely occurs in one of:
- Script-driven resource loaders (`rescue.rrf`, `victims.srf`, `rescue.evs`) with hidden random branches
- Runtime manager functions reachable from map-load / mission-start pipeline
- Internal CRT random functions not trivially string-labeled in this binary

## 9) "Physics" Findings (Broad)

### 9.1 Engine-level evidence
Genesis imports strongly imply collision/movement is engine-mediated (`geWorld_Collision` etc.).

### 9.2 Gameplay control layer evidence
`FUN_004395c0` registers movement/control actions:
- `MOVE_FORWARD`, `MOVE_BACKWARD`, `MOVE_LEFT`, `MOVE_RIGHT`
- `STEER_YAW`, `STEER_PITCH`
- `JUMP`, `CROUCH`, `USE`, `ACTION`

This is not raw physics code but confirms input/action binding architecture and where to pivot for movement behavior.

### 9.3 Script command side-effects
Dispatcher commands include spatial/interaction semantics (`iffacing`, `ifclear`, etc.) and effect controls (`quake`, fog, audio locational commands), suggesting script-level control over gameplay simulation triggers.

## 10) Files to Prioritize Next (for your target questions)

On disk high-priority:
- `Data/Scripts/rescue.evs`
- `Data/ResDefs/rescue.rrf`
- `Data/ResDefs/victims.srf`
- `Data/ResDefs/hose.rrf`

Code high-priority:
- `FUN_0047a1f0` (master dispatcher)
- `FUN_00475020` (runrescuebot handler)
- `FUN_00455230` (likely core rescuebot execution target)
- `FUN_00478150` (map load + rescue resource hookup)
- Victim debug-linked region around global state at `DAT_00502aac + 0x6a9`

## 11) Concrete Next-Step Plan (for second pass)
1. Rename key dispatcher functions and annotate command->handler mapping in Ghidra comments.
2. Decompile and map `FUN_00455230` deeply to characterize what rescuebot actually does (state machine, pathing, objective logic, etc.).
3. Parse/extract text from `rescue.rrf`, `victims.srf`, `rescue.evs` to map script vocabulary to code handlers.
4. Locate RNG primitives by tracing calls from victim-related code paths (especially mission start, victim creation loops, and resource load callbacks).
5. Build a victim spawn timeline: map load -> resource parse -> candidate spawn points -> random selection -> activation conditions.

## 12) Confidence Notes
- High confidence: command names, handler connectivity, map listing/loading, debugvictims toggle, runrescuebot command existence.
- Medium confidence: rescuebot semantics (execution path confirmed, behavior still uncharacterized).
- Low confidence (currently): exact RNG algorithm and victim spawn decision logic (not yet isolated).

## 13) Second-Pass Addendum (RescueBot + Physics)

### RescueBot (deeper)
- Core RescueBot object initialized at manager offset `+0x340c` via `FUN_00451480`.
- Manual trigger path confirmed:
  - `runrescuebot` -> `FUN_00475020` -> `FUN_00455230`.
- Automatic trigger path confirmed in periodic update `FUN_00455580`:
  - gated by mission state and internal cooldown timers.
- Candidate eligibility in `FUN_00455230`:
  - runtime victim state field `victim+0x120` (shown by Ghidra as `victim[0x48]`) in `{4, 7}`
  - victim flag `*(victim+0x1fd) == 0`
  - line-of-sight check `FUN_00490130`/`FUN_0048fea0`
  - distance must be `>= 600.0` (constant at `0x004e1f50`)
- Selection is first qualifying victim in iteration order, not explicit random choice in this function.
- Commit function `FUN_00454750`:
  - sets victim state via `FUN_0045bf70` (`victim+0x120 = 6`, emits `botrescue` command)
  - increments mission rescue counter at manager `+0x3530`
  - sets active rescue timeout `now + 4000 + rand(0..3999)`.

### Training-stage suppression evidence
- `FUN_00455580`:
  - early suppression path when `*(world+0x18c) == 6`
  - auto-trigger attempts only when state in `{3,4,5}`.
- This is strong evidence that RescueBot is intentionally disabled in at least one mode likely corresponding to training/tutorial behavior.

### Physics (deeper)
- In movement block around `0x004531xx`:
  - input vector magnitude normalized to <= 1.0 (double constant at `0x004dd808` = 1.0)
  - state-dependent speed constants include 260, 400, 800
  - damping/interp constants include approx 0.95 and 0.20
  - multiple geometry/world vectors are added into velocity-like accumulators, supporting slope-influenced speed outcomes
- This is consistent with observed slope speedup and jump momentum retention.

### New documentation files
- `rescue_bot.md` created for full RescueBot-specific deep dive.
- `physics.md` created for movement/physics-focused analysis and next-step proof plan.

## 13) Second-Pass Addendum (RescueBot + Physics)

### RescueBot (deeper)
- Core RescueBot object initialized at manager offset `+0x340c` via `FUN_00451480`.
- Manual trigger path confirmed:
  - `runrescuebot` -> `FUN_00475020` -> `FUN_00455230`.
- Automatic trigger path confirmed in periodic update `FUN_00455580`:
  - gated by mission state and internal cooldown timers.
- Candidate eligibility in `FUN_00455230`:
  - runtime victim state field `victim+0x120` (shown by Ghidra as `victim[0x48]`) in `{4, 7}`
  - victim flag `*(victim+0x1fd) == 0`
  - line-of-sight check `FUN_00490130`/`FUN_0048fea0`
  - distance must be `>= 600.0` (constant at `0x004e1f50`)
- Selection is first qualifying victim in iteration order, not explicit random choice in this function.
- Commit function `FUN_00454750`:
  - sets victim state via `FUN_0045bf70` (`victim+0x120 = 6`, emits `botrescue` command)
  - increments mission rescue counter at manager `+0x3530`
  - sets active rescue timeout `now + 4000 + rand(0..3999)`.

### Training-stage suppression evidence
- `FUN_00455580`:
  - early suppression path when `*(world+0x18c) == 6`
  - auto-trigger attempts only when state in `{3,4,5}`.
- This is strong evidence that RescueBot is intentionally disabled in at least one mode likely corresponding to training/tutorial behavior.

### Physics (deeper)
- In movement block around `0x004531xx`:
  - input vector magnitude normalized to <= 1.0 (double constant at `0x004dd808` = 1.0)
  - state-dependent speed constants include 260, 400, 800
  - damping/interp constants include approx 0.95 and 0.20
  - multiple geometry/world vectors are added into velocity-like accumulators, supporting slope-influenced speed outcomes
- This is consistent with observed slope speedup and jump momentum retention.

### New documentation files
- `rescue_bot.md` created for full RescueBot-specific deep dive.
- `physics.md` created for movement/physics-focused analysis and next-step proof plan.

## 14) RescueBot Deliverables (2D Visual/Stage Inventory)
Added concrete rescuebot analysis artifacts:

- 2D Gas Station visual (candidate order):
  - `extracted/gas_r/rescuebot_gasstation_order.png`
- 2D Gas Station visual (trigger checks + 600-unit gate):
  - `extracted/gas_r/rescuebot_gasstation_checks.png`
- Full stage victim-order export used for rescuebot candidate-order modeling:
  - `extracted/rescuebot_stage_victim_order.csv`
  - `extracted/rescuebot_stage_victim_order.json`
- Generator script:
  - `tools/render_rescuebot_diagrams.py`

Notes:
- These visuals are intentionally top-down 2D for readability/handoff.
- Candidate order is modeled from stage `RCS_VictimRescue` entity order and aligned with the first-eligible selection behavior of `FUN_00455230`.

## 15) Plain-Language + Full 2D RescueBot Visual Pack
- `rescue_bot.md` now includes a plain-language section explaining:
  - cooldown meaning, units, and measured-from behavior
  - per-victim checks in practical terms
  - what `victim+0x120` (Ghidra `victim[0x48]`) means and what it does *not* map to in data files
  - LOS/collision gate in gameplay terms
- New all-stage 2D rescuebot visual set generated:
  - `extracted/<stage>/rescuebot_explainer_order.png`
  - `extracted/<stage>/rescuebot_explainer_checks.png`
- Generator updated: `tools/render_rescuebot_diagrams.py`

## 16) RescueBot Verification Corrections
- Corrected LOS polarity in `rescue_bot.md`: `FUN_0048fea0` returns clear/no-collision as true, but `FUN_00455230` continues only when that result is `0`, so RescueBot wants a blocked player-to-victim trace.
- Clarified timers:
  - selected-victim active timer: `now + 4000 + random(0..3999)` in `FUN_00454750`
  - field `+0x10`: `now + 120000 + random(0..119999)` in `FUN_00454100`
  - direct auto-attempt gate uses field `+0x14` and `FUN_004541a0`
- Added third all-stage LOS/blockage plot:
  - `extracted/<stage>/rescuebot_explainer_los.png`
- Added player extent diagram:
  - `extracted/player_collision_extents.png`

## 17) RescueBot Difficulty Scaling Confirmed
- Confirmed the `FUN_004541a0` auto-attempt scale selector is the game difficulty field, not just an unknown state:
  - `FUN_004792c0` handles the `difficulty` script command.
  - It writes `0` for `easy`, `1` for `med`, and `2` for `hard` to `*(DAT_00502aac + 0x6e4 + 0x2c)`.
  - `FUN_00423f20` displays the same values as `easy`, `medium`, and `hard`.
  - `FUN_004541a0` reads the same field at `004541e8..004541f4`.
- RescueBot first-attempt base-time scales:
  - easy: `0.6`
  - medium/normal: `0.75`
  - hard: `1.0`
- RescueBot recurring auto-attempt scales:
  - easy: `1.0 * 60000 + random(0..59999)` ms
  - medium/normal: `2.0 * 60000 + random(0..59999)` ms
  - hard: `1000.0 * 60000 + random(0..59999)` ms, effectively preventing repeated auto-rescues during normal play
- Updated `rescue_bot.md` with proof snippets and a plain-language frequency explanation.

## 18) RescueBot Timer Lifecycle Completed
- Expanded `rescue_bot.md` with a complete timer lifecycle:
  - `FUN_00451480` constructs the RescueBot block and sets enabled, but does not start the meaningful auto-attempt timer.
  - `FUN_00454ba0` is the verified startup/reset routine that schedules `+0x10` and calls `FUN_004541a0` to schedule `+0x14`.
  - `FUN_00455580` is called from gameplay/world update at `00453876`, so RescueBot is polled each gameplay update rather than using an independent timer thread.
  - The auto-attempt branch opens only when `DAT_00502afc > rescuebot+0x14`.
  - The expired timer still has gates: RescueBot enabled, mission state allowed, not already busy, world state `3/4/5`, and a difficulty-scaled victim cap.
- Added difficulty-scaled RescueBot victim-cap factors found in `FUN_00455580`:
  - easy: `0.4 * [world+0x3528]`
  - medium/normal: `0.25 * [world+0x3528]`
  - hard: `0.0 * [world+0x3528]`
- Added drop-off lifecycle:
  - selected victim sets `+0x04` current victim and `+0x00 = now + 4000 + random(0..3999)`
  - while `+0x04` is nonzero, the auto-attempt branch is blocked
  - when drop-off expires, `FUN_00454130` pushes future deadlines by 10 seconds and `FUN_00454100` resets the message timer

## 19) RescueBot Stage Timing Tables
- Confirmed `Data/ResDefs/thegame.lev` `time:` values feed the first-attempt base:
  - parser at `FUN_00423160` matches `"time:"`
  - stores parsed float at current level definition `+0x10`
  - multiplies by constant `60.0` at `0x004df224`, so `time:` is minutes converted to seconds
- Updated `rescue_bot.md` with first auto-attempt windows by campaign rescue stage and difficulty:
  - formula: easy `timeMinutes * 54 + random(0..29.999s)`
  - formula: medium/normal `timeMinutes * 67.5 + random(0..29.999s)`
  - formula: hard `timeMinutes * 60 + random(0..29.999s)`
- Added recurring attempt table by stage/difficulty:
  - recurring timing is not stage-specific after the first attempt
  - easy base recurring: `60..119.999s`
  - medium/normal base recurring: `120..179.999s`
  - hard base recurring: `60000..60059.999s`
  - successful rescues usually add `10s` to the next attempt when drop-off completes
- Clarified that stage timing tables are attempt windows, not guaranteed rescues:
  - broad gates can block the selector before victim scanning
  - victim cap can skip selection
  - `FUN_00455230` can log `"RESCUEBOT: No eligible victims"`
  - actual rescue requires runtime state `4/7`, flag `+0x1fd == 0`, blocked trace, and distance at least `600.0`

## 20) Victim State 4/7 Meaning
- Corrected the RescueBot eligibility field: Ghidra's `victim[0x48]` expression is pointer-array notation for the real offset `victim+0x120`.
- Confirmed RescueBot checks `victim+0x120` directly:
  - `00455399 MOV EAX,dword ptr [ESI + 0x120]`
  - `0045539f CMP EAX,0x4`
  - `004553a4 CMP EAX,0x7`
- State `4` is the ordinary spawned/idle/available victim state:
  - `FUN_0045b690` victim creation writes `victim+0x120 = 4`.
  - `FUN_00459a50` animation/idle helper also writes `victim+0x120 = 4` for normal victim profiles.
- State `7` is tied to victim `roam` script handling:
  - `FUN_0045cdb0` compares against the string `"roam"` at `0x004e2544`.
  - It parses the numeric argument and calls `FUN_0045c7f0`.
  - `FUN_0045c7f0` writes `victim+0x120 = 7`, stores the roam radius/limit at `victim+0x224`, and tests nearby `+/-5` offsets.
- The `norescuebot` per-victim exclusion flag is also verified:
  - `FUN_0045cdb0` compares against `"norescuebot"` at `0x004e2538`.
  - It writes byte `victim+0x1fd = 1`.
  - RescueBot requires `victim+0x1fd == 0`.
- Static map data makes state `7` visible indirectly through scripts such as Gas Station `S_Script01: roam 250`, `roam 80`, `roam 400`, and `roam 350`.
- Static map/data person type remains separate:
  - `D_Type` values come from `Data/ResDefs/victims.srf` (`boy`, `woman`, `man`, `fatman`, `girl`, `dog`, `herodown`).
  - No current evidence maps state `4` or `7` to a person type like dog/woman/man.

## 21) Streamlined RescueBot Handoff
- Added `rescue_bot_streamlined.md` as a clearer non-technical handoff document.
- It summarizes:
  - the practical RescueBot order of events
  - stage/difficulty first-attempt timings
  - recurring attempt timings
  - difficulty victim caps
  - victim eligibility checks
  - manual/debug trigger behavior
  - victim scan order for every extracted rescue-capable stage
- This document is intended for gameplay experts who do not need the full decompiler proof trail but still need exact timings and stage-specific victim order.

## 23) Crouch Deep Dive + Crouch Bug
- Added `crouch.md` with a full crouch-focused reverse engineering pass.
- Confirmed crouch/climb command parser path in `FUN_00451870` handles `crouch`, `uncrouch`, `climb`, and `unclimb`.
- Confirmed crouch state machine core:
  - standing state `3`
  - crouch state `4`
  - climb state `5`
  - state field at `player+0x18c`
- Confirmed crouch changes:
  - vertical offset `player+0x88` set to `25.0` on crouch and restored to `85.0` on successful uncrouch.
  - movement speed cap reduced from `400.0` to `260.0` in crouch (`0x00453163` block).
- Confirmed ladder integration from map scripts (`touch: action Player (climb 1)` and `untouch: action Player unclimb`) across many stages.
- Reconstructed crouch bug mechanism:
  - crouch sets low offset (`+0x88 = 25`) and state `4`
  - ladder `climb` sets state `5` without restoring offset
  - ladder `unclimb` sets state `3` without restoring offset
  - result: standing state with crouch squat retained.
- Identified additional likely inducer:
  - climb-exit via `FUN_0044e990` also sets state `5 -> 3` without restoring offset.

## 24) Bug Hunt Pass: Additional Speedrun-Relevant Surfaces (2026-05-12)

- Added dedicated report: `bug_find.md`.
- Confirmed parser-level command surface in `FUN_00451870` includes movement/state commands (`climb`, `unclimb`, `escape`, `finalexit`, `finalexitloadmap`) that map scripts can invoke.
- Confirmed additional crouch-bug inducer path:
  - `FUN_0044e990` (`jump` while climbing) can force `state 5 -> 3` without offset restore, same desync class as ladder `unclimb`.
- Collected stage-level trigger density from extracted scripts:
  - highest climb/unclimb density: `subway_a`, `subway_b`, `oil_rig_C`.
- Confirmed map-side sequence trigger surface:
  - `finalexitloadmap` appears in multiple stages (`oil_rig_A/B/C`, `subway_a`, `boat_a`) and is parsed through player command path.
- Confirmed unusual OOB/secret trigger in `mbase` (`secret1`) explicitly includes climb/unclimb + bonus text.
- Community signal (limited external verification in current environment):
  - speedrun listing snippet references Downtown Bar "ladder exit" optimization.
