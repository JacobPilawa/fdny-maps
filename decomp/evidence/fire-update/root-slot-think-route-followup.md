# FireRescue root slot to visual Think route

Snapshot: 2026-10-11. This reconciles already-preserved vtable and shared-dispatch evidence for FireRescue; it is a local virtual-call route, not a recovered recurring schedule.

## Verified local route

- FireRescue primary vtable `004E1780` has slot `+0x14` at row 5, pointing to thunk `0043AFE0` (`item-pose/live-extinguisher/message-components/firerescue-primary-vtable.tsv`).
- The constructor installs secondary vtable `004E1774` at object `+0x24`; its slot 0 points to Fire Think `00449FE0` (`fire-update/training-think-registration-audit.md`, `item-pose/live-extinguisher/message-components/firerescue-visual-secondary-vtable.tsv`).
- The shared thunk `0043AFE0` adds `0x24` to `this` and jumps to `00470740`. The shared updater checks the ScriptActor wait state, then calls secondary virtual slot 0; the equivalent BModel route and gate are documented in `training-liftable-audit/bmodel-scriptactor-dispatch-ghidra-audit.md`.

Together these facts establish that a call to a FireRescue object's primary root slot `+0x14` can reach `00449FE0`, subject to the updater's state gate. They do not identify a caller of that root slot for FireRescue, its cadence, or its order relative to the owner pass, message drain, tool action, and Player/victim updates.

## Bounded caller-candidate follow-up

The 50-site fixed-offset scan is in `item-pose/live-extinguisher/message-components/owner-vtable-indirect-calls.tsv`. Candidate `00436091` is now classified and excluded as an actor scheduler: it is inside the method beginning at `00435E80`, which walks `this+0x10` pointer entries up to the count at `this+0x14`, filters active records by two supplied points against integer rectangles, then dispatches selection-transition callbacks (`+0x14` on the old selection and `+0x18` on the new selection). It also invokes hover/press/release-related callback slots. This is 2D hit-target behavior; the exact containing class and dynamic caller remain unidentified, so “UI” is a likely role rather than a proven type name.

The candidate was not a separately defined function in the preserved Ghidra database. A boundary for `00435E80` was defined only in `/private/tmp/fdny-36091-scratch`; the saved read-only query output is [`00435e80-ghidra.txt`](root-slot-think-route-followup/00435e80-ghidra.txt). Queries were `DecompileFunctions.java 00435E80 00436091`, `InstructionContext.java 00436080 12 12`, and `AddressXrefs.java 00435E80 00436091 004E0530`. Incoming references to `00435E80` were data/vtable words; no direct caller was found. Executable SHA-256 is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, Ghidra 12.0.4. The preserved reference project was not modified.

### Other ambiguous fixed-offset sites

Read-only instruction contexts in [`candidate-classification-ghidra.txt`](root-slot-think-route-followup/candidate-classification-ghidra.txt) further classify several apparent candidates:

- `00408414` is a byte-reader interface call. Its return value is written to a byte buffer; `0xFFFFFFFF` exits the read loop. This is not a recurring entity dispatch.
- `0042BDDA` is a small three-argument forwarder that calls its generic `this` object's virtual `+0x14` method and returns. The inferred entry `0042BDA0` was not a function boundary in the preserved project. Its data references are vtable words, including `004DF968`, `004DFC40`, and `004E1400` through `004E14EC`; none is FireRescue primary `004E1780` or visual `004E1774`. No caller or class provenance was established.
- `0049EE0D` and `0049EECE` call `+0x14` on a subobject at parent `+0x24`; this is byte slot 5 of that subobject's vtable. Fire Think `00449FE0` is slot 0. The containing object's class is unknown, so these calls do not establish a FireRescue update route.
- `004649CC` calls `+0x14` on a transform subobject, not the containing actor's root. The caller loads that subobject from parent `+0x130` into `EBX`, then performs the virtual call through its vtable. Constructor `00464660` initializes the parent fields and invokes `0042D2D0(parent+0x130, 0)`; `0042D2D0` installs vtable `004DFC30` and initializes the matrix diagonal to `1.0`. Therefore this site is a transform operation, not evidence of an actor scheduler.

The follow-up used `DecompileFunctions.java 0042D2D0`, `DataValue.java 004DFC30 004DFC34 004DFC38 004E2A20 004E4540`, and `InstructionContext.java 004649CC 18 4` against the preserved project. Its read-only log is `/private/tmp/fdny-rootroute-confirm.log`. Earlier candidate contexts and commands remain under `/private/tmp/fdny-remaining-*.log`.

The inspected sites now classify as a UI hit-target method (`00436091`), byte reader (`00408414`), generic forwarder with no FireRescue vtable reference (`0042BDDA`), unrelated subobject slot-5 calls (`0049EE0D/0049EECE`), and transform virtual call (`004649CC`). Other entries already classified in `scriptactor-common-scheduler-audit/fixed-offset-call-candidates.md` are parsers, serialization, Player item/use paths, or a conditional self-call. The known owner call at `0042AED4` is on the global engine/input object, not an entity root. This bounded scan still cannot rule out register-assembled displacements or other computed dispatch, and no live game trace is claimed.

### Search pivot

The literal fixed-offset scan has not yielded a recurring actor updater. Avoid repeating it as the primary next step. Continue from the proven root wrappers (Player `0044FE20`, FireRescue thunk `0043AFE0`, victim `00459990`) and their class/vtable registration paths; follow the owning object/list registration or a computed engine callback to the actual update loop. The earlier project conversation `Reverse-engineer FDNY Training` suggested the same root-wrapper-first approach. Keep the caller, cadence, and cross-system order open until that path or a safe bounded runtime trace is established.
