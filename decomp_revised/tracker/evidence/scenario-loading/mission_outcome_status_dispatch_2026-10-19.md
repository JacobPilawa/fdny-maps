# Player rescue outcome status dispatch (2026-10-19)

## Scope and source

Fresh headless analysis used Ghidra 12.0.4 on the pinned
`inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The direct field-use scan covered decoded instructions whose rendered operands
mention Player offsets `+0x3420`, `+0x3438`, `+0x343C`, `+0x3528`, `+0x352C`,
`+0x3530`, and `+0x3534`. The resulting pseudocode is saved in
[`mission_outcome_state_fresh_2026-10-19.c`](mission_outcome_state_fresh_2026-10-19.c).
The scan is bounded to decoded, direct offset references; it does not rule out
indirect writes, missed code, or values written through a pointer alias.

## Result state and feedback dispatch

Player initialization at `0044D550` clears `Player+0x3438`. The final-exit
availability routine `004504C0` and the `finalexit` command handler `00450A00`
set it to `1` when their source rescue-completion condition passes. These
writers are also part of the existing
[completion and exit path evidence](rescue_completion_and_exit_path.md).
The field is additionally an outer Player message gate, so the port currently
stores it as `RescueExitState.messageGate`.

In the visible-status branch of `00458400`, `Player+0x3438 == 1` performs three
source calls: the status widget at `PlayerStatus+0x824` receives state `0x47`,
the Player chatter manager at `Player+0x3540` receives `RC_ResLvlWin` with
flag `1`, and the status-owned sound instance at `PlayerStatus+0x7DC` is
submitted for playback. For value `2`, the corresponding calls use widget
state `0x48`, chatter set `RC_ResLvlLose`, and the sound instance at
`PlayerStatus+0x800`. `004576B0` constructs those two instances using the names
`RescueWin` and `RescueLose`.

The numeric gate is not itself a reliable mission-win event for the shared
host. `UpdateFinalExitAvailability` (`004504C0`) also writes
`Player+0x3438 = 1` when the rescue threshold passes but no exit zone exists,
then calls `0044C360`; the available trace does not establish that call as a
completed mission outro. The source win feedback branch is real, but a port
needs a source-confirmed completed-mission event instead of treating every
`RescueExitState.messageGate == 1` as a win. See
[`rescue_completion_and_exit_path.md`](rescue_completion_and_exit_path.md).

The status updater routes named chatter through `004897C0`, which finds the
set and chooses `00489110` for flag `1`. That routine advances its set cursor
and wraps at the entry count before requesting the selected sound through
`004A11A0`. Its diagnostic text says “Playing shuffled sound,” but the code in
this flag-1 path advances the cursor; the separate flag-0 path `00488D40`
chooses an index using the retail random helper. The win and lose chatter sets
each contain one entry, so their selected resource is unambiguous either way.
`004895C0` registers the named sets.

The status child at `PlayerStatus+0x824` is a timed widget state. Helper
`0048D180` stores the requested selector (`0x47` or `0x48`) at child `+0x108`,
records the current global clock at `+0x12C`, and marks it active at `+0x13C`.
In `00458400`, the matching active check at `PlayerStatus+0x960` aliases this
same byte: `PlayerStatus+0x824+0x13C == PlayerStatus+0x960`. This joins the
status updater's result-child gate to the setter without depending on the
printed direct-operand scan. It does not establish who clears the active byte
or supply the result child's duration fields.
Constructor `0048D390` initializes its color and leaves it hidden. Updater
`0048D1A0` makes it visible and uses the elapsed ratio
`(clock - child+0x12C) / child+0x130`. When that ratio exceeds 1.0, it calls
`0047EB60` with viewport center `(320, 240)` and angle `ratio * 1800` degrees;
`0047E970` confirms the helper rotates each quad vertex about the supplied
center. The child reports completion only after the clock is strictly greater
than `child+0x12C + child+0x130 + child+0x134`. This confirms an authored
rotation phase and completion gate, but not the result child's desktop
placement or runtime pixels. Result parent constructor
`004576B0` calls the child constructor but contains no explicit timing-field
assignments in the reviewed pseudocode. A fresh Ghidra operand scan for direct
references to parent offsets `+0x954`/`+0x958` (child `+0x130`/`+0x134`)
returned no matches. This scan only recognizes decoded instructions with those
printed offsets; pointer aliases, computed addressing, indirect calls, and
runtime writes remain possible. Fresh selected pseudocode and binary32 reads
are saved in
[`mission_outcome_widget_callers_fresh_2026-10-20.c`](mission_outcome_widget_callers_fresh_2026-10-20.c).
The rotation helper, conversion constants, and result-completion handoff are
saved in
[`mission_outcome_frontend_handoff_fresh_2026-10-20.c`](mission_outcome_frontend_handoff_fresh_2026-10-20.c).

When `0048D1A0` reports completion, `00458400` calls `004295B0` with whether
`Player+0x3438 == 1`. If manager field `+0x6E4` is nonzero, the helper writes
manager field `+0x6EC` to `1` for the win path or `2` otherwise. This verifies
the handoff values only; their downstream UI, transition, or return behavior
has not been identified.

That trace also checks a tempting source for the missing values: `0048D330`
initializes a related widget with `+0x130 = 1000` and `+0x134 = 5000`, but its
direct callers are `00458B20` and `004653F0`. The Player result parent
`004576B0` calls `0048D390` for its child and does not call `0048D330` in the
reviewed pseudocode. A separate parent path (`00465580` constructor and
`00464FE0` updater) also uses `0048D390`/`0048D1A0` at a different child
offset. These sibling paths do not establish the result child's timing values;
no duration is inferred from them.

The renderer trace closes the selector-to-resource lookup. Generic sprite
render callback `0047DF10` reads child `+0x108`; when no texture object is
preloaded, it passes that integer to resource lookup `00493D00` and renders the
returned bitmap. The sprite base constructor `0047EE90` initializes the
selector to `-1` and clears the preloaded texture object. The fresh pseudocode
is in
[`mission_outcome_widget_resource_fresh_2026-10-19.c`](mission_outcome_widget_resource_fresh_2026-10-19.c).

The original `rescue.rrf` maps selector 71 (`0x47`) to `paperwin-r.bmp` and
selector 72 (`0x48`) to `paperlose-r.bmp`; both use `paperwin_a.bmp` as their
alpha texture. The parallel `hose.rrf` uses the same IDs for
`paperwin-h.bmp`/`paperlose-h.bmp`. The original asset manifest hashes the
rescue images as `fd1fc8271787d87c981066a21fc8e5a0bcb15ede04daa3425419ea5081af3987`
(win), `cca68aabc1fc6e868058beee36f5f56424d904c6eca4fd1de461b5920445daba`
(lose), and `d1aee42b811b4690b3f4edc14e4943f95fec923fcdb32acc60993947791d0b66`
(shared alpha). Static inspection of the rescue bitmaps shows the win headline
“Heroic Firefighter Rescues Helpless Victims from Blazing Inferno!” and the
loss headline “Careless Firefighter Perishes With Other Victims in Raging
Blaze!” This establishes authored selector/resource/art mapping. The update
helper's viewport-centered rotation is source-confirmed, while desktop
placement, alpha behavior, timing-field values, and runtime pixels remain
unverified.

The original `sound.rrf` maps the chatter resources as follows:

| Set / instance | Resource | File |
| --- | --- | --- |
| `RC_ResLvlWin` | `RC_ResLvlWin0` | `data\\snd\\chatter\\win2.wav` |
| `RC_ResLvlLose` | `RC_ResLvlLose0` | `data\\snd\\chatter\\lose4_b.wav` |
| `RescueWin` | direct owner sound | `data\\snd\\win2.wav` |
| `RescueLose` | direct owner sound | `data\\snd\\lose2.wav` |

The direct sounds and chatter clips are distinct resources. The status updater
contains no visible one-shot latch around these state branches in the reviewed
function. How often that updater runs, how the sound manager handles repeated
requests, and the result child's timing-field values, desktop placement, and
rendered runtime appearance for widget states `0x47` and `0x48` are not
established here.

## Follow-up: frontend result-state consumer (2026-10-20)

The saved Ghidra project was copied to ignored `.local/mission-lifecycle-ghidra`
and opened headlessly for this read-only follow-up. A decoded-instruction scan
for manager offsets `+0x6EC` and `+0x6E4`, followed by fresh decompilation,
confirms that `ProcessEverGameUpdate` (`0042AEA0`) consumes the result state
written by `004295B0`. State `1` calls `00424650` with the currently selected
level ordinal from the menu selection record; state `2` skips that call. Both
paths reset frontend rendering/navigation state and switch menu child: state
`2` selects child index `2`, while the other written state (`1`) selects child
index `5` through `0042D3E0`.

Existing same-hash constructor evidence identifies child `2` as the level
selector (`0042F670`). Fresh constructor review identifies child `5` as
`00432BB0`. Its base constructor requests menu resources `11` and `17`, which
`menu.rrf` maps to `sub.bmp` and `ttl-records.bmp`; it also requests alpha
texture `54`, mapped to the left-arrow color/alpha pair. This identifies child
5 as the Records page. The preserved asset manifest records arrow image hashes
`fad3d360ed0e138c53e07530b6a28a4bb85928768993c6fa005d629a879a750f` and
`a146951476a6df0be3565aa651a17bd69d0ce4e97515b36e777e75ac41efc528`.

The earlier child-5 callback summary was incomplete: its excerpt stopped at the
first conditional branch and incorrectly treated actions `1` and `2` as
no-ops. Rechecking vtable slot 1, `004346A0`, against the same hash-pinned
executable shows action `0` calls `0042D460` to move menu history back; action
`1` decrements child `+0x354` and clamps it to zero; and action `2` increments
that selected-record index and clamps it below the menu manager's record count
at `+0x6B4`. Both navigation actions call `004332E0` to rebuild the record
page. The generic menu button callback `00435780` forwards its configured
action ID to the page vtable.

Page updater `004332E0` looks up the selected index in the menu manager's
catalog collection at `+0x6AC` and rebuilds content from the selected record.
The selected decompilation is saved in
[`mission_outcome_records_page_fresh_2026-10-20.c`](mission_outcome_records_page_fresh_2026-10-20.c).
This narrows the page purpose and navigation behavior, but not the exact record
field labels, full layout, activation visuals, or runtime pixels. The win-only
`00424650` path also obtains or creates a profile record and updates it with
the selected ordinal and a menu-owned string; the profile schema and
progression semantics remain open.

The focused frontend exports are
[`mission_outcome_frontend_consumer_fresh_2026-10-20.c`](mission_outcome_frontend_consumer_fresh_2026-10-20.c)
and [`mission_outcome_records_page_fresh_2026-10-20.c`](mission_outcome_records_page_fresh_2026-10-20.c).
They record the executable hash above. The menu resource table has SHA-256
`ae89458ef0aa2017351c1fa92fa22286eb4d4052cd55bbee4c1b917a1b9104b0`.
The control flow establishes the frontend child indices, win-only mutation,
active-flag alias, and Records navigation. It does not establish the exact
rendered record fields, result-child duration, activation visuals, map-loading
handoff, or natural runtime timing. No host transition was added from these
static findings alone.

## Follow-up: result-child timing constructor scan (2026-10-20)

A full-file `objdump -d -Mintel` operand scan of the pinned executable found
no direct `[base + 0x954]` or `[base + 0x958]` accesses for the result child's
parent-relative timing fields. The verified construction chain is
`0044D550` → `004576B0` → `0048D390` at `PlayerStatus+0x824`; the Player
initializer has no whole-object clear, and the reviewed widget/sprite base
constructors contain no writes to child `+0x130/+0x134`. `0048D180` writes the
start clock, active byte and selector, while `0048D1A0` reads the duration/tail
fields. This does not establish the outer allocator's clearing behavior and
cannot rule out computed or indirect alias writes. Do not substitute zero or
the sibling widget's `1000/5000` defaults.

## Open state and limits

The first decoded-operand scan found no direct root-relative store of value `2`
to `Player+0x3438`. A fresh subobject-offset trace of the Player movement
update at `00452570` found the producer: `ESI` is `Player+0x140`, and when
`ESI+0x32FC` (`Player+0x343C`) becomes negative, the body clamps the timer to
zero and stores `EDI=2` through `ESI+0x32F8` (`Player+0x3438`). The status
callback already traced above routes that state to the lose widget and
audio. The result-child timing and full frontend handoff remain open.

The same field is the Player message gate used by the shared rescue port to
reject victim-pickup messages. Retail's final-exit availability routine can
also set value `1` when no exit zone exists, while ordinary completion with an
authored exit only enables that exit until a later `finalexit` command passes
its remaining-victim check. The status callback's value-1 win branch therefore
does not prove that every gate-setting path is a completed-mission win. Value
`2` is now tied to the timer-expiry route; the host keeps this numeric state in
the same gate field.

`Player+0x343C` is initialized from the selected level time and decremented by
the Player update in seconds; it is also read as the input to escape/RescueBot
deadline scheduling. Its negative-value path writes `+0x3438 = 2`. The shared
host now tracks that countdown, writes state 2, and stops its Player movement
step, but it does not yet implement result audio/widget playback or frontend
return. The frontend manager's static result consumer is traced to child 5 for
state 1 and the level selector (child 2) for state 2; exact child timing and
complete outro remain open. PlayerHose has a separate result field and no
confirmed consumer, so do not apply this rescue result path to it.

These findings are static decompilation and resource-definition checks. No
retail runtime, natural mission completion, audio capture, rendered result
state, or parity comparison was performed.
