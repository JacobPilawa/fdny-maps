/* Ghidra decompilation; names/types are provisional.
 * Entry: 004395c0
 * Function: FUN_004395c0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __thiscall FUN_004395c0(void *this,int *param_1)

{
  int *this_00;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004ce959;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined4 *)((int)this + 4) = 0;
  local_4 = 0;
  *(undefined ***)this = &PTR_LAB_004e0b44;
  FUN_00437840((undefined4 *)((int)this + 0x10));
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined4 *)((int)this + 8) = &PTR_LAB_004e0a14;
  this_00 = (int *)((int)this + 0x334);
  local_4._0_1_ = 1;
  FUN_00439230(this_00);
  local_4._0_1_ = 2;
  FUN_00437f30((undefined4 *)((int)this + 8),this_00);
  FUN_00437890((undefined4 *)((int)this + 0x10));
  *(undefined1 *)((int)this + 0x330) = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"DBG_TOGGLE_FRAMERATE");
  local_4._0_1_ = 3;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"DBG_TOGGLE_RENDERSTATS");
  local_4._0_1_ = 4;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"DBG_CLOSE_ALL_AREAS");
  local_4._0_1_ = 5;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"DBG_SKIP_HOTSPOT");
  local_4._0_1_ = 6;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"DBG_ACTIVATE_RESCUEBOT");
  local_4._0_1_ = 7;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"DBG_TOGGLE_SPECT");
  local_4._0_1_ = 8;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"DBG_WIN_LEVEL");
  local_4._0_1_ = 9;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"DBG_CRASH_TEST");
  local_4._0_1_ = 10;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"SELECT_AXE");
  local_4._0_1_ = 0xb;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"SELECT_EXT");
  local_4._0_1_ = 0xc;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"SELECT_HOSE");
  local_4._0_1_ = 0xd;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"SELECT_GOGGLES");
  local_4._0_1_ = 0xe;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"STEER_YAW");
  local_4._0_1_ = 0xf;
  FUN_00438d70(this_00,(int *)&param_1,2,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"STEER_PITCH");
  local_4._0_1_ = 0x10;
  FUN_00438d70(this_00,(int *)&param_1,2,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_FORWARD");
  local_4._0_1_ = 0x11;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_BACKWARD");
  local_4._0_1_ = 0x12;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_LEFT");
  local_4._0_1_ = 0x13;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_RIGHT");
  local_4._0_1_ = 0x14;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"ACTION");
  local_4._0_1_ = 0x15;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"USE");
  local_4._0_1_ = 0x16;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"JUMP");
  local_4._0_1_ = 0x17;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"CROUCH");
  local_4._0_1_ = 0x18;
  FUN_00438d70(this_00,(int *)&param_1,1,0);
  local_4._0_1_ = 2;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00502a78 != '\0') {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"DBG_TOGGLE_RENDERSTATS");
    local_4._0_1_ = 0x19;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x3c);
    local_4._0_1_ = 2;
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
    FUN_00401d80(&param_1,"DBG_TOGGLE_FRAMERATE");
    local_4._0_1_ = 0x1a;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x3d);
    local_4._0_1_ = 2;
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
    FUN_00401d80(&param_1,"DBG_CLOSE_ALL_AREAS");
    local_4._0_1_ = 0x1b;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x3e);
    local_4._0_1_ = 2;
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
    FUN_00401d80(&param_1,"DBG_SKIP_HOTSPOT");
    local_4._0_1_ = 0x1c;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x40);
    local_4._0_1_ = 2;
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
    FUN_00401d80(&param_1,"DBG_ACTIVATE_RESCUEBOT");
    local_4._0_1_ = 0x1d;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x40);
    local_4._0_1_ = 2;
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
    FUN_00401d80(&param_1,"DBG_TOGGLE_SPECT");
    local_4._0_1_ = 0x1e;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x41);
    local_4._0_1_ = 2;
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
    FUN_00401d80(&param_1,"DBG_WIN_LEVEL");
    local_4._0_1_ = 0x1f;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x42);
    local_4._0_1_ = 2;
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
    FUN_00401d80(&param_1,"DBG_CRASH_TEST");
    local_4._0_1_ = 0x20;
    FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x57);
    local_4._0_1_ = 2;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"SELECT_AXE");
  local_4._0_1_ = 0x21;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,2);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"SELECT_EXT");
  local_4._0_1_ = 0x22;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,3);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"SELECT_GOGGLES");
  local_4._0_1_ = 0x23;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,4);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"USE");
  local_4._0_1_ = 0x24;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x2a);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"USE");
  local_4._0_1_ = 0x25;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x36);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"USE");
  local_4._0_1_ = 0x26;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x39);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"STEER_YAW");
  local_4._0_1_ = 0x27;
  FUN_00438bf0(this_00,(int)this,2,(int *)&param_1,0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"STEER_PITCH");
  local_4._0_1_ = 0x28;
  FUN_00438bf0(this_00,(int)this,2,(int *)&param_1,1);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_FORWARD");
  local_4._0_1_ = 0x29;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,200);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_BACKWARD");
  local_4._0_1_ = 0x2a;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0xd0);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_LEFT");
  local_4._0_1_ = 0x2b;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0xcb);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_RIGHT");
  local_4._0_1_ = 0x2c;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0xcd);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_FORWARD");
  local_4._0_1_ = 0x2d;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x11);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_BACKWARD");
  local_4._0_1_ = 0x2e;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x1f);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_LEFT");
  local_4._0_1_ = 0x2f;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x1e);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"MOVE_RIGHT");
  local_4._0_1_ = 0x30;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x20);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"CROUCH");
  local_4._0_1_ = 0x31;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x2e);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"CROUCH");
  local_4._0_1_ = 0x32;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x1d);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"CROUCH");
  local_4._0_1_ = 0x33;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x9d);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"ACTION");
  local_4._0_1_ = 0x34;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x103);
  local_4._0_1_ = 2;
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
  FUN_00401d80(&param_1,"JUMP");
  local_4._0_1_ = 0x35;
  FUN_00437410((void *)((int)this + 8),this_00,(int *)&param_1,0x105);
  local_4 = CONCAT31(local_4._1_3_,2);
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return this;
}

