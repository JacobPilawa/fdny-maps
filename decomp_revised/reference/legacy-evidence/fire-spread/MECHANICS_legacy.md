# FDNY fire mechanics — technical reference

## This is how a fire works

Most fires are put in specific places by the level designer. Each fire has a size and a strength. Spraying it reduces its strength, which makes the flames fade and reduces how much it can burn you. **If it has not been fully extinguished, its strength can recover after you stop spraying.** That is the main behavior in the code that explains fires coming back.

- **Where it starts:** most fires already have a starting location in the map. Events such as an explosion can make particular fires larger or start a spreading sequence.
- **How it grows:** a fire can regain strength on its own between spray hits. Its physical size can also change when the level tells it to grow or shrink. Regaining strength and becoming physically larger are separate things.
- **How it spreads:** the clearest example is the gas-station fire. The level tells it to travel along a particular route, leaving new fires behind. The game can also create small fires at random nearby positions. We have not found enough evidence to say that any nearby wooden object, furniture or other material can automatically catch fire.
- **How you put it out:** both the extinguisher and the carried hose reduce fire strength. In the code checked so far, a sufficiently successful hose hit can also mark a fire as fully extinguished. The extinguisher hits we traced reduce strength without making that final change, leaving room for recovery. There is no single guaranteed “spray for this many seconds” rule: the fire's settings and where the spray lands affect the result.
- **How it hurts you:** ordinary fire burns you when you are close enough. Stronger fire and closer contact increase the burn amount. A blocked line between you and the fire reduces this damage in the checked code, but does not necessarily eliminate it. Burns build up until you pass out; when the game's burn indicators are clear, that buildup decreases. Running out of air is a separate cause of passing out.
- **Other dangerous flames:** some flame jets and explosions use a different damage system from ordinary fires. Their strength can be set by the level, so they can burn you much faster. They also have their own rules about when they are active and close enough to affect you.
- **The truck-hose stages work differently:** their window fires are managed as groups of targets, with a separate sequence for selecting and putting them out. The ordinary-fire rules above do not fully describe those stages.

These points are based on the game files and code we have checked. We still need to work out some timing and spray-contact details, and test them against the running game. We have not established why you sometimes seem to die instantly, or finished explaining every fire-related behavior.

---

Updated October 10, 2026; native follow-up included. Companion: `SUMMARY.md`. Pseudocode is explanatory: it omits allocation/error handling and does not claim exact FPU or message-delivery equivalence. Addresses refer to the included `binary/FDNYFirefighter.exe` in `evidence.zip`, identified by the archive manifest.

## Evidence and confidence

**Verified** means checked against fresh decompilation, and assembly where Ghidra lost arguments or x87 arithmetic. **Derived** means applying that code to authored fields. **Open** means a caller, field mapping, or retail observation remains unresolved.

Archive paths below are relative to `evidence.zip`. Fresh exports use provisional `FUN_…` names. Older human names can be misleading: notably `0x00449340` was called `FireRescueEnt::Spawn`, but it advances spread; the actual spawn routine inspected here is `0x00448D60`. `0x00463EF0` was described as an ember-only routine in the old database, but its exported body directly tests fires and calls suppression.

Two object origins matter: FireRescue's primary base and its script/Think subobject at primary +0x24. Thus Think's `+0x104` is primary `+0x128` (intensity), and Think's mode `+0x4C` is primary `+0x70`. Do not merge offsets from different origins without adjustment.

| Routine | Address | Evidence |
|---|---|---|
| Scripted size change | 0x00447920 | `native/00447920_FUN_00447920.c`, `assembly/fire_assembly.txt` |
| Randomized nearby child-flame generation | 0x00447A90 | `native/00447a90_FUN_00447a90.c`, same assembly |
| Proximity update / nearby-child call | 0x00448320 | `native/00448320_FUN_00448320.c`, `assembly/damage_region.txt` |
| Fire exposure callback | 0x004483A0 | `native/004483a0_FUN_004483a0.c`, `assembly/damage_assembly.txt` |
| Floor alignment | 0x00448630 | `native/00448630_FUN_00448630.c` |
| Suppression | 0x00448970 | `native/00448970_FUN_00448970.c`, `assembly/spray_assembly.txt` |
| Start path movement | 0x00448A90 | `native/00448a90_FUN_00448a90.c` |
| Spawn / automatic spray time | 0x00448D60 | `native/00448d60_FUN_00448d60.c`, `assembly/fire_assembly.txt` |
| Advance/deposit spreading fire | 0x00449340 | `native/00449340_FUN_00449340.c`, same assembly |
| Spray message handler | 0x00449A40 | `native/00449a40_FUN_00449a40.c`, `assembly/spray_assembly.txt` |
| Fire script verbs | 0x00449A80 | `native/00449a80_FUN_00449a80.c` |
| Fire Think / recovery | 0x00449FE0 | `native/00449fe0_FUN_00449fe0.c`, `assembly/fire_assembly.txt` |
| Ellipsoidal influence helper | 0x0044A7A0 | `native/0044a7a0_FUN_0044a7a0.c` |
| Size interpolation and key insertion | 0x0044AE80 / 0x0044B620 | corresponding native exports |
| Air update / burn damage / player Think | 0x00451560 / 0x00451640 / 0x00452570 | corresponding native exports |
| Player damage-message decoder | 0x00453A70 | `native/00453a70_FUN_00453a70.c` |
| Extinguisher sampling / rescue hose contact | 0x00461F80 / 0x00463EF0 | corresponding exports, `assembly/spray_assembly.txt` |
| Spray contact-message builder | 0x00481A60 | corresponding export, same assembly |

