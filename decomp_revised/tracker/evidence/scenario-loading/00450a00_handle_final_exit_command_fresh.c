/* Fresh Ghidra MCP decompilation refreshed 2026-10-13.
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

/* Algorithm:
   - Lazily resolve the total victim count and compute total minus rescued minus the required
   threshold.
   - When the result is zero, set Player +0x3438 and call 0x0044C360.
   - Otherwise pass the remaining count and 7.0 to 0x004564C0.

   Parameters:
   - `param_1`: Player instance.

   Returns:
   - Nonzero low-byte status when the final-exit condition passes; otherwise forwards the helper
   status. */

int __fastcall HandleFinalExitCommand(void *param_1)

{
  undefined4 uVar1;
  uint uVar2;
  int *piVar3;

  if (*(int *)((int)param_1 + 0x3528) == -1) {
    FUN_0044fba0(*(int *)(DAT_00503a78 + 0x30));
  }
  piVar3 = (int *)((*(int *)((int)param_1 + 0x3528) - *(int *)((int)param_1 + 0x3530)) -
                  *(int *)((int)param_1 + 0x352c));
  if (piVar3 == (int *)0x0) {
    *(undefined4 *)((int)param_1 + 0x3438) = 1;
    uVar1 = FUN_0044c360(param_1);
    return CONCAT31((int3)((uint)uVar1 >> 8),1);
  }
  FUN_00483f20((void *)((int)param_1 + 0x5d8),(undefined4 *)&stack0xfffffff4);
  uVar2 = FUN_004564c0((void *)((int)param_1 + 0x5f4),7.0,piVar3);
  return uVar2 & 0xffffff00;
}
