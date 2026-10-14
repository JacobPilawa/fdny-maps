# FireRescue `setsize` command in the shared scene host

## Provenance and contract

This port slice is based on the pinned `FDNYFirefighter.exe`
(`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`), the
fresh `00449A80` / `00447920` decompilations, saved `00447920` timing
instructions, and `0044B620` / `0044AE80` timed-vector behavior. The relevant
source notes are in [the FireRescue movement trace](../rescue-moveto/README.md)
and [the Gas R button route](gas_r_button1_use_path.md).

Retail FireRescue `setsize` takes three integer dimensions and a fourth float
duration. `00447920` resets its timed-vector track, anchors the currently
visible dimensions at the game clock plus 10, and inserts the requested size
at the rounded timestamp `clock + duration * 1000`. `0044AE80` interpolates
between active keys and applies per-axis scale. `0044B620` allocates a later
key when the requested timestamp precedes the current clock; otherwise it
creates/replaces the next key or appends a key according to timestamp order.

## Ported behavior

`Scripts::run` already resolves `tell <actor> <command>` through the shared
actor-name table and forwards the command with its target entity ID to the
host callback. The map-neutral viewer now recognizes `setsize` for loaded
`PreviewFire` entities, validates the authored three-integer/one-float shape,
rounds the target game-clock value, and replaces that FireRescue's timed size
track. The common `FireVisual::sizeUpdate` path continues to update the
interpolated dimensions used for render placement and fire-contact bounds.

The host timestamp is the viewer's SDL tick value stored on `Scripts` each
frame. Duration conversion and host update cadence therefore remain
approximations of the retail global clock and actor callback schedule. The
saved evidence does not establish exact callback ordering, visual-driver
parity, or the original game’s frame-visible result.

This implementation is class-generic; it does not check a map or entity name.
Gas R's `button1 -> *gasoff` route can now issue its six authored FireRescue
size commands through that host path and then execute its existing global
`GasIsOn = 0` script assignment. The port has not captured the original game
accepting the use action, nor compared those effects or timing side-by-side.
The separate `boat_a.evs` `setsize 3` command targets exploder actors and has a
different command contract; this FireRescue handler does not claim to cover
that path.

## Verification and remaining scope

The macOS `fdny-scene-viewer` target compiled after this change. No new EXE
function was reviewed, and the dashboard's EXE function count is unchanged.
Build success confirms the host integration compiles; it does not confirm a
live Gas R interaction or retail parity.

- **Verified from retail:** command operand shape, timed-track reset, current
  size anchor, duration scaling, rounded target time, and interpolation path.
- **Implemented in the port:** shared `tell` target dispatch to FireRescue
  `setsize`, using the existing timed-size renderer path.
- **Provisional:** host clock substitution, frame cadence, timestamp wrap and
  negative-time behavior, and exact render/contact timing parity.
- **Open:** reproduce Gas R `button1` contact/selection and use in the original
  game, capture the six resulting updates, and compare visible/fire-contact
  results in both runtimes. Full mission coverage remains outstanding.
