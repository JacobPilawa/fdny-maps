# Physics and Movement Notes (First Deep Pass)

Date: 2026-05-12
Target: `FDNYFirefighter.exe`
Focus: slope speed and repeated-jump speed behavior.

## Scope and Confidence
Because one large movement routine is currently not cleanly function-bounded in Ghidra, this pass uses instruction-level analysis and nearby callable routines.

Confidence levels:
- High: movement vector normalization, damping constants, state-dependent speed limits exist.
- Medium: slope geometry contributes additional velocity vectors.
- Medium: repeated jumping can retain/build speed due reduced damping while airborne and additive updates.

## Evidence: Movement Update Block (instruction range ~`0x00453100`-`0x004539xx`)

### 1) Input vector magnitude + normalization
Observed pattern:
- computes `sqrt(x*x + y*y + z*z)` from stack vector
- compares against `1.0` (`0x004dd808` is double `1.0`)
- when above 1.0, scales components down by reciprocal

Implication:
- classic movement input normalization (prevents diagonal input over-speed in pure input space).

### 2) State-dependent movement caps
Observed constants set based on movement state flags:
- `0x43c80000` = `400.0`
- `0x43820000` = `260.0`
- `0x44480000` = `800.0` in one state path

Implication:
- movement speed cap/target is not fixed; it depends on state (`[ESI+0x4c]`) and flags (`[ESI+0x240]`, etc.).

### 3) Damping / interpolation factors
Observed constants:
- `0x3f733333` ~ `0.95`
- `0x3e4ccccd` ~ `0.20`

These are selected by state checks and fed into follow-up vector blending routines.

Implication:
- there are different drag/accel smoothing regimes (likely ground vs air / special mode).
- lower damping in a state can preserve horizontal momentum more aggressively.

### 4) Slope / geometry vector influence
In the same block, code repeatedly:
- obtains geometry/world vectors via calls (`00495eb0`, `0042d050`, `00446390`, `00482fc0`, etc.)
- adds vectors into velocity-like accumulators at offsets near `[ESI-0x44]`, `[ESI-0x40]`, `[ESI-0x3c]`
- does additional projection/combination with environment vectors

Implication:
- final movement is not just raw input; it is input plus world-derived vectors.
- this is a plausible mechanism for slope-assisted speed changes.

## Why slope speedup is plausible
Given the observed math:
- movement vector is transformed with geometry vectors before final integration,
- and there are state-specific damping factors,
- and velocity accumulators are additive over frames,

you can get a net horizontal increase when moving over inclined surfaces if vertical/downhill components project back into forward/horizontal channels with insufficient clamping.

This matches your observed “faster on slopes” behavior.

## Why repeated-jump speedup is plausible
Likely mechanism from current evidence:
- jump/air states switch to a different damping regime (`0.95` vs `0.20`-style pathways),
- horizontal velocity is not fully reset on jump transitions,
- per-frame additive updates can preserve/build momentum if timing aligns.

That combination can produce bunny-hop-like speed preservation or gain.

## Important related systems
- Input/action binding setup (`FUN_004395c0`) registers:
  - `MOVE_*`, `STEER_*`, `JUMP`, `CROUCH`, `ACTION`, `USE`
- Movement tool logic in nearby systems (`FUN_0044f340`) also mixes action state with vector accumulation and world vectors.

## What we still need to prove rigorously
1. Exact identity of the large movement function containing `0x004531xx-0x004539xx` block.
2. Exact variable typing for offsets around `[ESI-0x44..-0x30]` and `[ESI+0x4c]` state enum.
3. Exact branch used when grounded vs airborne vs crouched.
4. Final clamp order (before/after slope projection and jump impulse).

## Suggested verification plan (next pass)
1. Recover full movement function boundaries and rename it.
2. Label offsets as `vel`, `wishdir`, `groundNormal`, `state`, `airborne` with evidence.
3. Trace jump transition writes and compare friction constants pre/post jump.
4. Build pseudocode with exact equations and clamp order.
5. Create a test matrix (flat vs slope, jump cadence) and map each to branch paths.

## Working Hypothesis
- Slope speed gain: **yes, likely real and code-consistent**.
- Repeated-jump speed gain: **yes, likely real via momentum preservation + state damping**.

Both are currently medium-confidence until full function reconstruction is completed.
