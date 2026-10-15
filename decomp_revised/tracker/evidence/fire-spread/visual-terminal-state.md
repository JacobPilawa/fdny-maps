# FireRescue terminal-mode visual update audit

Status: fresh static Ghidra trace; terminal component gating is ported, while visible removal timing remains unresolved.

## Plain-language explanation

When a fire is fully suppressed through the terminal path, retail switches it to mode 5 and turns off its visual component. The fire update callback then stops refreshing the flame children and their polygons. The source does not destroy those children, set their alpha to zero, or establish exactly when polygons already submitted disappear.

## Pinned evidence

- Input: `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Active Ghidra program: `FDNYFirefighter.exe`; saved after adding address-specific names and prototypes for the update helpers.
- Fire suppression: [`00448970_fresh.c`](00448970_fresh.c) and [`00448970_suppression.asm`](00448970_suppression.asm).
- FireRescue actor callback: [`00449FE0_fresh.c`](../rescue-moveto/00449FE0_fresh.c), also the existing `UpdateFireRescueActor` review entry.

## Confirmed state and update relationship

`ApplyFireSuppressionContact` (`0x00448970`) writes terminal state 5 to the FireRescue root at `+0x70`. `UpdateFireRescueActor` receives the embedded actor subobject at root `+0x24`, so its state field at actor `+0x4C` aliases root `+0x70`. Fresh assembly begins its callback with:

```asm
00449fe6: CMP dword ptr [ESI + 0x4c],0x5
00449fea: JZ  0x0044a28a
```

The target is the callback epilogue. When state is 5, this updater returns before its ordinary work: it does not advance the FireRescue visual-effects helper, process the strength-to-color/size branch, or call the sprite-alpha helper. The normal nonterminal path reaches `UpdateFireRescueVisualEffects` (`0x0044B060`) and `ApplyFireRescueSpriteAlpha` (`0x0044A610`). The alpha helper updates the three fire sprites and the glow child; the glow receives a separate scaled alpha. `DisableFireRescueSprites` (`0x0044A6E0`) disables the four child systems' `+0xCC` flag when the callback's strength threshold branch selects it.

`0044B270` initializes the visual component enable byte at component `+0x9C` to 1. The FireRescue root byte at `+0x1C8` aliases this field. `0044B060` returns immediately when that byte is zero; when enabled, it advances the visual track and visits four child records through `0047F960`, which can submit polygons through `geWorld_AddPolyOnce`.

Terminal mode therefore stops future component updates and child polygon submissions through this path. It does not clear the child active bits, destroy the child systems, set alpha to zero, or establish how long already-submitted polygons remain visible. `0044A6E0` clears the children’s `+0xCC` position-override flag, not their active state; terminal mode skips the actor path that invokes it. External callback/render ordering remains open.

## Corrected field interpretation

The terminal suppression body clears root byte `+0x1C8`, which aliases the visual component enable byte at `+0x9C`. It separately sets recently-sprayed byte `+0x104` on a non-mode-5 update. Keep these fields distinct.

## Port consequence

`FireEntity::spray` now mirrors the source-backed component byte into `FireVisual.enabled`. This stops the native renderer and animation updater from submitting future fire child sprites after terminal suppression. It does not synthesize child destruction, an alpha fade, or immediate removal of already-submitted geometry.

## Host verification

The `fdny-training` target builds. Its `--fire-suppression-check` matched all 2,160 calls in the preserved legacy oracle (SHA-256 `8b648a95d113f94fe734ddd7215803958636fcf3a01338c661da844da519f615`), including the raw flag and its mapping into `FireVisual.enabled`. The original oracle was copied to a temporary asset root because the old default `evidence/fire-suppression-oracle.json` path is absent from this checkout. The packaged rescue-all viewer was also rebuilt with executable SHA-256 `ecfb5374652253ade1128a5576e74b5ca69760e0b1ed752fa52700bea480d179`; its Bank and Gas R rescue checks passed. `--fire-render-check` could not create an SDL display in the unattended shell (`The video driver did not add any displays`), so no rendered terminal-state image was captured.

## Address-specific mappings saved in Ghidra

- `0x00449FE0` — `UpdateFireRescueActor`
- `0x0044B270` — FireRescue visual component initializer (sets component `+0x9C` to 1)
- `0x0044A610` — `ApplyFireRescueSpriteAlpha`
- `0x0044A6E0` — `DisableFireRescueSprites`
- `0x0044B060` — `UpdateFireRescueVisualEffects`
- `0x0047F960` — FireRescue child visual update/polygon submission helper

The assembly excerpt above is from a fresh disassembly of the pinned executable; prior analysis did not establish the runtime callback interval or visible terminal effect.
