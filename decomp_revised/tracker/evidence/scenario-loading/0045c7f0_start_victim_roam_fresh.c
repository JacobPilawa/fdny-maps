/* Fresh Ghidra decompilation: StartVictimRoam at 0x0045c7f0; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* The victim roam command stores its parsed float at +0x224, sets state +0x120 to 7, copies the
   current location into target +0x1C8, initializes +0x228 to 36.0, builds nearby obstacles, and
   tests four diagonal offsets at +/-5. If all fail it logs that the victim is stuck and sets state
   4; otherwise it requests a victim animation. */

undefined4 __thiscall StartVictimRoam(void *this,int param_1)

{
  uint uVar1;
  void *pvVar2;
  undefined4 uVar3;
  int *extraout_ECX;
  int *extraout_ECX_00;
  int *piVar4;
  int *piVar5;
  int iVar6;
  float local_9c [12];
  undefined1 *local_6c;
  undefined1 *local_68;
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0fe8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(int *)((int)this + 0x224) = param_1;
  *(undefined4 *)((int)this + 0x120) = 7;
  *(undefined4 *)((int)this + 0x1c8) = *(undefined4 *)((int)this + 0x4c);
  *(undefined4 *)((int)this + 0x1cc) = *(undefined4 *)((int)this + 0x50);
  *(undefined4 *)((int)this + 0x1d0) = *(undefined4 *)((int)this + 0x54);
  *(undefined4 *)((int)this + 0x228) = 0x42100000;
  BuildVictimRoamObstacleList((int)this);
  local_9c[0] = 5.0;
  local_9c[1] = 0.0;
  local_9c[2] = 5.0;
  uVar1 = ValidateVictimRoamCandidate(this,local_9c);
  if ((char)uVar1 == '\0') {
    local_9c[3] = 5.0;
    local_9c[4] = 0.0;
    local_9c[5] = -5.0;
    uVar1 = ValidateVictimRoamCandidate(this,local_9c + 3);
    if ((char)uVar1 == '\0') {
      local_9c[6] = -5.0;
      local_9c[7] = 0.0;
      local_9c[8] = 5.0;
      uVar1 = ValidateVictimRoamCandidate(this,local_9c + 6);
      if ((char)uVar1 == '\0') {
        local_9c[9] = -5.0;
        local_9c[10] = 0.0;
        local_9c[0xb] = -5.0;
        uVar1 = ValidateVictimRoamCandidate(this,local_9c + 9);
        if ((char)uVar1 == '\0') {
          iVar6 = 0;
          local_6c = &stack0xffffff50;
          piVar5 = extraout_ECX;
          FUN_00454430(this,(undefined4 *)&stack0xffffff50);
          local_68 = &stack0xffffff4c;
          local_4 = 0;
          piVar4 = extraout_ECX_00;
          (**(code **)(*(int *)this + 0x60))(&stack0xffffff4c);
          local_4._0_1_ = 1;
          ppuStack_64 = &PTR_FUN_004dc554;
          FUN_004a3118();
          pcStack_60 = "rescue\\VictimRescueEnt.cpp";
          uStack_5c = 0x2d7;
          uStack_58 = 0;
          local_4._0_1_ = 3;
          FUN_004a3a19(auStack_48,"Victim ");
          pvVar2 = (void *)FUN_0040f020(&ppuStack_64,piVar4);
          local_4 = CONCAT31(local_4._1_3_,4);
          FUN_004a3a19((void *)((int)pvVar2 + 0x1c)," (");
          pvVar2 = (void *)FUN_0040f020(pvVar2,piVar5);
          FUN_004a3a19((void *)((int)pvVar2 + 0x1c),") is stuck when spawned!");
          FUN_00401360(pvVar2,iVar6);
          local_4 = 0xffffffff;
          uVar3 = FUN_004012c0(&ppuStack_64);
          *(undefined4 *)((int)this + 0x120) = 4;
          goto LAB_0045c9f3;
        }
      }
    }
  }
  uVar3 = SelectVictimRoamAnimation(this,2);
LAB_0045c9f3:
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)uVar3 >> 8),1);
}
