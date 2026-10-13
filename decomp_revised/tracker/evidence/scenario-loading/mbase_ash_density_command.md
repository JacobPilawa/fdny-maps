# MBase `setashdensity` command path

This extends the reused Training ash-pool work with MBase's authored entry
point. Static evidence is for `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## MBase input and dispatch timing

`Data/Maps/mbase.evs` contains, in order:

```text
exec rescue.evs
tell player (setashdensity 0)
tell mortarfire1 (moveto mortar1 1 10 120)
```

The selected map's initial mode-1 EVS pass admits only `set` and `exec`
statements. It follows `exec rescue.evs` immediately, but skips MBase's own
`tell player` line. That map-authored command is eligible only during the
WorldEntity's later full mode-0 EVS load, after four calls to its `+0x14`
callback. BSP Player registration needs a later entity-factory queue-pump
invocation, and the exact ordering of that pump against the fourth callback is
still unresolved. Therefore the source shows the route and the timing gate, but
does not establish whether this MBase command reaches the registered Player at
mission start. See
[`entity_factory_queue_update_order.md`](entity_factory_queue_update_order.md).

## Current-hash command-to-field trace

1. `HandlePlayerScriptActorCommand` (`00451870`) recognizes `setashdensity`
   case-insensitively. For each argument it calls `ParseFloatFromString`
   (`0040CC50`); on a successful conversion, it passes the value to
   `SetPlayerAshDensity` (`0044BC90`) with the enclosing PlayerRescue root.
2. `SetPlayerAshDensity` adds `0x55c` to that root and calls
   `AssignClampedAshDensity` (`00446350`).
3. The helper stores through its receiver at `+0x54`, which is PlayerRescue
   offset `+0x5b0`. The raw bound bytes at `004dfc24` and `004dc50c` are,
   respectively, IEEE-754 `1.0f` (`0000803f`) and `0.0f` (`00000000`). Thus
   ordinary ordered numeric inputs are clamped to `[0,1]`. The x87 comparisons
   do not explicitly reject an unordered input such as NaN.
4. MBase supplies `0`. The reused same-hash Training audit maps this scalar to
   the Player ash pool's spawn-prefix scan: the initial pool count is 100, so
   density 0 selects zero entries for that later scan. It does not itself clear
   already-active particles or prove that MBase visibly draws no ash.

## Reuse of earlier work

[`reference/previous-work/decomp/evidence/scripts/training-setashdensity/audit.md`](../../../reference/previous-work/decomp/evidence/scripts/training-setashdensity/audit.md)
already maps Player initialization, the default density `1.0`, Training's
authored values, the 100-entry pool, and the density-controlled scan. Its
`Player::setAshDensity` implementation in the native prototype uses the same
clamp. That work is reused here rather than repeating the ash-pool analysis;
the new part is binding MBase's command and delayed EVS path to the current
hash's setter and PlayerRescue field.

Fresh Ghidra pseudocode and assembly are saved as
`0044bc90_set_player_ash_density_fresh.c/.asm` and
`00446350_assign_clamped_ash_density_fresh.c/.asm`; the bound bytes are in the
two `memory_*_ash_density_*.json` files beside them.

## Limits

No original-game runtime capture confirms whether the delayed MBase `tell`
arrives after Player registration, and the shared engine's recurring callback
cadence remains unresolved. The existing native ash-pool and renderer pieces
are not connected to the map-neutral rescue scene host.
