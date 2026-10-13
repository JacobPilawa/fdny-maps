/* Fresh Ghidra decompilation: BeginVictimPickup at 0x0045bef0;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The previous project contains many exports of this function from unrelated investigations.
   This fresh view isolates the pickup transition and the authored `pickup` event dispatch.
   */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall BeginVictimPickup(int param_1)

{
  int *piVar1;

  *(undefined4 *)(param_1 + 0x120) = 5;
  FUN_00481140((void *)(param_1 + 0x170),'\0');
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff4,"pickup");
  DispatchScriptActorEvent((void *)(param_1 + 0xd4),piVar1);
  return;
}
