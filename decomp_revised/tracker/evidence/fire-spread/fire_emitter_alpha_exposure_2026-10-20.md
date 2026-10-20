# Player fire exposure uses the flame alpha override (2026-10-20)

Status: source-backed scale mapping and shared-port integration; initial value and callback timing remain unresolved.

## Pinned input

- `inputs/original/binaries/FDNYFirefighter.exe`
- SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- The addresses below were checked against that local retail image with `objdump -d -Mintel` and the saved same-hash Ghidra exports. Constructor/allocation excerpts are preserved in [`fire_sprite_alpha_init_chain_2026-10-20.asm`](fire_sprite_alpha_init_chain_2026-10-20.asm).

## Trace

`004483A0` calls `0044A7A0` with the FireRescue visual subobject at root `+0x12C`. The falloff helper obtains the first child pointer from visual `+0x60`, then reads child `+0xD0` at `0044A829`. The value is therefore a dynamic first-flame-sprite field, not a fixed FireRescue root field.

The actor callback `00449FE0` receives the actor subobject at root `+0x24`; actor `+0x104` aliases root suppression strength `+0x128`. After recovery, its final strength branch uses `1.0` for strength at least `0.5`, otherwise doubles the strength, and passes the result to visual helper `0044A610`. The checked instructions at `0044A24B..0044A273` show the strength load, compare, double, and call.

`0044A610` compares the input against its upper limit and substitutes binary32 `0.999` when above it. It walks the four sprite pointers beginning at visual `+0x60` and calls `0047F1F0` for each. At `0047F1F4`, that setter stores the supplied float at child `+0xD0`; at `0047F1FA`, it enables the override byte at child `+0xD4`. The existing same-hash `2026-10-17_fire_sprite_color_paths.c` export shows the sprite renderer consuming the override. The separate glow child receives a separately multiplied alpha and is not the field read by `0044A7A0`.

Thus Player exposure's falloff multiplier is the first flame's alpha override:

```text
alpha = strength >= 0.5 ? 1.0 : 2.0 * strength
emitterAlpha = min(alpha, 0.999)
```

It modulates spatial exposure along with the separate suppression-strength intensity blend in `004483A0`; it is not an independent authored radius multiplier.

## Port consequence and limits

`PlayerFireContact::evaluate` now takes an explicitly named `emitterAlpha`, and the rescue host passes `FireEntity::alpha` for both the pre-visibility and obstructed calculations. `FireEntity::think` tracks the same strength-derived alpha after its host Fire Think update. This closes the prior unit-scale assumption for the mapped update path.

The constructor path resolves the two fields differently. FireRescue root constructor `0044A330` calls `0044B790`, which initializes the visual child-pointer slots; script setup `00448D60` later invokes `0044B270` to create three flame children and one glow child. Sprite constructor `0047F850` calls base setup `0047D370` and animated-sprite initializer `0047F500`. The latter explicitly initializes child `+0xD4` to zero at `0047F584`. The inspected constructors and descriptor setup do not initialize child `+0xD0`.

FireRescue allocates each child through `004A5A84`; the ordinary heap path reaches `004B7D5F` with allocation flags zero, so it does not request zero-filled memory. A pool path is also possible. The first explicit `+0xD0` write found on this FireRescue path is still the later `00449FE0` → `0044A610` → `0047F1F0` update. The raw constructor and allocator excerpts are saved in `fire_sprite_alpha_init_chain_2026-10-20.asm`.

Therefore the alpha value read by `0044A7A0` before that first update, whether any indirect or pooled-memory path writes it, and the actor callback's ordering relative to Player exposure are unresolved. The override flag starts disabled, but `0044A7A0` reads `+0xD0` directly without checking that flag. The host currently starts its alpha at 1.0 and follows its source-shaped update when its Fire Think runs; that initial timing is a host assumption, not a retail parity result. Do not treat the unresolved pre-update value as a confirmed retail bug. Callback cadence, x87 near-cutoff precision, and live damage feel are also open.

This is a static binary trace and a source-derived host integration. It does not establish runtime scheduling or retail gameplay parity.
