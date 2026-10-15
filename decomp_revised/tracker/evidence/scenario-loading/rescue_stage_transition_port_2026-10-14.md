# Linked rescue stage transition port checkpoint (2026-10-14)

## Retail evidence

`HandlePlayerFinalExitLoadMap` (`00450A70`) subtracts the Player's two
remaining-victim fields and only queues a level request when the result is
zero. It checks `data\\maps\\<authored-key>.bsp` and passes a separate context
string plus `data\\resdefs\\rescue.rrf` to the pending-level setter
(`0042A260`). That setter stores the context string as the pending map name;
the level loader appends `.bsp` to that field. Current static evidence does
not establish that the checked authored key and forwarded context string are
equal. See `00450a70_player_finalexitloadmap_fresh.c`,
`0042a260_set_pending_level_request_fresh.c`, and
`pending_level_request_fields.md`.

The authored stage command inventory identifies Boat A → Boat B, Subway A →
Subway B, and Oil Rig A → B → C → D. Their source keys are validated against
the copied BSP inventory in `rescue_exit_trigger_inventory.csv` and each
target package is exported under the local resources `scenes/` folder.

## Port behavior

The shared script host now handles `finalexitloadmap <key>`. It applies the
retail remaining-victim gate, accepts only a safe alphanumeric/underscore key,
and resolves that key to a sibling scene package containing the required
entity and collision data. A successful request ends the current scene loop;
the rescue chooser/runtime chain loads the sibling package and continues
until the next stage does not request another map. A repeated stage key stops
the chain to avoid a loop.

This is an explicit viewer adapter around the authored key. It does **not**
claim that the retail pending-context string is the same value. The normal
interactive run path and chooser both use the stage chain. Non-interactive
diagnostic modes stay on their supplied scene.

## Verification and confidence

**Verified:** the six authored inter-stage commands resolve to available
siblings; all five stage packages (Boat B, Subway B, Oil Rig B/C/D) load
through `SceneData`; all eight primary rescue packages pass the current
synthetic one-victim release check; the rescue mission catalog loads all eight
entries through the app bundle.

**Inferred:** the authored command key is the viewer's intended next package
key. This follows the map key documented by `00450A70` but does not reproduce
the retail setter's separate context parameter.

**Open:** live player contact with each enabled stage exit, confirmation that
every exit trigger enables at the retail point in the rescue flow, retail
key/context identity, scene teardown/reinitialization side effects, transition
timing, and full staged mission completion. No interactive run was performed
for this checkpoint.

## 2026-10-15 authored stage-exit check

The shared `--rescue-check` now discovers every BoundsTrigger `touch` binding
that calls `finalexitloadmap`. It dispatches each candidate once after the
first victim is released and requires the retail remaining-victim gate to keep
the stage request empty. After all victims are released, it dispatches each
authored stage trigger, requires the command to queue a package, and loads that
target with `SceneData` to verify that the requested key matches the package.

All 13 bundled Training/rescue/stage packages pass `--rescue-check`. The six
authored transition commands resolve as follows:

- Boat A trigger 280 → Boat B.
- Subway A triggers 562 and 568 → Subway B.
- Oil Rig A trigger 67 → Oil Rig B.
- Oil Rig B trigger 92 → Oil Rig C.
- Oil Rig C trigger 362 → Oil Rig D.

The MBase no-final-exit gate and all primary-map rescue counts continue to
pass; `--mission-catalog-check` loads all eight retail rescue choices. The
packaged and CMake viewer binaries match at SHA-256
`a52df161eef336d0cd4768a8454da378551ab82f18e237dfe991fcea96f6f564`.

This check directly dispatches controlled `touch` events at the authored
bounds centers. It verifies the command's remaining-victim gate and the
package handoff seam; it does not verify natural travel/contact, trigger enable
timing, the retail key/context relationship, teardown state, or complete
mission behavior across a live stage change.
