/* Fresh Ghidra pseudocode captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0049BC90 (RegisterContactManagerEntity)
 * Analysis output, not recovered original source.
 */
/* Summary:
   Registers an eligible entity with the owner's type-specific contact list and active-entity list.
   
   Algorithm:
   - Reads the entity category/type from root +0x18 and chooses the corresponding list in the
   contact manager.
   - Calls that list's slot +0x04 registration method with the entity and active flag 1.
   - Increments the manager count and appends a node containing the entity to the manager's linked
   active list.
   
   Parameters:
   - ECX/this: contact-manager registration interface.
   - entity: root entity whose type selects a contact list.
   
   Returns:
   - No value; updates the selected category list and manager active list.
   
   Evidence:
   - Fresh Ghidra decompilation at 0x0049BC90.
   - Data reference at 0x004E411C and registration bridge in 0x00429490.
   - Gas R successful factory/spawn admission is traced in rescue_contact_manager_route.md.
   
   Limits:
   - The static successful-allocation route is mapped for Gas R BModel and BoundsTrigger records.
   Whether runtime allocation succeeds and when their contacts update require a capture. */

void __thiscall RegisterContactManagerEntity(void *this,void *entity)

{
  undefined4 *puVar1;
  
  (**(code **)(*(int *)((int)this + (*(int *)((int)entity + 0x18) + 1) * 0x48) + 4))(entity,1);
  *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + 1;
  puVar1 = operator_new(0xc);
  puVar1[1] = 0;
  *puVar1 = entity;
  *(undefined1 *)(puVar1 + 2) = 1;
  if (*(int *)((int)this + 0x14) == 0) {
    *(undefined4 **)((int)this + 0x10) = puVar1;
    *(undefined4 **)((int)this + 0x14) = puVar1;
    return;
  }
  *(undefined4 **)(*(int *)((int)this + 0x14) + 4) = puVar1;
  *(undefined4 **)((int)this + 0x14) = puVar1;
  return;
}


