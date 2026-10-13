/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 00477AF0
 * Analysis output, not recovered original source.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   Copy the current ScriptActor name from this+0x24 and call HandleActorSpatialSoundCommand
   (004778C0) with the parsed sound command and a null optional parameter. This is the `playsnd`
   path used by the generic script command dispatcher.

   Parameters:
   this is the ScriptActor executing the command; param_1 is the parsed command data.

   Returns:
   Truth-like handled result.

   Evidence:
   The `playsnd` branch in script command handler 0047A1F0 calls this method. The actor context and
   sound string are forwarded to 004778C0.

   Limits:
   The command-to-position path is static evidence; this does not prove a successful run or audible
   mix. */

uint __thiscall HandlePlaySoundCommand(void *this,int *param_1)

{
  undefined1 uVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  undefined1 *puVar6;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;

  puStack_8 = &LAB_004d2d10;
  local_c = ExceptionList;
  puVar6 = (undefined1 *)0x0;
  local_4 = 0;
  ExceptionList = &local_c;
  piVar5 = this;
  FUN_00401cb0(&stack0xffffffdc,(undefined4 *)((int)this + 0x24));
  piVar2 = param_1;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar4 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + 1;
    piVar4 = param_1;
  }
  local_4 = local_4 & 0xffffff00;
  iVar3 = HandleActorSpatialSoundCommand(this,piVar4,piVar5,puVar6);
  uVar1 = (undefined1)iVar3;
  local_4 = 0xffffffff;
  if (piVar2 != &DAT_00501908) {
    iVar3 = *piVar2 + -1;
    *piVar2 = iVar3;
    if (iVar3 < 1) {
      iVar3 = FUN_004011c0(&param_1);
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)iVar3 >> 8),uVar1);
}
