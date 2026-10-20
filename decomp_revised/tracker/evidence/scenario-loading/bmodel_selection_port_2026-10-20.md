# BModel interaction selection and carry port slice (2026-10-20)

## Provenance and scope

Retail executable `inputs/original/binaries/FDNYFirefighter.exe` has SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The
selection recheck used read-only Ghidra 12.0.4 against
`.local/mission-lifecycle-ghidra/FDNYPort` with `-noanalysis`. Fresh analysis
output for `00459350` and `00459630` is in
[`bmodel_selection_and_dispatch_fresh_2026-10-20.c`](bmodel_selection_and_dispatch_fresh_2026-10-20.c).
The file is Ghidra output, not recovered source.

## Retail selection path

Player's BModel use-selection object owns a pointer array at `+0x188`, with
count/capacity state at `+0x18C`. Its add routine `00459000` fills the first
null slot before appending, and removal `00459090` leaves a hole. BModel
contact update `0043DC60` registers its `+0xA0` use interface when use mode
`+0x258` is nonzero and removes it when disabled. This maps membership behavior;
the actual owner/contact-manager callback cadence and resulting live candidate
order are still unknown.

`00459350` clears the previous selected index/action/query pointer, copies the
Player origin at `+0x34CC`, and reads a basis vector through the embedded frame
at `+0x34A4`. It scans registered slots in ascending order, skipping holes. A
candidate's slot-0 callback returns an action and may supply a retained query
interface. A nonzero action with no query interface is accepted immediately.
Otherwise the query object's slot `+0x14` receives the origin and basis vector,
returns a point and score, and the candidate is eligible only within 200 units
of the origin. Equal scores replace the prior choice, so the later registered
slot wins a tie. The selected index, action, and retained query interface are
stored on the selection object.

Mode 1 action 1004 is subject to `0044C520`: Hose Spray slot 4 must be absent
or not in mode 3, Player byte `+0x12C` must be nonzero, no BModel may already
be held at Player `+0x3440`, and the candidate BModel state `+0x25C` must be
zero. Other nonzero use modes return 1000; its meaning remains unknown. The
collision callback `0049E0A0` sets Player `+0x12C` for upward-facing contact;
integrator `0049DBC0` copies it to `+0x12D` and clears `+0x12C`. The
constructor/reset route also seeds both bytes to 1, so its live value at a
selection callback is transient and callback-order-sensitive.

Within Player movement update `00452570`, the cached action path at `004533C2`
precedes the selection refresh call at `0045386B`. The refresh routine
`00459630` calls `00459350`; when an entry remains selected it calls
`0047DA90` with the action and updates control/UI state. Static call order is
mapped, while the host input edge, update cadence, and live action callback
equivalence are not.

For mode 1, `0043D2B0` reaches `0044BC40`, which stores the BModel root in
Player `+0x3440`, sets BModel state `+0x25C` to 1, calls the Player action
status service with 3, and sets Player state `+0x18C` to 7. The source
arithmetic for the later held target and center correction is documented in
[`mission_stat_bmodel_liftable_pose_2026-10-20.md`](mission_stat_bmodel_liftable_pose_2026-10-20.md).

## Shared host integration

The shared scene host now uses
[`bmodel_interaction_selection.h`](../../../port/spikes/training-native-prototype/src/bmodel_interaction_selection.h)
for first-hole registry behavior, ordered query selection, reach filtering,
immediate query-less actions, and later-slot tie replacement. The axis-box
helper normalizes the direction before its slab query and uses that same unit
vector to reconstruct the hit point. In `scene_viewer.cpp`, candidates are
currently registered from per-frame host overlap tests. A key edge uses the
cached selected entity/action before the host refreshes candidates later in its
loop; mode 1 initializes the carry state and the `use` ScriptActor event is
dispatched. The mode-1 action gate now receives the host collision latch only
when `Player::step()` ran during this frame, avoiding stale host state on frames
when movement is skipped. This is an adapter rather than an exact `+0x12C`
port: host contact resolution and retail collision/integrator callback order
have not been shown equivalent.

The carry target uses the source's 90/-60 basis offsets and exponential
0.1-second center blend, then updates child poses through the collision scene.
The camera frame currently stands in for the retail Player frame. This is a
provisional adapter: a separate audit leaves the numeric relation between the
Player follower at `+0x34A4` and the rendered Genesis camera unresolved.

The shared collision adapter now retains the first authored child model index
separately from children that have usable PATH records. After each BModel state
update, any nonzero `useObjectState` excludes that first child from the host's
swept and point/ray queries. This follows retail `0043D330` setting bit `0x10`
at child model `+0xCC` and Genesis collision scans skipping that bit. The
adapter keeps the state-derived collision gate separate from
`PreviewWorldModelFlags`; the BModel bit's render effect has not been
established. Host frame scheduling is still provisional, so exact same-frame
transition timing is not claimed.

## Verification and limits

The macOS CMake `fdny-scene-viewer` target compiled successfully after the
selection, action, carry, current-frame collision-latch gate, and BModel
first-child collision-flag changes. The executable SHA-256 is
`562f20b0396099f5a03ad919105865ad0ec092370a2d5cb96a2a5636444661f3`. The
linker warned that the installed `json-c` dylib targets macOS 27.0 while this
target links for 13.3. No tests, interactive session, gameplay capture, or
retail parity check was performed.

The host's transformed child-box union has not been proven equivalent to the
retail BModel aggregate axis box. Candidate registration timing/order,
Player byte `+0x12C` adapter, the camera-to-Player frame relation and ray sign, the
meaning of action 1000, Player action-status service effects, live key/polling
cadence, and event timing remain unresolved. Throw trajectory, drop handling,
timed-list cleanup, and retail carry/drop animation integration are not
implemented. The held-object source branch checks its angle whenever the held
pointer is non-null, with no separate throw input edge, but the two vector
meanings and Player callback cadence are still open. Treat current behavior as
a source-shaped prototype, not 1:1 selection or carry parity.
The trajectory substep is traced inside `0043DC60`; shared update integration
and the outer callback cadence remain unresolved. See the
[throw update route and authored-data audit](bmodel_throw_update_route_2026-10-20.md).
