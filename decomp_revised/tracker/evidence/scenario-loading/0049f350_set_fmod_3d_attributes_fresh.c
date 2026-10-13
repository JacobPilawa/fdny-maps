/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 0049F350
 * Function: SetFMOD3DAttributes
 * Analysis output, not recovered original source.
 */

/* Thin FMOD wrapper: for a non-null channel handle, call FSOUND_3D_SetAttributes(channel, xyz, null
   velocity). Fresh EXE pseudocode, 2026-10-13. */

void __thiscall
SetFMOD3DAttributes(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;

  if (*(int *)this != 0) {
    local_8 = param_2;
    local_c = param_1;
    local_4 = param_3;
    _FSOUND_3D_SetAttributes_12(*(int *)this,&local_c,0);
  }
  return;
}
