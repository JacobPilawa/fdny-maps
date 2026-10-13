# Player spawn and BSP collision source trace

Updated: 2026-10-13

## Scope and pinned inputs

This trace follows the authored MBase PlayerRescue spawn into the preview's
floor/collision behavior, using the checked-in `FDNYFirefighter.exe` with
SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
It complements, and does not repeat, the seven-map simulation in
[`scene_preview_collision_texture_diagnostics.md`](scene_preview_collision_texture_diagnostics.md).

## Findings

- The hash-pinned MBase export has one `RCS_PlayerRescue` at authored
  `ZZ_location = (-132, -36, 140)`. The class is constructed and initialized by
  the already traced WorldEntity spawn queue; its static path is documented in
  [`player_rescue_factory_and_spawn_path.md`](../scenario-loading/player_rescue_factory_and_spawn_path.md).
- Genesis collision wrapper `00492CE0` calls `geWorld_Collision` with the
  entity's current position and position-plus-delta, the supplied minimum and
  maximum hull vectors, contents mask `0x43`, and collision type `2`. It returns
  the hit point and plane normal to the caller. The pinned pseudocode is
  [`00492ce0_collision_wrapper.c`](../genesis-boundary/00492ce0_collision_wrapper.c).
- The wrapper's owner has two possible world handles (`this+8` and `this+0xB4`,
  selected by a mode byte at `this+0x160`); the trace is skipped if the chosen
  handle is null. This makes “viewer has geometry but no collision world” a
  distinct failure mode worth checking at runtime.
- `0049DA00` is a separate ModelEntity floor-placement routine, not evidence
  that the PlayerRescue spawn is automatically moved to a floor. It starts a
  `-1000` Y sweep from the model origin after adding 48 Y, stores the collision
  result, and raises the result by 1 Y. Do not apply this model-spawn rule to
  the player without further evidence.
- In the preview code, `Player::step` sweeps a hull with `mins=(-30,-30,-30)`
  and standing `maxs=(30,85,30)`, using the package mask, BSP planes, and
  collision models. The prior headless diagnostic reports an upward-facing
  floor hit near `y=-205.9` and grounded state after five seconds on MBase.

## Cause of the reported fall-through

The original headless check accidentally created a fresh `Collision` object
for its floor probe, so it did not inherit the disabled-model list used by the
viewer. The interactive loader added every BoundsTrigger's missing `model`
value as integer 0. World model 0 is the complete static BSP; this disabled its
floor collision and also removed it from the renderer. The loader and
diagnostic now share the actual collision object, and missing trigger model
fields are skipped. All seven rescue maps report no disabled model and settle
on an upward-facing world-model floor.

The shipped Genesis.dll route is now decompiled through the top-level
`geWorld_Collision` dispatch, model broadphase, BSP traversal, and convex leaf
test. The C++ Training implementation also passes the reused retail-engine
oracle: 611 hull contacts with zero mismatches (maximum point error
`7.45058e-09`) and 207 point traces with zero mismatches. This does not prove
dynamic or every rescue-map submodel parity; see
[`genesis_collision_path.md`](genesis_collision_path.md).

## Next concrete step

The next runtime check is the interactive macOS window: its actual presentation,
mouse capture, initial camera framing, and movement feel have not been observed
in this shell. If the rebuilt app still behaves differently from its passing
offscreen checks, capture its title/status and compare its package path, spawn,
disabled-model list, camera/player coordinate, and render transform. The
existing `F` floor-snap control remains a recovery aid.

## Evidence boundary

The source statements above are static findings from the pinned EXE and saved
Ghidra decompilation; the map location and preview state are package/runtime
diagnostic findings. There is no live original-game capture for this trace.
