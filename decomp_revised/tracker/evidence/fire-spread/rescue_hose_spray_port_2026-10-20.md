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

## Generic equipment frame chain

The rescue HoseSpray setup `00463700` calls shared helper `0045F660`, which
configures the item's embedded transform component at item `+0x58` in mode 2
and stores its follower pointer at item `+0x98`. The component has an identity
local transform and is parented to Player view follower `+0x34A4`; setup
`00497E90` stores its resolved ActorInfo pointer at component `+0x94`.
During Player view update, `0044F340` builds the view frame at `+0x3460`,
refreshes follower `+0x34A4` through `00483720`, then invokes inventory-item
post-view callbacks. Component setup `00497E90` stores mode and parent/source
fields and resolves ActorInfo through `00494500`; updater `00497F70` composes
the hierarchy and calls `00492110`, which writes ActorInfo's matrix and
dispatches `00492450` to apply it through `geActor_SetPose`.

This closes the generic writer/consumer route but not the HoseSpray's numeric
world matrix. The source-to-parent values, any HoseSpray-specific local pose
mutation, and the numeric relation between Player follower `+0x34A4` and the
rendered Genesis camera remain unresolved. The port's camera-derived item
frame is still an approximation. The `touchhose N` list at `+0xCB8` is not
linked to this transform by current evidence. See
[`equipment_actor_render_path_2026-10-13.md`](../scenario-loading/equipment_actor_render_path_2026-10-13.md),
[`equipment_view_frame_trace_2026-10-17.md`](../scenario-loading/equipment_view_frame_trace_2026-10-17.md),
and
[`equipment_action_pose_followup_2026-10-17.md`](../scenario-loading/equipment_action_pose_followup_2026-10-17.md).

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

After terminal suppression, `00463EF0` checks residual fire intensity
`>= 0.01` and compares the unsigned elapsed-millisecond clock directly with
item deadline `+0x3B0`. When due, it draws a random value in `[10,250]`, adds
the current clock in x87, and converts the sum toward zero through `004B9400`
before storing the next deadline. The saved instruction listing
[`00463EF0_hose_contact.asm`](00463EF0_hose_contact.asm) preserves this clock
addition, which the C-like export omits. The helper then creates a child from
the template list at `+0x3D0`, initializes it at the FireRescue transform,
zeros acceleration, draws velocity values in order X/Z `[-50,50]` around Y
`[100,400]`, and passes them to the vector writer as `{last,height,first}`;
child word `+0xA4` is set to `0x20`.

`004634F0` configures resource-14 effect templates, and `hose.rrf` maps resource
14 to `bsmoke.bmp` with `sm_alpha.bmp`. This does not establish the runtime
resource manager selected for the spawned child or the final draw binding.
The host now ports the residual and due-time gates, shared-RNG delay/deadline,
and velocity spawn request. It assumes allocation succeeds, matching the
unbounded pool in normal operation. Child `+0xA4` flags are not represented by
the host renderer. A fresh trace of the particle path shows that HoseSpray
starts with track key 0 selected and its size getter returns that key's scalar
`3.0`; the later key contains `5.0`, but is not active at initialization. The
host now passes the current `3.0` scalar through its per-particle preview-size
field. Mapping that Genesis polygon scalar to the OpenGL quad's side length is
provisional. The preview reuses the extinguisher renderer's linear motion,
constant alpha 191, and resource-14 texture pair. The HoseSpray path now
advances its two-key track at the source-derived 2,000 ms key deadline: size
changes from 3.0 to 5.0, then the zero-duration terminal key deactivates on the
next host update through flag `0x8`. Extinguisher particle lifetime remains on
its existing path. Child render flags, allocation failure, final texture
binding, source alpha, and exact update/draw cadence remain provisional or
unresolved. The interval before terminal deactivation follows the host update
loop; retail pool scheduling is not established.
See
[`rescue_hose_spray_particle_trace_2026-10-20.md`](rescue_hose_spray_particle_trace_2026-10-20.md).

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
remains unverified. The host starts an effect deadline at action start +100 ms.
After terminal suppression it applies the residual-intensity and direct
unsigned deadline gates, adds a shared-RNG `[10,250]` delay to the millisecond
clock and truncates it toward zero, then draws the velocity in source axis
order. It assumes the child pool allocation succeeds and hands the request to
the shared preview particle path. HoseSpray's key 0 to key 1 transition and
flag-`0x8` deactivation are ported; the child render flags, final resource
binding, source alpha, and pool scheduling remain unresolved.
Its source-shaped stream
component is in
[`rescue_hose_spray_stream.h`](../../../port/spikes/training-native-prototype/src/rescue_hose_spray_stream.h).
It ports the current-pressure smoothing, eight-sample history, 0.75-second
ballistic trace, 15-section strip path, 10-to-40 contact padding, world
collision gate, previous-frame contact ordering, and the
`ApplyFireSuppressionContact(finalize=1)` threshold and state transition.
The extinguisher's message path continues to use its separate nonterminal
`finalize=0` route.

