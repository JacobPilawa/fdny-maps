/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0046F6F0
 * Analysis output, not recovered original source.
 */

/* Algorithm:
   Find the ScriptActor event entry at this+8 using param_1 as the event key. If absent, allocate an
   event record and add param_2 as its first command. If present, clear its existing command list,
   then add param_2 as the replacement.

   Parameters:
   this is the ScriptActor; param_1 is the event key; param_2 is the parsed command to associate.

   Returns:
   Truth-like success value.

   Evidence:
   HandleCommonScriptActorCommand (0046F9D0) calls this for parsed onmsg declarations.
   ParseScriptActorCommandText (00474180) maps semicolon event syntax to onmsg.
   DispatchScriptActorEvent (004705B0) later looks up the key and routes its command list.

   Limits:
   This maps static handler registration and dispatch; it does not prove a specific contact event
   fires at runtime. */

undefined4 __thiscall SetScriptActorEventCommands(void *this,int *param_1,int *param_2)

{
  byte *pbVar1;
  int *this_00;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *local_10;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;

  piVar2 = param_1;
  puStack_8 = &LAB_004d24e8;
  local_c = ExceptionList;
  pbVar1 = (byte *)(param_1 + 3);
  local_4 = 1;
  ExceptionList = &local_c;
  local_10 = this;
  iVar3 = FUN_0046e210((void *)((int)this + 8),pbVar1);
  if ((iVar3 == 0) || (this_00 = (int *)(iVar3 + 0xc), this_00 == (int *)0x0)) {
    local_10 = _malloc(0x18);
    puVar4 = local_10;
    for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    }
    *local_10 = 1;
    local_10[3] = 1;
    local_4._0_1_ = 2;
    FUN_00407540(&local_10,&param_2);
    FUN_0046f670((void *)((int)this + 8),pbVar1,&local_10);
    local_4 = CONCAT31(local_4._1_3_,1);
    FUN_004075f0((int *)&local_10);
  }
  else {
    if (1 < *(int *)*this_00) {
      FUN_00405d00(this_00);
    }
    if (*(int *)(*this_00 + 4) != 0) {
      FUN_00407100(this_00,0);
    }
    FUN_00407540(this_00,&param_2);
  }
  local_4 = local_4 & 0xffffff00;
  if ((piVar2 != &DAT_00501908) && (iVar3 = *piVar2, *piVar2 = iVar3 + -1, iVar3 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  local_4 = 0xffffffff;
  if ((param_2 != &DAT_00501908) && (iVar3 = *param_2, *param_2 = iVar3 + -1, iVar3 + -1 < 1)) {
    param_2 = (int *)FUN_004011c0(&param_2);
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)param_2 >> 8),1);
}