Immutable values decoded directly from PE sections are saved in `derived/constants.json`. Sources, file hashes and sizes are in `manifest.json`. Existing notes and conversations are under `prior/`; they are historical evidence, not automatically accepted conclusions.

## 1. Authored sources and spawn

**Verified/derived.** Relevant BSP fields:

| Field | Role / limitation |
|---|---|
| `A_Name` | Actor name used by `tell`; distinct from editor `%name%`. |
| `ZZ_location` | Authored origin; floor alignment can alter runtime presentation. |
| `F_FireType` | All placed rescue fires use 0. Alternative kinds are not established by these maps. |
| `H_Radius`, `H_Height` | Base horizontal/vertical dimensions. The “radius” label must not be assumed to equal the final exposure radius. |
| `K_SprayTime` | Explicit denominator, or automatic sizing when near zero. |
| `L_FloorMe` | Downward world collision/floor-frame setup. |
| `M_Smoking` | Enables a separate smoke component. |
| `N_IsAlive` | All 430 authored sources are 1; runtime spawn also writes spawn-def +0x24 to 1. Exact disabled-definition semantics not independently exercised. |
| `P_NumKeys`, `P_KeyNSize`, `P_KeyNTimeOffset` | Up to four authored size/time keys; all placed sources have count zero. Runtime children and scripts can populate them. |
| `T_UseHeading`, `T_Heading` | Optional orientation override; important when interpreting local volumes. |

```text
Spawn(fire, definition, now):
    initialize rendering/smoke and optional named script actor / loop sound
    intensity = 1
    recentlySprayed = false
    currentPathPoint = none
    if no authored size keys:
        insert size key at now:
            (0.75 * baseHorizontal, 0.25 * baseHeight, 0.75 * baseHorizontal)
    else:
        insert authored keys at now + authored millisecond offsets
        recover maximum base dimensions from those keys
    install position/orientation; optionally FloorMe()
    if -1e-28 < sprayTime < 1e-28:
        sprayTime = 1 + 3 * clamp((baseHorizontal - 24) / (288 - 24), 0, 1)
```

`0x00447770` is the clamped inverse-lerp helper; assembly at `0x004492D3–0x00449312` verifies the near-zero test, 24/288 endpoints, ×3 and +1. The catalog's decimal values are explanatory double-precision calculations, not float32-exact retail stores.

The initial `(0.75,0.25,0.75)` size factors and later interpolation mean an overlay using only `H_Radius`/`H_Height` would be a base-dimension view, not necessarily the native active volume.

## 2. Suppression: intensity loss versus permanent dousing

**Verified.** Primary offsets: mode +0x70; definition pointer +0x78; intensity +0x128; recently-sprayed byte +0x104.

```text
Spray(fire, contactPosition, influence, finalize):
    if mode == 5: return
    strength = 1 - (1 - influence)^2
    intensity -= frameDt / effectiveSprayTime * strength
    if intensity < 0: intensity = 0
    if finalize and intensity < 0.1:
        intensity = 0
        mode = 5
        increment statistic "Fires Doused"
        stop loop sound if present
        clear recentlySprayed
    recentlySprayed = true
```

The strict 0.1 comparison uses a double constant. Float storage/rounding matters near the boundary. No upper clamp appears in this routine: the native caller's influence must not be silently clamped into [0,1]. Algebraically, strength is maximal at influence 1, returns to zero at 2, and is negative above 2. Whether extreme influences are achievable in an ordinary contact remains to be tested.

**Callers verified in assembly:**

