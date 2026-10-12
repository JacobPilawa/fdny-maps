/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047d660
 * Match: function entry
 * Entry: 0047d660 FUN_0047d660
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_0047d660(void *param_1)

{
  int *piVar1;
  int iVar2;
  ulonglong uVar3;
  
  piVar1 = (int *)((int)param_1 + 4);
  FUN_0048d490(*(void **)((int)param_1 + 0xc),*(int *)((int)param_1 + 4));
  uVar3 = FUN_004b9400();
  if ((uint)uVar3 <= DAT_00502afc) {
    *(uint *)((int)param_1 + 0x14) = DAT_00502afc;
    *piVar1 = *piVar1 + 1;
    iVar2 = FUN_0047d2b0(param_1,piVar1);
    if (((char)iVar2 != '\0') && ((*(byte *)((int)param_1 + 0x18) & 8) != 0)) {
      *(undefined1 *)((int)param_1 + 0x30) = 0;
    }
    *(int *)((int)param_1 + 8) = *piVar1 + 1;
    FUN_0047d2b0(param_1,(int *)((int)param_1 + 8));
  }
  return;
}


