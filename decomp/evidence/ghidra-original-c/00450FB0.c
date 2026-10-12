/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00450fb0
 * Match: function entry
 * Entry: 00450fb0 FUN_00450fb0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00450fb0(void *this,int param_1)

{
  int iVar1;
  void *this_00;
  int *piVar2;
  char cVar3;
  
  if (param_1 == 0) {
    this_00 = (void *)((int)this + 0x454);
  }
  else {
    if (param_1 != 1) goto LAB_00450fd6;
    this_00 = (void *)((int)this + 0x430);
  }
  FUN_004a11a0(this_00,(undefined1 *)0x0);
LAB_00450fd6:
  cVar3 = '\x01';
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar2 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff0,"RC_PassOut");
  FUN_004897c0((void *)((int)this + 0x3540),piVar2,cVar3);
  if (param_1 == 2) {
    FUN_0044ff10(this,'\x01');
    return;
  }
  iVar1 = *(int *)((int)this + 0x357c);
  *(undefined4 *)((int)this + 0x342c) = DAT_00502afc;
  *(undefined4 *)((int)this + 0x18c) = 6;
  if (iVar1 != 0) {
    FUN_0048d490((int *)((int)this + 0x3564),iVar1);
    (**(code **)(**(int **)(*(int *)((int)this + 0x3564) + iVar1 * 4) + 0x1c))();
  }
  return;
}