- `0x00461F80`: extinguisher samples fire influence with 60-unit padding and submits **twice the influence** to `0x00481A60`.
- `0x00481A60`: stores position + influence in a 16-byte payload (up to 32 records) and sends message **30000 / 0x7530**.
- `0x00449A40`: decodes that message and calls Spray with **finalize=false**, explicitly `PUSH 0` at `0x00449A5A`.
- `0x00463EF0`: rescue hose tests its current contact position/padding and calls Spray directly with **finalize=true**, explicitly `PUSH 1` at `0x00463FA3`.

The two direct x86 call sites found for Spray are `0x00449A74` and `0x00463FC3`. This pass has not exhaustively excluded indirect callers. The older phrase “terminal particle” is replaced here with the observed boolean role, `finalize`.

Do not infer douse duration as simply K_SprayTime. Strength, multiple contacts, doubled extinguisher influence, contact delivery, regrowth and the finalization flag all affect the outcome.

**New derived consequence of the alpha writer:** once alpha has synchronized and is ≤0.999, the doubled extinguisher influence is ≤1.998 for nonnegative padding and points inside the influence volume. At the mathematical influence maximum 0.999, its suppression factor is `1-(1-1.998)^2 = 0.003996`; at undoubled influence 0.5, the doubled input is 1 and suppression factor is 1. Thus this native response curve is nonmonotonic for the extinguisher. It is not valid to replace it with “closer/stronger contact always extinguishes faster.” Whether an actual aim/sample configuration reaches either example must be established from tool sampling and world contact, so these are conditional arithmetic rather than recommended aiming instructions. The rescue-hose call does not use this doubled message input.

## 3. Recovery and presentation

**Verified branch logic; full wall-clock rate still requires a scheduler replay.**

```text
FireThink(scriptSubobject):
    if mode == 5: return
    update light colour from intensity
    if not recentlySprayed and mode != 4:
        decrement tickDivider
        if tickDivider != 0:
            update size-key interpolation / render
            return
    tickDivider = 3
    update sound and other bookkeeping
    if mode == 4: AdvanceSpread(primaryBase)
    update size-key interpolation / render
    presentationScale = 1 if intensity approximately 1
                        else trunc(intensity * 10) * 0.1
    if not recentlySprayed:
        scaledDt = frameDt if mode == 4 else frameDt * 4
        intensity = min(1, intensity + scaledDt * 0.1)
    alphaInput = 1 if intensity >= 0.5 else 2 * intensity
    alpha = min(0.999, alphaInput)
    write alpha through 0x0044A610 → 0x0047F1F0 to particle +0xD0; set +0xD4 override flag
    recentlySprayed = false
    exposureRecordCount = 0
```

The Think code does **not** use a random call to compute that presentationScale: Ghidra omitted the x87 multiply before `__ftol`. Stationary unsprayed fires typically execute the recovery branch every third Think, so saying “0.4 per second” without the divider is misleading. Spreading mode bypasses the divider. Keep frame order and callback delivery explicit in a future simulation. Spawn also assigns the divider from a global cycle of 0, 1, 2, 3. A byte initialized to 0 can decrement through wraparound before reaching 0 again unless spraying or spreading bypasses the branch; check that startup case in retail rather than assuming every initial update waits at most three frames.

## 4. Authored path spread

**Verified core arithmetic; child lifecycle not fully reconstructed.** Script verbs: `moveto`, `stopmove`, `setsize`.

```text
MoveTo(pointName, travelArg, holdSeconds, fadeSeconds):
    resolve named PathPoint; reject immediate recursive target
    delta = targetPosition - firePosition
    if travelArg > 0: speed = length(delta) / travelArg
    else: speed = -travelArg
    direction = normalize(delta)
    segmentOrigin = firePosition
    progress = lastDepositProgress = 0
    remember holdSeconds, fadeSeconds
    clear active size-key sequence
    mode = 4

AdvanceSpread():
    progress += dt * speed              # routine contains a ×4 branch outside mode 4
    candidate = segmentOrigin + direction * progress
    detect passing the destination by direction/remaining-vector test
    if passed destination:
        candidate = targetPosition
        mode = 0
        notify the PathPoint arrival machinery
    else if progress - lastDepositProgress <= baseHorizontal * 0.5:
        return
    lastDepositProgress = progress
    factory-create a child RCS_FireRescue
    orient child along segment; give it size keys bridging previous/current dimensions
    set later child-size key intervals using holdSeconds*1000 and fadeSeconds*1000
    queue child creation through entity messaging
    reset parent's size keys; put a zero-size key at now-10ms
    restore parent toward its base size after deterministic speed-based delay
    update parent position to candidate; optionally FloorMe()
```

The parent restore delay recovered from assembly is approximately:

```text
trunc(250 + 1750 * (1 - (clamp(speed,10,1000) - 10) * 0.001)) milliseconds
```

