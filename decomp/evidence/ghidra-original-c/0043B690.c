/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0043b690
 * Match: scratch-defined function boundary
 * Entry: 0043b690 FUN_0043b690
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void FUN_0043b690(float *param_1,float *param_2,float param_3)

{
  ushort uVar1;
  undefined4 *puVar2;
  void *unaff_EBX;
  int unaff_EDI;
  float *unaff_retaddr;
  
  uVar1 = FUN_004817a0((void *)(unaff_EDI + 0x240),unaff_retaddr,param_1,param_2,param_3);
  if ((char)uVar1 != '\0') {
    puVar2 = (undefined4 *)FUN_0049d350(unaff_EBX,(undefined4 *)&stack0x0000001c);
    FUN_004901e0((void *)(unaff_EDI + 0x160),*puVar2,puVar2[1]);
  }
  return;
}


