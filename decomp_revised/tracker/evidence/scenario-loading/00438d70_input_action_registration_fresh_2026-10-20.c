/* Fresh read-only Ghidra decompilation; names and types are provisional.
 * Retail input: FDNYFirefighter.exe, SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 *
 * Entry: 00438d70
 */


void __thiscall FUN_00438d70(void *this,int *param_1,undefined4 param_2,undefined1 param_3)

{
  int iVar1;
  int iVar2;
  void *this_00;
  undefined4 *puVar3;
  undefined1 *puVar4;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ce68b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar3 = operator_new(0x20);
  if (puVar3 == (undefined4 *)0x0) {
    puVar3 = (undefined4 *)0x0;
  }
  else {
    puVar3[6] = 0x10;
    puVar3[5] = FUN_00438d20;
    puVar3[1] = 0;
    puVar3[2] = 0;
    puVar3[3] = 0;
    puVar3[4] = 0;
    *puVar3 = 0;
    *(undefined1 *)(puVar3 + 7) = 0;
  }
  *puVar3 = param_2;
  *(undefined1 *)(puVar3 + 7) = param_3;
  iVar1 = *(int *)((int)this + 4);
  local_4 = 0xffffffff;
  FUN_0043b6e0(this,iVar1 + 1);
  *(undefined4 **)(*(int *)this + iVar1 * 4) = puVar3;
  iVar2 = *param_1;
  this_00 = *(void **)((int)this + 0x18);
  if ((this_00 != (void *)0x0) && (*(int *)((int)this_00 + 4) != 0)) {
    FUN_0043b6e0(this_00,0);
  }
  puVar4 = FUN_004387d0((void *)((int)this + 0x18),(int *)((int)this + 0x2c),(byte *)(iVar2 + 0xc));
  *(int *)(puVar4 + 0xc) = iVar1;
  ExceptionList = local_c;
  return;
}
