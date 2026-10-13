/* Fresh Ghidra decompilation: FUN_00490130 at 0x00490130;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   Ghidra currently types the function as void, but the instruction listing shows
   CALL 0x0048fea0 immediately followed by stack cleanup and RET. EAX therefore carries
   the lower helper's boolean result to callers, including UpdateVictimRoamTarget.
   */

bool __cdecl CheckActiveWorldSegmentClearance(float *param_1,float *param_2,char param_3)

{
  bool bVar1;

  bVar1 = IsWorldSegmentClear(*(int *)(DAT_00503a78 + 4),param_1,param_2,param_3);
  return bVar1;
}
