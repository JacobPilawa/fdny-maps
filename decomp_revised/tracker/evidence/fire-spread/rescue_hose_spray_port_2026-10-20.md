# Rescue PlayerRescue HoseSpray shared-host port (2026-10-20)

## Evidence boundary

Retail evidence is pinned to `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The source traces in this workspace include
[`004640F0_fresh.c`](004640F0_fresh.c),
[`00463EF0_fresh.c`](00463EF0_fresh.c),
[`hose_contact_and_playerhose_fresh_2026-10-20.c`](hose_contact_and_playerhose_fresh_2026-10-20.c),
[`playerhose_pressure_stream_fresh_2026-10-20.c`](playerhose_pressure_stream_fresh_2026-10-20.c),
and the shared-component collision writer described in
[`hose_contact_frame_gap_2026-10-20.md`](hose_contact_frame_gap_2026-10-20.md).
These are static, current-hash traces. They do not establish scheduler cadence
or retail runtime parity.

## Confirmed slot, mode, and action transitions

`touchhose N` creates or refreshes HoseSpray in Player slot 4, builds the
attachment record list, and requests mode 2 through `00463370`; that command
does not start spraying. Player's slot-4 selection path `0044C660` sets the
selected slot to 4, activates its item callback, and requests mode 3. The
`004640F0` updater admits mode 1 or 3. On deactivation, HoseSpray callback
`00463400` changes mode 3 to 1, stops its action through vtable slot `+0x1C`,
and resets the stream component. Action-start `004632D0` independently sets
`+0xC58` and deadline `globalFrame + 100`; action-stop `00463300` clears
`+0xC58`. Host selection and action handling follow those boundaries.

Retail does not establish the keyboard or mouse mapping for selecting slot 4
or invoking these callbacks in the current evidence. The shared host exposes
slot 4 through key `4` and `select hose`; held left mouse starts/stops the
action. These are provisional host controls, not recovered retail bindings.

## Shared-host implementation

The shared rescue host now keeps slot-4 acquisition separate from selection,
transitions mode 2 to mode 3 on selection and mode 3 to 1 on deactivation, and
models the action byte as a separate held action. Its source-shaped stream
component is in
[`rescue_hose_spray_stream.h`](../../../port/spikes/training-native-prototype/src/rescue_hose_spray_stream.h).
It ports the current-pressure smoothing, eight-sample history, 0.75-second
ballistic trace, 15-section strip path, 10-to-40 contact padding, world
collision gate, previous-frame contact ordering, and the
`ApplyFireSuppressionContact(finalize=1)` threshold and state transition.
The extinguisher's message path continues to use its separate nonterminal
`finalize=0` route.

The shared scene host uses the player camera frame as the HoseSpray item frame
and mouse steering as its aim input. Both are provisional: retail constructs
the source point and direction through the HoseSpray attachment frame at
`+0x58`, whose composed parent/hand transform is not yet reproduced here. The
host draws the traced dual-curve stream with a temporary color; resource-3
texture, UVs, alpha/material state, effect cooldown and splash particles, and
the action sound instance are still open. It applies terminal dousing and
stops the existing fire loop sound immediately.

The shared macOS executable and rescue-all app rebuilt successfully. Standalone
and packaged executable SHA-256 both equal
`250854c4f2db167051b9d4726b907d9d1ca8eff026ddec5f64c8be64c814f355`.
The app plist validated; the bundle contains all eight retail rescue choices
and four PlayerHose maps. The build log enumerated all eight rescue choices;
the packaged `city_h`, `const_h`, `dock_h`, and `oil_rig_h` scene directories
were present. `git diff --check` passed. The existing Homebrew json-c macOS
27.0 versus deployment target 13.3 warning remains. No tests, interactive run,
or retail comparison was performed.

## Next work

Trace the HoseSpray attachment matrix, action binding, sound/effect resources,
and update scheduling. Then compare a controlled host contact/douse sequence
with a retail capture when available. Keep PlayerHose's WindowFire stage and
rescue-player HoseSpray's FireRescue suppression as independent mechanics.
