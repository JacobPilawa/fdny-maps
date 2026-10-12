# BModel USE/lift component scope

`src/bmodel_use.h` records a narrow translation of the static Ghidra findings
for `0043DB40`, `0044C520`, `0043D2B0` and `0044BC40`.

## Mode/action gate

- BModel use mode 0 yields no candidate action/query interface.
- A nonzero mode exposes the axis-box query interface. Mode 1 obtains the
  Player action result; the lift action is literal `0x3EC` (1004) only when
  Player item-array slot 4 is absent or its object field `+0xC68` is not 3,
  Player byte `+0x12C` is nonzero, Player held-object pointer `+0x3440` is
  null, and BModel state `+0x25C` is zero. The fixed `+0x10` item-array
  access is slot 4, used by the Player's Hose Spray path (`0044CAB0`).
- Other nonzero modes return literal 1000. Its intended meaning is unknown.
- Byte `Player+0x12C` remains unnamed, but its reset default is now traced:
  `0044CD10` writes root bytes `+0x12C = 1` and `+0x12D = 1`, called by the
  Player constructor and HeroDown path. The similarly numbered stores in
  `00459630` are on the interaction-selection object at Player root `+0x3254`,
  so they touch Player `+0x3380`, not the root gate. Other same-offset store
  candidates have unresolved receiver identity.

## Mode-1 use transition

When `0043D2B0` handles mode 1, `0044BC40` stores the BModel root in Player
root `+0x3440`, sets BModel root `+0x25C` to 1, calls
`004571A0(Player+0x5F4, 3, nullptr)`, then sets Player root `+0x18C` to
state 7. The native helper exposes the `004571A0` call through an ordered
callback; its service effects remain unimplemented. The original `0043D2B0`
handler next forwards a `use` event to the ScriptActor at BModel root `+0x24`;
that event and its callbacks are outside this component.

## Evidence and boundary

`bmodel-use-pcode-oracle.json` executes the original `0044C520` gate for
controlled item-slot, byte, held-pointer and object-state inputs. It also
executes `0044BC40` while intercepting `004571A0` as a controlled service stub;
the comparison checks receiver/arguments and that the callback occurs after
the held-pointer/object-state stores but before Player state 7. This proves
the source order and call boundary, not `004571A0`'s service effects or any
live Training field values.

The source mapping and Training object/motion prerequisites are recorded in
`../training-liftable-audit.md` and
`../training-liftable-audit/throw-path-ghidra-audit.md`. The read-only
preserved-project decompilation covered `0043DB40`, `0044C520`, `0043D2B0`,
`0044BC40`, `004571A0`, `0044CAB0`, and `00459630`; the caller context at
`0045386B`, together with the temporary-copy boundary at `00452570`, proves
`00459630` receives the constructor-initialized object at Player root `+0x3254`.
Its `+0x12C` field is root `+0x3380`, distinct from Player root `+0x12C`.
Follow-up source ordering and reset evidence are in
`bmodel-selection-pass-audit.md`. P-code output is pinned to the executable
hash in the fixture.

The component and controlled P-code checks do not prove native selection,
input reachability, carry transforms, callback effects beyond the recorded
boundary, recurring scheduling, or Training parity.

Still open: the meaning and intervening-write lifecycle of root `+0x12C`, mode
2/action 1000, axis-box query composition, Training list membership and runtime
registration order, event delivery and latency, held-object positioning/
release integration, and the caller cadence. `00459050/00459090` and BModel
updater `0043DC60` now explain one conditional registration path; the exact
Training conditions and schedule remain open. The Player update block
dispatches its cached selection before refreshing it, but that local order
does not prove a frame schedule.
