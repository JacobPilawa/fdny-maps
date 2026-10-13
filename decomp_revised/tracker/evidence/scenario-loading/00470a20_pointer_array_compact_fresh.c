/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* Summary: Stable-compacts a nullable pointer array and resizes it to the number of non-null
   entries.
   
   Algorithm: Scan from front to back, move each non-null pointer to the next write slot, clear the
   remaining tail slots, then resize the array. The relative order of retained entries is unchanged.
   
   Parameters: param_1 is the dynamic pointer-array object.
   
   Returns: void.
   
   Evidence: Fresh decompilation at 0x00470A20; called after queue-head removal and by another
   actor-registry removal path. */

void __fastcall CompactNullablePointerArray(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  iVar2 = 0;
  if (0 < param_1[1]) {
    do {
      iVar1 = *(int *)(*param_1 + iVar2 * 4);
      if (iVar1 != 0) {
        *(int *)(*param_1 + iVar3 * 4) = iVar1;
        iVar3 = iVar3 + 1;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < param_1[1]);
  }
  iVar2 = iVar3;
  if (iVar3 < param_1[1]) {
    do {
      *(undefined4 *)(*param_1 + iVar2 * 4) = 0;
      iVar2 = iVar2 + 1;
    } while (iVar2 < param_1[1]);
  }
  FUN_0043b6e0(param_1,iVar3);
  return;
}
