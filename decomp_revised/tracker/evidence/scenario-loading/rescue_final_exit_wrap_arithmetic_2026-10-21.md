# Rescue final-exit counter arithmetic — 2026-10-21

## Retail source

The pinned `FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The hash-matched exports are [`00450a00_handle_final_exit_command_fresh.c`](00450a00_handle_final_exit_command_fresh.c)
and [`00450a70_player_finalexitloadmap_fresh.c`](00450a70_player_finalexitloadmap_fresh.c).
The saved `00450A70` instruction excerpt performs:

```text
00450abf  SUB ECX, EDX    ; Player +0x3528 - Player +0x3530
00450ac1  SUB ECX, EAX    ; result - Player +0x352c
00450ac3  JNZ ...         ; only exact zero takes the accepted path
```

`00450A00` has the same two-subtraction expression in its current-hash
decompilation and compares the resulting `int *` value with null. Both command
paths therefore use 32-bit wraparound and accept only an exact zero remainder.
The distinct `004504C0` availability path keeps its signed-positive test; that
logic is also represented with a wrapped `uint32_t` remainder.

## Shared port correction

Before this correction, the interactive `finalexit` and
`finalexitloadmap` handlers widened the two counters to `uint64_t` and clamped
any over-count to zero. That could accept an exit which retail would keep
gated. `remainingRescuesForFinalExit` in `victim_exit.h` now returns the
source-shaped wrapped remainder, and both handlers use exact-zero acceptance.
The shared availability helper uses the same arithmetic primitive. For an
over-count, the hint formats the remainder as the signed 32-bit value passed by
the retail handlers.

This only changes counter edge cases. Ordinary in-range rescue counts are
unchanged. No counter-corruption or natural-exit gameplay capture was made, so
this is a static-source correction, not a retail runtime parity claim.

## macOS build

`tools/build_scene_viewer_app.sh` completed successfully. The standalone and
app-bundle arm64 executables match at SHA-256
`6d83512a8bd33163eb551ed0259cd2839b03d2c87f1bee8c650beeaafe18ffa8`. The
bundle contains all 18 rescue and hose scene packages, emits eight rescue
choices, and passes `Info.plist` validation. The linker warns that Homebrew
`json-c` targets macOS 27.0 while the build target is 13.3. No tests,
interactive gameplay, or retail parity check were run.
