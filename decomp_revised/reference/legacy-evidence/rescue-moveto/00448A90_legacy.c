/* Ghidra decompilation; names/types are provisional.
 * Entry: 00448a90
 * Function: FUN_00448a90
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __thiscall
FUN_00448a90(void *this,int *param_1,float param_2,undefined4 param_3,undefined4 param_4)

{
  void *this_00;
  float fVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  float *pfVar6;
  float *pfVar7;
  undefined4 *puVar8;
  int *piVar9;
  undefined1 *local_7c [3];
  float local_70 [3];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  piVar2 = param_1;
  puStack_8 = &LAB_004cf910;
  local_c = ExceptionList;
  local_7c[0] = &stack0xffffff70;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  piVar9 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_1 = *param_1 + 1;
    piVar9 = param_1;
  }
  iVar3 = FUN_00470c50(&DAT_00502fb0,piVar9);
  uVar5 = 0;
  if (iVar3 != 0) {
    if (*(int *)((int)this + 0x100) != 0) {
      FUN_0049d350(this,local_7c);
      FUN_00442c10(*(int *)((int)this + 0x100));
    }
    uVar4 = FUN_0049d4a0((int *)(iVar3 + 0x28));
    uVar5 = *(uint *)((int)this + 0x100);
    if ((uVar5 == 0) || (uVar4 != uVar5)) {
      *(uint *)((int)this + 0x100) = uVar4;
      if (uVar4 != 0) {
        this_00 = (void *)((int)this + 300);
        pfVar6 = (float *)FUN_00447870(this_00,local_7c);
        pfVar7 = (float *)FUN_0043b2d0(local_70,(float *)(uVar4 + 0x4c),pfVar6);
        fVar1 = _DAT_004dc50c;
        pfVar6 = (float *)((int)this + 0xdc);
        *pfVar6 = *pfVar7;
        *(float *)((int)this + 0xe0) = pfVar7[1];
        *(float *)((int)this + 0xe4) = pfVar7[2];
        if (param_2 <= fVar1) {
          fVar1 = -param_2;
        }
        else {
          fVar1 = SQRT(*(float *)((int)this + 0xe4) * *(float *)((int)this + 0xe4) +
                       *(float *)((int)this + 0xe0) * *(float *)((int)this + 0xe0) +
                       *pfVar6 * *pfVar6) / param_2;
        }
        *(float *)((int)this + 0xfc) = fVar1;
        fVar1 = (float)_DAT_004dd808 /
                SQRT(*(float *)((int)this + 0xe4) * *(float *)((int)this + 0xe4) +
                     *(float *)((int)this + 0xe0) * *(float *)((int)this + 0xe0) + *pfVar6 * *pfVar6
                    );
        *pfVar6 = fVar1 * *pfVar6;
        *(float *)((int)this + 0xe0) = fVar1 * *(float *)((int)this + 0xe0);
        *(float *)((int)this + 0xe4) = fVar1 * *(float *)((int)this + 0xe4);
        puVar8 = (undefined4 *)FUN_00447870(this_00,local_70);
        *(undefined4 *)((int)this + 0xf0) = *puVar8;
        *(undefined4 *)((int)this + 0xf4) = puVar8[1];
        *(undefined4 *)((int)this + 0xf8) = puVar8[2];
        *(undefined4 *)((int)this + 0xb4) = param_4;
        *(undefined4 *)((int)this + 0xe8) = 0;
        *(undefined4 *)((int)this + 0xec) = 0;
        *(undefined4 *)((int)this + 0xb0) = param_3;
        iVar3 = FUN_0044ad70((int)this_00);
        *(undefined4 *)((int)this + 0x70) = 4;
        local_4 = 0xffffffff;
        if (piVar2 != &DAT_00501908) {
          iVar3 = *piVar2 + -1;
          *piVar2 = iVar3;
          if (iVar3 < 1) {
            iVar3 = FUN_004011c0(&param_1);
          }
        }
        ExceptionList = local_c;
        return CONCAT31((int3)((uint)iVar3 >> 8),1);
      }
    }
    else {
      local_64 = &PTR_FUN_004dc554;
      FUN_004a3118();
      local_60 = "rescue\\FireRescueEnt.cpp";
      local_5c = 699;
      local_58 = 0;
      local_4._0_1_ = 1;
      FUN_004a3a19(local_48,"ERROR! Recursive PathPoint.");
      FUN_00401360(&local_64,0);
      local_4 = (uint)local_4._1_3_ << 8;
      uVar5 = FUN_004012c0(&local_64);
    }
  }
  local_4 = 0xffffffff;
  if (piVar2 != &DAT_00501908) {
    uVar5 = *piVar2 - 1;
    *piVar2 = uVar5;
    if ((int)uVar5 < 1) {
      uVar5 = FUN_004011c0(&param_1);
    }
  }
  ExceptionList = local_c;
  return uVar5 & 0xffffff00;
}

