/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 004A12F0
 * Function: ConfigureAndRegisterSoundInstance
 * Analysis output, not recovered original source.
 */

/* Replace the sound instance's resource reference if needed; store source-position pointer and
   mode, then register the instance with the active sound manager. Fresh EXE pseudocode, 2026-10-13.
    */

void __thiscall
ConfigureAndRegisterSoundInstance(void *this,undefined4 *param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  int *piVar2;

  puVar1 = (undefined4 *)((int)this + 0x20);
  if (*(int *)(*(int *)((int)this + 0x20) + 4) != 0) {
    FUN_0049f340(this);
    piVar2 = (int *)*puVar1;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    if ((DAT_00503a78 != 0) && (*(void **)(DAT_00503a78 + 8) != (void *)0x0)) {
      thunk_FUN_00429fe0(*(void **)(DAT_00503a78 + 8),(int)this);
    }
  }
  piVar2 = (int *)*puVar1;
  if (piVar2 != (int *)*param_1) {
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    piVar2 = (int *)*param_1;
    *puVar1 = piVar2;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + 1;
    }
  }
  *(int *)((int)this + 0x10) = param_2;
  *(int *)((int)this + 0x14) = param_3;
  FUN_004a0b10(*(void **)(DAT_00503a78 + 8),this);
  return;
}
