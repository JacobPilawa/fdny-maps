/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0043d330
 * Match: function entry
 * Entry: 0043d330 FUN_0043d330
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0043d330(void *param_1)

{
  float fVar1;
  float fVar2;
  int iVar3;
  uint uVar4;
  undefined4 extraout_ECX;
  int *piVar5;
  float *pfVar6;
  bool bVar7;
  float local_78;
  char *local_74;
  char *local_70;
  undefined4 local_6c;
  undefined1 local_60 [36];
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  if (*(int *)((int)param_1 + 0x25c) == 0) {
    if (*(int *)((int)param_1 + 0x1c4) < 1) {
      pfVar6 = &local_78;
      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
        *pfVar6 = 0.0;
        pfVar6 = pfVar6 + 1;
      }
      local_78 = 1.4013e-45;
      local_74 = "Array index out of bounds";
      local_70 = "..\\BluelAPI/Container/SArray.h";
      local_6c = 0x1cd;
      FUN_00422060((int *)&local_78);
    }
    iVar3 = **(int **)((int)param_1 + 0x1c0);
    uVar4 = *(uint *)(iVar3 + 0xcc) & 0xffffffef;
  }
  else {
    if (*(int *)((int)param_1 + 0x1c4) < 1) {
      pfVar6 = &local_78;
      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
        *pfVar6 = 0.0;
        pfVar6 = pfVar6 + 1;
      }
      local_78 = 1.4013e-45;
      local_74 = "Array index out of bounds";
      local_70 = "..\\BluelAPI/Container/SArray.h";
      local_6c = 0x1cd;
      FUN_00422060((int *)&local_78);
    }
    iVar3 = **(int **)((int)param_1 + 0x1c0);
    uVar4 = *(uint *)(iVar3 + 0xcc) | 0x10;
  }
  *(uint *)(iVar3 + 0xcc) = uVar4;
  if (*(int *)((int)param_1 + 0x25c) == 2) {
    iVar3 = DAT_00502afc - *(int *)((int)param_1 + 0x208);
    fVar1 = (float)iVar3;
    local_78 = 0.0;
    local_74 = (char *)0x0;
    local_70 = (char *)0x0;
    if (iVar3 < 0) {
      fVar1 = fVar1 + (float)_DAT_004dfc90;
    }
    fVar1 = fVar1 * (float)_DAT_004dfa28;
    if (*(float *)((int)param_1 + 0x20c) < fVar1) {
      *(undefined4 *)((int)param_1 + 0x25c) = 0;
      if (*(char *)((int)param_1 + 0x220) == '\0') {
        pfVar6 = (float *)((int)param_1 + 0x1fc);
      }
      else {
        pfVar6 = (float *)((int)param_1 + 0x1f0);
      }
      FUN_0043baa0(param_1,pfVar6,'\x01');
      bVar7 = *(char *)((int)param_1 + 0x220) == '\0';
      piVar5 = (int *)CONCAT31((int3)((uint)extraout_ECX >> 8),bVar7);
      *(bool *)((int)param_1 + 0x220) = bVar7;
      FUN_00402790(&stack0xffffff78,"drop");
      FUN_004705b0((void *)((int)param_1 + 0x24),piVar5);
      FUN_004a11a0((void *)((int)param_1 + 0x124),(undefined1 *)0x0);
      return;
    }
    if (*(char *)((int)param_1 + 0x220) != '\0') {
      fVar1 = *(float *)((int)param_1 + 0x20c) - fVar1;
    }
    fVar2 = fVar1 / *(float *)((int)param_1 + 0x20c);
    local_78 = fVar2 * *(float *)((int)param_1 + 0x1fc) +
               (_DAT_004dfc24 - fVar2) * *(float *)((int)param_1 + 0x1f0);
    local_70 = (char *)(fVar2 * *(float *)((int)param_1 + 0x204) +
                       (_DAT_004dfc24 - fVar2) * *(float *)((int)param_1 + 0x1f8));
    local_74 = (char *)((fVar1 * *(float *)((int)param_1 + 0x210) + *(float *)((int)param_1 + 0x214)
                        ) * fVar1 + *(float *)((int)param_1 + 0x218));
    FUN_0043baa0(param_1,&local_78,'\x01');
    gePath_Sample(*(undefined4 *)((int)param_1 + 0x21c),local_60);
    if (*(int *)((int)param_1 + 0x1c4) < 1) {
      pfVar6 = &local_78;
      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
        *pfVar6 = 0.0;
        pfVar6 = pfVar6 + 1;
      }
      local_78 = 1.4013e-45;
      local_74 = "Array index out of bounds";
      local_70 = "..\\BluelAPI/Container/SArray.h";
      local_6c = 0x1cd;
      FUN_00422060((int *)&local_78);
    }
    iVar3 = *(int *)(DAT_00503a78 + 4);
    if (*(char *)(iVar3 + 0x160) == '\0') {
      iVar3 = iVar3 + 8;
    }
    else {
      iVar3 = iVar3 + 0xb4;
    }
    geWorld_GetModelXForm(*(undefined4 *)(iVar3 + 0x78),**(undefined4 **)((int)param_1 + 0x1c0));
    local_3c = local_c;
    local_38 = local_8;
    local_34 = local_4;
    if (*(int *)((int)param_1 + 0x1c4) < 1) {
      pfVar6 = &local_78;
      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
        *pfVar6 = 0.0;
        pfVar6 = pfVar6 + 1;
      }
      local_78 = 1.4013e-45;
      local_74 = "Array index out of bounds";
      local_70 = "..\\BluelAPI/Container/SArray.h";
      local_6c = 0x1cd;
      FUN_00422060((int *)&local_78);
    }
    iVar3 = *(int *)(DAT_00503a78 + 4);
    if (*(char *)(iVar3 + 0x160) == '\0') {
      iVar3 = iVar3 + 8;
    }
    else {
      iVar3 = iVar3 + 0xb4;
    }
    geWorld_SetModelXForm(*(undefined4 *)(iVar3 + 0x78),**(undefined4 **)((int)param_1 + 0x1c0));
  }
  return;
}


