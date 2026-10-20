# PlayerHose spline builder and camera rail trace (2026-10-20)

Retail `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Fresh Ghidra 12.0.4 excerpts from the hash-pinned project are preserved in
[`player_hose_curve_pipeline_fresh_2026-10-20.c`](player_hose_curve_pipeline_fresh_2026-10-20.c)
and [`player_hose_camera_pose_runtime_fresh_2026-10-20.c`](player_hose_camera_pose_runtime_fresh_2026-10-20.c).
They are analysis evidence, not reconstructed source or a runtime-parity claim.

## UserData and authored path binding

`00468050` calls generic validator `0049D160` with a 20-byte expected UserData
size and stores the copied UserData pointer at PlayerHose `+0x194`. It then
copies UserData floats at `+4/+8/+0xC` into the entity origin at `+0x4C/+0x50/+0x54`.
`0049D160` copies the pointer from the create input and checks its byte size
against the registered class size. The remaining pointer at UserData `+0x10`
is read by the spline builder as the first route node. Since the maps' sole
RCS_PlayerHose pointer property is `FirstPathPoint` and it resolves to an
RCS_HosePath, this is a strong layout/data correspondence; the compiled
property descriptor's byte offset has not been recovered directly.

`004661A0` follows each node's `+0x10` link, reads a related node at `+0x14`,
and tests a marker at `+0x18`. Those offsets strongly match the authored
`RCS_HosePath` fields `A_NextPoint`, `B_LookAtPoint`, and
`C_IsAHotSpot`, respectively, based on the retail access pattern and ordered
entity properties. The property descriptors are not independently decoded.
The builder accepts up to 32 nodes and closes a null-terminated A chain back
to its first point. Each node contributes its authored X/Z position; the third
control coordinate is zero. Its B link supplies a world XYZ LookAt point. A
missing LookAt leaves the previous value in use after logging.

The current hash-matched BSP exports and A-link traversal are:

| Map | BSP SHA-256 | Exported HosePath nodes | Reachable A chain | FirstPathPoint |
| --- | --- | ---: | ---: | --- |
| `city_h` | `9bc788dcaceb477fea52efd49cfbb2fd8f86824907d73c819280710b04fcb27a` | 19 | 10, null-terminated | `RCS_HosePath19` |
| `const_h` | `f4dc99b6dd3b4465c1ba68cc29c09c075368f45f744a32a470b0663251862984` | 34 | 19, returns to first | `RCS_HosePath25` |
| `dock_h` | `17427ebd2964f31b47317e53c0881af41a05e78d57a7c95ef9783d10697a6d7c` | 17 | 13, null-terminated | `RCS_HosePath49` |
| `oil_rig_h` | `6681a6ecb688f69b37bbbc25c6c0dea3f030f33880e7210c7cbed48a108bd4d0` | 13 | 7, null-terminated | `RCS_HosePath13` |

`RCS_CamHose` is looked up separately as a runtime helper component during
PlayerHose initialization. Its absence from all four BSP entity tables does
not block binding the rail: the route chain is strongly mapped to the
authored HosePath graph.

## Curve samples and timing

`00490510` allocates 32-byte control records and 16-byte samples, with ten
subdivisions per segment and a closed-path flag. `00490D00` stores the
control-point values, node pointer and LookAt XYZ. `004905A0` samples each
segment with centered half-difference tangents and cubic Hermite
(Catmull-Rom-style) interpolation, then accumulates the sampled polyline
distance into time keys and the total path duration. Its actual scalar
operations and loop boundaries are in the fresh excerpt.

`00490AB0` accepts normalized path input, scales by total duration, finds two
adjacent samples and linearly interpolates XYZ. `00490BB0` uses the same scaled
time to find the control-record interval and linearly interpolates LookAt XYZ;
it returns the selected control-record address when that record differs from
the cached address, then updates the cache on every successful sample. The
record is therefore a change notification: the first selected record is
reported because `00490510` initializes the cache to null, and subsequent
records are reported when the sample crosses their time key. `004677D0`
resolves the notified record's `+0x10` node pointer and accepts the node when
it matches the expected next-hotspot pointer or retains its first-hotspot
marker. On acceptance it clears `M_IsFirstHotSpot`, stores `D_NextHotSpot` as
the next expected anchor, enters state 5, and stores mode-scaled `M_SpotTime`
as a budget at PlayerHose `+0x1A84`. `00465FB0` later copies that budget to the
live countdown at `+0x1A7C` when entering state 4. The precise scheduler-frame
gap between those calls is unresolved.

The shared host now tracks each route control node's sampled arc-length key,
emits an event only when its selected record changes, starts the first marked
hotspot on its anchor, and starts each D-linked hotspot only when its A-rail
anchor is selected. If target cycles finish first, the host waits at that
boundary. This follows the source event shape; host frame cadence and rail
speed remain provisional. The updater uses
constant `+500` in normalized progress advance, but retail frame-delta
calibration is not established. Treating this as 500 host world units per
second is a port assumption, not verified timing parity.

## Camera placement

The PlayerHose constructor trace establishes entity origin storage, while
`004677D0` samples the path and updates the rail position. It probes vertically
from X/Z at Y `+50` down to Y `-2000` through `0048FF30` using world trace
arguments `1,2`, then places the PlayerHose origin at sampled X/Z and hit Y
`+32`. `0048FF30` returns zero on a miss. The sampled LookAt target is
subtracted from the rail position to form the view direction. `00465EF0`
computes the stream source point from the entity frame as
`origin + 10*basis3 + 15*basis2 - 10*basis1`; `00466780` applies its steering
rotations to a temporary aim frame, so aim direction and source point use
separate frames. No live retail capture was available for camera cadence,
floor-miss behavior, mouse-to-nozzle aim interaction, or orientation rounding.

The shared host now builds the same authored A-link rail, samples ten
subdivisions per segment with the corresponding cubic curve, queries floor
height, and constructs a camera frame toward the interpolated B target. It
advances at 500 units per host second as an explicitly provisional timing
assumption. It offsets the stream source along the rail frame using the
`00465EF0` expression and applies host mouse steering around the rail-derived
aim; mouse scaling, accumulator behavior, and rotation order remain
provisional. The build is a code compilation check, not a controlled retail
comparison or parity result.
