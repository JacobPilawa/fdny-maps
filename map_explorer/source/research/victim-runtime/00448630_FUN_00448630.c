/* Ghidra decompilation; names/types are provisional.
 * Entry: 00448630
 * Function: FUN_00448630
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00448630(int param_1)

{
  int iVar1;
  void *pvVar2;
  float *pfVar3;
  float *pfVar4;
  undefined4 *puVar5;
  int *extraout_ECX;
  int *piVar6;
  undefined *puVar7;
  float local_88;
  float local_84;
  undefined4 local_80;
  undefined4 local_7c [3];
  float local_70 [3];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf8d3;
  local_c = ExceptionList;
  pvVar2 = (void *)(param_1 + 300);
  ExceptionList = &local_c;
  FUN_00447870(pvVar2,&local_88);
  _DAT_00502c28 = local_84 - _DAT_004e1710;
  _DAT_00502c1c = local_84 + _DAT_004e170c;
  _DAT_00502c18 = local_88;
  _DAT_00502c20 = local_80;
  _DAT_00502c24 = local_88;
  _DAT_00502c2c = local_80;
  iVar1 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar1 + 0x160) == '\0') {
    iVar1 = iVar1 + 8;
  }
  else {
    iVar1 = iVar1 + 0xb4;
  }
  iVar1 = geWorld_Collision(*(undefined4 *)(iVar1 + 0x78),0,0,&DAT_00502c18,&DAT_00502c24,1,2,0,0);
  if (iVar1 == 0) {
    piVar6 = extraout_ECX;
    FUN_00401cb0(&stack0xffffff64,(undefined4 *)(param_1 + 0x48));
    local_4 = 0;
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "rescue\\FireRescueEnt.cpp";
    local_5c = 0x231;
    local_58 = 0;
    local_4 = CONCAT31(local_4._1_3_,2);
    FUN_004a3a19(local_48,"FireRescue \"");
    pvVar2 = (void *)FUN_0040f020(&local_64,piVar6);
    FUN_004a3a19((void *)((int)pvVar2 + 0x1c),"\" Couldn\'t FloorMe()");
    FUN_00401360(pvVar2,iVar1);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_64);
    ExceptionList = local_c;
    return;
  }
  if ((DAT_00502be8 & 1) == 0) {
    DAT_00502be8 = DAT_00502be8 | 1;
    DAT_00502bdc = 0.0;
    DAT_00502be0 = 0.0;
    DAT_00502be4 = 0.0;
    _atexit((_func_4879 *)&DAT_004db720);
  }
  if ((DAT_00502be8 & 2) == 0) {
    DAT_00502be8 = DAT_00502be8 | 2;
    FUN_0042d2d0(&DAT_00502b98,0);
    _atexit((_func_4879 *)&DAT_004db710);
  }
  iVar1 = FUN_0044a6a0((int)pvVar2);
  FUN_00447890(&DAT_00502b98,iVar1);
  _DAT_00502bc4 = _DAT_00502bfc + _DAT_004e16c4;
  _DAT_00502bb0 =
       (float)_DAT_004dd808 /
       SQRT(_DAT_00502c08 * _DAT_00502c08 +
            _DAT_00502c0c * _DAT_00502c0c + _DAT_00502c10 * _DAT_00502c10);
  _DAT_00502ba8 = _DAT_00502c08 * _DAT_00502bb0;
  _DAT_00502bac = _DAT_00502c0c * _DAT_00502bb0;
  _DAT_00502bb0 = _DAT_00502c10 * _DAT_00502bb0;
  DAT_00502bdc = _DAT_00502ba8;
  DAT_00502be0 = _DAT_00502bac;
  DAT_00502be4 = _DAT_00502bb0;
  pfVar3 = (float *)FUN_00446ee0(&DAT_00502b98,&local_88);
  pfVar4 = (float *)FUN_00447830(&DAT_00502b98,local_7c);
  puVar5 = (undefined4 *)FUN_004476b0(local_70,pfVar4,pfVar3);
  _DAT_00502bb4 = *puVar5;
  _DAT_00502bb8 = puVar5[1];
  _DAT_00502bbc = puVar5[2];
  pfVar3 = (float *)FUN_00447830(&DAT_00502b98,local_70);
  pfVar4 = (float *)FUN_0049e770(&DAT_00502b98,local_7c);
  puVar5 = (undefined4 *)FUN_004476b0(&local_88,pfVar4,pfVar3);
  _DAT_00502b9c = *puVar5;
  _DAT_00502ba0 = puVar5[1];
  _DAT_00502ba4 = puVar5[2];
  puVar7 = &DAT_00502b98;
  pvVar2 = (void *)FUN_0044a6a0((int)pvVar2);
  FUN_00447890(pvVar2,(int)puVar7);
  ExceptionList = local_c;
  return;
}

