
void __fastcall FUN_0042aea0(char *param_1)

{
  char *this;
  
  if (param_1[0x68c] != '\0') {
    param_1[0x68c] = '\0';
    FUN_0042ab00(param_1,param_1 + 0x660);
  }
  FUN_0042c080((int *)&DAT_00502ab8);
  (**(code **)(*(int *)*DAT_00503a78 + 0x14))();
  if (param_1[0x64c] != '\0') {
    FUN_0042d8c0((int)(param_1 + 0x3c8));
    FUN_004a0660(*(int **)(param_1 + 0x3b0));
    return;
  }
  if (DAT_00502a78 != '\0') {
    FUN_0042a540(param_1);
  }
  FUN_00429520((int)param_1);
  FUN_00494670((void *)DAT_00503a78[1],(undefined *)0x0);
  if (*(int *)(param_1 + 0x6f0) == 3) {
    if (*(int *)(param_1 + 0x688) != 0) {
      FUN_0042a730(param_1,(undefined4 *)0x2,(int *)(*(int *)(param_1 + 0x688) + 0xc));
    }
    param_1[0x6f0] = '\x01';
    param_1[0x6f1] = '\0';
    param_1[0x6f2] = '\0';
    param_1[0x6f3] = '\0';
  }
  if (*(int *)(param_1 + 0x6ec) != 0) {
    if (*(int *)(param_1 + 0x6ec) == 1) {
      FUN_00424650(*(void **)(param_1 + 0x6e4),*(int *)(*(int *)(param_1 + 0x688) + 0x18));
    }
    this = param_1 + 0x3c8;
    FUN_0042d480((int)this);
    FUN_0042cef0((int)this);
    FUN_0042cf10(this,0);
    FUN_0042cf10(this,1);
    if (*(int *)(param_1 + 0x6ec) == 2) {
      FUN_0042d3e0(this,2);
      return;
    }
    FUN_0042d3e0(this,5);
  }
  return;
}

