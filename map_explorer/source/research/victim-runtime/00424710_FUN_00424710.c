/* Ghidra decompilation; names/types are provisional.
 * Entry: 00424710
 * Function: FUN_00424710
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00424710(void *this,int *param_1)

{
  void *this_00;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd168;
  local_c = ExceptionList;
  this_00 = (void *)((int)this + 0x30);
  ExceptionList = &local_c;
  local_10 = this;
  FUN_004883d0((int)this_00);
  if ((char)param_1 != '\0') {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Completion Time");
    local_4 = 0;
    FUN_00488660(this_00,(int *)&param_1,1,0,0,0,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    FUN_004868a0((int)this_00);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Bonus Points");
    local_4 = 1;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    FUN_004868a0((int)this_00);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Rescues");
    local_4 = 2;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Shortest Rescue Time");
    local_4 = 3;
    FUN_00488660(this_00,(int *)&param_1,1,1,999999999,0,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Longest Rescue Time");
    local_4 = 4;
    FUN_00488660(this_00,(int *)&param_1,1,2,0,0,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    FUN_004868a0((int)this_00);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Axe Hits");
    local_4 = 5;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Gallons Sprayed");
    local_4 = 6;
    FUN_00488660(this_00,(int *)&param_1,2,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Objects Thrown");
    local_4 = 7;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Air Time");
    local_4 = 8;
    FUN_00488660(this_00,(int *)&param_1,1,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Fires Doused");
    local_4 = 9;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Serious Burns");
    local_4 = 10;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Jumps");
    local_4 = 0xb;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_10 = &DAT_00501908;
    FUN_00401d80(&local_10,"Total Steps");
    local_4 = 0xc;
    FUN_00488660(this_00,(int *)&local_10,0,0,0,0,0);
    local_4 = 0xffffffff;
    if ((local_10 != &DAT_00501908) && (*local_10 = *local_10 + -1, *local_10 < 1)) {
      FUN_004011c0(&local_10);
    }
  }
  ExceptionList = local_c;
  return;
}

