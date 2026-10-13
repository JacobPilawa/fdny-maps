/* Fresh Ghidra decompilation: BuildVictimRoamObstacleList at 0x0045c4d0; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Builds the victim's obstacle array from world entities with kind field 6. It compares entity
   position to the victim's +0x4C location using obstacle radius plus the stored roam value, records
   obstacle centers and an expanded squared radius, and logs when the post-insertion count exceeds
   9. Exact entity-kind semantics remain open. */

void __fastcall BuildVictimRoamObstacleList(int param_1)

{
  int *this;
  undefined4 *puVar1;
  int iVar2;
  float *pfVar3;
  int *piVar4;
  float fStack_8c;
  int local_88;
  undefined4 *local_84;
  float afStack_80 [3];
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  float fStack_68;
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  uint local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0fc3;
  pvStack_c = ExceptionList;
  this = (int *)(param_1 + 0x1c4);
  ExceptionList = &pvStack_c;
  if (1 < **(int **)(param_1 + 0x1c4)) {
    ExceptionList = &pvStack_c;
    FUN_0045a6c0(this);
  }
  if (*(int *)(*this + 4) != 0) {
    FUN_0045b520(this,0);
  }
  FUN_00481c00(&local_88);
  local_4 = 0;
  puVar1 = local_84;
  while( true ) {
    if ((local_88 == 0) || (puVar1 == (undefined4 *)0x0)) {
      piVar4 = (int *)0x0;
    }
    else {
      piVar4 = (int *)*puVar1;
    }
    if ((local_88 != 0) && (puVar1 != (undefined4 *)0x0)) {
      puVar1 = (undefined4 *)puVar1[1];
      local_84 = puVar1;
    }
    if (piVar4 == (int *)0x0) break;
    if (piVar4[6] == 6) {
      iVar2 = (**(code **)(*piVar4 + 0xc))();
      fStack_8c = *(float *)(iVar2 + 0xc);
      if ((fStack_8c < (float)_DAT_004e24f0) || ((float)_DAT_004e24e8 < fStack_8c)) {
        ppuStack_64 = &PTR_FUN_004dc554;
        FUN_004a3118();
        pcStack_60 = "rescue\\VictimRescueEnt.cpp";
        uStack_5c = 0xd8;
        uStack_58 = 0;
        local_4._0_1_ = 1;
        FUN_004a3a19(auStack_48,"Invalid bounding radius for obstacle!");
        FUN_00401360(&ppuStack_64,0);
        local_4 = (uint)local_4._1_3_ << 8;
        FUN_004012c0(&ppuStack_64);
        if (_DAT_004e24bc <= fStack_8c) {
          if (_DAT_004e24b8 < fStack_8c) {
            fStack_8c = 1200.0;
          }
        }
        else {
          fStack_8c = 24.0;
        }
      }
      pfVar3 = (float *)(param_1 + 0x4c);
      iVar2 = (**(code **)(*piVar4 + 0x18))();
      pfVar3 = (float *)FUN_0043b2d0(afStack_80,(float *)(iVar2 + 0x28),pfVar3);
      puVar1 = local_84;
      if (SQRT(pfVar3[2] * pfVar3[2] + pfVar3[1] * pfVar3[1] + *pfVar3 * *pfVar3) <=
          fStack_8c + *(float *)(param_1 + 0x224)) {
        iVar2 = (**(code **)(*piVar4 + 0x18))();
        uStack_74 = *(undefined4 *)(iVar2 + 0x28);
        uStack_70 = *(undefined4 *)(iVar2 + 0x2c);
        uStack_6c = *(undefined4 *)(iVar2 + 0x30);
        fStack_68 = (fStack_8c + _DAT_004e0bbc) * (fStack_8c + _DAT_004e0bbc);
        FUN_0045b460(this,&uStack_74);
        puVar1 = local_84;
        if (9 < *(int *)(*this + 4)) {
          ppuStack_64 = &PTR_FUN_004dc554;
          FUN_004a3118();
          pcStack_60 = "rescue\\VictimRescueEnt.cpp";
          uStack_5c = 0xe9;
          uStack_58 = 0;
          local_4 = CONCAT31(local_4._1_3_,2);
          FUN_004a3a19(auStack_48,"Max obstacles exceeded for \"");
          FUN_004a3a19(auStack_48,*(char **)(*(int *)(param_1 + 0x150) + 0x10));
          FUN_004a3a19(auStack_48,"\" -- decrease radius!");
          iVar2 = 0;
LAB_0045c79a:
          FUN_00401360(&ppuStack_64,iVar2);
          local_4 = local_4 & 0xffffff00;
          FUN_004012c0(&ppuStack_64);
          local_4 = 0xffffffff;
          if (local_88 != 0) {
            FUN_00429fe0((void *)(local_88 + 0x28),(int)&local_88);
          }
          ExceptionList = pvStack_c;
          return;
        }
      }
    }
  }
  ppuStack_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  pcStack_60 = "rescue\\VictimRescueEnt.cpp";
  uStack_5c = 0xef;
  uStack_58 = 0;
  iVar2 = *(int *)(*this + 4);
  local_4 = CONCAT31(local_4._1_3_,3);
  FUN_004a3a19(auStack_48,"\"");
  FUN_004a3a19(auStack_48,*(char **)(*(int *)(param_1 + 0x150) + 0x10));
  FUN_004a3a19(auStack_48,"\": ");
  FUN_004a3f7a(auStack_48,iVar2);
  FUN_004a3a19(auStack_48," obstacles found");
  iVar2 = 3;
  goto LAB_0045c79a;
}
