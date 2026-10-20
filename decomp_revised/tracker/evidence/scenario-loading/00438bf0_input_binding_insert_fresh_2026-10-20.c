/* Fresh read-only Ghidra decompilation; names and types are provisional.
 * Retail input: FDNYFirefighter.exe, SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 *
 * Entry: 00438bf0
 */


void __thiscall FUN_00438bf0(void *this,int param_1,int param_2,int *param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar1 = FUN_0046e210((void *)((int)this + 0x18),(byte *)(*param_3 + 0xc));
  if ((iVar1 != 0) && ((int *)(iVar1 + 0xc) != (int *)0x0)) {
    iVar1 = *(int *)(iVar1 + 0xc);
    FUN_0048d490(this,iVar1);
    piVar3 = *(int **)(*(int *)this + iVar1 * 4);
    if ((piVar3 != (int *)0x0) && (*piVar3 == param_2)) goto LAB_00438c6c;
  }
  piVar3 = &local_18;
  for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
    *piVar3 = 0;
    piVar3 = piVar3 + 1;
  }
  local_18 = 0;
  piVar3 = (int *)0x0;
  local_14 = "Invalid binding";
  local_10 = "main\\IO\\GameControls.cpp";
  local_c = 0x23;
  FUN_00422060(&local_18);
LAB_00438c6c:
  piVar2 = operator_new(8);
  if (piVar2 == (int *)0x0) {
    piVar2 = (int *)0x0;
  }
  else {
    *piVar2 = 0;
    piVar2[1] = -1;
  }
  iVar1 = *piVar2;
  piVar2[1] = param_4;
  if (iVar1 != param_1) {
    if (iVar1 != 0) {
      FUN_00429fe0(*(void **)(iVar1 + 4),(int)piVar2);
      *piVar2 = 0;
    }
    if (param_1 != 0) {
      FUN_0041b130((void *)(param_1 + 4),piVar2);
      *piVar2 = param_1;
    }
  }
  iVar1 = piVar3[2];
  FUN_0043b6e0(piVar3 + 1,iVar1 + 1);
  *(int **)(piVar3[1] + iVar1 * 4) = piVar2;
  return;
}
