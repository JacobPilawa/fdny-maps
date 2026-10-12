# Training zone actor-membership component

`src/zone_membership.h` implements the isolated membership rebuild in `00484260` against the existing `TrainingZones` lookup. Each call resets the keyed zone lists and no-zone diagnostic list, then walks the caller-supplied actor list in source order. It accepts only type `2` actors, skips states `5` and `6`, obtains the source position through a callback corresponding to vtable slot `+0x18`, and uses `TrainingZones::lookup` to select a zone. It appends each actor ID to that zone's label-keyed list. If no zone contains the actor, it records the ID and invokes the diagnostic callback before continuing.

The membership actor order is preserved within each list. Zone lookup already preserves the source inclusive bounds, smallest summed-member-box volume rule, and later case-sensitive tree entry on equal-volume ties; those policies remain owned by `zones.h`. The input actor vector must represent the global actor list order. This component does not create actors, connect a Player state schedule, or trigger periodic rebuilds.

## Ghidra evidence

Fresh Ghidra 12.0.4 read-only query of preserved `FDNYFirefighter.exe` decompiled `00484260` plus `00481C00`, `00481BA0`, `00483E10`, and `00483C80`; the preserved project was locked, so queries used a scratch project copy under `/private/tmp/fdny-victim-rescue-audit-ghidra`. Raw outputs are `/private/tmp/zone-membership-query.txt`, `/private/tmp/zone-membership-details.txt`, and `/private/tmp/zone-membership-iter.txt`.

In `00484260`, the first loop traverses the zone tree and clears each nonempty actor array. The following list iterator reads one actor record and advances its linked-list cursor before processing the next record. It tests actor `+0x18 == 2`, then skips state `+0x120` equal to 5 or 6. Eligible actors call vtable slot `+0x18`; the returned frame position at `+0x28` is passed to `00483E10`. A null zone triggers the `TMapZoneManager: **ERROR** Victim "..." is not located in any zone!` diagnostic, then continues iteration. A matching zone grows its actor array and writes the actor pointer at the old count index, preserving source-list order.

`00483E10` enumerates the ordered zone tree and updates its best match on `candidateVolume <= currentVolume`, so a later equal-volume match wins. `00483C80` checks member trigger bounds through `00481840`; the existing native `TrainingZones` implementation carries the source-derived inclusive bounds and label/tree ordering documented in [`../../training-zones-runtime-audit.md`](../../training-zones-runtime-audit.md). The executable hash is recorded in [`../../HANDOFF.md`](../../HANDOFF.md).

## Validation and integration

Run from `decomp/`:

```sh
c++ -std=c++17 -Wall -Wextra -pedantic tools/zone_membership_check.cpp -o /tmp/zone-membership-check
/tmp/zone-membership-check
```

The standalone check exercises smallest-volume selection, inclusive edges, equal-volume tie selection, state/type filtering, membership clearing, list-order append, multiple zone members, and the diagnostic/no-zone path. It uses a minimal `V`/volume harness and verifies the native component rather than executing the original binary. Integration should provide actors in original global-list order, source positions from the actor position getter, and a diagnostic adapter that resolves the actor name. Refresh cadence remains governed by verified callsites such as pickup and exit handling; this component assigns no per-frame schedule.
