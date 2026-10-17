/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __fastcall FUN_0042d460(void *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)((int)param_1 + 0x3c);
  *piVar1 = *piVar1 + -2;
  if (*piVar1 < 0) {
    *(undefined4 *)((int)param_1 + 0x3c) = 0;
  }
  FUN_0042d3e0(param_1,*(int *)((int)param_1 + *(int *)((int)param_1 + 0x3c) * 4 + 0x14));
  return;
}
