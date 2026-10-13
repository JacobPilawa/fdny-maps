/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 004A10D0
 * Function: UpdateSound3DAttributes
 * Analysis output, not recovered original source.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* If the instance has a source vector and 3D-attribute flag, send coordinates to
   SetFMOD3DAttributes. Mode value 2 computes a listener-relative vector and transforms it by the
   current listener orientation; other modes pass the stored vector directly. Fresh EXE pseudocode,
   2026-10-13. */

void __fastcall UpdateSound3DAttributes(void *param_1)

{
  float *pfVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float local_18 [3];
  float local_c;
  undefined4 local_8;
  undefined4 local_4;

  pfVar1 = *(float **)((int)param_1 + 0x10);
  if ((pfVar1 != (float *)0x0) && (*(char *)((int)param_1 + 0x1c) != '\0')) {
    if (*(int *)((int)param_1 + 0x18) == 2) {
      iVar2 = *(int *)(DAT_00503a78 + 8);
      fVar3 = pfVar1[2] - *(float *)(iVar2 + 0x84);
      fVar5 = pfVar1[1] - *(float *)(iVar2 + 0x80);
      fVar4 = *pfVar1 - *(float *)(iVar2 + 0x7c);
      local_18[2] = SQRT(fVar3 * fVar3 + fVar5 * fVar5 + fVar4 * fVar4);
      if (local_18[2] < _DAT_004e169c) {
        local_18[2] = _DAT_004e169c;
      }
      local_18[0] = 0.0;
      local_18[1] = 0.0;
      FUN_00495d20(&local_c,iVar2 + 0x54,local_18);
      SetFMOD3DAttributes(param_1,local_c,local_8,local_4);
      return;
    }
    SetFMOD3DAttributes(param_1,*pfVar1,pfVar1[1],pfVar1[2]);
  }
  return;
}