This is 2000 ms at speed ≤10 and about 267 ms at speed ≥1000 (not 250 ms). The exact child key list can include a previous-key link. Preserve the native code rather than assuming a single fixed child growth envelope.

No `Rand_*` call occurs in this stepper. Its conversion calls at `0x004497F5`, `0x00449844`, and `0x0044997F` are float-to-integer conversions of deterministic expressions. Crossings/deposits are discretized by frames; dt-scaled movement is not a guarantee of bit-identical deposit positions at every frame rate.

`setsize x y z seconds` sets mode 0, clears the current path target, clears active size keys, anchors current size at now+10ms and schedules the requested integer dimensions at now + seconds×1000. It is not an extinguish command and does not reset intensity in the inspected body. `stopmove` sets mode 0 and clears the path marker.

## 5. Random nearby flames and their RNG

Fresh evidence: `native/spread/` and `assembly/spread/`. The old database label `Rand_PlusMinus1` for `0x00404AC0` is incorrect: the returned float is in [0,1), not [-1,1].

### Attempts, placement and shape

Only direct entry found: `0x004483A0` exposure callback → `0x00448320` proximity bookkeeping → `0x00447A90` random-child routine. Assembly computes maximum absolute world X/Y/Z separation, truncates to an integer, and calls the generator when it is strictly below 750. This is candidate-check driven, not an independent timer for all fires.

The parent needs a nonempty actor name, current X size≥0.01 and `now >= primary+0x124` (next-attempt time). First draw U0 passes when `U0 <= intensity`. Rejection writes now+500ms. The inclusive comparison admits U0=0 even at strength zero; do not claim that endpoint is impossible. Its gameplay frequency has not been measured.

`0x00447850` returns current size X/Y/Z. Let `R=0.5*min(sizeX,sizeZ)`, `h=intensity`, `v=U1*h`:

```text
radius = R + R*v
angle = RandRange(0, float32(2*pi))
P = volumeOrigin + (radius*cos(angle), 20, radius*sin(angle))
b = (10 + 90*(1-v)) * RandRange(0.9,1.1)
peak = (b*(0.25+0.75*v)*RandRange(0.9,1.1),
        (10+30*v)*RandRange(0.8,1.2), b)
Tgrow = trunc(RandRange(750,3000))
Tend = Tgrow + trunc(RandRange(1000,6000))  # milliseconds
```

Before the three shape draws, helper `0x00492CE0` sweeps a box min=(-1,-1,-1), max=(1,1,1) downward by (0,-200,0) from P. `geWorld_Collision` uses mask 0x43/flags 2. A hit is required; absolute vertical difference must be≥0.001 and lateral X/Z differences each≤0.1. No fuel/material selection appears here. The definition stores P, not the impact point; FloorMe subsequently aligns the child. Geometry failure does not write a new retry deadline in this body.

At h=1, radius ranges from R to about 2R; at h=0.5, R to about 1.5R. Uniform radius draws are not uniform sampling over circular area. The child has three size keys: (0,0,0) at 0; peak at Tgrow; (0,0,0.25*b) at Tend. End-of-size schedule is not proof of exact deletion time. Child type=0, name=null, smoke=0, FloorMe=1 and horizontal heading follows the chosen angle.

Assembly 0x00447F46–0x00447FAB recovers successful retry timing that the decompiler omitted:

```text
q = max(1, 12/nearbyNamedFireCounter)
nextAttempt = trunc(now + Tend/q)
```

For positive count N<12, retry delay is Tend*N/12; for N≥12 it is Tend. At N=6 it is half Tend; at N=24 it is Tend, not twice Tend. FCOM/TEST AH,0x5/JP at 0x00447F68–0x00447F77 keeps values≥1 and replaces values<1 with 1; this assembly branch establishes max, not min. The counter is proximity bookkeeping, not an instantaneous count of every rendered flame. A zero count uses x87 division behavior; do not emulate it with integer division or presume a safe minimum count without tracing bookkeeping. This gives faster attempts for smaller positive counts rather than imposing a hard limit of 12 children.

Successful core generation uses eight range draws: acceptance, radius, angle, three shape factors, grow time, extra time. Probability failure consumes one; geometry failure consumes three. Later child spawn and unrelated systems can consume additional draws. Branches therefore change the RNG stream position.

### Can children spread again?

Both random and route-deposit definitions explicitly set name pointer +4 to null. Fresh children have empty actor names and fail the random routine's first test. The normal creation path therefore does not make a recursively branching chain, although named parents can make repeated children.

