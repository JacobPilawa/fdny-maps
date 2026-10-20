/* Fresh read-only Ghidra decompilations, captured 2026-10-20.
 * Binary: inputs/original/binaries/FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Ghidra: 12.0.4 headless, saved .local/mission-lifecycle-ghidra/FDNYPort project, -readOnly -noanalysis.
 * The pseudocode is analysis output, not recovered original source.
 */

/* Function address: 0043BE00. Ghidra label retained from the analysis database. */
void __fastcall FUN_0043be00(int param_1)

{
  uint uVar1;
  undefined ***this;
  void *pvVar2;
  char *pcVar3;
  int iVar4;
  float *pfVar5;
  float local_bc;
  char *local_b8;
  char *local_b4;
  undefined4 local_b0;
  float local_a4;
  float local_a0;
  float local_9c;
  float local_98;
  undefined **local_94;
  char *local_90;
  undefined4 local_8c;
  undefined1 local_88;
  undefined1 local_78 [60];
  undefined1 local_3c [36];
  float local_18;
  float local_14;
  float local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ce9fc;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  if (*(int *)(param_1 + 0x1c4) < 1) {
    pfVar5 = &local_bc;
    for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
      *pfVar5 = 0.0;
      pfVar5 = pfVar5 + 1;
    }
    local_bc = 1.4013e-45;
    local_b8 = "Array index out of bounds";
    local_b4 = "..\\BluelAPI/Container/SArray.h";
    local_b0 = 0x1cd;
    ExceptionList = &local_c;
    FUN_00422060((int *)&local_bc);
  }
  iVar4 = geWorld_ModelGetMotion(**(undefined4 **)(param_1 + 0x1c0));
  if (iVar4 == 0) {
    local_94 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_90 = "sharedents\\BModelEnt.cpp";
    local_8c = 0x165;
    local_88 = 0;
    local_4 = 0;
    FUN_004a3a19(local_78,"Missing keyframes for liftable object ");
    pcVar3 = *(char **)(*(int *)(param_1 + 0xec) + 4);
LAB_0043c152:
    FUN_004a3a19(local_78,pcVar3);
    this = &local_94;
  }
  else {
    iVar4 = geMotion_GetPath(iVar4,0);
    *(int *)(param_1 + 0x21c) = iVar4;
    if ((iVar4 == 0) || (iVar4 = gePath_GetKeyframeCount(iVar4,1), iVar4 != 2)) {
      local_94 = &PTR_FUN_004dc554;
      FUN_004a3118();
      local_90 = "sharedents\\BModelEnt.cpp";
      local_8c = 0x16b;
      local_88 = 0;
      local_4 = 1;
      FUN_004a3a19(local_78,"Incorrect keyframes for liftable object ");
      pcVar3 = *(char **)(*(int *)(param_1 + 0xec) + 4);
      goto LAB_0043c152;
    }
    if (*(int *)(param_1 + 0x1c4) < 1) {
      pfVar5 = &local_bc;
      for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
        *pfVar5 = 0.0;
        pfVar5 = pfVar5 + 1;
      }
      local_bc = 1.4013e-45;
      local_b8 = "Array index out of bounds";
      local_b4 = "..\\BluelAPI/Container/SArray.h";
      local_b0 = 0x1cd;
      FUN_00422060((int *)&local_bc);
    }
    iVar4 = *(int *)(DAT_00503a78 + 4);
    if (*(char *)(iVar4 + 0x160) == '\0') {
      iVar4 = iVar4 + 8;
    }
    else {
      iVar4 = iVar4 + 0xb4;
    }
    geWorld_GetModelRotationalCenter
              (*(undefined4 *)(iVar4 + 0x78),**(undefined4 **)(param_1 + 0x1c0),&local_a0);
    gePath_GetKeyframe(*(undefined4 *)(param_1 + 0x21c),0,2,&local_a4,local_3c);
    uVar1 = FUN_0043b120(local_a4,0.0,2);
    if ((char)uVar1 != '\0') {
      local_b4 = (char *)(local_10 + local_98);
      local_b8 = (char *)(local_14 + local_9c);
      local_bc = local_18 + local_a0;
      *(float *)(param_1 + 0x1f0) = local_bc;
      *(char **)(param_1 + 500) = local_b8;
      *(char **)(param_1 + 0x1f8) = local_b4;
      gePath_GetKeyframe(*(undefined4 *)(param_1 + 0x21c),1,2,&local_a4,local_3c);
      uVar1 = FUN_0043b120(local_a4,1.0,2);
      if ((char)uVar1 != '\0') {
        *(float *)(param_1 + 0x1fc) = local_18 + local_a0;
        *(float *)(param_1 + 0x200) = local_14 + local_9c;
        *(float *)(param_1 + 0x204) = local_10 + local_98;
        *(undefined4 *)(param_1 + 600) = 1;
        *(undefined1 *)(param_1 + 0x220) = 0;
        ExceptionList = local_c;
        return;
      }
      pvVar2 = FUN_00401290(&local_94,"sharedents\\BModelEnt.cpp",0x187);
      pcVar3 = *(char **)(*(int *)(param_1 + 0xec) + 4);
      iVar4 = 0;
      local_4 = 3;
      pvVar2 = FUN_0040c2f0(pvVar2,"Incorrect keyframe times for liftable object ");
      this = FUN_0040c2f0(pvVar2,pcVar3);
      goto LAB_0043c162;
    }
    this = FUN_00401290(&local_94,"sharedents\\BModelEnt.cpp",0x17a);
    local_4 = 2;
    FUN_004a3a19(this + 7,"Incorrect keyframe times for liftable object ");
    FUN_004a3a19(this + 7,*(char **)(*(int *)(param_1 + 0xec) + 4));
  }
  iVar4 = 0;
