/* Fresh Ghidra decompilation
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: InitializePlayerRescueEntity at 0044D550
 * Ghidra analysis: current hash-pinned project; saved 2026-10-13
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: Read the entity's ScriptActor argument and register the embedded ScriptActor at entity
   +0x50 with the exact name "Player" through InitializeAndRegisterScriptActor. Then initialize the
   PlayerRescue entity's camera-rescue model, transform bounds, rescue state, movement, sound, and
   render fields. The separate PlayerHose initializer (00466D60) also registers "Player"; authored
   BSP class inventory separates these classes by map family.

   Parameters: param_1 points to the PlayerRescue entity.

   Returns: none. */

void __fastcall InitializePlayerRescueEntity(int *param_1)

{
  undefined4 *puVar1;
  void *this;
  int iVar2;
  int **ppiVar3;
  char *pcVar4;
  int *piVar5;
  int *piVar6;
  int *piVar7;
  char cVar8;
  int *local_98;
  undefined4 local_94;
  int **local_8c;
  int *local_88;
  undefined4 uStack_84;
  undefined4 local_80;
  int *local_78;
  char *local_74;
  char *local_70;
  char *local_6c;
  undefined4 local_68;
  undefined1 auStack_5c [72];
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined4 local_c;

  local_c = 0xffffffff;
  puStack_10 = &LAB_004cfddb;
  pvStack_14 = ExceptionList;
  ExceptionList = &pvStack_14;
  puVar1 = (undefined4 *)FUN_0049d350(param_1,&local_78);
  local_8c = (int **)&stack0xffffff48;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar5 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff48,"Player");
  piVar7 = param_1 + 0x50;
  InitializeAndRegisterScriptActor(piVar7,piVar5,puVar1);
  local_98 = operator_new(0x20);
  if (local_98 == (int *)0x0) {
    local_98 = (int *)0x0;
  }
  else {
    *local_98 = 0;
    local_98[1] = 0;
    local_98[2] = 0;
  }
  local_8c = operator_new(0x14);
  local_c = 0;
  if (local_8c == (int **)0x0) {
    this = (void *)0x0;
  }
  else {
    this = (void *)FUN_0049f230(local_8c);
  }
  local_c = 0xffffffff;
  piVar5 = FUN_0049b760(&local_78,"RCS_CamRescue",100);
  local_c = 1;
  iVar2 = FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar5);
  local_c = 0xffffffff;
  FUN_0049b6f0((int)&local_78);
  if (iVar2 != 0) {
    *(int *)((int)this + 0xc) = iVar2;
  }
  *(undefined4 *)((int)this + 8) = 0;
  FUN_0049d130(this,local_98);
  FUN_0049d4c0(this);
  param_1[0x1d] = 0x42340000;
  FUN_0042d280(param_1 + 0x1e,&local_78,0xc1f00000,0xc1f00000,0xc1f00000);
  FUN_0042d280(param_1 + 0x21,&local_78,0x41f00000,DAT_004e181c,0x41f00000);
  param_1[0xd0a] = 0;
  param_1[0xd10] = 0;
  param_1[0xd0c] = 0;
  param_1[0xd0d] = 0;
  FUN_004590c0(param_1 + 0xc95);
  param_1[0xd28] = (int)(param_1 + 9);
  piVar5 = param_1 + 0xd29;
  FUN_00483a20(piVar5,(int)(param_1 + 0xd18),'\x01');
  local_78 = (int *)0x3d4ccccd;
  local_74 = (char *)0x0;
  local_70 = (char *)0x3d4ccccd;
  local_88 = (int *)0x41a00000;
  uStack_84 = 0x447a0000;
  local_80 = 0x41a00000;
  FUN_004836a0(piVar5,&local_88,(float *)&local_78);
  FUN_00483a80(piVar5,0.0,0.05,0.0);
  FUN_00483670((int)piVar5);
  FUN_004576b0(param_1 + 0x17d,*(int **)(DAT_00503a78 + 4),param_1);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"Jump");
  local_c = 2;
  ConfigureAndRegisterSoundInstance(param_1 + 0xe8,&local_98,(int)(param_1 + 0xe5),2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"LandingImpact");
  local_c = 3;
  ConfigureAndRegisterSoundInstance(param_1 + 0xfa,&local_98,(int)(param_1 + 0xe5),2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"Breathe");
  local_c = 4;
  ConfigureAndRegisterSoundInstance(param_1 + 0xf1,&local_98,0,1);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"OxyRelease");
  local_c = 5;
  ConfigureAndRegisterSoundInstance(param_1 + 0x103,&local_98,0,2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"PassOut_O2");
  local_c = 6;
  ConfigureAndRegisterSoundInstance(param_1 + 0x10c,&local_98,0,2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"PassOut_Fire");
  local_c = 7;
  ConfigureAndRegisterSoundInstance(param_1 + 0x115,&local_98,0,2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"GrabItem");
  local_c = 8;
  ConfigureAndRegisterSoundInstance(param_1 + 0x127,&local_98,0,2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"ValveHookUp");
  local_c = 9;
  ConfigureAndRegisterSoundInstance(param_1 + 0x130,&local_98,0,2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"ThrowObject");
  local_c = 10;
  ConfigureAndRegisterSoundInstance(param_1 + 0x139,&local_98,0,2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_98 = &DAT_00501908;
  FUN_00401d80(&local_98,"ActionFailed");
  local_c = 0xb;
  ConfigureAndRegisterSoundInstance(param_1 + 0x142,&local_98,0,2);
  local_c = 0xffffffff;
  if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
    FUN_004011c0(&local_98);
  }
  pcVar4 = "FootStep1";
  local_8c = &local_78;
  local_78 = (int *)0x4e1910;
  local_74 = "FootStep2";
  local_70 = "FootStep3";
  local_6c = "FootStep4";
  local_68 = 0;
  do {
    local_98 = operator_new(0x24);
    local_c = 0xc;
    if (local_98 == (int *)0x0) {
      puVar1 = (undefined4 *)0x0;
    }
    else {
      puVar1 = InitializeSoundInstance(local_98);
    }
    local_c = 0xffffffff;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_98 = &DAT_00501908;
    FUN_00401d80(&local_98,pcVar4);
    local_c = 0xd;
    ConfigureAndRegisterSoundInstance(puVar1,&local_98,(int)(param_1 + 0xe5),2);
    local_c = 0xffffffff;
    if ((local_98 != &DAT_00501908) && (*local_98 = *local_98 + -1, *local_98 < 1)) {
      FUN_004011c0(&local_98);
    }
    iVar2 = 0;
    if (0 < param_1[0x151]) {
      piVar5 = (int *)param_1[0x150];
      do {
        if (*piVar5 == 0) goto LAB_0044ddc2;
        iVar2 = iVar2 + 1;
        piVar5 = piVar5 + 1;
      } while (iVar2 < param_1[0x151]);
    }
    FUN_0043b6e0(param_1 + 0x150,iVar2 + 1);
LAB_0044ddc2:
    local_8c = local_8c + 1;
    *(undefined4 **)(param_1[0x150] + iVar2 * 4) = puVar1;
    pcVar4 = (char *)*local_8c;
    if ((int *)pcVar4 == (int *)0x0) {
      local_8c = (int **)(param_1 + 0xd50);
      param_1[0x156] = 0;
      param_1[0xe1] = 0;
      *(undefined1 *)(param_1 + 0xe0) = 0;
      FUN_004895c0((int *)local_8c);
      param_1[0xd0e] = 0;
      if (*(int *)(DAT_00502aac + 0x688) == 0) {
        param_1[0xd0f] = 0x44160000;
      }
      else {
        param_1[0xd0f] = *(int *)(*(int *)(DAT_00502aac + 0x688) + 0x10);
      }
      iVar2 = *(int *)(*(int *)(DAT_00502aac + 0x6e4) + 0x2c);
      local_78 = (int *)0x0;
      local_74 = (char *)0x3ff00000;
      local_98 = (int *)0x0;
      local_94 = 0x3ff80000;
      local_88 = (int *)0x0;
      uStack_84 = 0x3ff80000;
      if (iVar2 == 0) {
        ppiVar3 = &local_88;
      }
      else {
        ppiVar3 = &local_98;
        if (iVar2 != 1) {
          ppiVar3 = &local_78;
        }
      }
      param_1[0xd0f] = (int)((float)param_1[0xd0f] * (float)*(double *)ppiVar3);
      if (param_1[0xd5a] != 0) {
        FUN_0043b6e0(param_1 + 0xd59,0);
      }
      FUN_0043b6e0(param_1 + 0xd59,5);
      param_1[0xd5f] = 0;
      FUN_004465d0(param_1 + 0x157,*(undefined4 *)(DAT_00503a78 + 4),param_1);
      FUN_00481dd0((int)(param_1 + 0xcb));
      iVar2 = (**(code **)(*param_1 + 0x18))();
      param_1[0x14d] = *(int *)(iVar2 + 0x28);
      param_1[0x14e] = *(int *)(iVar2 + 0x2c);
      param_1[0x14f] = *(int *)(iVar2 + 0x30);
      param_1[0x14c] =
           (int)(-*(float *)(param_1[0x65] + 4) * (float)_DAT_004e1120 * (float)_DAT_004e1118 -
                (float)_DAT_004e1110);
      FUN_0044cd10(param_1);
      FUN_0042d350(param_1 + 0xd18,auStack_5c);
      FUN_00483a20(param_1 + 0xd29,(int)(param_1 + 0xd18),'\x01');
      FUN_00454ba0((int)(param_1 + 0xd03));
      if (*(char **)(param_1[0x65] + 8) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 8));
        piVar5 = local_98;
        local_c = 0xe;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0xc) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0xc));
        piVar5 = local_98;
        local_c = 0xf;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x10) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x10));
        piVar5 = local_98;
        local_c = 0x10;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x14) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x14));
        piVar5 = local_98;
        local_c = 0x11;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x18) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x18));
        piVar5 = local_98;
        local_c = 0x12;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x1c) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x1c));
        piVar5 = local_98;
        local_c = 0x13;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x20) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x20));
        piVar5 = local_98;
        local_c = 0x14;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x24) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x24));
        piVar5 = local_98;
        local_c = 0x15;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x28) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x28));
        piVar5 = local_98;
        local_c = 0x16;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      if (*(char **)(param_1[0x65] + 0x2c) != (char *)0x0) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        local_98 = &DAT_00501908;
        FUN_00401d80(&local_98,*(char **)(param_1[0x65] + 0x2c));
        piVar5 = local_98;
        local_c = 0x17;
        if (local_98[1] != 0) {
          local_88 = (int *)&stack0xffffff48;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (local_98 != &DAT_00501908) {
            *local_98 = *local_98 + 1;
            piVar6 = local_98;
          }
          RouteParsedScriptActorCommand(piVar7,piVar6,0);
        }
        local_c = 0xffffffff;
        if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
          FUN_004011c0(&local_98);
        }
      }
      cVar8 = '\x01';
      local_88 = (int *)&stack0xffffff48;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      FUN_00401d80(&stack0xffffff48,"RC_ResLvlStart");
      FUN_004897c0(local_8c,piVar7,cVar8);
      ExceptionList = pvStack_14;
      return;
    }
  } while( true );
}
