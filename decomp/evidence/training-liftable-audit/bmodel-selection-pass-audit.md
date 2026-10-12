# BModel selection pass and Player update order

Read-only Ghidra follow-up for the interaction-selection subobject reached
from the Player update block. The executable is the hash-pinned
`FDNYFirefighter.exe`; queries used `-noanalysis -readOnly` against the
preserved `evidence/ghidra/fdny` project. This is static source evidence, not a
live input trace.

## Candidate scan

`00459350` owns the selection scan. Its receiver-relative fields are:

- `+0x188`: candidate pointer array;
- `+0x18C`: candidate count;
- `+0x1A0`: selected index;
- `+0x1A4`: selected action;
- `+0x1A8`: retained selected-object/interface pointer.

At entry, it sets the selected index to `-1`, clears the selected action, and
releases the previous retained pointer if present. It then scans array indices
in increasing order. Each non-null candidate receives virtual slot 0, which
returns an action and may return a query interface. A nonzero action with no
query interface is selected immediately and ends the scan. With a query
interface, slot `+0x14` performs the geometric query; a hit is retained only
when its squared distance is at most the squared 200-unit range. Equal scores
replace the current choice (`currentScore <= candidateScore`), so later array
entries win exact score ties.

`00458FA0` dispatches the cached candidate's virtual slot `+4` with the saved
action. If that candidate still exists, it then clears the `UseObject` state
through `004A11A0` and returns success. Candidate construction, array insertion
order, and the meaning of the query-less immediate-selection branch are not
recovered here.

## Order inside the Player update block

The instruction window around `004533C2` shows the Player subobject at
`Player+0x3114` passed to `00458FA0` only when the preceding `00438550` call
returns nonzero. If dispatch reports no selection, the code follows with
`0044C480` on Player root `-0x140`.

Later in the same instruction stream, `00453865` forms `Player+0x3114` again
and `0045386B` calls `00459630`. That updater calls `00459350` to refresh the
cached selection. Therefore this update block dispatches an already-cached
selection before refreshing the candidate choice. This establishes ordering
inside the block only; the caller cadence, reachability of the input edge in a
normal Training run, and any event-queue latency remain unknown.

The `00459630` receiver is `Player+0x3114`; its own byte `+0x12C` is therefore
`Player+0x3240`, not the Player-root byte `+0x12C` read by `0044C520`.
`004590C0` initializes a nested interaction/UI structure and clears a list at
its own `+0x188`. The Player constructor calls it with receiver
`Player+0x3254`, which is `Player+0x3114+0x140`; it is nested under the
selection subobject but its list begins at `Player+0x33DC`. The scan in
`00459350` reads a different array at `Player+0x329C` (count at `+0x32A0`).
Thus `004590C0` does not initialize the array being scanned. Bounded
direct-store scans of offsets `+0x188` and `+0x18C` find same-offset stores on
untyped receivers and do not identify the list producer.

`FieldOffsetReferences.java 0x3114` found only the two direct Player-root
references listed above; a scan of root offset `0x329C` found no explicit
accesses. These bounded operand scans do not rule out a computed alias or a
producer that receives the subobject pointer indirectly. The code that fills
the scanned candidate list remains unknown.

## Player-root eligibility byte `+0x12C`

Player helper `0044CD10` writes byte `Player+0x12C = 1` and adjacent byte
`Player+0x12D = 1`. Direct call references show `0044D550` (Player
constructor) and `0044FF10` (HeroDown path) both call this helper. The
`0044C520` lift gate thus sees a nonzero default immediately after either
reset path, unless another write intervenes. This does not name the byte or
prove its value at the time Training processes input.

A bounded `FieldStoreAudit.java 0x12c` scan found byte stores at `00446FC9`
and `00447440` without a containing function boundary, plus other word/dword
stores on unknown receivers. The scan does not establish that any is a write
to Player root `+0x12C`; aliases and computed stores are outside its scope.
`00459630`'s two byte stores are at Player `+0x3240`, as shown by its caller
at `0045386B`.

## Reproduction queries

Using `DecompileFunctions.java 0044CD10 0044D550 0044FF10 004590C0
00459350 00459630 00458FA0`, `AddressXrefs.java 0044CD10 00459350
00458FA0 0044C520 0043D2B0`, `InstructionContext.java 0045386B 16 16`
and `InstructionContext.java 004533C2 16 16`, `FieldStoreAudit.java 0x12c`,
`0x188` and `0x18c`, and `FieldOffsetReferences.java 0x3114` and `0x329c`.
All preserved-project queries used `-noanalysis -readOnly`.

Next source work: identify the candidate-list producer and its ordering, then
connect that list and the already recovered BModel query/action methods without
substituting guessed actor scheduling or live state.
