/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0049da00
 * Match: function entry
 * Entry: 0049da00 FUN_0049da00
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0049da00(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  void *pvVar3;
  int *extraout_ECX;
  int *piVar4;
  int *piVar5;
  float *pfVar6;
  int *piVar7;
  undefined4 *puVar8;
  int local_8c;
  int local_88;
  int local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  float local_74 [3];
  undefined1 *puStack_68;
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d4790;
  pvStack_c = ExceptionList;
  puVar8 = &local_80;
  local_8c = 0;
  ExceptionList = &pvStack_c;
  param_1[0x14] = (int)((float)param_1[0x14] + (float)_DAT_004e43e8);
  local_88 = 0;
  local_84 = 0;
  local_80 = 0;
  local_7c = 0;
  local_78 = 0;
  local_74[0] = 0.0;
  local_74[1] = -1000.0;
  local_74[2] = 0.0;
  pvVar3 = *(void **)(DAT_00503a78 + 4);
  piVar7 = &local_8c;
  pfVar6 = local_74;
  piVar5 = param_1 + 0x21;
  piVar4 = param_1 + 0x1e;
  iVar1 = (**(code **)(*param_1 + 0x18))();
  uVar2 = FUN_00492ce0(pvVar3,(float *)(iVar1 + 0x28),piVar4,piVar5,pfVar6,piVar7,puVar8);
  if ((char)uVar2 == '\0') {
    iVar1 = 0;
    puStack_68 = &stack0xffffff60;
    piVar4 = extraout_ECX;
    (**(code **)(*param_1 + 0x58))(&stack0xffffff60);
    uStack_4 = 0;
    ppuStack_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_60 = "ROC\\ROC AI\\Entity\\ModelEntity.cpp";
    uStack_5c = 0x4f;
    uStack_58 = 0;
    uStack_4 = CONCAT31(uStack_4._1_3_,2);
    FUN_004a3a19(auStack_48,"Error placing \"");
    pvVar3 = (void *)FUN_0040f020(&ppuStack_64,piVar4);
    FUN_004a3a19((void *)((int)pvVar3 + 0x1c),"\": cannot find floor!");
    FUN_00401360(pvVar3,iVar1);
    uStack_4 = 0xffffffff;
    FUN_004012c0(&ppuStack_64);
  }
  param_1[0x13] = local_8c;
  param_1[0x14] = local_88;
  param_1[0x15] = local_84;
  param_1[0x14] = (int)((float)param_1[0x14] + _DAT_004dfc24);
  ExceptionList = pvStack_c;
  return;
}


