# Native normal-world actor render gate

`src/actor_render_flags.h` exposes `ActorRenderFlags::eligibleForNormalWorldRender(flags)`, a small pure predicate implementing the Genesis normal actor submission test `(flags & 0x1400) != 0`. The backing source analysis, exact binary hash, and scope limits are documented in [`actor-render-flags.md`](actor-render-flags.md).

`tools/actor_render_flags_check.cpp` is a standalone boundary check. From `decomp/`, run:

```sh
c++ -std=c++17 tools/actor_render_flags_check.cpp -o /tmp/actor-render-flags-check
/tmp/actor-render-flags-check
```

It checks both mask bits independently, their combination, unrelated flags, and the Training victim's initial `0x400` versus pickup's `0` state. The component intentionally contains no frame scheduling or rendering calls.

## Integration recommendation

At the actor submission point in the normal world pass, call the predicate using the same raw `worldActorFlags` word updated by victim state transactions. A false result should skip normal-pass submission; a true result should continue the existing draw path. For Training, this makes initial `0x400` render eligible and pickup's zero flags ineligible. Rescue restoration to `0x400` will restore eligibility when that transaction is implemented. Keep this wiring at the draw/submission boundary and use a separately verified schedule to decide when actor state is updated; this evidence does not establish a runtime schedule. The `0x1800` reflection gate is distinct and must not be inferred from this helper.
