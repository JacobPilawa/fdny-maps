# PlayerHose relative mouse steering trace and port (2026-10-20)

## Scope and provenance

This trace follows retail mouse data through the input action map into the
`PlayerHose` pitch/yaw accumulators, then ports its default scale and signs to
the shared host. The analyzed executable is
`inputs/reference/raw-copy/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 headless read a disposable copy of the existing project with
analysis disabled. The saved selected pseudocode and vtable-target assembly
are in [`playerhose_mouse_steer_fresh_2026-10-20.c`](playerhose_mouse_steer_fresh_2026-10-20.c).

## Retail path

`FUN_00437F30` registers `MOUSE_X` as relative axis 0 and `MOUSE_Y` as
relative axis 1. `FUN_004395C0` initializes the `STEER_YAW` and `STEER_PITCH`
axis actions, then binds them to indices 0 and 1 respectively. The shared
action accessor `FUN_004386F0` resolves an action and sums values returned by
its bound input controls.

Mouse provider update `FUN_00437AB0` clears its X/Y accumulators at `+0x08` and
`+0x0C`, calls DirectInput `GetDeviceData` with `0x14`-byte event records, and
adds signed `dwData` for `dwOfs` 0 and 4. Thus the values are per-poll relative
motion, not cursor positions. Vtable target `00437620` exposes the X/Y values
for axis indices 0/1. The outer axis wrapper at `00439310` multiplies the
provider value by the float at `004FF480`, then negates it. The image stores
`0.10000000149011612` there (`float32` bits `0x3DCCCCCD`), and the fresh direct
data-reference scan found the read at `0043931E`. A later settings write was
not established, so this is the executable image default.

The provider object chain is also resolved. `FUN_004395C0` installs the outer
action vtable and nests the device provider at `this+8`. Provider vtable
`004E0A14` slot 5 targets `004394A0`; that unbounded target advances `this+8`,
calls `00437AB0` to sample the mouse, then calls `00437C80` for the associated
keyboard state. The provider's slot-3 axis getter `00437620` reads the fields
that the mouse poll updates through that subobject offset. This proves that
mouse samples refresh through a virtual device-update callback. Static
references do not identify its manager scheduler or how often it dispatches
the callback relative to PlayerHose.

`FUN_00466780` subtracts `STEER_PITCH` and `STEER_YAW` from accumulators at
PlayerHose `+0x230` and `+0x234`. Combined with the wrapper sign, positive
relative X/Y counts increase those accumulators by 0.1 degrees per count.
Pitch clamps to the active path record's symmetric `N_PitchSpread`; yaw clamps
to +/-60 degrees. It also constrains the resulting third frame axis to an
angle between 10 and 170 degrees from world up, then rotates the temporary aim
frame by positive pitch followed by negative yaw. The stream origin continues
to come from the unrotated entity frame through `00465EF0`.

## Shared host change

`PlayerHoseStage` now queues relative SDL X/Y counts separately from ordinary
camera yaw/pitch, applies the 0.1-degree source scale once before each valid
route-frame aim update, and keeps separate degree accumulators. It applies the
authored +/-60-degree yaw limit, the current hotspot pitch spread, and the
world-up guard represented as an +/-80-degree absolute-elevation constraint
relative to the route frame. The temporary basis rotations use the traced
`+pitch`, `-yaw` order. This keeps the PlayerHose aim independent of the
free-look camera controls used by the other rescue host.

## Open limits

Static analysis does not establish how often retail polls DirectInput relative
to its gameplay update, so the host consumes all SDL relative events once per
host gameplay frame. The image-default scalar may be changed by untraced user
settings. Exact x87 rounding, event-to-frame timing, a live nozzle orientation,
and retail parity remain unverified. This was a source trace and compile
change only; no interactive run or controlled retail comparison was made.

## Build record

The macOS rescue-all build completed. The standalone executable and packaged
bundle executable both have SHA-256
`2815935508483ab4ec56e45ec57991561a0e4ea910886c9bb0b2aa2a98084637`. The
build script validated `Contents/Info.plist`, generated the eight-entry rescue
catalog, and packaged `city_h`, `const_h`, `dock_h`, and `oil_rig_h`. The
existing Homebrew json-c deployment-target warning remains. This is compile
and packaging evidence only.
