/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 0049FDA0
 * Function: FindOrCreateSoundResourceByName
 * Analysis output, not recovered original source.
 */

/* Algorithm: Find an existing sound resource by name or create one from the supplied definition,
   then initialize its sample/stream payload.
   Parameters: manager/resource table, sound name, source path, and the type flag forwarded from the
   resource-definition parser.
   Returns: the resource payload associated with the name.
   Evidence: Fresh pseudocode; called by ParseSoundBufferDefinition and reached by sound-name
   resolution.
   Limits: Resource lifetime and duplicate-name policy beyond the shown lookup/create branch are not
   generalized here. */

undefined1 * __thiscall
FindOrCreateSoundResourceByName(void *this,int *param_1,int *param_2,undefined1 param_3)

{
  void *this_00;
  int iVar1;
  undefined1 *puVar2;
  undefined4 *this_01;

  if (*(char *)this == '\0') {
    FUN_0049f6f0(this);
  }
  this_01 = (undefined4 *)((int)this + 8);
  iVar1 = FindSoundResourceNodeByName(this_01,(byte *)(*param_1 + 0xc));
  if ((iVar1 == 0) || (puVar2 = (undefined1 *)(iVar1 + 0xc), puVar2 == (undefined1 *)0x0)) {
    iVar1 = *param_1;
    this_00 = (void *)*this_01;
    if ((this_00 != (void *)0x0) && (*(int *)((int)this_00 + 4) != 0)) {
      FUN_0043b6e0(this_00,0);
    }
    puVar2 = FUN_0049fa20(this_01,(int *)((int)this + 0x1c),(byte *)(iVar1 + 0xc));
    puVar2 = puVar2 + 0xc;
    InitializeFMODSoundResource(puVar2,param_2,param_3);
  }
  return puVar2;
}
