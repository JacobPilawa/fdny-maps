# Training module delivery roadmap

Updated 2026-10-12. This is the module-level view of the work; the [checklist](CHECKLIST.md), [mechanics ledger](mechanics.json), and linked evidence keep the address-level details.

## What currently runs

The native app runs a **playable Training scene prototype** with the original map, player spawn, movement, basic collision, doors, selected tool actions, and authored fire/victim visuals. It does **not** yet run the complete source-accurate Training mission. In particular, actor update cadence, several interaction paths, fire/victim lifecycles, mission services, and whole-run RNG order are not connected or verified.

There is no defensible overall completion percentage. Many small source routines already have strong comparisons, but those do not measure how much of a playable mission is integrated.

## Finish modules in this order

| Order | Module and current state | Existing files to finish | New code expected | Module is done when |
|---|---|---|---|---|
| 1 | **Runtime, startup, and actor schedule — prototype foundation** | `src/main.cpp`, `src/world_startup.h`, `src/startup.h`, `src/scripts.h`, `src/spawn_queue.h`, `src/clock.h`, `src/bmodel_contact.h` | One `src/training_schedule.h` coordinator if the proven source order cannot stay clear in `main.cpp`; one integrated start/update trace fixture and checker | Training's authored startup and recurring Player, victim, BModel, and FireRescue callbacks run in source order; script/contact/message latency and shared draw order match a controlled original trace. This is the shared dependency for later modules. |
| 2 | **Player movement and world interactions — partly playable** | `src/main.cpp`, `src/motion.h`, `src/core.h`, `src/bmodel_contact.h`, `src/bmodel_use.h`, `src/use_manager.h`, `src/interaction_box.h`, `src/throw_scalar.h`, door/ladder/liftable evidence | Only any missing adapter for the selected Training action; focused composed interaction fixture/checker | Axe, valve, liftable, and door/ladder actions reach their real source handlers from player input, with candidate order, carry/throw/destruction, event latency, and resulting frames checked as one playable interaction sequence. |
| 3 | **Fire response and tools — partial** | `src/fire_entity.h`, `src/fire_update.h`, `src/fire_size.h`, `src/fire_message_queue.h`, `src/fire_suppression.h`, `src/fire_contact.h`, `src/extinguisher_contacts.h`, `src/extinguisher_effects.h`, `src/extinguisher_pose.h`, all `src/hose_*.h`, `src/main.cpp` | A thin `src/fire_runtime.h` only if the composed update/message/presentation path needs a separate owner; integrated fire-response fixture/checker | Placed Training fires update on the real schedule; extinguisher and hose contacts change fire state and visuals, consume the correct charge/RNG, and stop or recover at source-accurate boundaries. Hose stream/follower is included only if the source proves a Training creation/use path. |
| 4 | **Victim lifecycle and mission flow — partial** | `src/victim_*.h`, `src/player_fire_*.h`, `src/player_passout.h`, `src/player_herodown_tail.h`, `src/zones.h`, `src/zone_membership.h`, `src/main.cpp` | A thin `src/victim_runtime.h` if event/carry/exit coordination cannot remain in the existing owners; one end-to-end rescue/failure fixture/checker | Victim movement, pickup, carry, rescue, passout, exit, counters, and mission outcome run through the actual authored Training flow and source-order event/callback sequence. |
| 5 | **Presentation and engine services — partial** | `src/fire_renderer.h`, `src/actor_renderer.h`, `src/actor_pose.h`, `src/actor_animation.h`, `src/sprite_animation.h`, `src/player_ash_*.h`, `src/sound_queue.h`, `src/ui.h`, `src/assets.h` | Add service adapters or renderer modules only where an existing interface is missing; retain asset decoders/tools under `tools/` | Training's visible actors, fire/smoke/particles, tools, audio, HUD, goggles, statistics, restart, and completion presentation follow the same gameplay state and timing. |
| 6 | **Whole-Training parity and runnable handoff — not verified** | All modules above; `decomp/run.sh`, `decomp/CMakeLists.txt`, `decomp/README.md`, `decomp/CHECKLIST.md`, `decomp/mechanics.json` | One repeatable authored Training replay/trace harness plus module-level fixtures for any remaining untested boundaries; bundle manifest if needed | A full run from matching RNG state, input, and frame timing reaches the same interactions, victims, fires, sounds/effects, mission result, and every shared RNG draw in order. A clean documented native build can run it using the user's retail assets. |

## How work is grouped from here

Finish one row before broadening the next: source behavior, connected C++ path, module-level scenario, comparison evidence, and a short handoff update. Keep isolated helper checks as support; they do not close a module. Preserve useful findings for the eventual full game, but defer unrelated map-specific routines until a Training module needs them.

The two latest delegated checks refined the first and third rows: FireRescue's local Think route is known but no recurring caller was proved (`evidence/fire-update/root-think-dispatch-query-20261012.md`), and ordinary level loading has no authored `RCS_PlayerHose`/`RCS_HosePath` instance in the preserved Training records. The latter does not rule out dynamic creation, so hose actor work stays conditional on finding a real Training path.

## Progress reporting

Use the module states above and `training-progress.html` for the at-a-glance status. Refresh the HTML viewer at completed module milestones, not for each helper or evidence-only finding. The detailed [checklist](CHECKLIST.md) remains the source of truth for remaining mechanics and [HANDOFF](HANDOFF.md) records the latest verified work.
