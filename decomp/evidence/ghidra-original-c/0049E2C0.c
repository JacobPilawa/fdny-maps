/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0049e2c0
 * Match: function entry
 * Entry: 0049e2c0 FUN_0049e2c0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_0049e2c0(int param_1)

{
  int *piVar1;
  
  if (*(char *)(param_1 + 0x28) == '\0') {
    piVar1 = (int *)(param_1 + 0x2c);
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      FUN_00471310(*(char **)(param_1 + 0x24),'\0');
      *(undefined1 *)(param_1 + 0x28) = 1;
      *(undefined4 *)(DAT_00502aac + 0x6f0) = 3;
    }
  }
  return;
}