Pooling remains an edge case: factory `0x0044A470` reuses unlinked objects; reinitialization `0x004475F0` does not itself clear the actor name. Cleanup `0x00448550` → `0x0046E190` → `0x00470A70` unregisters the actor but does not clear the string in the inspected chain. Normal Think cleanup requires an empty name, helping keep named parents out of that retirement route. Other retirement/reset paths remain open. No reused-name spreading bug has been demonstrated; the non-recursion claim is bounded to fresh/unnamed children.

### Generator, seed and repeatability

`0x00498B30(a,b)` returns `a+(b-a)*U`. `0x00404AC0` draws from 624 state words, a 397-word twist offset, constant 0x9908B0DF and Mersenne Twister-style tempering. Its older seed initialization is not Python's default RNG sequence.

```text
Startup: Seed(timeGetTime())
Seed(s):                            # 0x00404B50
    state[0] = (s & 0x8ffffffd) | 1
    for i=1..623: state[i] = uint32(69069*state[i-1])
    remaining = 0
NextFloat():                        # 0x00404AC0, reload 0x00404960
    obtain next twisted/tempered word w
    return float32_from_bits(0x3f800000 | (w & 0x007fffff)) - 1
```

Float output has 23 bits of fractional resolution, in [0,1). Startup `0x0042C800` passes `timeGetTime()` to the seed function; that was its only direct caller found. `0x00404930` is internal fallback seed initialization used by reload. No stage-load reseed was found. State is global and shared with fire appearance/sound draws and other gameplay callers. Matching seed alone is insufficient: call order and branch outcomes must match as well. This pass has not captured a retail RNG replay.

### Specific saved routes

`derived/spread_audit.json` records scans of all 21 external scripts and embedded script fields in all 17 maps. Only two external fire movement sequences occur, and no literal moveto/stopmove occurs in the embedded fields:

- Gas Station Fire42 → PathPoint1 → PathPoint2 → PathPoint3 → PathPoint4; each travel argument=1 second; child timing arguments=99999,99999 seconds. Trigger `unfog2` executes the explosion2 variable only if GasIsOn=1, once. Shutoff gasoff changes GasIsOn and other fire sizes, but does not explicitly stop Fire42.
- Military Base mortarfire1 → mortar1; startup tell; travel argument=1 second; child timing arguments=10,120 seconds.

All five authored `RCS_PathPoint` entities are these destinations. Their names/positions and parent origins are preserved in the audit/catalog. Both parent definitions have base horizontal size 100, yielding a 50-unit deposit threshold; actual deposits are frame checked and also happen at endpoint crossing. This is a bounded content scan, not exclusion of every conceivable indirect/runtime script. The route stepper itself has no RNG draws; spawned visuals and other systems still draw from RNG.

## 6. Exposure and player burns

**Verified callback arithmetic.** `0x004483A0` accepts a candidate entity and asks its virtual provider for a position/padding record. It ignores state-5 fires and limits stored exposure records to four until the full Think resets the count.

```text
FireExpose(candidate):
    if mode == 5 or exposureRecordCount outside [0,3]: return
    record = candidate.virtualPositionProvider()
    if record is null: return
    distanceGate = trunc(max(abs(record.position - fireAnchor)))  # X/Y/Z maximum
    updateProximity(distanceGate)       # can invoke randomized child-flame routine
    if distanceGate > 500: return
    influence = FireInfluence(currentFireVolume, record.position, record.padding)
    severity = influence * (0.25 + 0.75 * intensity)
    if severity < 0.0001: return
    if not visibilityHelper(world, record.position, fireAnchor, false):
        severity *= 0.1
    send message 30001 / 0x7531 with { firePointer, severity } to candidate
    exposureRecordCount += 1
```

The max-axis computation is lost from the C export but visible in `assembly/damage_assembly.txt`. The anchor is stored at primary +0x7C and initialized from the positioned fire plus a four-unit Y offset, before/alongside floor handling.

Shared helper at `0x0044A7A0`:

```text
r0 = (currentSizeX + currentSizeZ) / 2
if r0 < 0.01: return 0
r = r0 + padding
(dx,dy,dz) = samplePosition - volumeOrigin
d2 = dx*dx + (2*dy)*(2*dy) + dz*dz
if d2 > r*r: return 0
return (emitterParameterD0 / r) * (r - sqrt(d2))
```

