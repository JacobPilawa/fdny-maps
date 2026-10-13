/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 00477740
 * Analysis output, not recovered original source.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   Read the supplied ScriptActor name. Map the special `<console>` name to `player`, look up the
   name in the shared ScriptActor registry, then resolve its entity handle through 0049B7E0.

   Parameters:
   param_1 points to the reference-counted actor-name string.

   Returns:
   The currently registered entity object for the sound origin, or null when the name is not
   registered.

   Evidence:
   Called by HandleActorSpatialSoundCommand (004778C0). The `<console>` alias, shared
   FindScriptActorByName lookup, and generation-checked entity resolution are visible in this
   function.

   Limits:
   This resolves a source entity; it does not establish that a sound is audible or which
   renderer/mixer plays it. */

undefined4 __cdecl FindScriptActorSoundOrigin(undefined4 *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  char *pcVar4;
  bool bVar5;
  int *piVar6;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  puStack_8 = &LAB_004d2cb8;
  local_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar3 = (int *)*param_1;
  local_10 = &DAT_00501908;
  ExceptionList = &local_c;
  if (piVar3 != &DAT_00501908) {
    ExceptionList = &local_c;
    *piVar3 = *piVar3 + 1;
    local_10 = piVar3;
  }
  iVar2 = 10;
  bVar5 = true;
  piVar3 = local_10 + 3;
  pcVar4 = "<console>";
  do {
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    bVar5 = (char)*piVar3 == *pcVar4;
    piVar3 = (int *)((int)piVar3 + 1);
    pcVar4 = pcVar4 + 1;
  } while (bVar5);
  local_4 = 0;
  if (bVar5) {
    FUN_00401d80(&local_10,"player");
  }
  piVar3 = local_10;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar6 = &DAT_00501908;
  if (local_10 != &DAT_00501908) {
    *local_10 = *local_10 + 1;
    piVar6 = local_10;
  }
  iVar2 = FindScriptActorByName(&DAT_00502fb0,piVar6);
  if (iVar2 == 0) {
    local_4 = 0xffffffff;
    if ((piVar3 != &DAT_00501908) && (iVar2 = *piVar3, *piVar3 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_10);
    }
    ExceptionList = local_c;
    return 0;
  }
  uVar1 = FUN_0049b7e0(*(void **)(DAT_00503a78 + 0x10),(int *)(iVar2 + 0x28));
  local_4 = 0xffffffff;
  if ((piVar3 != &DAT_00501908) && (iVar2 = *piVar3, *piVar3 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_10);
  }
  ExceptionList = local_c;
  return uVar1;
}
