/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 004977b0
 * Match: function entry
 * Entry: 004977b0 FUN_004977b0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void * __cdecl FUN_004977b0(void *param_1,int param_2)

{
  undefined1 local_44 [64];
  undefined4 local_4;
  
  if (*(int *)(param_2 + 0x40) != 0) {
    FUN_00497300(local_44,*(int *)(param_2 + 0x40),param_2);
    local_4 = *(undefined4 *)(*(int *)(param_2 + 0x40) + 0x40);
    FUN_004977b0(param_1,(int)local_44);
    return param_1;
  }
  FUN_0042d0a0(param_1,param_2);
  return param_1;
}


