
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_00478150(undefined4 param_1,int *param_2)

{
  int *piVar1;
  bool bVar2;
  void *this;
  int *piVar3;
  int iVar4;
  int *local_6c;
  int *local_68;
  int *piStack_64;
  undefined **ppuStack_60;
  char *pcStack_5c;
  undefined4 uStack_58;
  undefined1 uStack_54;
  undefined1 auStack_44 [56];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  puStack_8 = &LAB_004d2d98;
  pvStack_c = ExceptionList;
  local_4 = 0;
  ExceptionList = &pvStack_c;
  FUN_00422de0(&local_6c,&param_1);
  local_68 = (int *)&stack0xffffff88;
  local_4 = CONCAT31(local_4._1_3_,1);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar3 = &DAT_00501908;
  if (local_6c != &DAT_00501908) {
    piVar3 = local_6c;
    if (local_6c != &DAT_00501908) {
      *local_6c = *local_6c + 1;
    }
  }
  bVar2 = FUN_004097f0(piVar3);
  if (!bVar2) {
    iVar4 = 3;
    piStack_64 = (int *)&stack0xffffff88;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar3 = &DAT_00501908;
    piVar1 = (int *)&stack0xffffff88;
    if (local_68 != &DAT_00501908) {
      piVar3 = local_68;
      piVar1 = (int *)&stack0xffffff88;
      if (local_68 != &DAT_00501908) {
        *local_68 = *local_68 + 1;
        piVar1 = piStack_64;
      }
    }
    piStack_64 = piVar1;
    ppuStack_60 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_5c = "script\\scripttrigger.cpp";
    uStack_58 = 0x35a;
    uStack_54 = 0;
    FUN_004a3a19(auStack_44,"File not found: ");
    this = (void *)FUN_0040f020(&ppuStack_60,piVar3);
    FUN_00401360(this,iVar4);
    FUN_004012c0(&ppuStack_60);
    if (local_68 != &DAT_00501908) {
      *local_68 = *local_68 + -1;
      if (*local_68 < 1) {
        FUN_004011c0(&local_68);
      }
      else {
        local_68 = &DAT_00501908;
      }
    }
    if (param_2 != &DAT_00501908) {
      *param_2 = *param_2 + -1;
      if (*param_2 < 1) {
        param_2 = (int *)FUN_004011c0(&param_2);
      }
    }
    ExceptionList = puStack_8;
    return (uint)param_2 & 0xffffff00;
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_64 = &DAT_00501908;
  FUN_00401d80(&piStack_64,"data\\resdefs\\rescue.rrf");
  SetPendingLevelRequest(DAT_00502aac,&param_2,&piStack_64);
  if (piStack_64 != &DAT_00501908) {
    *piStack_64 = *piStack_64 + -1;
    if (*piStack_64 < 1) {
      FUN_004011c0(&piStack_64);
    }
    else {
      piStack_64 = &DAT_00501908;
    }
  }
  if (local_68 != &DAT_00501908) {
    *local_68 = *local_68 + -1;
    if (*local_68 < 1) {
      FUN_004011c0(&local_68);
    }
    else {
      local_68 = &DAT_00501908;
    }
  }
  if ((param_2 != &DAT_00501908) && (*param_2 = *param_2 + -1, *param_2 < 1)) {
    param_2 = (int *)FUN_004011c0(&param_2);
  }
  ExceptionList = puStack_8;
  return CONCAT31((int3)((uint)param_2 >> 8),1);
}
