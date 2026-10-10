/* Ghidra decompilation; names/types are provisional.
 * Entry: 0044e990
 * Function: FUN_0044e990
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0044e990(int *param_1)

{
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfe98;
  local_c = ExceptionList;
  if (param_1[0xe2] + 500U <= DAT_00502afc) {
    if (param_1[99] == 5) {
      ExceptionList = &local_c;
      param_1[99] = 3;
      param_1[0x40] = 0x43c80000;
    }
    else {
      if ((char)param_1[0x4b] == '\0') {
        return;
      }
      ExceptionList = &local_c;
      param_1[0x40] = (int)((float)param_1[0x40] + _DAT_004e1710);
    }
    local_10 = param_1;
    FUN_00402790(&local_10,"Jumps");
    local_4 = 0;
    FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_10,1);
    local_4 = 0xffffffff;
    if (local_10 != &DAT_00501908) {
      *local_10 = *local_10 + -1;
      if (*local_10 < 1) {
        FUN_004011c0(&local_10);
      }
      else {
        local_10 = &DAT_00501908;
      }
    }
    FUN_004a11a0(param_1 + 0xe8,(undefined1 *)0x0);
    param_1[0xe2] = DAT_00502afc;
  }
  ExceptionList = local_c;
  return;
}

