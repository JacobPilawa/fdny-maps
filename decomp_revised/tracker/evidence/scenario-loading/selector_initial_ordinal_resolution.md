# Level selector initial ordinal resolution

## Scope and pinned input

This static trace uses the pinned `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The selector scene/open route and relevant instructions were checked in raw x86 bytes; no live menu capture was used. Constructor behavior and the shared collection lookup are also documented in [the level-to-EVS trace](level_to_evs_static_trace.md).

## Scene activation path

Scene constructor `0042DEC0` allocates the selector object (`0x3D8` bytes), calls constructor `0042F670`, and stores it at child-array index 2. Constructor `0042F670` installs the selector vtable but does not write the selection field at `this+0x2C`.

Scene switcher `0042D3E0` sends the newly selected child a call through vtable slot 0 with argument `1`; it sends argument `0` to the prior child. The selector table at `004DFD14` maps slot 0 to `0042F600`, so opening child 2 invokes `0042F600(this,1)`. This is the path that seeds the selection.

## Selection sources

On activation, `0042F600` first calls base method `00435DF0`. If `app+0x6EC` is zero and `app+0x688` is non-null, it reads the record ordinal from `*(app+0x688)+0x18` and calls `0042F280(this,ordinal)`. That helper resolves the ordinal against the shared level collection and stores the matched record's ordinal at selector `+0x2C`.

If that active-record branch is unavailable or gated off, `0042F600` falls back to candidate probing through `0042F280`. It starts from the value read at `*(app+0x6E4)+4`, plus one, and decrements until a valid record resolves. The first valid candidate is stored as the current ordinal. The meaning of the `+0x6E4` object is not assigned here.

Startup `0042B4E0` calls `0042DEC0` and then `0042D480`; the latter does not seed selector `+0x2C`. The menu switcher's child-activation callback does. Which selection source is taken on any particular opening depends on the two app-state conditions above; no runtime capture was made.

## Evidence and limits

The pinned raw-copy instructions establish selector child index 2, its constructor call, the scene switcher's slot-0 activation, the selector vtable target, the `0042F600` branch conditions, and the call into `0042F280`. Existing same-hash pseudocode in the scenario-loading evidence establishes that `0042F280` resolves a catalog record and writes its ordinal to `+0x2C`. The shared collection identity and zero-based parser ordinals are documented in [catalog collection owner resolution](catalog_collection_owner_resolution.md).

This closes the initial menu ordinal writer statically. It does not establish which app-state branch a live menu uses or whether the fallback field contains a menu index with any particular higher-level meaning.
