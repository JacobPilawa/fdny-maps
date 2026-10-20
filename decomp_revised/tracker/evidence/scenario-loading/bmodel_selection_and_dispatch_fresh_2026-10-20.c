/* Fresh read-only Ghidra 12.0.4 decompilation, captured 2026-10-20.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Project: .local/mission-lifecycle-ghidra/FDNYPort (-noanalysis -readOnly)
 * These are analysis outputs, not recovered original source.
 */

/* 00459350: candidate scan and query selection. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00459350(int param_1)

{
  int *this;
  char cVar1;
  float *pfVar2;
  int iVar3;
  void *this_00;
  int iVar4;
  int *piVar5;
  float fStack_88;
  float local_84;
  int local_80;
  int *local_7c;
  float local_78;
  undefined4 local_74;
  undefined4 local_70;
  float afStack_6c [6];
  float afStack_54 [3];
  int local_48;
  char *local_44;
  char *local_40;
  undefined4 local_3c;
  int local_30;
  char *local_2c;
  char *local_28;
  undefined4 local_24;
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined4 local_c;
  
  local_c = 0xffffffff;
  puStack_10 = &LAB_004d0e08;
  pvStack_14 = ExceptionList;
  this = (int *)(param_1 + 0x1a4);
  iVar4 = 0;
  ExceptionList = &pvStack_14;
  *(undefined4 *)(param_1 + 0x1a0) = 0xffffffff;
  piVar5 = (int *)(param_1 + 0x1a8);
  *this = 0;
  if (*piVar5 != 0) {
    FUN_00429fe0(*(void **)(*piVar5 + 4),(int)piVar5);
    *piVar5 = 0;
  }
  iVar3 = *(int *)(DAT_00503a78 + 0x30);
  local_78 = *(float *)(iVar3 + 0x34cc);
  local_74 = *(undefined4 *)(iVar3 + 0x34d0);
  local_70 = *(undefined4 *)(iVar3 + 0x34d4);
  local_84 = 0.0;
  FUN_0044f030(&local_80);
  local_c = 0;
  FUN_0049e770((void *)(*(int *)(DAT_00503a78 + 0x30) + 0x34a4),afStack_6c + 3);
  if (0 < *(int *)(param_1 + 0x18c)) {
    do {
      if ((iVar4 < 0) || (*(int *)(param_1 + 0x18c) <= iVar4)) {
        piVar5 = &local_48;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_48 = 1;
        local_44 = "Array index out of bounds";
        local_40 = "..\\BluelAPI/Container/SArray.h";
        local_3c = 0x1cd;
        FUN_00422060(&local_48);
      }
      if (*(int *)(*(int *)(param_1 + 0x188) + iVar4 * 4) != 0) {
        local_80 = 0;
        if (local_7c != (int *)0x0) {
          FUN_00429fe0((void *)local_7c[1],(int)&local_7c);
          local_7c = (int *)0x0;
        }
        if ((iVar4 < 0) || (*(int *)(param_1 + 0x18c) <= iVar4)) {
          piVar5 = &local_30;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar5 = 0;
            piVar5 = piVar5 + 1;
          }
          local_30 = 1;
          local_2c = "Array index out of bounds";
          local_28 = "..\\BluelAPI/Container/SArray.h";
          local_24 = 0x1cd;
          FUN_00422060(&local_30);
        }
        local_80 = (**(code **)**(undefined4 **)(*(int *)(param_1 + 0x188) + iVar4 * 4))(&local_7c);
        if (local_80 != 0) {
          if (local_7c == (int *)0x0) {
            *(int *)(param_1 + 0x1a0) = iVar4;
            piVar5 = (int *)(param_1 + 0x1a8);
            *this = local_80;
            if (*piVar5 != 0) {
              FUN_00429fe0(*(void **)(*piVar5 + 4),(int)piVar5);
              *piVar5 = 0;
            }
            if (local_7c == (int *)0x0) {
              ExceptionList = pvStack_14;
              return;
            }
            this_00 = (void *)local_7c[1];
            goto LAB_004595ca;
          }
          afStack_6c[0] = 0.0;
          afStack_6c[1] = 0.0;
          afStack_6c[2] = 0.0;
          cVar1 = (**(code **)(*local_7c + 0x14))(&local_78,afStack_6c + 3,afStack_6c,&fStack_88);
          if (((cVar1 != '\0') &&
              (pfVar2 = (float *)FUN_0043b2d0(afStack_54,afStack_6c,&local_78),
              pfVar2[2] * pfVar2[2] + pfVar2[1] * pfVar2[1] + *pfVar2 * *pfVar2 <=
              _DAT_004e21d4 * _DAT_004e21d4)) &&
             ((*(int *)(param_1 + 0x1a0) == -1 || (local_84 <= fStack_88)))) {
            local_84 = fStack_88;
            *(int *)(param_1 + 0x1a0) = iVar4;
            FUN_00459300(this,&local_80);
          }
        }
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < *(int *)(param_1 + 0x18c));
  }
  if (local_7c != (int *)0x0) {
    this_00 = (void *)local_7c[1];
LAB_004595ca:
    local_c = 0xffffffff;
    FUN_00429fe0(this_00,(int)&local_7c);
  }
  ExceptionList = pvStack_14;
  return;
}

 (GhidraScript)

/* 00459630: selection refresh and cached action dispatch. */
void __fastcall FUN_00459630(void *param_1)

{
  void *this;
  int iVar1;
  char cVar2;
  int local_10 [4];
  
  FUN_00459350((int)param_1);
  iVar1 = *(int *)((int)param_1 + 0x1a0);
  if ((-1 < iVar1) && (iVar1 < *(int *)((int)param_1 + 0x18c))) {
    FUN_0048d490((int *)((int)param_1 + 0x188),iVar1);
    if (*(int *)(*(int *)((int)param_1 + 0x188) + iVar1 * 4) != 0) {
      FUN_0047da90(param_1,*(undefined4 *)((int)param_1 + 0x1a4));
      this = (void *)((int)param_1 + 0x1c);
      cVar2 = FUN_0047dcd0((int)this);
      if (cVar2 == '\0') {
        RequestSoundInstancePlayback((void *)((int)param_1 + 0x164),(undefined1 *)0x0);
      }
      FUN_0047dc90(this,'\x01');
      *(undefined1 *)((int)param_1 + 300) = 0;
      FUN_0047d300(*(void **)((int)param_1 + 0x120),0);
      local_10[0] = 0x118;
      local_10[2] = 0x16d;
      local_10[1] = 0x168;
      local_10[3] = 0x1bd;
      FUN_0047edf0(this,local_10,0);
      return;
    }
  }
  *(undefined1 *)((int)param_1 + 300) = 1;
  return;
}

 (GhidraScript)