LAB_0043c162:
  FUN_00401360(this,iVar4);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_94);
  ExceptionList = local_c;
  return;
}

 (GhidraScript)

/* Function address: 0043BC80. Ghidra label retained from the analysis database. */
void __thiscall FUN_0043bc80(void *this,float *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  float local_54;
  float local_50;
  float local_4c;
  int local_48;
  char *local_44;
  char *local_40;
  undefined4 local_3c;
  undefined1 local_30 [36];
  float local_c;
  float local_8;
  float local_4;
  
  iVar2 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar2 + 0x160) == '\0') {
    iVar2 = iVar2 + 8;
  }
  else {
    iVar2 = iVar2 + 0xb4;
  }
  uVar1 = *(undefined4 *)(iVar2 + 0x78);
  if (*(int *)((int)this + 0x1c4) < 1) {
    piVar3 = &local_48;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *piVar3 = 0;
      piVar3 = piVar3 + 1;
    }
    local_48 = 1;
    local_44 = "Array index out of bounds";
    local_40 = "..\\BluelAPI/Container/SArray.h";
    local_3c = 0x1cd;
    FUN_00422060(&local_48);
  }
  geWorld_GetModelRotationalCenter(uVar1,**(undefined4 **)((int)this + 0x1c0),&local_54);
  if (*(int *)((int)this + 0x1c4) < 1) {
    piVar3 = &local_48;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *piVar3 = 0;
      piVar3 = piVar3 + 1;
    }
    local_48 = 1;
    local_44 = "Array index out of bounds";
    local_40 = "..\\BluelAPI/Container/SArray.h";
    local_3c = 0x1cd;
    FUN_00422060(&local_48);
  }
  geWorld_GetModelXForm(uVar1,**(undefined4 **)((int)this + 0x1c0),local_30);
  *param_1 = local_54 + local_c;
  param_1[1] = local_50 + local_8;
  param_1[2] = local_4c + local_4;
  return;
}

 (GhidraScript)

/* Function address: 0043B340. Ghidra label retained from the analysis database. */
char __cdecl FUN_0043b340(float *param_1,float *param_2)

{
  uint uVar1;
  
  uVar1 = FUN_0043b120(*param_1,*param_2,2);
  if ((char)uVar1 == '\0') {
    uVar1 = FUN_0043b120(param_1[2],param_2[2],2);
    if ((char)uVar1 == '\0') {
      uVar1 = FUN_0043b120(param_1[1],param_2[1],2);
      return '\x01' - ((char)uVar1 != '\0');
    }
  }
  return '\0';
}

 (GhidraScript)

/* Function address: 0043BD90. Ghidra label retained from the analysis database. */
void __fastcall FUN_0043bd90(void *param_1)

{
  float *pfVar1;
  char cVar2;
  float *pfVar3;
  float local_18 [3];
  float local_c [3];
  
  FUN_0043bc80(param_1,local_18);
  pfVar1 = (float *)((int)param_1 + 0x224);
  cVar2 = FUN_0043b340(pfVar1,local_18);
  if (cVar2 != '\0') {
    cVar2 = '\0';
    pfVar3 = FUN_00482fc0(local_c,local_18,pfVar1);
    FUN_0043baa0(param_1,pfVar3,cVar2);
  }
  *(float *)((int)param_1 + 0x230) = *pfVar1;
  *(undefined4 *)((int)param_1 + 0x234) = *(undefined4 *)((int)param_1 + 0x228);
  *(undefined4 *)((int)param_1 + 0x238) = *(undefined4 *)((int)param_1 + 0x22c);
  return;
}

 (GhidraScript)

