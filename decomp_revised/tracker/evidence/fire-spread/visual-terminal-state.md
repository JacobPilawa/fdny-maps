# FireRescue terminal-mode visual update audit

Status: fresh static Ghidra trace; runtime presentation still unresolved.

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

This proves that terminal mode stops this particular actor callback from refreshing those visual properties. It does **not** prove that flames disappear immediately, persist indefinitely, or stop rendering: the consumer/lifecycle of the child systems and any other render-side terminal-state check have not been traced, and the callback's external scheduling/render ordering is still open.

## Corrected field interpretation

The terminal suppression body clears one byte at root `+0x1C8` and separately sets recently-sprayed byte `+0x104` on a non-mode-5 update. Do not describe `+0x1C8` as the recently-sprayed flag. The legacy audit called it flame-enabled, but the currently confirmed instructions only establish that this byte is cleared; its field meaning remains unverified.

## Port consequence

The prior C++ `FireEntity::spray` behavior that immediately disables `FireVisual` animations is a port-side approximation. The mapped source supports a mode transition and skipped actor updates, but it does not yet establish the exact visible fade/removal behavior. Keep the native behavior tagged as an open parity item until the render/lifecycle path is mapped or captured.

## Ghidra names saved

- `0x00449FE0` — `UpdateFireRescueActor`
- `0x0044A610` — `ApplyFireRescueSpriteAlpha`
- `0x0044A6E0` — `DisableFireRescueSprites`
- `0x0044B060` — `UpdateFireRescueVisualEffects`

The assembly excerpt above is from a fresh disassembly of the pinned executable; prior analysis did not establish the runtime callback interval or visible terminal effect.
