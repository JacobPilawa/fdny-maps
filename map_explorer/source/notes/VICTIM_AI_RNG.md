# Victim AI, random spawns, ordering, and model time scale

This folder preserves the earlier FDNY research and native decompilations. The explorer reads authored BSP data; it does not run the retail game or predict live victim positions. Native symbols and Ghidra types remain provisional.

## Where the information is in the explorer

Click a victim or alternate spot for its coordinates, authored scripts, contact radius, BSP victim order, and linked candidate spots. The optional Roam guides and Spawn connections overlays are in the map tools menu. The Map data group opens the complete entity table, CSV, and original script files. These research notes remain in the copied source folder. Per-map `victim_roster.csv` and `.json` list every victim/candidate, candidate number, coordinates, roam command, and contact radius. Original tables and PDFs are retained separately.

## Roaming radius and movement

`roam N` reaches the victim handler at `0x0045c7f0`. It stores the radius at offset `+0x224`, temporarily sets state 7, copies the current position into `+0x1c8..+0x1d0`, and initializes a value of 36 at `+0x228`. It probes four diagonal offsets: `(5,0,5)`, `(5,0,-5)`, `(-5,0,5)`, `(-5,0,-5)`. If all fail, it reports a stuck spawn and sets state 4. The subsequent animation/state helper can also alter state; an authored roam command alone does not prove the victim is currently walking.

The movement check at `0x0045ab40` compares squared 3D distance from a saved runtime anchor at `+0x218` against the square of the stored radius. It also performs collision, obstacle, and floor checks. This is constrained movement, not a uniform random position anywhere in a circle. The exact runtime anchor's full initialization lineage is not proven here. The explorer therefore shows an illustrative X/Z radius around each authored spawn candidate, rather than claiming an exact accessible movement region.

`J_Radius` is a separate contact/pickup box extent. The native Training contact path uses ±60 units (including the native fallback), while the actor's physical collision hull includes ±24-unit extents. Neither value should be substituted for `roam N`.

Evidence: [roam initialization](../research/victim-runtime/0045c7f0_FUN_0045c7f0.c), [movement checks](../research/victim-runtime/0045ab40_FUN_0045ab40.c), [collision correction](../research/COLLISION_CORRECTION.md), and [earlier victim notes](victims.md).

## Victim and spot ordering

V1, V2, etc. retain the April 30 labels and BSP entity order. For each victim, candidate 0 is the base position, and candidates 1 onward follow the authored alternate spots in BSP order. Alternate spots' `A_Victim` field links to the victim's internal `%name%` identifier; the explorer exposes both that original link and the corresponding V-label.

The recovered RescueBot assignment routine scans for the first qualifying victim. Eligibility includes victim state 4 or 7, an unclaimed flag, sufficient distance (600-unit gate in the recovered code), and blocked line of sight. This is not a direct random selection of a victim index. The complete runtime container initialization order has not been verified, so BSP order is useful evidence, not a guaranteed rescue sequence. Training explicitly disables RescueBot with `setrescuebot 0`.

Evidence: [assignment scan](../research/rescuebot/find_assign_victim.c), [RescueBot source notes](../research/rescuebot/README.md), [authored order table](../research/rescuebot_stage_victim_order.csv), and [earlier RescueBot notes](rescue_bot.md).

## RNG and candidate selection

The recovered generator at `0x00404930..0x00404B1F` has a 624-word MT19937 state, the older 69069 state expansion, a lazy default seed of 4357, and a float constructed from the low 23 bits of the output. It is a shared generator: other startup consumers can affect which draw reaches victim construction.

The newer reconstruction corrected the candidate range to `count - 0.01` (constant at `0x004DFC78`), followed by x87 nearest-even integer conversion. Earlier copied notes that use `count - 1` and their probability examples are historical and should not be treated as the current finding. The native placement routine checks the converted index is in range before relocating. The copied TypeScript implementation clamps the upper index, which is a port choice rather than proof that retail does the same thing.

The exact fresh-game draw ordinal, initialization sequence, and all upper-range cases still need retail runtime instrumentation. These files do not establish reliable seed-to-spawn prediction or equal selection probabilities.

Evidence: [generator initialization](../research/victim-runtime/00404930_FUN_00404930.c), [float generation](../research/victim-runtime/00404ac0_FUN_00404ac0.c), [candidate placement](../research/victim-runtime/0045d5b0_FUN_0045d5b0.c), [copied RNG port](../research/fdny-random.ts), and [later parity findings](../research/TRAINING_PARITY.md).

## `settimescale`

In the animated model script handler at `0x0043e680`, `settimescale` parses a numeric argument and stores it at entity offset `+0x1b8`. The model update at `0x0043dc60` advances or reverses model animation time (`+0x1b4`) by `frame_delta * timescale` during opening, closing, or looping.

For this handler it changes the addressed animated model's playback rate, rather than the global game clock. A value of 4 means four times normal playback; the nominal traversal duration becomes one quarter, subject to frame steps, endpoint clamps, and callbacks. The original shared `doors.evs` macro uses 3.0. Subway B D10 and D11 explicitly use `touch: settimescale 4 | touch: open | untouch: close`.

Evidence: [command handler](../research/door-runtime/0043e680_FUN_0043e680.c), [animation update](../research/door-runtime/0043dc60_FUN_0043dc60.c), [original doors script](../scripts/scripts/doors.evs), and [Subway B entity table](../subway_b/annotated_entities.csv).