/* Function address: 0043BAA0. Ghidra label retained from the analysis database. */
void __thiscall FUN_0043baa0(void *this,float *param_1,char param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  float local_84;
  float local_80;
  float local_7c;
  int local_78;
  char *local_74;
  char *local_70;
  undefined4 local_6c;
  int local_60;
  char *local_5c;
  char *local_58;
  undefined4 local_54;
  int local_48;
  char *local_44;
  char *local_40;
  undefined4 local_3c;
  undefined1 local_30 [36];
  float local_c;
  float local_8;
  float local_4;
  
  iVar2 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar2 + 0x160) == '\0') {
    iVar2 = iVar2 + 8;
  }
  else {
    iVar2 = iVar2 + 0xb4;
  }
  uVar1 = *(undefined4 *)(iVar2 + 0x78);
  iVar2 = 0;
  if (0 < *(int *)((int)this + 0x1c4)) {
    do {
      if ((iVar2 < 0) || (*(int *)((int)this + 0x1c4) <= iVar2)) {
        piVar4 = &local_48;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_48 = 1;
        local_44 = "Array index out of bounds";
        local_40 = "..\\BluelAPI/Container/SArray.h";
        local_3c = 0x1cd;
        FUN_00422060(&local_48);
      }
      geWorld_GetModelRotationalCenter
                (uVar1,*(undefined4 *)(*(int *)((int)this + 0x1c0) + iVar2 * 4),&local_84);
      if ((iVar2 < 0) || (*(int *)((int)this + 0x1c4) <= iVar2)) {
        piVar4 = &local_78;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_78 = 1;
        local_74 = "Array index out of bounds";
        local_70 = "..\\BluelAPI/Container/SArray.h";
        local_6c = 0x1cd;
        FUN_00422060(&local_78);
      }
      geWorld_GetModelXForm(uVar1,*(undefined4 *)(*(int *)((int)this + 0x1c0) + iVar2 * 4),local_30)
      ;
      local_c = *param_1 - local_84;
      local_8 = param_1[1] - local_80;
      local_4 = param_1[2] - local_7c;
      if ((iVar2 < 0) || (*(int *)((int)this + 0x1c4) <= iVar2)) {
        piVar4 = &local_60;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_60 = 1;
        local_5c = "Array index out of bounds";
        local_58 = "..\\BluelAPI/Container/SArray.h";
        local_54 = 0x1cd;
        FUN_00422060(&local_60);
      }
      geWorld_SetModelXForm(uVar1,*(undefined4 *)(*(int *)((int)this + 0x1c0) + iVar2 * 4),local_30)
      ;
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)((int)this + 0x1c4));
  }
  if (param_2 != '\0') {
    *(float *)((int)this + 0x224) = *param_1;
    *(float *)((int)this + 0x228) = param_1[1];
    *(float *)((int)this + 0x22c) = param_1[2];
  }
  FUN_0043b7a0((int)this);
  return;
}

 (GhidraScript)

/* Function address: 00482FC0. Ghidra label retained from the analysis database. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float * __cdecl FUN_00482fc0(float *param_1,float *param_2,float *param_3)

{
  float *pfVar1;
  float *pfVar2;
  float10 fVar3;
  float fStack00000010;
  float local_18 [3];
  float local_c [3];
  
  fVar3 = (float10)FUN_004b9a20();
  fStack00000010 = (float)fVar3;
  pfVar1 = (float *)FUN_00446390(local_18,param_3,_DAT_004dfc24 - fStack00000010);
  pfVar2 = (float *)FUN_00446390(local_c,param_2,fStack00000010);
  FUN_0042d050(param_1,pfVar2,pfVar1);
  return param_1;
}

 (GhidraScript)

/* Function address: 0043B120. Ghidra label retained from the analysis database. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __cdecl FUN_0043b120(float param_1,float param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  if (param_1 == _DAT_004dc50c) {
    param_1 = param_2;
    if ((float)_DAT_004e0b68 <= param_2) {
      return 0;
    }
  }
  else {
    if (param_2 != _DAT_004dc50c) {
      uVar1 = (uint)param_1 >> 0x17 & 0xff;
      uVar3 = (uint)param_2 >> 0x17 & 0xff;
      if (uVar1 < uVar3) {
        uVar1 = uVar3;
      }
      if (uVar1 == 0xff) {
        if (param_1 == param_2) {
          return 1;
        }
        return 0;
      }
      if (uVar1 == 0) {
        iVar4 = ((uint)param_1 & 0x7fffff) - ((uint)param_2 & 0x7fffff);
        if (iVar4 < 0) {
          iVar4 = -iVar4;
        }
        iVar2 = 1 << ((byte)param_3 & 0x1f);
        return CONCAT31((int3)((uint)iVar2 >> 8),iVar4 < iVar2);
      }
      return (uint)(((uint)(param_1 - param_2) >> 0x17 & 0xff) < (uVar1 - 0x17) + param_3);
    }
    if ((float)_DAT_004e0b68 <= param_1) {
      return 0;
    }
  }
  if (param_1 <= (float)_DAT_004e0b60) {
    return 0;
  }
  return 1;
}

 (GhidraScript)
