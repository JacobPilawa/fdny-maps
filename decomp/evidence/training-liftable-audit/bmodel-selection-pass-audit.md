# Player BModel selection pass and receiver mapping

Read-only Ghidra follow-up for the Player movement method and its interaction
selection object. The executable is `FDNYFirefighter.exe` (SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). The
preserved Ghidra project remained query-only. For the movement method,
`00452570`, the vtable-verified entry was defined only in a temporary project
copy under `/private/tmp` to recover its containing function boundary. No live
game trace was used.

## The interaction-selection object

Player movement-subobject vtable `004E1C40` has method `00452570` in slot 0;
the Player root `+0x14` route reaches it through `0044FE20` and shared
ScriptActor update `00470740`. As a result, `00452570` receives
`this = Player+0x140`, not the Player root. Inside that method, both
`004533BC` and `00453865` form `this+0x3114`, which is `Player+0x3254`.

That is the same object initialized by Player constructor `0044D550`, which
calls `004590C0(Player+0x3254)`. `004590C0` clears its candidate array at
`+0x188` (absolute Player `+0x33DC`), resets the count at `+0x18C` through its
container helper, and sets selected index `+0x1A0` to `-1`. The candidate scan
and dispatch use this same receiver:

| Receiver-relative field | Player-root address | Use |
|---|---:|---|
| `+0x188` | `+0x33DC` | candidate pointer array |
| `+0x18C` | `+0x33E0` | candidate count |
| `+0x1A0` | `+0x33F4` | selected index |
| `+0x1A4` | `+0x33F8` | selected action |
| `+0x1A8` | `+0x33FC` | retained selected-object/query pointer |

This establishes construction and initial empty-list state, but not who adds
candidates or in what order. The `FieldOffsetReferences.java 0x3114` result is
two movement-method operands at `004533BC` and `00453865`; that displacement is
relative to the movement subobject, not the Player root.

## Candidate scan and input order

`00459350` resets selected index to `-1`, clears the selected action, and
releases the previous retained pointer if present. It scans candidates in
increasing array index order. Each non-null candidate receives virtual slot 0,
which returns an action and may return a geometric-query interface. A nonzero
action with no query interface is selected immediately and ends the scan. With
a query interface, slot `+0x14` performs the geometric query; a hit is retained
only when its squared distance is at most the squared 200-unit range. Equal
scores replace the current choice (`currentScore <= candidateScore`), so later
array entries win exact score ties.

`00458FA0` dispatches the cached candidate's virtual slot `+4` with the saved
action. If a candidate is still selected, it then clears the `UseObject` state
through `004A11A0` and returns success.

Within `00452570`, the triggered action dispatch occurs at `004533C2` on
`Player+0x3254`, before the same receiver is passed to `00459630` at
`0045386B`; `00459630` calls `00459350` to refresh the selection. The action
therefore uses the cached choice from before that refresh. The Player root
`+0x14` wrapper and `00470740` wait-state gate determine when the movement
method runs; its recurring caller/cadence remains unresolved. See
[`../player-update-scheduling-audit.md`](../player-update-scheduling-audit.md).

`00459630` reads/writes its own byte `+0x12C` at absolute Player `+0x3380`.
That field is separate from the Player-root eligibility byte `+0x12C` read by
`0044C520`.

## Candidate registration and slot reuse

The interaction list is populated through a small add/remove adapter pair.
`00459050(candidate)` checks the candidate subobject's byte `+4`; if clear, it
calls `00459000(Player+0x3254, candidate)` and sets the byte. `00459000` scans
the array from index 0 through the current count and fills the first null slot.
If there is no hole, it grows the array to `count + 1` and stores the candidate
at the old count. So a new registration appends only while the list is dense.

`00459090(candidate)` removes only when that marker is set and both the
singleton and Player pointer are available. It calls `00429FE0` on the array at
Player root `+0x33DC`, then clears the marker. `00429FE0` finds the first slot
containing the exact candidate pointer, invokes the array's `+0x10` callback,
and writes null to that same slot. It does not compact the array. The next
registration reuses the lowest-numbered hole, which can give a newly active
candidate an earlier tie-break position than older entries still in the list.
The array callback's ownership effects are not yet resolved.

Incoming references and local code establish three add paths: the concrete
constructor at `00463700` registers its subobject at `this+0x1FC` at
`00463D31`; the BModel updater `0043DC60` conditionally registers its subobject
at `this+0x7C` at `0043DDB8`; and an untyped routine registers `ESI+0x7C` at
`004440D8`. The same BModel updater removes `this+0x7C` at `0043DDBF`, and the
untyped routine has a paired removal at `00444100`. Several owner teardown
paths also call the removal adapter (`0043AF10/20`, `0043F180`, `004448F0`,
and `00463D50`).

For BModel update `0043DC60`, `this+0x218` is set when helper `00490300` on
`this+0x13C` returns true. If it returns false, the byte is cleared only when
`004903A0` returns true; otherwise its prior value remains. When `this+0x234`
is nonzero, that byte selects add versus remove for `this+0x7C`, before the
routine calls `0043D330`. The two helper results and fields are still
unlabelled; this maps the registration edge, not the exact Training condition
or its recurring update schedule.

The list's static append/first-hole rules and BModel add/remove caller are now
source-mapped. Actual Training membership and moment-by-moment order remain
open: they depend on actor construction plus runtime registration/removal
events, and the BModel updater's recurring caller has not been found. Do not
build the native selection list from BSP entity order alone.

## Player-root eligibility byte `+0x12C`

Player helper `0044CD10` writes root byte `Player+0x12C = 1` and adjacent byte
`Player+0x12D = 1`. Direct call references show `0044D550` (Player
constructor) and `0044FF10` (HeroDown path) both call it. The `0044C520` lift
gate therefore sees a nonzero reset default unless another root write
intervenes. This does not name the byte or prove its value when Training
processes input.

A bounded `FieldStoreAudit.java 0x12c` scan found other stores on untyped
receivers, including byte stores at `00446FC9` and `00447440` without function
boundaries. It does not prove those are Player-root stores; aliases and
computed writes are outside its scope. The two `00459630` stores belong to the
interaction-selection object at root `+0x3380`, as shown by the corrected
receiver calculation above.

## Reproduction queries

Using `DecompileFunctions.java 0044CD10 0044D550 0044FF10 004590C0
00459350 00459630 00458FA0`, temporary-copy
`DefineAndDecompile.java 00452570`, `AddressXrefs.java 0044CD10 00459350
00458FA0 0044C520 0043D2B0`, `InstructionContext.java 0045386B 16 16`
and `InstructionContext.java 004533C2 16 16`, `DecompileFunctions.java
00459000 00459050 00459090 00429FE0 00463700 0043AF10 0043AF20 0043F180
004448F0 00463D50`, temporary-copy `DefineAndDecompile.java 0043DC60`,
`AddressXrefs.java 00459000 00459050 00459090`, `InstructionContext.java
0043DDB8 18 20` and `004440D8 18 20`, `DataValue.java 004E0B7C 004E0B80
004E0B84 004E0B88 004E0B8C 004E0B90`, `FieldStoreAudit.java 0x12c 0x188
0x18c`, and `FieldOffsetReferences.java 0x3114`. All queries against the
preserved project used `-noanalysis -readOnly`; missing function boundaries
were defined only in the scratch copy under `/private/tmp`.

Next source work: identify the Training BModel instances for which `+0x234`
enables registration, resolve the two `+0x13C` checks and their update caller,
then connect those dynamically ordered entries to the BModel axis-box query
and action methods without guessing scheduler or live field values.
