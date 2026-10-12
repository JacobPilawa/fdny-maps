/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047d2b0
 * Match: function entry
 * Entry: 0047d2b0 FUN_0047d2b0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


int __thiscall FUN_0047d2b0(void *this,int *param_1)

{
  int iVar1;
  int iVar2;
  uint3 uVar3;
  
  if (*(int *)((int)this + 0xc) == 0) {
    return 0;
  }
  iVar1 = *(int *)(*(int *)((int)this + 0xc) + 4);
  iVar2 = *param_1;
  uVar3 = (uint3)((uint)iVar2 >> 8);
  if (iVar2 < iVar1) {
    return (uint)uVar3 << 8;
  }
  if ((*(byte *)((int)this + 0x18) & 1) == 0) {
    *param_1 = iVar1 + -1;
    return CONCAT31(uVar3,1);
  }
  if (iVar1 == 1) {
    *param_1 = 0;
    return CONCAT31(uVar3,1);
  }
  *param_1 = iVar2 % iVar1;
  return CONCAT31((int3)((uint)(iVar2 / iVar1) >> 8),1);
}


