/* Ghidra decompilation; names/types are provisional.
 * Entry: 0045ab40
 * Function: FUN_0045ab40
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __thiscall FUN_0045ab40(void *this,float *param_1)

{
  int *this_00;
  float fVar1;
  void *this_01;
  float fVar2;
  float fVar3;
  float fVar4;
  undefined2 extraout_var;
  float *pfVar5;
  undefined2 extraout_var_00;
  undefined2 uVar7;
  uint uVar6;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 uVar11;
  float local_54;
  float local_50;
  undefined4 local_4c;
  float local_48 [3];
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  this_01 = *(void **)(DAT_00503a78 + 4);
  FUN_0042d050(local_48,(float *)((int)this + 0x1c8),param_1);
  uVar7 = extraout_var;
  if (*(char *)(DAT_00502aac + 0x6a9) != '\0') {
    uVar11 = 0x40a00000;
    iVar10 = 0;
    iVar9 = 0xff;
    iVar8 = 0;
    local_54 = 0.0;
    local_50 = 24.0;
    local_4c = 0;
    pfVar5 = (float *)FUN_0042d050(local_c,local_48,&local_54);
    FUN_00492ee0(local_48,pfVar5,iVar8,iVar9,iVar10,uVar11);
    uVar7 = extraout_var_00;
  }
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  fVar1 = param_1[2] * param_1[2] + param_1[1] * param_1[1] + *param_1 * *param_1;
  fVar2 = (float)_DAT_004dfa28;
  uVar6 = CONCAT22(uVar7,(ushort)(fVar1 < fVar2) << 8 | (ushort)(NAN(fVar1) || NAN(fVar2)) << 10 |
                         (ushort)(fVar1 == fVar2) << 0xe);
  if (fVar1 >= fVar2) {
    pfVar5 = (float *)FUN_0043b2d0(local_c,local_48,(float *)((int)this + 0x218));
    fVar1 = pfVar5[2] * pfVar5[2] + pfVar5[1] * pfVar5[1] + *pfVar5 * *pfVar5;
    fVar2 = *(float *)((int)this + 0x224) * *(float *)((int)this + 0x224);
    uVar6 = CONCAT22((short)((uint)pfVar5 >> 0x10),
                     (ushort)(fVar2 < fVar1) << 8 | (ushort)(NAN(fVar2) || NAN(fVar1)) << 10 |
                     (ushort)(fVar2 == fVar1) << 0xe);
    if ((fVar2 >= fVar1) &&
       (uVar6 = FUN_00492ce0(this_01,(float *)((int)this + 0x1c8),(undefined4 *)((int)this + 0x78),
                             (undefined4 *)((int)this + 0x84),param_1,&local_30,&local_3c),
       (char)uVar6 == '\0')) {
      this_00 = (int *)((int)this + 0x1c4);
      iVar8 = 0;
      if (0 < *(int *)(*(int *)((int)this + 0x1c4) + 4)) {
        iVar9 = 0;
        do {
          FUN_00484800(this_00,iVar8);
          if (1 < *(int *)*this_00) {
            FUN_0045a6c0(this_00);
          }
          pfVar5 = (float *)FUN_0043b2d0(local_c,(float *)(*this_00 + 0x14 + iVar9),local_48);
          FUN_00484800(this_00,iVar8);
          if (1 < *(int *)*this_00) {
            FUN_0045a6c0(this_00);
          }
          fVar2 = pfVar5[2] * pfVar5[2] + pfVar5[1] * pfVar5[1] + *pfVar5 * *pfVar5;
          fVar1 = *(float *)(*this_00 + 0x20 + iVar9);
          uVar6 = CONCAT22((short)((uint)*this_00 >> 0x10),
                           (ushort)(fVar2 < fVar1) << 8 | (ushort)(NAN(fVar2) || NAN(fVar1)) << 10 |
                           (ushort)(fVar2 == fVar1) << 0xe);
          if (fVar2 < fVar1) goto LAB_0045ae3b;
          iVar8 = iVar8 + 1;
          iVar9 = iVar9 + 0x10;
        } while (iVar8 < *(int *)(*this_00 + 4));
      }
      pfVar5 = (float *)FUN_0045a190(param_1,local_c,15.0);
      FUN_0042d050(local_24,(float *)((int)this + 0x1c8),pfVar5);
      pfVar5 = (float *)FUN_0045a190(param_1,local_c,-15.0);
      FUN_0042d050(local_18,(float *)((int)this + 0x1c8),pfVar5);
      fVar1 = (float)_DAT_004e2318 + ABS(*(float *)((int)this + 0x7c));
      local_54 = 0.0;
      local_50 = 0.0;
      local_4c = 0;
      fVar2 = *(float *)((int)this + 0x1cc) - ABS(*(float *)((int)this + 0x7c));
      uVar6 = FUN_0048ff30(local_24,fVar1,&local_54);
      if ((char)uVar6 != '\0') {
        fVar3 = ABS(local_50 - fVar2);
        fVar4 = (float)_DAT_004e2310;
        uVar6 = CONCAT22((short)(uVar6 >> 0x10),
                         (ushort)(fVar3 < fVar4) << 8 | (ushort)(NAN(fVar3) || NAN(fVar4)) << 10 |
                         (ushort)(fVar3 == fVar4) << 0xe);
        if ((fVar3 < fVar4 || (fVar3 == fVar4) != 0) &&
           (uVar6 = FUN_0048ff30(local_18,fVar1,&local_54), (char)uVar6 != '\0')) {
          fVar1 = ABS(local_50 - fVar2);
          fVar2 = (float)_DAT_004e2310;
          uVar6 = CONCAT22((short)(uVar6 >> 0x10),
                           (ushort)(fVar1 < fVar2) << 8 | (ushort)(NAN(fVar1) || NAN(fVar2)) << 10 |
                           (ushort)(fVar1 == fVar2) << 0xe);
          if (fVar1 < fVar2 || (fVar1 == fVar2) != 0) {
            return CONCAT31((int3)(uVar6 >> 8),1);
          }
        }
      }
    }
  }
LAB_0045ae3b:
  return uVar6 & 0xffffff00;
}

