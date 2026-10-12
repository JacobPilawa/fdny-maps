/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045a3d0
 * Match: function entry
 * Entry: 0045a3d0 FUN_0045a3d0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_0045a3d0(int param_1)

{
  float *this;
  float fVar1;
  float fVar2;
  float *pfVar3;
  float local_c [3];
  
  *(undefined4 *)(param_1 + 0x68) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(param_1 + 0x6c) = *(undefined4 *)(param_1 + 0x50);
  *(undefined4 *)(param_1 + 0x70) = *(undefined4 *)(param_1 + 0x54);
  local_c[0] = 24.0;
  this = (float *)(param_1 + 0x84);
  local_c[1] = 24.0;
  *this = 24.0;
  *(undefined4 *)(param_1 + 0x88) = 0x41c00000;
  local_c[2] = 24.0;
  *(undefined4 *)(param_1 + 0x8c) = 0x41c00000;
  pfVar3 = (float *)FUN_0042d2b0(this,local_c);
  *(float *)(param_1 + 0x78) = *pfVar3;
  *(float *)(param_1 + 0x7c) = pfVar3[1];
  *(float *)(param_1 + 0x80) = pfVar3[2];
  fVar1 = *(float *)(param_1 + 0x78);
  fVar1 = SQRT(*(float *)(param_1 + 0x80) * *(float *)(param_1 + 0x80) +
               *(float *)(param_1 + 0x7c) * *(float *)(param_1 + 0x7c) + fVar1 * fVar1);
  fVar2 = SQRT(*(float *)(param_1 + 0x8c) * *(float *)(param_1 + 0x8c) +
               *(float *)(param_1 + 0x88) * *(float *)(param_1 + 0x88) + *this * *this);
  if (fVar1 < fVar2) {
    *(float *)(param_1 + 0x74) = fVar2;
    return;
  }
  *(float *)(param_1 + 0x74) = fVar1;
  return;
}


