/* FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
   Fresh Ghidra 12.0.4 headless decompilation, read-only project query, 2026-10-15.
   Functions configure and update the Player oxygen visor effect child. */

void __thiscall FUN_00455ab0(void *this,char param_1)

{
  ulonglong uVar1;

  *(int *)((int)this + 0x124) = DAT_00502afc + -10;
  uVar1 = ConvertX87FloatToIntegerTowardZero();
  *(int *)((int)this + 0x128) = (int)uVar1;
  if (param_1 == '\0') {
    *(undefined4 *)((int)this + 300) = 0x2e0;
    *(undefined4 *)((int)this + 0x130) = 0xffffff00;
  }
  else {
    *(undefined4 *)((int)this + 300) = 0xffffff00;
    *(undefined4 *)((int)this + 0x130) = 0x2e0;
  }
  *(undefined1 *)((int)this + 0x17c) = 1;
  RequestSoundInstancePlayback((void *)((int)this + 0x134),(undefined1 *)0x0);
  return;
}

/* 00455F60 computes the screen rectangle while the effect is active. */
void __fastcall FUN_00455f60(void *param_1)

{
  ulonglong uVar1;
  undefined4 *puVar2;
  int local_10 [3];
  undefined4 local_4;

  if ((*(uint *)((int)param_1 + 0x124) < DAT_00502afc) &&
     (DAT_00502afc < *(uint *)((int)param_1 + 0x128))) {
    FUN_0047ec80(param_1,0x3dcccccd,0x3dcccccd,0x3f800000,0x3f800000);
    local_10[0] = 0;
    local_10[1] = 0x280;
    uVar1 = ConvertX87FloatToIntegerTowardZero();
    local_10[2] = (int)uVar1;
    uVar1 = ConvertX87FloatToIntegerTowardZero();
    local_4 = (undefined4)uVar1;
    FUN_0047edf0(param_1,local_10,0);
    FUN_0047dc90(param_1,'\x01');
    return;
  }
  if (*(char *)((int)param_1 + 0x17c) != '\0') {
    if (*(int *)((int)param_1 + 300) < *(int *)((int)param_1 + 0x130)) {
      RequestSoundInstancePlayback((void *)((int)param_1 + 0x158),(undefined1 *)0x0);
      puVar2 = (undefined4 *)0x4;
    }
    else {
      puVar2 = (undefined4 *)0x5;
    }
    FUN_0042a730(DAT_00502aac,puVar2,(int *)&DAT_00502ab4);
    *(undefined1 *)((int)param_1 + 0x17c) = 0;
  }
  FUN_0047dc90(param_1,'\0');
  return;
}

/* Player status initialization calls this on status + 0x1b6c. */
void __fastcall FUN_00456680(void *param_1)

{
  int *local_14;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0b00;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0047ef60(param_1,0x2be,'\x01');
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"VisorUpDown");
  local_4 = 0;
  ConfigureAndRegisterSoundInstance((void *)((int)param_1 + 0x134),&local_14,0,1);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (*local_14 = *local_14 + -1, *local_14 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_10 = &DAT_00501908;
  FUN_00401d80(&local_10,"VisorLock");
  local_4 = 1;
  ConfigureAndRegisterSoundInstance((void *)((int)param_1 + 0x158),&local_10,0,1);
  local_4 = 0xffffffff;
  if ((local_10 != &DAT_00501908) && (*local_10 = *local_10 + -1, *local_10 < 1)) {
    FUN_004011c0(&local_10);
  }
  *(undefined4 *)((int)param_1 + 0x180) = 0x3f19999a;
  ExceptionList = local_c;
  return;
}
