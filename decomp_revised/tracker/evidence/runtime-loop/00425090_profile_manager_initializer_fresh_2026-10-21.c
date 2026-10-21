/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */
/* Fresh headless Ghidra 12.0.4 export from a disposable project copy. */
/* The primary saved Ghidra project was not modified. */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00425090 FUN_00425090 */

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * __fastcall FUN_00425090(int *param_1)

{
  undefined **ppuVar1;
  int *piVar2;
  bool bVar3;
  void *this;
  int *this_00;
  int *piVar4;
  int iVar5;
  int *piVar6;
  uint unaff_retaddr;
  int *local_134;
  int *local_130;
  undefined **local_12c;
  int local_128;
  int *local_124;
  undefined1 *local_120;
  int *local_11c;
  void *local_118;
  CHAR local_114 [260];
  uint local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_004cd1fa;
  pvStack_c = ExceptionList;
  local_10 = DAT_005007e0 ^ unaff_retaddr;
  ExceptionList = &pvStack_c;
  param_1[5] = 0x10;
  param_1[4] = (int)FUN_0043f660;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  local_4 = 0;
  param_1[6] = 0;
  local_11c = param_1;
  GetCurrentDirectoryA(0x104,local_114);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_134 = &DAT_00501908;
  FUN_00401d80(&local_134,local_114);
  local_4._0_1_ = 1;
  FUN_00406440(&local_130,(int *)&local_134,'/');
  local_4._0_1_ = 3;
  if (local_134 != &DAT_00501908) {
    *local_134 = *local_134 + -1;
    if (*local_134 < 1) {
      FUN_004011c0(&local_134);
    }
    else {
      local_134 = &DAT_00501908;
    }
  }
  FUN_00401d00(&local_130,"Saves\\");
  local_12c = (undefined **)&stack0xfffffeb8;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar6 = &DAT_00501908;
  if (local_130 != &DAT_00501908) {
    piVar6 = local_130;
    if (local_130 != &DAT_00501908) {
      *local_130 = *local_130 + 1;
    }
  }
  FUN_00409b40(piVar6);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar6 = &DAT_00501908;
  local_124 = &DAT_00501908;
  local_4._0_1_ = 4;
  local_128 = 0;
  if (PTR_s___128_128_255_Blade__255_255_255_004ff460 != (undefined *)0x0) {
    local_12c = &PTR_s___128_128_255_Blade__255_255_255_004ff460;
    do {
      ppuVar1 = local_12c;
      this_00 = (int *)0x0;
      this = operator_new(0x54);
      local_4._0_1_ = 5;
      local_118 = this;
      if (this != (void *)0x0) {
        local_120 = &stack0xfffffeb8;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar6 = &DAT_00501908;
        FUN_00401d80(&stack0xfffffeb8,*ppuVar1);
        this_00 = FUN_00423db0(this,piVar6);
      }
      local_4._0_1_ = 4;
      FUN_00402770(&local_124,(byte *)"saves\\player%i.rb");
      piVar6 = local_124;
      local_120 = &stack0xfffffeb8;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar4 = &DAT_00501908;
      if (local_124 != &DAT_00501908) {
        *local_124 = *local_124 + 1;
        piVar4 = local_124;
      }
      bVar3 = PathExistsFindFirstFile(piVar4);
      if (bVar3) {
        local_120 = &stack0xfffffeb8;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar4 = &DAT_00501908;
        if (piVar6 != &DAT_00501908) {
          *piVar6 = *piVar6 + 1;
          piVar4 = piVar6;
        }
        FUN_00424140(this_00,piVar4);
      }
      else {
        piVar2 = (int *)this_00[4];
        piVar4 = this_00 + 4;
        if (piVar2 != piVar6) {
          if (piVar2 != &DAT_00501908) {
            *piVar2 = *piVar2 + -1;
            if (*(int *)*piVar4 < 1) {
              FUN_004011c0(piVar4);
            }
            else {
              *piVar4 = (int)&DAT_00501908;
            }
          }
          *piVar4 = (int)piVar6;
          if (piVar6 != &DAT_00501908) {
            *piVar6 = *piVar6 + 1;
          }
        }
        FUN_004243f0(this_00);
      }
      iVar5 = 0;
      if (0 < param_1[1]) {
        piVar4 = (int *)*param_1;
        do {
          if (*piVar4 == 0) goto LAB_004253f3;
          iVar5 = iVar5 + 1;
          piVar4 = piVar4 + 1;
        } while (iVar5 < param_1[1]);
      }
      FUN_0043b6e0(param_1,iVar5 + 1);
LAB_004253f3:
      *(int **)(*param_1 + iVar5 * 4) = this_00;
      ppuVar1 = local_12c + 1;
      local_12c = local_12c + 1;
      local_128 = local_128 + 1;
    } while (*ppuVar1 != (undefined *)0x0);
  }
  local_4._0_1_ = 3;
  if ((piVar6 != &DAT_00501908) && (iVar5 = *piVar6, *piVar6 = iVar5 + -1, iVar5 + -1 < 1)) {
    FUN_004011c0(&local_124);
  }
  local_4 = (uint)local_4._1_3_ << 8;
  if ((local_130 != &DAT_00501908) && (*local_130 = *local_130 + -1, *local_130 < 1)) {
    FUN_004011c0(&local_130);
  }
  ExceptionList = pvStack_c;
  return param_1;
}
