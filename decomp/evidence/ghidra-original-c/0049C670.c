/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0049c670
 * Match: function entry
 * Entry: 0049c670 FUN_0049c670
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_0049c670(int param_1)

{
  undefined4 *_Memory;
  void *_Memory_00;
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int local_3c;
  int local_38;
  undefined4 local_34;
  undefined4 local_30;
  code *local_2c;
  undefined4 local_28;
  int local_24;
  char *pcStack_20;
  char *pcStack_1c;
  undefined4 uStack_18;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d4698;
  pvStack_c = ExceptionList;
  local_28 = 0x10;
  local_2c = FUN_0043f660;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_4 = 0;
  ExceptionList = &pvStack_c;
  while (iVar1 = local_38, _Memory = *(undefined4 **)(param_1 + -0x30), _Memory != (undefined4 *)0x0
        ) {
    iVar4 = _Memory[1];
    _Memory_00 = (void *)*_Memory;
    *(int *)(param_1 + -0x30) = iVar4;
    if (iVar4 == 0) {
      *(undefined4 *)(param_1 + -0x2c) = 0;
    }
    FUN_00429d20((void *)(param_1 + -0x40),(int)_Memory);
    _free(_Memory);
    *(int *)(param_1 + -0x28) = *(int *)(param_1 + -0x28) + -1;
    if (_Memory_00 == (void *)0x0) break;
    piVar2 = (int *)(**(code **)(**(int **)((int)_Memory_00 + 0xc) + 8))(_Memory_00);
    FUN_00429490(DAT_00502aac,piVar2);
    FUN_0043b6e0(&local_3c,iVar1 + 1);
    *(int **)(local_3c + iVar1 * 4) = piVar2;
    FUN_0043f660();
    _free(_Memory_00);
  }
  iVar4 = 0;
  if (0 < iVar1) {
    do {
      if ((iVar4 < 0) || (iVar1 <= iVar4)) {
        piVar2 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        local_24 = 1;
        pcStack_20 = "Array index out of bounds";
        pcStack_1c = "..\\BluelAPI/Container/SArray.h";
        uStack_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      (**(code **)(**(int **)(local_3c + iVar4 * 4) + 0x20))();
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar1);
  }
  iVar4 = 0;
  if (0 < iVar1) {
    do {
      if ((iVar4 < 0) || (iVar1 <= iVar4)) {
        piVar2 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        local_24 = 1;
        pcStack_20 = "Array index out of bounds";
        pcStack_1c = "..\\BluelAPI/Container/SArray.h";
        uStack_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      (**(code **)(**(int **)(local_3c + iVar4 * 4) + 0x24))();
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar1);
  }
  local_4 = 0xffffffff;
  if (iVar1 != 0) {
    FUN_0043b6e0(&local_3c,0);
  }
  ExceptionList = pvStack_c;
  return;
}


