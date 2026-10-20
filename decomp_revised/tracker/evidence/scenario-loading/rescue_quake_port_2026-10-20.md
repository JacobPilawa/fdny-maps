# Rescue CameraRescue quake path and shared-host port

## Evidence boundary

The retail evidence below is from `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The current Ghidra project and command logs are local under `.local/` and are
not part of the mirror. This note records the relevant addresses, fields, and
control flow so the trace is reproducible without those files.

## Retail command and state

`0047A1F0` parses the `quake` command's two floating-point arguments and calls
`00478900(arg1, arg2)`. The latter walks the world actor list and calls
`0049C890(this, arg1, arg2)` for actors whose runtime type field at `+0x18` is
5. `00446B40` is the recurring actor update wrapper and calls `0049CD80`
followed by `0049CF10`.

The setter at `0049C890` writes `arg2` to `+0xCC`, `arg1` to `+0xD0`, the
current unsigned millisecond clock (`00502AFC`) to `+0xC8`, and zero to the
next-sample deadline at `+0xC4`. The update at `0049CD80` therefore confirms
`arg1` is strength and `arg2` is duration in seconds; it stores
`durationSeconds * 1000` as binary32 milliseconds.

While the unsigned clock is at or after the start tick and no later than the
start plus duration, the updater smooths the current vector at `+0x4C/+0x50`
toward target values at `+0xD4/+0xD8` using
`min(frameDeltaSeconds * 20, 1)`. When the sample deadline is due, it advances
that deadline by 50 ms, computes progress from unsigned elapsed milliseconds
over the binary32 duration, and scales strength by these phases:

- `0.5 * progress` before progress `0.25`;
- unchanged from `0.25` through `0.5`;
- `1 - 2 * (progress - 0.5)` after `0.5`.

It requests a random X target in `[-strength, strength]`, then a random Y target
in the same range multiplied by `0.5`; both draws go through `00498B30` and
the shared retail RNG. Expiry clears the `+0x4C` vector with `0042D280` and
sets `+0xC4` to `0x540BE3FF`. The current host models the same field roles,
50 ms sample interval, easing, envelope, and X-then-Y draw order. The unsigned
clock remains exact through the x87-style comparison in the host; the stored
duration retains the binary32 multiplication result.

## Authored coverage and port status

The current raw-data scan found ten `quake` command references in nine
campaign map sources: one each in the Bank and Boat B BSP script slots; one
each in Bar R, Boat A, and Oil Rig A/B/C/D EVS; and two in Gas R EVS. This is a
static authored count only; it does not prove any trigger is reached in retail.

The shared scene viewer now dispatches authored `quake` commands into
`RescueQuake` and applies its current vector during scene rendering. A bounded
exact-offset x86 scan found the quake fields' use in the initializer/updater,
but did not resolve the final connection from `+0x4C/+0x50` to Genesis's
rendered `geCamera`. The viewer currently maps those values to camera-local
right/up translation as a provisional presentation choice. Camera consumer,
event cadence, cross-system RNG interleaving, and live retail parity remain
open.

The rescue-all macOS app built from this port. Standalone and packaged viewer
SHA-256 match at
`2421a5019daf5a398db4e1926b3beac8d31cd4cde27a5eac32bf1c60aadab16f`; the
bundle plist passed validation, and the package generator emitted eight rescue
catalog entries. The existing json-c deployment-target warning remains. No
tests, interactive run, or retail parity check was performed.