This is vertically compressed ellipsoidal falloff. `emitterParameterD0` is read through volume +0x60, then emitter +0xD0. The write chain is now verified: `0x00449FE0` computes the intensity-dependent input, `0x0044A610` caps values above the double constant 0.999, and `0x0047F1F0` writes +0xD0 and sets +0xD4 to 1. Rendering `0x0047F960` uses +0xD0 as the alpha override. Therefore this is a gameplay-relevant alpha value, not an independent unknown radius. Initialization before the first full Think and stale values between callbacks still need ordering review. The player candidate provider at `0x0049DB90` → `0x0049D790` returns transformed position at primary +0x68; spawn `0x0044D550` initializes the fourth float (+0x74) to 45. Later changes to that padding have not been fully audited; do not rename it “fire radius” just because a reconstruction helper did. This helper subtracts world positions and doubles the world-Y difference without applying the fire orientation transform; rotated visual geometry must not be substituted for this metric.

Player message decoder `0x00453A70` accepts damage only when primary +0x3438 is zero. It extracts the source fire position and passes severity to `0x00451640`.

```text
TakeFireDamage(source, severity):
    if playerState == 6: return
    set directional burn-indicator flags via 0x0044BB00
    if painCooldown < now:
        play RC_Pain
        rearm with a random 1000–2500ms interval
        increment "Serious Burns"
    burnAccumulator += dt * 1.2 * severity
    if burnAccumulator > 1: PassOut(burnReason=1)

PlayerThink():
    # separate branch, while player is not already passed out:
    if four effectFlags are all clear:
        burnAccumulator -= dt * 0.1
    burnAccumulator = max(0, burnAccumulator)
    reset four effectFlags
```

Primary burn offset +0x3538 corresponds to Think-subobject +0x33F8. At constant severity 1, the uncoolled mathematical threshold is just over 0.833 seconds; actual behavior depends on cooling, callback multiplicity and scheduling. “Serious Burns” is rate-limited event counting, not the accumulator itself. The pain cooldown's exact absolute-time composition should be read in assembly before emulating it: the decompiler drops arithmetic around the conversion.

## 6a. Direct flame/explosion emitter damage

**Verified:** constructor `0x00442900` maps authored `C_Type=5` to `0x00445600(kind=2)` and `C_Type=6` to that same constructor with kind 1. Its vtable `0x004E1438` has damage callback `0x00445490` at +0x24, update `0x00444D00` at +0x20 and strength setter `0x00463240` at +0x28. Base constructor `0x00444C40` initializes active +0x45 to true and strength +0x48 to 1. Turn-on/off `0x00444C90/CB0` write active; scripts can override the defaults.

```text
EmitterExposure(impl, candidate):
    if kind not in {1,2} or candidate.kind != 1 or not active: return
    if kind == 2 and firstParticle.frameIndex not in [3,11]: return
    P, padding = candidate.exposureRecord()   # player spawn padding = 45
    A, B, radius = impl.start, impl.end, impl.radius
    if A == B: return no influence
    t = dot(P-A, B-A) / squaredLength(B-A)
    if t < 0 or t > 1: return no influence
    Q = A + t*(B-A)
    d = length(Q-P)
    r = radius + padding
    if d > r: return no influence
    falloff = 1-d/r
    contact = (Q+P)/2
    severity = clamp(falloff * strength * 0.75, 0, 100)
    if not approximatelyZero(severity): TakeFireDamage(contact, severity)
```

`0x00483330` verifies the segment formula; `0x0043AE70` uses approximately ±1e-28 for the zero test. There are **no hemispherical endpoint extensions**: projection outside [0,1] is rejected. No ordinary-fire intensity, finalized-state test, 500-unit prefilter or visibility test appears inside this callback. **Outer gate:** `0x00441F10` clears parent +0x80, compares implementation origin to the current camera entity against squared distance 4,000,000 (2000 units), and calls `0x0048FE20`. That wrapper uses `geWorld_GetLeaf` and `geWorld_MightSeeLeaf`, not a line trace. Only success sets +0x80 and runs implementation update. Parent forwarding entry `0x00441DF0` checks +0x80 before tail-calling implementation +0x24 with the candidate argument. Older notes labeled it “Render”; this pass describes its observed forwarding behavior rather than accepting that name as semantics. The ordinary fire message receiver's +0x3438 check is bypassed by this direct call; do not assume immunity semantics until that field and broader dispatch are identified.

The update builds the hazard in transformed coordinates. Kind 1 uses the transformed origin and transformed local `(0,0,200)` endpoint, with radius **100** (`0x42C80000`). Kind 2 also constructs a transformed segment and derives a radius from its working `(0,100,0)` vector; its x87 transform/scale details need assembly review before assigning exact bounds. `setsize` participates in the emitter transform, so treating its authored position alone as a complete hazard volume is wrong. `0x00445720` constructs three particles and the `Torch` sound for kind 1, four particles for kind 2.

The catalog identifies 16 type-6 sources in Subway B, one in Oil Rig B (scripted strength 10), four type-5 sources in Boat A, one in Boat B and two in Oil Rig B. Native potential and authored configuration are verified; simultaneous exposure and runtime activity are not.

