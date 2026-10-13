/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Applies ScriptActor context substitution, parses command text, and routes the resulting
   command.
   
   Algorithm: Store the explicit dispatch value at this+4, substitute `*ME` using the actor context
   at this+0x24, parse the command through ParseScriptActorCommandText, offer it to
   HandleCommonScriptActorCommand, and if unhandled call the derived vtable handler at slot +4. Log
   the source command when both handlers decline.
   
   Parameters: this is the ScriptActor; param_1 is the reference-counted command text; param_2 is
   the explicit dispatch value.
   
   Returns: void.
   
   Evidence: Fresh decompilation at 0x00470150; the context bridge
   DispatchScriptActorCommandWithContext (0x00475310) stores the current context at this+0x78 before
   calling this function. */

void __thiscall RouteParsedScriptActorCommand(void *this,int *param_1,int param_2)

{
  char cVar1;
  int *piVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  void *this_00;
  int *local_90;
  char local_89;
  int local_88 [9];
  undefined4 auStack_64 [22];
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_004d25b1;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(int *)((int)this + 4) = param_2;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_90 = &DAT_00501908;
  FUN_00401d80(&local_90,"*ME");
  local_4._0_1_ = 1;
  FUN_00406e80(&param_1,(int *)&local_90,(int *)((int)this + 0x24),'\x01');
  local_4._0_1_ = 0;
  if ((local_90 != &DAT_00501908) && (iVar4 = *local_90, *local_90 = iVar4 + -1, iVar4 + -1 < 1)) {
    FUN_004011c0(&local_90);
  }
  piVar2 = FUN_004052b0((int *)&param_1);
  if (*(char *)(*piVar2 + 0xc) == '\0') {
    if (param_1 == &DAT_00501908) {
      ExceptionList = local_c;
      return;
    }
    *param_1 = *param_1 + -1;
    iVar4 = *param_1;
  }
  else {
    puVar3 = (undefined4 *)FUN_0040c330(&param_1,&local_90);
    iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"<null>");
    local_89 = '\x01' - (iVar4 != 0);
    if (local_89 != '\0') {
      local_4 = 0xffffffff;
      if (param_1 == &DAT_00501908) {
        ExceptionList = local_c;
        return;
      }
      *param_1 = *param_1 + -1;
      if (0 < *param_1) {
        ExceptionList = local_c;
        return;
      }
      FUN_004011c0(&param_1);
      ExceptionList = local_c;
      return;
    }
    local_90 = (int *)&stack0xffffff64;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar2 = &DAT_00501908;
    if ((param_1 != &DAT_00501908) && (piVar2 = param_1, param_1 != &DAT_00501908)) {
      *param_1 = *param_1 + 1;
    }
    ParseScriptActorCommandText(local_88,piVar2);
    local_4._0_1_ = 2;
    uVar5 = HandleCommonScriptActorCommand(this,local_88);
    if ((char)uVar5 == '\0') {
      cVar1 = (**(code **)(*(int *)this + 4))();
      if (cVar1 == '\0') {
        iVar4 = 0;
        local_90 = (int *)&stack0xffffff60;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar2 = &DAT_00501908;
        if ((param_1 != &DAT_00501908) && (piVar2 = param_1, param_1 != &DAT_00501908)) {
          *param_1 = *param_1 + 1;
        }
        local_4._0_1_ = 3;
        puVar3 = FUN_00401290(auStack_64,"script\\scriptactor.cpp",0x7f);
        local_4._0_1_ = 5;
        FUN_004a3a19(puVar3 + 7,"Unknown script command: \"");
        this_00 = (void *)FUN_0040f020(puVar3,piVar2);
        FUN_004a3a19((void *)((int)this_00 + 0x1c),"\"");
        FUN_00401360(this_00,iVar4);
        local_4._0_1_ = 2;
        FUN_004012c0(auStack_64);
      }
      local_4 = (uint)local_4._1_3_ << 8;
      FUN_0046e950(local_88);
      local_4 = 0xffffffff;
      FUN_00401c90(&param_1);
      ExceptionList = local_c;
      return;
    }
    local_4 = (uint)local_4._1_3_ << 8;
    FUN_0046e950(local_88);
    if (param_1 == &DAT_00501908) {
      ExceptionList = local_c;
      return;
    }
    *param_1 = *param_1 + -1;
    iVar4 = *param_1;
  }
  local_4 = 0xffffffff;
  if (iVar4 < 1) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}
