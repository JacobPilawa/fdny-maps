/* Fresh Ghidra decompilation, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 004705B0 (DispatchScriptActorEvent)
 * The pseudocode and listing are analysis output, not recovered original source.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Looks up a ScriptActor event key, then routes each command associated with that event
   through RouteParsedScriptActorCommand. BModel's use action calls this with the event name `use`.
   Evidence: fresh decompilation at 0x004705B0 and BModel handler 0x0043D2B0. The array walk is
   static; runtime activation/cadence remain unknown. */

uint __thiscall DispatchScriptActorEvent(void *this,int *param_1)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int iVar8;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar6 = param_1;
  puStack_8 = &LAB_004d25e8;
  local_c = ExceptionList;
  iVar4 = 0;
  local_4 = 0;
  ExceptionList = &local_c;
  uVar2 = FUN_0046e210((void *)((int)this + 8),(byte *)(param_1 + 3));
  if ((uVar2 == 0) || (piVar1 = (int *)(uVar2 + 0xc), piVar1 == (int *)0x0)) {
    local_4 = 0xffffffff;
    if (piVar6 != &DAT_00501908) {
      uVar2 = *piVar6 - 1;
      *piVar6 = uVar2;
      if ((int)uVar2 < 1) {
        uVar2 = FUN_004011c0(&param_1);
      }
    }
    uVar2 = uVar2 & 0xffffff00;
  }
  else {
    iVar3 = *(int *)(*piVar1 + 4);
    if (0 < iVar3) {
      iVar5 = 0x14;
      do {
        iVar8 = 0;
        if ((iVar5 < 0x14) || (iVar3 <= iVar4)) {
          piVar6 = &local_24;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar6 = 0;
            piVar6 = piVar6 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x66;
          FUN_00422060(&local_24);
        }
        if (1 < *(int *)*piVar1) {
          FUN_00405d00(piVar1);
        }
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar6 = *(int **)(*piVar1 + iVar5);
        piVar7 = &DAT_00501908;
        if (piVar6 != &DAT_00501908) {
          *piVar6 = *piVar6 + 1;
          piVar7 = piVar6;
        }
        RouteParsedScriptActorCommand(this,piVar7,iVar8);
        iVar3 = *(int *)(*piVar1 + 4);
        iVar4 = iVar4 + 1;
        iVar5 = iVar5 + 4;
        piVar6 = param_1;
      } while (iVar4 < iVar3);
    }
    local_4 = 0xffffffff;
    if (piVar6 != &DAT_00501908) {
      iVar3 = *piVar6 + -1;
      *piVar6 = iVar3;
      if (iVar3 < 1) {
        iVar3 = FUN_004011c0(&param_1);
      }
    }
    uVar2 = CONCAT31((int3)((uint)iVar3 >> 8),1);
  }
  ExceptionList = local_c;
  return uVar2;
}