**Derived, conditional examples:** at severity 0.75, uncoolled threshold time from zero is strictly over 1/0.9 ≈ 1.111 seconds. At severity 7.5 it is strictly over 1/9 ≈ 0.111 seconds. Discrete callbacks, prior burns, actual falloff, cooling and scheduling change those values. Neither is a gameplay measurement. No global sum cap appears in `0x00451640`. Its state-6 early return prevents further burn processing once passed out. Session dt routine `0x0042BF70` caps dt at 0.1; this limits an individual call's dt but not the number of accepted hazards.

## 6b. Follow-up evidence and remaining damage questions

New evidence is under `native/followup/` in the archive. The matching reference and instruction dumps are under `assembly/followup/`. Function names/types remain provisional; `0x00444D00` was exported from its actual entry, and an exploratory export starting inside it was discarded.

`0x0044BB00` writes player +0x353C…353F by source-angle quadrant; it does not itself alter camera direction or movement. Consequently the previous “view kick” wording has been corrected. Burn addition and the independent cooling branch are established, but their exact interleaving with all entity callbacks is not.

Two static direct callers of `0x00451640` were found: the ordinary-fire message receiver and this emitter callback. This is evidence of two damage routes, not proof that indirect or script-driven health changes are impossible. Message dispatch `0x0049D4E0` → `0x0049B4C0`, mapper constructor `0x0042B1F0` and subsystem tick `0x00429520` have been exported; full queue/visitor order remains open. Multiple hazards can each call the routine; the measured callback multiplicity at a particular position remains a runtime question.

The +0xD0 writer is now recovered. After a synchronized update, define `a(h)=0.999` for h≥0.5, otherwise `min(0.999,2h)`. Ordinary-fire severity before occlusion is `a(h) × (1-d/r) × (0.25+0.75h)` within the gate/volume. At h=0 this is zero after alpha synchronization; the nonzero 0.25 factor alone does not establish residual damage. At h=0.25, the combined intensity factor is 0.5×0.4375=0.21875. These are derived equations, not measured survival times.

`0x0048FEA0` traces candidate position to the source anchor with `geWorld_Collision`, null extents, contents mask 0x43, collision flags 2 when called with false, and null callbacks. It returns true when the engine reports no collision. Exact contents/flag meaning and BSP-model handling belong to the external engine boundary and are not guessed here.

Still required for exact safe-distance calculations: initial/stale alpha timing, later player-padding changes, collision DLL semantics, and callback multiplicity. The current evidence does **not** establish an uninitialized-field bug.

## 7. Smoke, air, hose stages and unresolved boundaries

`0x00451560` independently updates player air at +0x3450: subtract 0.05 on a 3000 ms schedule, or add dt×0.1 in the refill/boost window. Pass-out threshold in this export is `<=0`. No smoke-density term appears in that function. A causal smoke→air link elsewhere has not been excluded by a full caller audit.

Truck/hotspot stages use `RCS_PlayerHose`, authored `RCS_HosePath` and dynamically created `RCS_WindowFire` objects. Preserve their authored path records from the catalog. They are distinct from the carried rescue hose and the `RCS_FireRescue` lifecycle above.

The old reference article claims automatically scheduled, aim-independent dousing. Additional verified entries now show why a single decrement function was insufficient:

```text
BeginDouse(window):                  # 0x00468850
    shared.latch_D = true
    window.ramping_25D = true
    progress_258 = 0
    UpdateVisual()

WindowThink(window):                 # actual vtable entry 0x00468870
    if not shared.latch_D: return
    if ramping_25D:
        progress_258 += 2*dt
        if progress_258 > 1:
            progress_258 = 1
            ramping_25D = false
        else:
            update particles and visuals
    if shared.flag_10:
        DouseStep()
    update state-dependent visual/sound branches

OnMessage(messageId):                # actual entry 0x00468A80
    if messageId == 30000: DouseStep()

DouseStep():                         # 0x00468680
    if shared.active_C and shared.selected_E:
        progress_258 -= dt / shared.blockSeconds_08
        if progress_258 < 0:
            shared.latch_D = false
            progress_258 = 0
        UpdateVisual()
        shared.flag_F = true         # set on every accepted step, not only expiry
```

`Hotspot::AdvanceActiveFire` `0x00465A20` will not advance while the current child's latch is set; otherwise clears that child's selected flag, advances the cursor, decrements cycles/remaining at wrap, selects the next child and calls BeginDouse while remaining>0. It can randomly trigger a flare. `0x00465940` sets active for all children and clears their latch when deactivating. `0x00465B90` builds children and initializes remaining from node +0x34.

