/* Fresh Ghidra decompilation, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0043D2B0 (DispatchBModelUseEvent)
 * The pseudocode and listing are analysis output, not recovered original source.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Handles the selected BModel use action. The secondary interface receiver is owner+0xA0.
   If useMode at owner+0x258 equals 1, it first calls the Player lift/carry transition; it then
   dispatches the `use` ScriptActor event on owner+0x24 for all modes. Gas R button1 has a use event
   and G_Liftable=0, so the static path reaches event dispatch without the mode-1 carry call.
   Evidence: fresh decompilation and raw x86; see Gas R use-path evidence. Limits: event activation
   and runtime cadence are not captured. */

void __fastcall DispatchBModelUseEvent(int param_1)

{
  undefined1 *puStack00000004;
  int *piVar1;
  
  if (*(int *)(param_1 + 0x1b8) == 1) {
    FUN_0044bc40(*(void **)(DAT_00503a78 + 0x30),param_1 + -0xa0);
  }
  puStack00000004 = &stack0xfffffff8;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff8,"use");
  DispatchScriptActorEvent((void *)(param_1 + -0x7c),piVar1);
  return;
}