The shared scene host uses the player camera frame as the HoseSpray item frame
and mouse steering as its aim input. Both are provisional. The generic
equipment component writer is now mapped through ActorInfo and
`geActor_SetPose`, but the final item matrix and numeric join from Player view
follower `+0x34A4` to the rendered Genesis camera remain open. The `touchhose N`
list records at `+0xCB8` are not tied to that transform. The host draws the
traced dual-curve stream with a temporary color;
resource-3 texture, UVs, alpha/material state, exact HoseSpray particle timing
under retail pool cadence, final resource binding, and the exact item frame
remain open. It applies
terminal dousing and stops the existing fire loop sound immediately.

The shared macOS executable and rescue-all app rebuilt successfully. Standalone
and packaged executable SHA-256 both equal
`b8bdedcba9de98a0bd3a217bee40ecb27465c832656dfd58429f140fb530bba8`.
The app plist validated; the bundle contains all eight retail rescue choices
and four PlayerHose maps. The build log enumerated all eight rescue choices;
the packaged `city_h`, `const_h`, `dock_h`, and `oil_rig_h` scene directories
were present. The `HoseSpray` sample row and source SHA were checked in all
eight rescue scene sound manifests and each packaged WAV matched the local
retail-source SHA. The plist and `git diff --check` passed. The existing
Homebrew json-c macOS 27.0 versus deployment target 13.3 warning remains. No
tests, interactive run, or retail comparison was performed.

After passing the source key-0 size scalar into the shared renderer, the
macOS rescue-all application rebuilt successfully. Standalone and packaged
executables match at SHA-256
`19a4f8e02b096e24e46a4dbce87f006ae44905fdf46a9c2fca56c9f7f520cc8c`. The
plist validated, the packaged rescue catalog contains eight entries, and all
four PlayerHose maps are present. The existing Homebrew json-c macOS 27.0
versus deployment target 13.3 linker warning remains. No tests, interactive
run, or retail comparison was performed.

## HoseSpray particle track advancement checkpoint (2026-10-20)

The shared renderer now carries a HoseSpray-only copy of the source track
state. Its main-loop update applies the 2,000 ms key-0 deadline, changes the
preview scalar from 3.0 to 5.0 once, then deactivates on the following update
through the configured flag-`0x8` non-looping end path. Extinguisher particles
retain their existing preview lifetime. The 5.0-to-quad mapping and the host
update interval remain provisional; the retail pool dispatch cadence is
unknown. The source details are recorded in
[`rescue_hose_spray_particle_trace_2026-10-20.md`](rescue_hose_spray_particle_trace_2026-10-20.md).

`tools/build_scene_viewer_app.sh rescue-all` rebuilt the macOS target and
rescue-all app. The standalone and packaged executables match at SHA-256
`9711510c53ea971da144df5eacea6d9d7e09af127b92bb51ad32ab56b515d43f`. The app
plist validated, all eight retail rescue choices were emitted, and all four
PlayerHose maps were packaged. `git diff --check` passed. No tests, interactive
run, or retail parity comparison was performed.

## Next work

Trace the HoseSpray attachment matrix and action binding, then resolve the
child allocation/render-resource chain, `+0xA4` flags, source alpha, and pool
scheduling. The cooldown, spawn request, key advancement, flag-`0x8`
deactivation, and initial size scalar are ported; the renderer's size-to-world
mapping and final resource binding remain provisional. Compare a controlled
host contact/douse sequence with a retail capture when available. Keep
PlayerHose's WindowFire stage and rescue-player HoseSpray's FireRescue
suppression as independent mechanics.