Thus progress can ramp upward and decrement during the same Think if shared flag +0x10 is enabled. Message-driven decrements are an additional route. Neither a bare zero initialization nor a bare countdown supports an exact aim-independent completion schedule. Shared flag writers, hose contact message routing and frame order must be closed before claiming that. Mapping all authored hose fields to the native node structure also remains open; field names and array capacities cannot be inferred from old labels alone.

The catalog filter has been corrected to include **RCS_HosePath**. Counts are 83 path nodes / 28 authored hotspots over the four hose stages (19/9, 34/8, 17/5, 13/6 for City, Construction, Dock, Oil Rig). Previously those records were preserved only in raw entity chunks and accidentally omitted from the JSON path list.

## 7a. Runtime child and victim boundaries

Runtime child routines `0x00447A90` and `0x00449340` request `RCS_FireRescue` from the registered factory. Factory `0x0044A470` constructs/reuses an ordinary fire via `0x0044A330`, whose primary vtable is 0x004E1780 and whose +0x48 entry is `0x004483A0`. The constructor calls `0x0049D330(this,1)` just as for placed fires. An empty actor name suppresses named script/sound setup; it is **not** an exclusion in the damage callback. This establishes damage capability, not that every child successfully contacts the player in every frame. Pool reuse, entity lifecycle and stale callback state still require closure.

Victim constructor `0x0045DDD0` installs primary vtable 0x004E25B8. Message entry +0x8 points to `0x0043F650`, verified as a no-op. The emitter callback accepts kind 1 only while victims are kind 2. Therefore neither inspected player-burn route supplies a victim burn accumulator. This is a bounded negative finding: it does not exclude scripted mission failure, collision constraints or another victim-specific path.

Message drain `0x0049B610` resolves the queued recipient with `0x0049B7E0`, calls virtual +0x8 with message and payload, then clears the record's active byte. This confirms deferred delivery for the ordinary-fire and extinguisher message routes. Scheduler phase order and payload reuse boundaries are still needed for an exact simulation.

## 7b. Completeness audit and stopping criteria

See SUMMARY.md's coverage matrix. **The native audit is partial.** The older database's claim that no unnamed cluster reveals another mechanic is historical research, not a proof of fire completeness; this pass has already corrected several of its labels and interpretations.

The audit used actual class names and vtable entries, original scripts/entity fields, direct references, and a field-writer scan. `assembly/coverage/coverage_scan.txt` preserves instruction matches for +0xD0/+0xD4 plus selected vtables and references. This is a syntactic candidate scan: it can miss aliases, bulk copies and undisassembled code, and it must not be advertised as every semantic writer in the executable. `derived/coverage_audit.json` records the bounded map/script inventory and class counts. Coverage exports exclude exploratory decompilations started inside a real function; actual window entries were subsequently recovered from the vtable.

For closure, enumerate all reachable entries and relevant field writers, resolve wrappers and engine imports, model lifecycle/update order, and compare with retail edge cases. Do not translate a lack of matching strings, a function-name count, or archive hash verification into “no mechanic exists.” Hash verification proves copied evidence integrity, not correctness of the inferred model.

Still open:

- The exact effect of difficulty on contact multiplicity, severity, tool charge and spread beyond the inspected formulas; no difficulty branch was found in the core Spray/AdvanceSpread bodies.
- Callback registration, delivery order, skipped Think updates and four-record limits under multiple victims/players/objects.
- Whether victim entities accept these damage messages or have independent fire behavior; the player receiver is verified, not every possible receiver.
- The role of materials, breakable props and animations in triggering fire changes beyond authored scripts; external Genesis collision/visibility behavior has not been reverse engineered here.
- Reignition after permanent dousing: `setsize`/`moveto` write mode, so a later command might reactivate updating; intensity and follow-on recovery make the actual result a testable question.
- Runtime child count, cleanup, child spray-time defaults and their interaction with named parents.

## Reproduce and continue without scattered notes

Run `python3 analyze.py` to verify every archived entry and rebuild `catalog.json`. The archive includes verbatim entity chunk bytes and all original `.evs` files found under the copied game Data directory, so the map statistics and scripts can be re-examined without the Desktop folders.

Fresh Ghidra work used a copied project, `-readOnly -noanalysis`, headless export scripts preserved under `tools/`, and logs under `logs/`. The copied project was only a temporary working artifact and is not needed to inspect the exported evidence. A new Ghidra import can use the included executable; original project labels are unnecessary for the addresses listed above.

Next pass should update these same two master documents and the evidence/catalog, not create a new note for every question. Prioritize one controlled retail experiment and the matching caller trace rather than porting uncertain mechanics into the dashboard.
