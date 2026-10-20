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
does not select the item or start spraying. Player's slot-4 selection path
`0044C660` sets the
selected slot to 4, activates its item callback, and requests mode 3. The
`004640F0` updater admits mode 1 or 3. On deactivation, HoseSpray callback
`00463400` changes mode 3 to 1, stops its action through vtable slot `+0x1C`,
and resets the stream component. Action-start `004632D0` requests the named
2D `HoseSpray` sound, sets `+0xC58`, and sets effect deadline `+0x3B0` to
the current game clock plus 100 ms. Action-stop `00463300` stops that sound
instance and clears `+0xC58`. Host selection/action follows those boundaries;
its choice to loop the sample while held is provisional because original
playback repetition has not been measured.

## Action sound and contact-effect trace

The pinned constructor `004634F0` configures the item's sound instance at
`+0x228` with the named `HoseSpray` 2D resource. The start callback `004632D0`
requests playback through `004A11D0`, sets action byte `+0xC58`, and stores
`currentClock + 100` at `+0x3B0`. The shared timer trace at
[`0042bf70_frame_clock_delta_fresh.c`](../runtime/0042bf70_frame_clock_delta_fresh.c)
establishes that this manager clock is elapsed milliseconds, so the initial
effect delay is 100 game-clock milliseconds. Stop callback `00463300` stops
the owned sound instance through `004A1080` and clears `+0xC58`.

The RRF resource is `Data/Snd/#waterhose1.wav`, mono PCM, 44.1 kHz, 3.21 s,
SHA-256 `8ba143e988a081c8451c856f04f02a890d0f192f2f4ace3c76751716dc10e587`;
its `Data/ResDefs/sound.rrf` definition is covered by hash
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`. The
shared EVS sound exporter now includes the name in every scene package so the
sample is loaded whichever map opens first. Host action start/stop uses the
same owned sound key and category 2. Its repeat policy loops while held and is
provisional; no retail audio capture was made.

After suppression, `00463EF0` checks residual fire intensity `>= 0.01` and
whether the item deadline is due. It chooses an integer delay by converting a
random value in `[10,250]` toward zero through `004B9400`, adds that value to
the elapsed-millisecond clock, and stores the next deadline at item `+0x3B0`.
It then creates a child from the template list at `+0x3D0`, initializes the
child at the fire transform, gives it randomized velocity X/Z `[-50,50]` and
Y `[100,400]`, and sets its word at `+0xA4` to `0x20`. Constructor setup adds
resource-14 effect templates; `hose.rrf` maps ID 14 to `bsmoke.bmp` with
`sm_alpha.bmp`. This links the template setup to the resource definition, but
does not prove the spawned child's final render binding. The host has not yet
ported this cooldown or particle lifecycle.

The generic equipment setup helper `0045F660` also creates an item follower
under Player view follower `+0x34A4` and stores it at item `+0x98`. HoseSpray
setup `00463700` calls it. `UpdateHoseSpray` consumes the transform component
at item `+0x58` to derive nozzle point/direction, but current saved evidence
does not identify the writer/composition feeding that component. No evidence
links the `touchhose N` intrusive-list records at `+0xCB8` to this transform.
Keep the host's camera frame provisional.

Retail does not establish the keyboard or mouse mapping for selecting slot 4
or invoking these callbacks in the current evidence. The shared host exposes
slot 4 through key `4` and `select hose`; held left mouse starts/stops the
action. These are provisional host controls, not recovered retail bindings.

## Shared-host implementation

The shared rescue host now keeps slot-4 acquisition separate from selection,
transitions mode 2 to mode 3 on selection and mode 3 to 1 on deactivation, and
models the action byte and owned HoseSpray sound as separate held-action state.
`export_evs_bundle.py` includes the `HoseSpray` entry in every map sound bundle
so the sample is available when the first scene initializes shared audio. The
exact source is `Data/Snd/#waterhose1.wav`, SHA-256
`8ba143e988a081c8451c856f04f02a890d0f192f2f4ace3c76751716dc10e587`; the
`sound.rrf` definition hash is
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`.
Its 3.21-second WAV is mono PCM. The host loops this sound until release or
deactivation, a practical held-action mapping whose retail repeat behavior
remains unverified. The host does not yet execute the source's initial 100-ms
effect gate or randomized contact-effect deadline. Its source-shaped stream
component is in
[`rescue_hose_spray_stream.h`](../../../port/spikes/training-native-prototype/src/rescue_hose_spray_stream.h).
It ports the current-pressure smoothing, eight-sample history, 0.75-second
ballistic trace, 15-section strip path, 10-to-40 contact padding, world
collision gate, previous-frame contact ordering, and the
`ApplyFireSuppressionContact(finalize=1)` threshold and state transition.
The extinguisher's message path continues to use its separate nonterminal
`finalize=0` route.

The shared scene host uses the player camera frame as the HoseSpray item frame
and mouse steering as its aim input. Both are provisional. Setup `00463700`
uses shared equipment helper `0045F660`, which creates an item follower under
Player view follower `+0x34A4`; that establishes the follower chain but not
the writer or composition that supplies the transform consumed from item
`+0x58`. The `touchhose N` list records at `+0xCB8` are not yet tied to that
transform. The host draws the traced dual-curve stream with a temporary color;
resource-3
texture, UVs, alpha/material state, effect cooldown and splash particles, and
the exact item frame remain open. It applies terminal dousing and stops the
existing fire loop sound immediately.

The shared macOS executable and rescue-all app rebuilt successfully. Standalone
and packaged executable SHA-256 both equal
`aa82c474a29d0a720977130d1a0c16c5423a4c706fea7792e4da3f102f0a1a74`.
The app plist validated; the bundle contains all eight retail rescue choices
and four PlayerHose maps. The build log enumerated all eight rescue choices;
the packaged `city_h`, `const_h`, `dock_h`, and `oil_rig_h` scene directories
were present. The `HoseSpray` sample row and source SHA were checked in all
eight rescue scene sound manifests and each packaged WAV matched the local
retail-source SHA. The plist and `git diff --check` passed. The existing
Homebrew json-c macOS 27.0 versus deployment target 13.3 warning remains. No
tests, interactive run, or retail comparison was performed.

## Next work

Trace the HoseSpray attachment matrix, action binding, sound/effect resources,
and effect cooldown/particle lifecycle; the `HoseSpray` action sound key and
start/stop callbacks are now ported with repeat policy provisional. Then compare a controlled host contact/douse sequence
with a retail capture when available. Keep PlayerHose's WindowFire stage and
rescue-player HoseSpray's FireRescue suppression as independent mechanics.
