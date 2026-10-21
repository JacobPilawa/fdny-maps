# Linked-stage live statistic collection lifetime

## Provenance

Static read-only Ghidra 12.0.4 exports for the pinned
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The profile/stat exports are saved in
[`mission_outcome_score_profile_fresh_2026-10-20.c`](mission_outcome_score_profile_fresh_2026-10-20.c),
and the linked request/loader exports are in
[`0042a260_set_pending_level_request_fresh.c`](0042a260_set_pending_level_request_fresh.c)
and
[`0042ab00_process_level_load_request_fresh.c`](0042ab00_process_level_load_request_fresh.c).
No game runtime was captured.

## Reset branch

`SetPendingLevelRequest` (`0042A260`) copies active level metadata when
EverGame `+0x688` is non-null and clears request fallback byte `+0x14`.
`ProcessLevelLoadRequest` (`0042AB00`) only calls `00424710` when that byte is
nonzero. Thus the mapped linked `finalexitloadmap` path skips the entire stat
initializer call.

When called, `00424710` first invokes `004883D0` on the profile/stat object at
`+0x30`, which clears the live collection. Its boolean argument then controls
whether it registers the 13 stat definitions. The distinction is material:
the linked path does not merely skip descriptor registration; it skips the
collection clear as well. The function is not called at all when fallback
byte `+0x14` is zero.

The pre-load flush's direct world-resource and ScriptEngine cleanup does not
target the profile object's `+0x30` collection. The selected-profile writer is
now mapped through profile-menu callback `00430060` and manager setter
`004239C0`; the live selected slot, loaded values, and all indirect callback
effects remain unobserved. Static evidence establishes the normal loader
branch's reset behavior, not runtime parity or every indirect effect.

## Shared-port comparison

`runRescueSceneChain` creates one `RescueStatisticsMap` before the linked-stage
loop and passes it through each scene's `run()` call. This matches the mapped
branch-level fact that linked loads skip the explicit profile stat-collection
reset. The host map is not the retail profile object, and no runtime parity is
claimed while indirect cleanup effects and live selected-profile values remain
unverified. The relevant host code is in
[`scene_viewer.cpp`](../../../port/spikes/training-native-prototype/src/scene_viewer.cpp).

The profile's separate win-result save path is `00424650`; it copies live
statistics into the selected level record and serializes the profile. No claim
is made here about result timing or successful save execution during a linked
stage transition. See also
[`level_transition_flush_2026-10-21.md`](level_transition_flush_2026-10-21.md)
and
[`mission_outcome_score_profile_fresh_2026-10-20.md`](mission_outcome_score_profile_fresh_2026-10-20.md).
