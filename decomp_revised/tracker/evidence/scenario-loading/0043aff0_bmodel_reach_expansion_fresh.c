/* Fresh Ghidra pseudocode captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0043AFF0 (CheckBModelReachExpansion)
 * Analysis output, not recovered original source.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Checks whether a BModel's authored interaction reach should be widened from its
   child-motion end time.
   
   Algorithm:
   - Returns false unless parsed F_HitPoints at definition +0x34 is nonzero and G_Liftable at +0x38
   is zero.
   - Reads the greatest child-motion end time at BModel root +0x1EC. InitializeBModelEntity fills
   this from geMotion_GetTimeExtents for child models with motions; it remains zero when none have
   motions.
   - Compares that value with double constants -1e-28 and +1e-28. Returns true at or beyond either
   boundary; returns false strictly inside the near-zero interval. The unordered comparison path
   also returns true.
   
   Parameters:
   - ECX/this: BModel root whose parsed definition and child motion end time are checked.
   
   Returns:
   - Nonzero when the hit-point/non-liftable gates pass and the maximum child-motion end time lies
   outside the near-zero interval; otherwise zero.
   
   Caller:
   - CheckBModelContactCandidate (0x0043B620) raises authored E_Reach to 48 only when this helper
   returns true and the authored value is less than 48.
   - Gas R entity 309's `%Model%` child `button1` maps to BSP model 91; no MOTIONS record exists for
   model 91, so its endpoint remains zero and the helper does not expand E_Reach=20. See
   gas_r_button1_use_path.md.
   
   Evidence:
   - Fresh Ghidra decompilation and raw x86 at 0x0043AFF0.
   - Gas R BSP SHA-256 f6822f8d598d2888896165bbee8b554351230cd76ad6331abf0d735a03b54cbc.
   - Genesis3D 1.1 GBSP/Motion loader behavior is linked from gas_r_button1_use_path.md. */

uint __fastcall CheckBModelReachExpansion(int param_1)

{
  float fVar1;
  float fVar2;
  uint uVar3;
  undefined2 uVar4;
  
  uVar3 = *(uint *)(param_1 + 0xec);
  if ((*(int *)(uVar3 + 0x34) != 0) && (*(int *)(uVar3 + 0x38) == 0)) {
    fVar1 = *(float *)(param_1 + 0x1ec);
    fVar2 = (float)_DAT_004e0b68;
    uVar4 = (undefined2)(uVar3 >> 0x10);
    uVar3 = CONCAT22(uVar4,(ushort)(fVar1 < fVar2) << 8 | (ushort)(NAN(fVar1) || NAN(fVar2)) << 10 |
                           (ushort)(fVar1 == fVar2) << 0xe);
    if (fVar1 < fVar2) {
      fVar2 = (float)_DAT_004e0b60;
      uVar3 = CONCAT22(uVar4,(ushort)(fVar1 < fVar2) << 8 | (ushort)(NAN(fVar1) || NAN(fVar2)) << 10
                             | (ushort)(fVar1 == fVar2) << 0xe);
      if (fVar1 >= fVar2 && (fVar1 == fVar2) == 0) goto LAB_0043b024;
    }
    return CONCAT31((int3)(uVar3 >> 8),1);
  }
LAB_0043b024:
  return uVar3 & 0xffffff00;
}
