# Training implementation plan

Snapshot: 2026-10-11. Read HANDOFF.md and CHECKLIST.md first. The current map is playable, but no complete Training run or shared RNG sequence has been verified. Component comparisons, static source recovery, and live integration are separate milestones.

## Architecture and evidence rules

Keep recovered transactions in small C++ components with original addresses. Use adapters for engine services; preserve source callback order, integer wrap, intermediate float rounding, and shared RNG calls. Do not choose a recurring schedule from a local call chain. The host frame order remains provisional until original dispatch is established. Preserve raw evidence locally, and publish authored notes and numeric fixtures under the repository's existing asset exclusions.

## Independently executable tasks

| ID | Task and completion evidence | Dependencies |
|---|---|---|
| V1 | Port escape/release `00451160→0045C000`; verify state, transform operations, flags, event, timer wrap and ordered Player cleanup as isolated transactions. | Existing lifecycle/frame evidence |
| V2 | Port pickup post-effects `00450440`: entity mode, `00490E60` storage, status callback, zone refresh, final clock sample. Validate field-4 branch, event ordering and carried-victim zone exclusion. | Existing pickup component |
| V3 | Apply Genesis normal render flag gate at native actor submission using per-entity actor state. Validate both mask bits and pickup/release visibility transitions. | Existing actor flags evidence |
| V4 | Recover continuous carry positioning and passout/drop, including exact motion RNG and vertical correction; make separate components and original-code fixtures. | V1/V2; source trace |
| S1 | Establish root secondary-slot dispatcher for Player, victims and BModels. Trace computed receivers and engine callback registration; record ruled-out paths and limits. | Existing scheduling audits |
| S2 | Establish FireRescue visual Think dispatcher, timing, and order relative to proximity, messages and tool action. | Existing fire dispatch audits |
| V5 | Connect proximity producer, generation-aware delivery, pickup effects, escape release and exits in verified order. Compare original event transcript and actor flags. | V1–V4, S1 |
| F1 | Connect Think recovery, size keys, terminal douse and spread using established schedule; compare fire state/size and message traces. | S2; verified fire components |
| F2 | Port particles, conditional RNG, smoke, sound and lighting; compare pool occupancy, draws and lifetime boundaries. | F1; emitter/particle traces |
| B1 | Connect BModel selection/lift/carry/throw and axe/destruction; compare interaction ties, transforms, contact/event queue latency. | S1; scalar throw and box fixtures |
| H1 | Port hose attachment frames, post-view stream, suppression and detach lifecycle. Compare transforms, contacts, charge/audio and draws. | S1/S2; hose evidence |
| G1 | Finish remaining script commands, damage/passout, goggles, rescue counters, exit gates and mission completion. | V5/F1/B1/H1; script dispatch |
| R1 | Integrate queued sound playback and every RNG consumer in original order; record a startup and complete Training trace from equal saved RNG state and input/frame timing. | All reachable gameplay systems |
| P1 | Compare final animation/skinned vertices, scene output, restart persistence; package runnable assets/dependencies and later platform port. | Runtime parity; own retail inputs |

V1 and V3 are delegated to GPT-6 Luna in this pass; V2, source verification, integration, review and the ledger remain with the lead. Source discovery may proceed in parallel on disjoint files. Shared main.cpp, trackers and commits have one integration owner. Do not mark a transaction complete at whole-level scope merely because isolated tests pass.

## Milestone validation and delivery

For the victim transaction milestone: build the native app; run standalone transaction/order checks, existing pickup geometry/message checks, startup and actor checks; run normal rendering validation when available. Record which checks compare original execution and which assert static-source behavior. Update the checklist, mechanics ledger and handoff, then commit/push decomp-only changes. Leave unrelated fire_spread/ work untouched.

For subsequent milestones: rerun affected original fixtures, compare integrated event and RNG transcripts, and stop at the first unexplained divergence. Never compensate for unknown consumers with discarded draws. Check account usage during extended work and before new delegation; pause when either five-hour or weekly remaining allowance reaches 1%.
