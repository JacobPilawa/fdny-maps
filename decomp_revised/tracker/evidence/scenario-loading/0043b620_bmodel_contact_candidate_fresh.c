/* Fresh Ghidra pseudocode captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0043B620 (CheckBModelContactCandidate)
 * Analysis output, not recovered original source.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary:
   Runs the BModel root's contact test against one member supplied by the shared contact manager.
   
   Algorithm:
   - Requires the BModel contact model and a candidate whose root type at +0x18 is 1.
   - Requests candidate geometry through virtual slot +0x0C.
   - Reads authored E_Reach from the BModel user data at +0x30.
   - For hit-point-enabled, non-liftable BModels whose greatest child-motion endpoint lies outside
   the near-zero interval, raises a reach below 48 units to 48.
   - Tests the candidate AABB against the BModel bounds with the effective reach.
   - On overlap, creates a candidate pair key and inserts it into the current contact list at BModel
   root +0x160.
   
   Parameters:
   - ECX/this: BModel root.
   - candidate: type-1 contact candidate; the type gate does not prove all admitted objects are
   Players.
   
   Returns:
   - No value; appends a pair only when the class and geometry gates pass and the bounds test
   overlaps.
   
   Evidence:
   - Root slot +0x48, manager list traversal, and Gas R factory/spawn admission are documented in
   rescue_contact_manager_route.md.
   - Fresh Ghidra decompilation at 0x0043B620.
   - Training BModel reach/motion predicate was independently documented from the same hash-pinned
   executable.
   
   Limits:
   - Static successful-path admission is mapped for Gas R button1. Its actual overlap, candidate
   ordering at a location, pair-update timing, and the effective reach pending child-motion decoding
   remain unresolved. */

void __thiscall CheckBModelContactCandidate(void *this,int *candidate)

{
  int *this_00;
  ushort uVar1;
  float *pfVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined4 auStack_8 [2];
  
  this_00 = candidate;
  if (((0 < *(int *)((int)this + 0x1c4)) && (candidate[6] == 1)) &&
     (pfVar2 = (float *)(**(code **)(*candidate + 0xc))(), pfVar2 != (float *)0x0)) {
    candidate = *(int **)(*(int *)((int)this + 0xec) + 0x30);
    uVar3 = FUN_0043aff0((int)this);
    if (((char)uVar3 != '\0') && ((float)candidate < _DAT_004e0bbc)) {
      candidate = (int *)0x42400000;
    }
    uVar1 = FUN_004817a0((void *)((int)this + 0x240),pfVar2,pfVar2 + 4,pfVar2 + 7,(float)candidate);
    if ((char)uVar1 != '\0') {
      puVar4 = (undefined4 *)FUN_0049d350(this_00,auStack_8);
      FUN_004901e0((void *)((int)this + 0x160),*puVar4,puVar4[1]);
    }
  }
  return;
}


