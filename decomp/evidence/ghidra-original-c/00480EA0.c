/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00480ea0
 * Match: function entry
 * Entry: 00480ea0 FUN_00480ea0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_00480ea0(void *this,int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  undefined4 *this_00;
  undefined ***pppuVar3;
  ulonglong uVar4;
  float local_c4;
  float local_c0;
  undefined **local_bc;
  char *local_b8;
  undefined4 local_b4;
  undefined1 local_b0;
  undefined1 local_a0 [60];
  undefined **local_64 [22];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3543;
  local_c = ExceptionList;
  if (((param_1 < 0) && (param_1 = *(int *)((int)this + 0x18), param_1 < 0)) ||
     (*(int *)((int)this + 0x10) <= param_1)) {
    local_bc = &PTR_FUN_004dc554;
    ExceptionList = &local_c;
    FUN_004a3118();
    local_b8 = "basic\\ActorAnimator.cpp";
    local_b4 = 0x2c;
    local_b0 = 0;
    local_4 = 0;
    FUN_004a3a19(local_a0,"Invalid motion ID");
    FUN_00401360(&local_bc,0);
    pppuVar3 = &local_bc;
  }
  else {
    ExceptionList = &local_c;
    iVar1 = geActor_GetMotionByIndex(*(undefined4 *)((int)this + 0xc),param_1);
    if (iVar1 == 0) {
      ExceptionList = local_c;
      return;
    }
    iVar2 = geMotion_GetTimeExtents(iVar1,&local_c4,&local_c0);
    if (iVar2 != 0) {
      *(undefined4 *)((int)this + 0x24) = param_2;
      *(float *)((int)this + 0x1c) = local_c4;
      *(float *)((int)this + 0x20) = local_c0 - local_c4;
      *(int *)((int)this + 0x14) = iVar1;
      *(int *)((int)this + 0x18) = param_1;
      *(undefined4 *)((int)this + 0x28) = DAT_00502afc;
      uVar4 = FUN_004b9400();
      *(int *)((int)this + 0x2c) = (int)uVar4;
      *(undefined1 *)((int)this + 0x30) = 1;
      ExceptionList = local_c;
      return;
    }
    this_00 = FUN_00401290(local_64,"basic\\ActorAnimator.cpp",0x35);
    local_4 = 1;
    FUN_004a3a19(this_00 + 7,"Error retrieving time extents for motion");
    FUN_00401360(this_00,0);
    pppuVar3 = local_64;
  }
  local_4 = 0xffffffff;
  FUN_004012c0(pppuVar3);
  ExceptionList = local_c;
  return;
}


