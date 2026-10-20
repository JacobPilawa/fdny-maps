# BModel liftable use-mode and held-pose trace (2026-10-20)

## Provenance

Retail executable: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The follow-up used read-only Ghidra 12.0.4 headless analysis against the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project with `-noanalysis`. Fresh
pseudocode for the liftable initializer, pose helpers, and vector comparison is
in [`bmodel_liftable_pose_fresh_2026-10-20.c`](bmodel_liftable_pose_fresh_2026-10-20.c).

The asset pass was reproduced with
[`audit_liftable_bmodels.py`](../../../tools/audit_liftable_bmodels.py) across
all 17 BSPs in `inputs/original/Data/Maps`. Its hash-pinned entity and MOTN/PATH
summary is [`liftable_bmodel_map_inventory_2026-10-20.json`](liftable_bmodel_map_inventory_2026-10-20.json).
The decoder is tied to Genesis.dll SHA-256
`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.

## Authored `G_Liftable` to BModel use mode

`InitializeBModelEntity` (`0043C670`) has the parsed BModel definition at
owner `+0xEC`. After binding its ScriptActor properties, it sets use mode
`+0x258` to 2 when a `use` event exists and the field is still zero. It then
checks parsed definition `+0x38` (`G_Liftable`); a nonzero value calls
`InitializeBModelLiftable` (`0043BE00`). That helper does not blindly enable
mode 1: it requires a first child model with motion, PATH 0, and two translation
keys whose times pass the source comparison against 0 and 1. It derives two
endpoint vectors from those keys and the child's rotational center, stores them
at BModel `+0x1F0..+0x1F8` and `+0x1FC..+0x204`, sets use mode `+0x258` to 1,
and clears endpoint selector `+0x220`. A successful liftable initialization
therefore overrides the earlier generic mode 2. A missing or malformed path
logs an error and does not take the successful mode-1 stores.

The use-action interface `0043DB40` asks the Player lift gate for an action
only in mode 1; other nonzero modes return action code 1000. The selected-use
handler `0043D2B0` calls Player carry transition `0044BC40` only for mode 1,
then dispatches the BModel's `use` event. `0044BC40` stores the BModel root at
Player `+0x3440`, sets BModel state `+0x25C` to 1, calls Player action status 3,
and sets Player state `+0x18C` to 7. These joins establish the static
initializer/selection/carry route, not live candidate ordering or input timing.

## Authored inventory across the preserved BSP set

The scan found 18 nonzero `G_Liftable` BModels in seven of 17 preserved BSPs.
Seventeen have a first child's PATH 0 with two translation keys at times
0.0 and 1.0 and therefore pass the static data checks visible in
`0043BE00`. Boat A entity 192 (`chair1`) has no child model and is not a
static mode-1 candidate. Runtime spawning, selection, and reaching the Player
throw branch remain unverified.

| Map | BModel entities and first children | Static mode-1 candidates |
| --- | --- | ---: |
| Bank | 432 `tr` → `throwbox`; 482 `tr1` → `throwbox1` | 2 |
| Bar R | 302 `liftablecrate1` → `obj_liftable_woodbox`; 303 `liftablecrate2` → `obj_liftable_woodbox1` | 2 |
| Boat A | 192 `chair1` → no child; 268 → `pushbox`; 269 → `throwbox`; 270 `hg` → `throwbox2` | 3 |
| MBase | 199 `underground_throw1` → `underground2_throw1`; 200 `underground_throw2` → `underground2_throw2` | 2 |
| Oil Rig B | 60 `oil barrel 01` → `oil barrel 01`; 61 `oil barrel 2` → `oil_barrel 02` | 2 |
| Oil Rig C | 233–236 → two rubble slabs and two girders | 4 |
| Training | 109 → `obj_liftable_metalbox`; 111 → `obj_liftable_metalbox1` | 2 |

`G_Liftable` is the use-mode gate; `E_Reach` and `F_HitPoints` are retained in
the inventory but do not decide the `0043BE00` mode-1 initialization shown
here. Map hashes, per-entity values, child model indices, and motion key times
are in the JSON artifact. The static asset scan does not qualify an object as
part of a particular rescue objective or prove it can be carried in a retail
run.

## Held-object pose and remaining adapter work

The Player update calls `0043BD90` at `0045391F`. The helper reads the first
child's rotational center and XForm translation through `0043BC80`, compares
the resulting center with BModel root `+0x224` through `0043B340`/`0043B120`,
and, when the comparison requests correction, passes current and target vectors
through `00482FC0`. That helper incorporates the shared frame delta at
`00502B04`; its scaling helper, the literal 0.1 passed at the call site, and
callback cadence are not yet mapped closely enough to claim exact smoothing
parity.

`0043BAA0` applies the resulting point across every BModel child: it reads each
child's rotational center and XForm, derives a translation from target point
minus rotational center, writes the transformed model, and refreshes aggregate
bounds through `0043B7A0`. The source of the carried Player's target point at
`+0x224`, including its relation to the hand/view pose, remains unresolved.
This is enough to specify the shared host adapter's model-transform boundary,
but not enough to claim faithful attachment placement or timing.

The Objects Thrown branch in the previous producer trace remains a held-BModel
transaction with a strict angle-under-10-degree gate. Static candidate coverage
is now mapped, but the shared mission scene still lacks the complete
selection/carry/pose/trajectory/drop/timer path. No counter was wired, no build
or gameplay test was run, and no retail parity claim is made. Next trace the
Player-side writer of the held target vector and the cadence around `0045391F`,
then promote the source-shaped sequence into the shared mission host with a
cross-platform BModel transform service.

## JUMP control bridge status

The Player action constructor registers `JUMP` to control ID `0x105`; the input
name table maps that ID to `MOUSE_RB`. The `0042D8C0` camera/input pass queries
right mouse at `0042DB06`, but discards the result. Its left-mouse query stores
its result at Player `+0x285`. The saved static references do not join the
right-mouse result to Player update `00453784`, state-5 setter `0044C0B0`, or
ScriptActor handler `00451870`. The literal `jump` ScriptActor branch at
`004518D4` directly calls producer `0044E990`; the separate state-5 branch at
`00451B92` has an unresolved command token. This control bridge remains open;
the host jump impulse and counter should not be described as retail-equivalent.
