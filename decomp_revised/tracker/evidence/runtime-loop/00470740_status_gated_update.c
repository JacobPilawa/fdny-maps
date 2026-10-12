
void __fastcall FUN_00470740(undefined4 *param_1)

{
  if (param_1[0x13] == 1) {
    if ((uint)param_1[0x12] <= DAT_00502afc) {
      param_1[0x13] = 0;
    }
  }
  else {
    (**(code **)*param_1)();
    param_1[0x14] = param_1[0x13];
  }
  if (param_1[0x13] == 0) {
    FUN_004704a0(param_1);
    return;
  }
  return;
}

