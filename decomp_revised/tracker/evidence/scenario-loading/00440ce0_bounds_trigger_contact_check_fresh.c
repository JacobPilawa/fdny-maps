/* Fresh Ghidra decompilation captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 00440CE0 (CheckBoundsTriggerContact)
 * The pseudocode/listing is analysis output, not recovered source.
 */
/* Summary: Checks one candidate entity against a BoundsTrigger and records a matching contact pair.
   Algorithm:
   - Requires the trigger's model data and a valid trigger model/bounds.
   - Obtains the candidate's world-space geometry through virtual slot +0x0C.
   - Calls the overlap helper with the trigger bounds and candidate geometry.
   - On overlap, forms a pair key and appends it to the contact container at BoundsTrigger root
   +0x100.
   Parameters:
   - this: BoundsTrigger root entity.
   - param_1: candidate entity supplied by the owner contact manager.
   Returns:
   - No value.
   Evidence:
   - Fresh Ghidra pseudocode and assembly at 0x00440CE0.
   - BoundsTrigger root slot +0x48, shared manager routing, and Gas R factory/spawn admission are
   documented in rescue_contact_manager_route.md.
   Limits:
   - Static successful-path admission is mapped for Gas R instances 265 and 280. Their actual
   overlap, live touch dispatch, and update timing have not been captured. */

void __thiscall CheckBoundsTriggerContact(void *this,int *param_1)

{
  ushort uVar1;
  int iVar2;
  float *pfVar3;
  undefined4 *puVar4;
  undefined4 auStack_8 [2];
  
  if ((((*(int *)(*(int *)((int)this + 0xfc) + 8) != 0) &&
       (iVar2 = FUN_00482630((int)this + 0x24), iVar2 != 0)) &&
      (pfVar3 = (float *)(**(code **)(*param_1 + 0xc))(), pfVar3 != (float *)0x0)) &&
     (uVar1 = FUN_004817a0((void *)((int)this + 0xe4),pfVar3,pfVar3 + 4,pfVar3 + 7,
                           *(float *)((int)this + 0x160)), (char)uVar1 != '\0')) {
    puVar4 = (undefined4 *)FUN_0049d350(param_1,auStack_8);
    FUN_004901e0((void *)((int)this + 0x100),*puVar4,puVar4[1]);
  }
  return;
}

