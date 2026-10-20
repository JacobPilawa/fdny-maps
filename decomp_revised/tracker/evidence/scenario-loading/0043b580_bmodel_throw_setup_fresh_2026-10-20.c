/* Fresh Ghidra decompilation, captured 2026-10-20.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0043B580 (BModel throw trajectory setup)
 * Ghidra 12.0.4 read-only inspection of saved project FDNYPort with
 * -readOnly -noanalysis; no auto-analysis or runtime capture.
 * Pseudocode is analysis output, not recovered original source.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0043b580(int param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float *pfVar4;
  float local_c [3];
  
  *(undefined4 *)(param_1 + 0x208) = DAT_00502afc;
  pfVar4 = (float *)FUN_0043b2d0(local_c,(float *)(param_1 + 0x1f0),(float *)(param_1 + 0x1fc));
  fVar1 = pfVar4[2];
  fVar2 = pfVar4[1];
  fVar3 = *pfVar4;
  *(undefined4 *)(param_1 + 0x210) = 0xc3c00000;
  *(undefined4 *)(param_1 + 0x218) = *(undefined4 *)(param_1 + 500);
  *(undefined4 *)(param_1 + 0x25c) = 2;
  fVar1 = SQRT(fVar1 * fVar1 + fVar2 * fVar2 + fVar3 * fVar3) * _DAT_004e0bb8;
  *(float *)(param_1 + 0x20c) = fVar1;
  *(float *)(param_1 + 0x214) =
       ((*(float *)(param_1 + 0x200) - *(float *)(param_1 + 500)) - fVar1 * fVar1 * _DAT_004e0bb4) /
       fVar1;
  return;
}
