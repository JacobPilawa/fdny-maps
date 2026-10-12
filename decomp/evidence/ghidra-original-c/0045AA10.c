/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045aa10
 * Match: function entry
 * Entry: 0045aa10 FUN_0045aa10
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_0045aa10(void *this,int *param_1,int *param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  puVar1 = *(undefined4 **)((int)this + 4);
  puVar4 = puVar1;
  if (*(char *)((int)puVar1[1] + 0x15) == '\0') {
    puVar2 = (undefined4 *)puVar1[1];
    do {
      if ((int)puVar2[3] < *param_2) {
        puVar3 = (undefined4 *)puVar2[2];
      }
      else {
        puVar3 = (undefined4 *)*puVar2;
        puVar4 = puVar2;
      }
      puVar2 = puVar3;
    } while (*(char *)((int)puVar3 + 0x15) == '\0');
  }
  if ((puVar4 != puVar1) && ((int)puVar4[3] <= *param_2)) {
    *param_1 = (int)puVar4;
    return;
  }
  *param_1 = (int)puVar1;
  return;
}


