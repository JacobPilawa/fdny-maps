/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047f960
 * Match: function entry
 * Entry: 0047f960 FUN_0047f960
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0047f960(void *this,void *param_1)

{
  float fVar1;
  undefined4 uVar2;
  float fVar3;
  float fVar4;
  float *pfVar5;
  int iVar6;
  int iVar7;
  float10 fVar8;
  ulonglong uVar9;
  byte local_1a;
  byte local_19;
  float local_18;
  undefined4 local_14;
  float local_10;
  float local_c;
  float local_8;
  float local_4;
  
  if (*(char *)((int)this + 0xad) != '\0') {
    FUN_0047d660(this);
  }
  if (*(char *)((int)this + 0xa0) != '\0') {
    FUN_0047f2a0((int)this);
  }
  if (*(char *)((int)this + 0x30) != '\0') {
    fVar1 = *(float *)((int)this + 0xb0);
    if (fVar1 == 0.0) {
      local_14 = 2;
      *(undefined4 *)((int)this + 0xd8) = *(undefined4 *)((int)this + 0x5c);
      *(undefined4 *)((int)this + 0xdc) = *(undefined4 *)((int)this + 0x60);
      *(undefined4 *)((int)this + 0xe0) = *(undefined4 *)((int)this + 100);
      local_18 = 1.4013e-45;
    }
    else if (fVar1 == 1.4013e-45) {
      local_14 = 0;
      if (*(char *)((int)this + 0x78) != '\0') {
        *(undefined1 *)((int)this + 0x78) = 0;
      }
      local_18 = 5.60519e-45;
    }
    else if (fVar1 == 2.8026e-45) {
      local_14 = 0;
      FUN_0047f5a0((int)this);
      local_18 = 5.60519e-45;
    }
    else if (fVar1 == 4.2039e-45) {
      local_14 = 0;
      local_18 = fVar1;
    }
    else if (fVar1 == 5.60519e-45) {
      local_14 = 0;
      local_18 = fVar1;
    }
    fVar1 = local_18;
    if (*(char *)((int)this + 0xac) != '\0') {
      iVar6 = 0;
      local_8 = 0.0;
      local_4 = 0.0;
      local_c = _DAT_004dc50c;
      if (3 < (int)local_18) {
        iVar7 = 3;
        pfVar5 = (float *)((int)this + 0xdc);
        do {
          iVar7 = iVar7 + 4;
          iVar6 = iVar6 + 4;
          local_c = local_c + pfVar5[-1] + pfVar5[8] + pfVar5[0x11] + pfVar5[0x1a];
          local_8 = local_8 + *pfVar5 + pfVar5[9] + pfVar5[0x12] + pfVar5[0x1b];
          local_4 = local_4 + pfVar5[1] + pfVar5[10] + pfVar5[0x13] + pfVar5[0x1c];
          pfVar5 = pfVar5 + 0x24;
        } while (iVar7 < (int)local_18);
      }
      if (iVar6 < (int)local_18) {
        iVar7 = (int)local_18 - iVar6;
        pfVar5 = (float *)((int)this + iVar6 * 0x24 + 0xdc);
        do {
          local_c = local_c + pfVar5[-1];
          iVar7 = iVar7 + -1;
          local_8 = local_8 + *pfVar5;
          local_4 = local_4 + pfVar5[1];
          pfVar5 = pfVar5 + 9;
        } while (iVar7 != 0);
      }
      iVar6 = (int)param_1 + 0xb4;
      local_18 = (float)_DAT_004dd808 / (float)(int)local_18;
      local_c = local_18 * local_c;
      local_8 = local_18 * local_8;
      local_4 = local_18 * local_4;
      if (*(char *)((int)param_1 + 0x160) == '\0') {
        iVar6 = (int)param_1 + 8;
      }
      geWorld_GetLeaf(*(undefined4 *)(iVar6 + 0x78),&local_c,&local_18);
      iVar6 = (int)param_1 + 0xb4;
      if (*(char *)((int)param_1 + 0x160) == '\0') {
        iVar6 = (int)param_1 + 8;
      }
      iVar6 = geWorld_MightSeeLeaf(*(undefined4 *)(iVar6 + 0x78),local_18);
      if (iVar6 == 0) {
        return;
      }
    }
    FUN_0047d6d0(this,&local_1a,&local_19);
    pfVar5 = (float *)FUN_0047d420((int)this);
    local_c = *pfVar5;
    local_8 = pfVar5[1];
    local_4 = pfVar5[2];
    if (*(char *)((int)this + 0xcc) != '\0') {
      local_c = *(float *)((int)this + 0xc0);
      local_8 = *(float *)((int)this + 0xc4);
      local_4 = *(float *)((int)this + 200);
    }
    if (*(char *)((int)this + 0xd4) != '\0') {
      uVar9 = FUN_004b9400();
      local_1a = (byte)uVar9;
    }
    fVar3 = local_c * _DAT_004e3524;
    *(float *)((int)this + 0xec) = fVar3;
    fVar4 = local_8 * _DAT_004e3524;
    *(float *)((int)this + 0xf0) = fVar4;
    local_10 = local_4 * _DAT_004e3524;
    *(float *)((int)this + 0xf4) = local_10;
    local_18 = (float)local_1a;
    *(float *)((int)this + 0x160) = local_10;
    *(float *)((int)this + 0xf8) = local_18;
    *(float *)((int)this + 0x164) = local_18;
    *(float *)((int)this + 0x110) = fVar3;
    *(float *)((int)this + 0x114) = fVar4;
    *(float *)((int)this + 0x118) = local_10;
    *(float *)((int)this + 0x11c) = local_18;
    *(float *)((int)this + 0x134) = fVar3;
    *(float *)((int)this + 0x138) = fVar4;
    *(float *)((int)this + 0x13c) = local_10;
    *(float *)((int)this + 0x140) = local_18;
    *(float *)((int)this + 0x158) = fVar3;
    *(float *)((int)this + 0x15c) = fVar4;
    local_18 = (float)FUN_0047d820(this,param_1);
    iVar6 = (int)param_1 + 0xb4;
    if (*(char *)((int)param_1 + 0x160) == '\0') {
      iVar6 = (int)param_1 + 8;
    }
    uVar2 = *(undefined4 *)(iVar6 + 0x78);
    fVar8 = FUN_0047d8a0((int)this);
    geWorld_AddPolyOnce(uVar2,(int)this + 0xd8,fVar1,local_18,local_14,
                        *(undefined4 *)((int)this + 0xa4),
                        (float)(fVar8 * (float10)*(float *)((int)this + 0xa8)));
    if ((*(byte *)((int)this + 0x18) & 2) != 0) {
      iVar6 = FUN_0047d3e0((int)this);
      iVar7 = FUN_0047d400((int)this);
      if (iVar6 != iVar7) {
        pfVar5 = (float *)FUN_0047d560((int)this);
        local_c = *pfVar5;
        local_8 = pfVar5[1];
        local_4 = pfVar5[2];
        if (*(char *)((int)this + 0xcc) != '\0') {
          local_c = *(float *)((int)this + 0xc0);
          local_8 = *(float *)((int)this + 0xc4);
          local_4 = *(float *)((int)this + 200);
        }
        fVar3 = local_c * _DAT_004e3524;
        *(float *)((int)this + 0xec) = fVar3;
        fVar4 = local_8 * _DAT_004e3524;
        *(float *)((int)this + 0xf0) = fVar4;
        local_18 = local_4 * _DAT_004e3524;
        *(float *)((int)this + 0xf4) = local_18;
        *(float *)((int)this + 0x160) = local_18;
        local_10 = (float)local_19;
        *(float *)((int)this + 0xf8) = local_10;
        *(float *)((int)this + 0x164) = local_10;
        *(float *)((int)this + 0x110) = fVar3;
        *(float *)((int)this + 0x114) = fVar4;
        *(float *)((int)this + 0x118) = local_18;
        *(float *)((int)this + 0x11c) = local_10;
        *(float *)((int)this + 0x134) = fVar3;
        *(float *)((int)this + 0x138) = fVar4;
        *(float *)((int)this + 0x13c) = local_18;
        *(float *)((int)this + 0x140) = local_10;
        *(float *)((int)this + 0x158) = fVar3;
        *(float *)((int)this + 0x15c) = fVar4;
        local_18 = (float)FUN_0047d860(this,param_1);
        iVar6 = (int)param_1 + 0xb4;
        if (*(char *)((int)param_1 + 0x160) == '\0') {
          iVar6 = (int)param_1 + 8;
        }
        uVar2 = *(undefined4 *)(iVar6 + 0x78);
        fVar8 = FUN_0047d960((int)this);
        geWorld_AddPolyOnce(uVar2,(int)this + 0xd8,fVar1,local_18,local_14,
                            *(undefined4 *)((int)this + 0xa4),
                            (float)(fVar8 * (float10)*(float *)((int)this + 0xa8)));
      }
    }
  }
  return;
}


