
void __fastcall FUN_0049a310(int *param_1)

{
  undefined4 *puVar1;
  void *_Memory;
  int iVar2;
  int *piVar3;
  
  param_1[0x1c] = param_1[0x1f];
  param_1[0x1d] = 0;
  *(undefined1 *)(param_1 + 0x1e) = 0;
  *(undefined1 *)((int)param_1 + 0x79) = 1;
  while (puVar1 = (undefined4 *)param_1[0x1c], puVar1 != (undefined4 *)0x0) {
    if (*(char *)((int)param_1 + 0x79) == '\0') {
      if ((char)param_1[0x1e] == '\0') {
        param_1[0x1d] = (int)puVar1;
        puVar1 = *(undefined4 **)(param_1[0x1c] + 4);
        param_1[0x1c] = (int)puVar1;
        if (puVar1 == (undefined4 *)0x0) break;
        piVar3 = (int *)*puVar1;
      }
      else {
        *(undefined1 *)(param_1 + 0x1e) = 0;
        piVar3 = *(int **)param_1[0x1c];
      }
    }
    else {
      *(undefined1 *)((int)param_1 + 0x79) = 0;
      piVar3 = (int *)*puVar1;
    }
    if (piVar3 == (int *)0x0) break;
    if ((char)piVar3[0x13] != '\0') {
      (**(code **)(*piVar3 + 0x74))();
    }
    if (*(char *)((int)piVar3 + 0x4d) != '\0') {
      (**(code **)(*piVar3 + 0x6c))();
      *(undefined1 *)((int)piVar3 + 0x4d) = 0;
      *(undefined1 *)(piVar3 + 0x13) = 0;
      _Memory = (void *)param_1[0x1c];
      iVar2 = *(int *)((int)_Memory + 4);
      param_1[0x1c] = iVar2;
      if (param_1[0x1d] != 0) {
        *(int *)(param_1[0x1d] + 4) = iVar2;
      }
      if ((void *)param_1[0x1f] == _Memory) {
        param_1[0x1f] = iVar2;
      }
      if ((void *)param_1[0x20] == _Memory) {
        param_1[0x20] = param_1[0x1d];
      }
      if ((param_1[0x20] == 0) && (param_1[0x1f] != 0)) {
        param_1[0x20] = param_1[0x1f];
      }
      FUN_00429d20(param_1 + 0x1b,(int)_Memory);
      _free(_Memory);
      param_1[0x21] = param_1[0x21] + -1;
      *(undefined1 *)(param_1 + 0x1e) = 1;
    }
  }
                    /* WARNING: Could not recover jumptable at 0x0049a3eb. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x60))();
  return;
}

