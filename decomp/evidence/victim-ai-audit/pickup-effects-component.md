# Native pickup post-effects component

`src/victim_pickup_effects.h` ports the ordered post-event boundary of Player handler `00450440`. A fresh preserved-project Ghidra 12.0.4 read-only query on 2026-10-11 confirmed `00450440`, `00490E60`, `0044FCE0`, and `00484260`. The executable hash is the one recorded in HANDOFF.md. Raw query output stays local under `pickup-post-effects-native/ghidra-query.log`.

The component invokes entity mode 1, writes value 20 for Player field `+0x18C==4` or 50 otherwise, clears the neighboring value, enables the byte, invokes the status adapter and zone-refresh adapter, then reads the current clock for Player `+0x5C4`. The zone predicate includes type 2 entities except states 5 and 6. The clock is deliberately read after the callbacks; it is not passed as an earlier snapshot.

`tools/victim_pickup_effects_check.cpp` joins the existing delivered-message component to this post-event component. It checks six field branches, flags/event/effects ordering, rejection of repeated pickup without effects, carried-victim exclusion, a changed callback clock, and 18 zone type/state combinations. These 24 checks assert the statically recovered transaction; they do not execute original machine code or verify a recurring schedule. CTest builds and runs the check.

Entity-mode internals, status/list operations, complete zone membership reconstruction and gameplay wiring remain adapter work. The native render loop now separately stores per-entity victim actor state and uses the source normal-pass flag predicate. No live pickup producer or escape trigger is connected by this milestone.
