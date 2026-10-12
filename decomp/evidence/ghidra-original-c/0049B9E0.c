/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0049b9e0
 * Match: function entry
 * Entry: 0049b9e0 FUN_0049b9e0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


undefined4 * __fastcall FUN_0049b9e0(undefined4 *param_1)

{
  int iVar1;
  int *piVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d45be;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  param_1[0x401] = &PTR_FUN_004e4158;
  param_1[0x410] = 0x10;
  param_1[0x40f] = FUN_0043f660;
  param_1[0x40b] = 0;
  param_1[0x40c] = 0;
  param_1[0x40d] = 0;
  param_1[0x40e] = 0;
  param_1[0x403] = 0;
  param_1[0x402] = 0;
  param_1[0x406] = 0;
  param_1[0x405] = 0;
  *(undefined1 *)((int)param_1 + 0x1011) = 0;
  *(undefined1 *)(param_1 + 0x404) = 0;
  param_1[0x407] = 0;
  local_4 = 0;
  *param_1 = 1;
  iVar1 = 0;
  piVar2 = param_1 + 0x201;
  do {
    piVar2[-0x200] = 0;
    *piVar2 = iVar1 + 100;
    (**(code **)(param_1[0x401] + 4))(piVar2,1);
    iVar1 = iVar1 + 1;
    piVar2 = piVar2 + 1;
  } while (iVar1 < 0x200);
  ExceptionList = local_c;
  return param_1;
}


