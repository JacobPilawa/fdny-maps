# Training startup integration gate

`--training-startup-check` runs the production Training initialization path under a fixed, selectable seed and checks the pieces together. `--seed <uint32>` overrides the native shared RNG seed; without it, the existing host-monotonic-millisecond seed remains in use.

The recovered startup sequence is: the early `training.evs` definitions-only load; the world route registration; the BSP actor batch; the entire first initialization pass; the entire second pass; then four world updates before the full level script load. The queue's constructor and registration events interleave per entity, followed by all first-pass callbacks and then all second-pass callbacks. This follows original `004297E0` factory order, `0049E4D0` BSP grouping, `0049C670` staged queue order, and World startup `0049E240`/`0049E2C0`. `evidence/spawn-queue-oracle.json` and `evidence/world-startup-oracle.json` independently compare the generic queue and countdown semantics against original execution.

The composed gate checks that:

- all 72 currently admitted Training actor records preserve the chosen factory order through construction, registration, and both passes;
- the early EVS load defines `Door` without installing event bindings or dispatching host commands;
- all seven Training FireRescue entities initialize and resolve their generation-checked routes;
- both authored victims initialize sound and motion in pass one, then choose among three candidates and successfully floor-place in pass two;
- the full EVS activation occurs only on world update four, with no unsupported Training commands;
- the resulting Player and 20-door script state is present.

Reproducible native integration run:

```text
./build/fdny-training --training-startup-check --seed 0x46444e59
Training startup integration: seed=1178881625, entities=72, phase-events=288, fires=7, victims=2, doors=20, host-commands=54, RNG-draws=76, EVS-update=4, unsupported=0, mismatches=0 (recurring actor cadence remains unresolved)
```

This is a native regression baseline under an explicit seed, not a captured integrated original-run trace. It does not prove full Training RNG order or recurring Player, victim, BModel, or FireRescue callback order. The read-only Ghidra audit establishes their local virtual routes but no recurring caller; see [`scriptactor-common-scheduler-audit.md`](scriptactor-common-scheduler-audit.md), [`training-bmodel-contact-scheduling-audit.md`](training-bmodel-contact-scheduling-audit.md), and [`fire-update/root-think-dispatch-query-20261012.md`](fire-update/root-think-dispatch-query-20261012.md). A controlled original runtime trace is still required before adding a source-order frame coordinator.
