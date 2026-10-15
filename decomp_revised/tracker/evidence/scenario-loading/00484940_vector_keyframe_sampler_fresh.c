/* Fresh read-only Ghidra 12.0.4 decompilation, 2026-10-15.
 * FDNYFirefighter.exe SHA-256:
 * 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function 00484940; current project input is the pinned retail executable.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * __thiscall FUN_00484940(void *this,int *param_1,float param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  ulonglong uVar5;
  float local_88;
  int local_84;
  char *local_80;
  char *local_7c;
  int local_78;
  float local_6c;
  float local_68;
  float local_64;
  int local_60;
  char *local_5c;
  char *local_58;
  int local_54;
  int local_48;
  char *local_44;
  char *local_40;
  undefined4 local_3c;
  int local_30;
  char *local_2c;
  char *local_28;
  undefined4 local_24;
  int local_18;
  char *local_14;
  char *local_10;
  int local_c;

  iVar3 = *(int *)(*(int *)((int)this + 4) + 4);
  iVar2 = 0;
  if (iVar3 != 1 && -1 < iVar3 + -1) {
    iVar3 = 0x14;
    do {
      if ((iVar3 < 0x14) || (*(int *)(*(int *)((int)this + 4) + 4) <= iVar2)) {
        piVar4 = &local_84;
        for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_84 = 1;
        local_80 = "Array index out of bounds";
        local_7c = "..\\BluelAPI/Container/SArray.h";
        local_78 = 0x66;
        FUN_00422060(&local_84);
      }
      if (param_2 < *(float *)(*(int *)((int)this + 4) + iVar3)) break;
      iVar2 = iVar2 + 1;
      iVar3 = iVar3 + 4;
    } while (iVar2 < *(int *)(*(int *)((int)this + 4) + 4) + -1);
  }
  iVar3 = iVar2 + -1;
  if (iVar3 < 0) {
    iVar3 = 0;
  }
  if (*(int *)(*(int *)((int)this + 4) + 4) <= iVar3) {
    piVar4 = &local_84;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_84 = 1;
    local_80 = "Array index out of bounds";
    local_7c = "..\\BluelAPI/Container/SArray.h";
    local_78 = 0x66;
    FUN_00422060(&local_84);
  }
  local_6c = *(float *)(*(int *)((int)this + 4) + 0x14 + iVar3 * 4);
  if ((iVar2 < 0) || (*(int *)(*(int *)((int)this + 4) + 4) <= iVar2)) {
    piVar4 = &local_60;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_60 = 1;
    local_5c = "Array index out of bounds";
    local_58 = "..\\BluelAPI/Container/SArray.h";
    local_54 = 0x66;
    FUN_00422060(&local_60);
  }
  local_68 = *(float *)(*(int *)((int)this + 4) + 0x14 + iVar2 * 4);
  if ((iVar3 < 0) || (*(int *)(*(int *)((int)this + 4) + 4) <= iVar3)) {
    piVar4 = &local_18;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_18 = 1;
    local_14 = "Array index out of bounds";
    local_10 = "..\\BluelAPI/Container/SArray.h";
    local_c = 0x66;
    FUN_00422060(&local_18);
  }
  local_88 = (param_2 - local_6c) /
             (local_68 - *(float *)(*(int *)((int)this + 4) + 0x14 + iVar3 * 4));
  if (_DAT_004dfc24 < local_88) {
    if (iVar2 == *(int *)(*(int *)((int)this + 4) + 4) + -1) {
      iVar3 = *(int *)(*(int *)this + 4) + -1;
      if ((iVar3 < 0) || (*(int *)(*(int *)this + 4) <= iVar3)) {
        piVar4 = &local_84;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_84 = 1;
        local_80 = "Array index out of bounds";
        local_7c = "..\\BluelAPI/Container/SArray.h";
        local_78 = 0x66;
        FUN_00422060(&local_84);
      }
      piVar4 = (int *)(iVar3 * 0x10 + 0x14 + *(int *)this);
      *param_1 = *piVar4;
      param_1[1] = piVar4[1];
      param_1[2] = piVar4[2];
      param_1[3] = piVar4[3];
      return param_1;
    }
    local_88 = 1.0;
  }
  if ((iVar3 < 0) || (*(int *)(*(int *)this + 4) <= iVar3)) {
    piVar4 = &local_84;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_84 = 1;
    local_80 = "Array index out of bounds";
    local_7c = "..\\BluelAPI/Container/SArray.h";
    local_78 = 0x66;
    FUN_00422060(&local_84);
  }
  local_68 = (float)(iVar3 * 0x10);
  piVar4 = (int *)((int)local_68 + 0x14 + *(int *)this);
  local_18 = *piVar4;
  local_14 = (char *)piVar4[1];
  local_10 = (char *)piVar4[2];
  local_c = piVar4[3];
  if ((iVar2 < 0) || (*(int *)(*(int *)this + 4) <= iVar2)) {
    piVar4 = &local_48;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_48 = 1;
    local_44 = "Array index out of bounds";
    local_40 = "..\\BluelAPI/Container/SArray.h";
    local_3c = 0x66;
    FUN_00422060(&local_48);
  }
  iVar1 = *(int *)this;
  local_64 = _DAT_004dfc24 - local_88;
  local_6c = (float)(iVar2 * 0x10);
  piVar4 = (int *)((int)local_6c + 0x14 + iVar1);
  local_60 = *piVar4;
  local_5c = (char *)piVar4[1];
  local_58 = (char *)piVar4[2];
  local_54 = piVar4[3];
  uVar5 = ConvertX87FloatToIntegerTowardZero();
  local_18 = (int)uVar5;
  if ((iVar3 < 0) || (*(int *)(iVar1 + 4) <= iVar3)) {
    piVar4 = &local_48;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_48 = 1;
    local_44 = "Array index out of bounds";
    local_40 = "..\\BluelAPI/Container/SArray.h";
    local_3c = 0x66;
    FUN_00422060(&local_48);
  }
  piVar4 = (int *)((int)local_68 + 0x14 + *(int *)this);
  local_60 = *piVar4;
  local_5c = (char *)piVar4[1];
  local_58 = (char *)piVar4[2];
  local_54 = piVar4[3];
  if ((iVar2 < 0) || (*(int *)(*(int *)this + 4) <= iVar2)) {
    piVar4 = &local_30;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_30 = 1;
    local_2c = "Array index out of bounds";
    local_28 = "..\\BluelAPI/Container/SArray.h";
    local_24 = 0x66;
    FUN_00422060(&local_30);
  }
  iVar1 = *(int *)this;
  piVar4 = (int *)((int)local_6c + 0x14 + iVar1);
  local_84 = *piVar4;
  local_80 = (char *)piVar4[1];
  local_7c = (char *)piVar4[2];
  local_78 = piVar4[3];
  uVar5 = ConvertX87FloatToIntegerTowardZero();
  local_10 = (char *)uVar5;
  if ((iVar3 < 0) || (*(int *)(iVar1 + 4) <= iVar3)) {
    piVar4 = &local_30;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_30 = 1;
    local_2c = "Array index out of bounds";
    local_28 = "..\\BluelAPI/Container/SArray.h";
    local_24 = 0x66;
    FUN_00422060(&local_30);
  }
  piVar4 = (int *)((int)local_68 + 0x14 + *(int *)this);
  local_84 = *piVar4;
  local_80 = (char *)piVar4[1];
  local_7c = (char *)piVar4[2];
  local_78 = piVar4[3];
  if ((iVar2 < 0) || (*(int *)(*(int *)this + 4) <= iVar2)) {
    piVar4 = &local_48;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_48 = 1;
    local_44 = "Array index out of bounds";
    local_40 = "..\\BluelAPI/Container/SArray.h";
    local_3c = 0x66;
    FUN_00422060(&local_48);
  }
  iVar1 = *(int *)this;
  piVar4 = (int *)((int)local_6c + 0x14 + iVar1);
  local_60 = *piVar4;
  local_5c = (char *)piVar4[1];
  local_58 = (char *)piVar4[2];
  local_54 = piVar4[3];
  uVar5 = ConvertX87FloatToIntegerTowardZero();
  local_14 = (char *)uVar5;
  if ((iVar3 < 0) || (*(int *)(iVar1 + 4) <= iVar3)) {
    piVar4 = &local_30;
    for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_30 = 1;
    local_2c = "Array index out of bounds";
    local_28 = "..\\BluelAPI/Container/SArray.h";
    local_24 = 0x66;
    FUN_00422060(&local_30);
  }
  piVar4 = (int *)((int)local_68 + 0x14 + *(int *)this);
  local_84 = *piVar4;
  local_80 = (char *)piVar4[1];
  local_7c = (char *)piVar4[2];
  local_78 = piVar4[3];
  if ((iVar2 < 0) || (*(int *)(*(int *)this + 4) <= iVar2)) {
    piVar4 = &local_48;
    for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    }
    local_48 = 1;
    local_44 = "Array index out of bounds";
    local_40 = "..\\BluelAPI/Container/SArray.h";
    local_3c = 0x66;
    FUN_00422060(&local_48);
  }
  piVar4 = (int *)((int)local_6c + 0x14 + *(int *)this);
  local_60 = *piVar4;
  local_5c = (char *)piVar4[1];
  local_58 = (char *)piVar4[2];
  local_54 = piVar4[3];
  uVar5 = ConvertX87FloatToIntegerTowardZero();
  *param_1 = local_18;
  param_1[1] = (int)local_14;
  param_1[2] = (int)local_10;
  param_1[3] = (int)uVar5;
  return param_1;
}


2026-10-15 01:47:36 INFO  (GhidraScript) TraceVictimPick.java>
