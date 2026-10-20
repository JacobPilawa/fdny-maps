# Catalog `time:` field and Player rescue deadline input — 2026-10-19

## Finding

The rescue chooser now carries the selected catalog record's `+0x10` value
into the shared rescue runtime. The retail parser stores `time:` multiplied by
`60.0f` at level record `+0x10`; Player initializer `0044D550` copies that
field to Player `+0x343C` and applies a mode multiplier. The port previously
fed the rescue transaction a fixed `600.0f`. It now uses the selected retail
value on the rescue-all launch path and retains it across that mission's
linked stages.

This is a deadline input, not a confirmed mission countdown. Fresh caller
references show `004541A0` is also called from two RescueBot-related routines.
The meanings of the written deadline fields and any mission expiry, objective
HUD, success/failure screen, or outro are still not established.

## Retail evidence

- Executable: `FDNYFirefighter.exe`, SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Catalog: `Data/ResDefs/thegame.lev`, SHA-256
  `ee6819873cd540e67613774d848506561e2861442b37ef301e27273805d558c6`.
- Ghidra 12.0.4 query: headless, `-readOnly -noanalysis`; fresh decompilation
  of `00451160`, `004541A0`, `00454BA0`, and `00455580` is preserved in
  [`004541a0_deadline_callers_fresh.c`](../scenario-loading/004541a0_deadline_callers_fresh.c).

The relevant source chain is:

1. `00423160` parses each level record's `time:` token, stores the float times
   `60.0f` at record `+0x10`, and uses `00422800`'s `1200.0f` record default
   when the token is absent.
2. `0044D550` reads active level `+0x10` into Player `+0x343C`, or selects
   `600.0f` when no level is active. It multiplies by `1.5` in modes 0/1 and
   by `1.0` otherwise.
3. `00451160` completes a carried-victim escape, updates rescue statistics,
   releases the victim, runs status action 2, then calls thunk `00454300` to
   `004541A0` with `Player+0x340C`. The helper writes the caller's `+0x14`
   word, which is Player `+0x3420` for this call. This occurs before the carry
   pointer is cleared and before exit availability is refreshed.
4. Fresh reference queries found two other direct callers: `00454BA0` at
   `0045520B`, which sets up RescueBot response text/deadlines, and `00455580`
   at `0045578F`, whose decompilation includes `RescueBot.cpp` diagnostics and
   victim-count handling. Their helper calls use their own object deadline
   words while reading the shared Player field.

The current helper decompiler output drops the x87 values used by
`004541A0`. The earlier source-pinned assembly/P-code audit in
[`rescue-deadline-exit-component.md`](../../../reference/previous-work/decomp/evidence/victim-ai-audit/rescue-deadline-exit-component.md)
recovers the clamp `[180,7200]`, mode-dependent threshold and delay, unsigned
clock comparison, conversion order, and shared RNG use; its original P-code
oracle matched 48 seeded calls. The helper's downstream gameplay meaning is
not inferred from those arithmetic results.

## Port change

`RescueMissionChoice` stores the parser-equivalent level `+0x10` from the
existing `rescue_missions.json` `time` field, including the retail record
default when absent. The selection loop passes it into `runRescueSceneChain`,
which passes the same selected record field through each linked rescue stage.
The runtime adapter now calls the existing
`initializePlayerDeadlineInput` implementation with the current mode. A
non-catalog single-map or custom-level launch follows the retail no-level
fallback; its initializer-scaled result is `900.0f` in the port's current
mode 0. Difficulty selection remains outside this port path.

| Rescue row | Authored `time` | Level `+0x10` | Player `+0x343C` in current mode 0 |
|---|---:|---:|---:|
| Training | 500 | 30000 | 45000 |
| MBase | 5 | 300 | 450 |
| Bar R | 10 | 600 | 900 |
| Gas R | 8 | 480 | 720 |
| Bank | 15 | 900 | 1350 |
| Boat A | 15 | 900 | 1350 |
| Subway A | 12 | 720 | 1080 |
| Oil Rig A | 10 | 600 | 900 |

The column values demonstrate the parser and initializer arithmetic. No unit
or mission-expiry interpretation is assigned to the catalog label.

## Build and limits

`tools/build_scene_viewer_app.sh rescue-all` built the macOS bundle. The CMake
binary and packaged executable both have SHA-256
`4deb4e9bc0ea82352bf2f0971270ce5fefd0c2898023811c0356e686056a60f4`. The
build emits the existing Homebrew `json-c` deployment-target warning (library
built for macOS 27.0, linked for 13.3).

The packaged `--mission-catalog-check` loaded all eight retail choices and
the local custom test room, printed the eight values above, and exited
successfully. `git diff --check` also passes. These are a build and controlled
package check, not a natural rescue, RescueBot scheduling, retail runtime, or
parity result. The active goal remains open on natural rescue flow, the
deadline fields' downstream effects, mission results/outro, and all remaining
scenario mechanics.
