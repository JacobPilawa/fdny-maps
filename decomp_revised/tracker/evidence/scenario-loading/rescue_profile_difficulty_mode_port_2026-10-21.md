# Rescue profile difficulty mode defaults

## Source evidence

The input `FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Fresh read-only Ghidra notes establish that a new `DifficultyLevel` profile
record and the profile reset path both write `1` at profile `+0x2C`. The
selected profile pointer resides at EverGame `+0x6E4`; `00471030` reads that
same value when it seeds EVS variable `DIFFICULTY`. Existing `.rb` profiles
can contain other values; the port does not load them yet.

Retail Player and RescueBot helpers use the profile difficulty in mode-based
formulas:

- `0044D550` applies a `1.5` countdown multiplier for modes 0 and 1, and
  `1.0` otherwise.
- `004541A0` uses mode-specific rescue deadline factors and delay intervals;
  mode 1 differs from mode 0 (`0.75` versus `0.6` threshold factor, and a
  `120,000 ms` versus `60,000 ms` delay base).
- `00455580` computes the recurring RescueBot victim cap with factors `0.4`
  for mode 0, `0.25` for mode 1, and `0` for other values.
- `0044FF10` enters the delayed HeroDown count/spawn branch only when the
  selected difficulty is exactly 2 and its helper argument is zero.

The port's `Scripts` adapter already seeded `DIFFICULTY="1"`, but
`scene_viewer.cpp` separately supplied mode 0 to the countdown, rescue
deadline, RescueBot, and passout paths. This made those gameplay systems
disagree with the fresh-profile script value.

## Port change

The shared scene runtime now supplies mode 1, matching the verified
fresh/reset profile default and the existing `DIFFICULTY` seed. Countdown
initialization keeps the same numeric result because modes 0 and 1 share its
`1.5` multiplier. Rescue deadline thresholds/delays and RescueBot caps now
follow mode 1's distinct source formulas.

This is a fresh-profile default only. Saved-profile loading and difficulty
selection are not in the playable mission path, so existing `.rb` values
remain unsupported. Mode-2 HeroDown spawning is also still unsupported; do
not choose mode 2 until its dynamic VictimRescue factory, inventory, and
resource path are integrated. No build, test, gameplay run, or retail runtime
comparison was made for this source-backed default correction.

Related evidence:
[profile default](profile_difficulty_constructor_default_2026-10-21.md),
[profile selection pointer](profile_selection_pointer_writer_2026-10-21.md),
[deadline formulas](../runtime/catalog_time_rescue_deadline_port_2026-10-19.md),
and [RescueBot path](rescuebot_update_path_2026-10-15.md).
