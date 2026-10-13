
/* Summary: Maps a Player equipment name to its item slot.

   Algorithm: Returns slot 1 for axe, slot 2 for extinguisher, slot 3 for goggles, and zero for any
   other token.

   Parameters: param_1 points to the parsed command argument.

   Returns: Equipment slot number, or zero when the name is unsupported. */

byte __cdecl GetPlayerEquipmentSlotFromName(int *param_1)

{
  undefined4 uVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  bool bVar5;

  iVar2 = 4;
  bVar5 = true;
  pcVar3 = (char *)(*param_1 + 0xc);
  pcVar4 = "axe";
  do {
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    bVar5 = *pcVar3 == *pcVar4;
    pcVar3 = pcVar3 + 1;
    pcVar4 = pcVar4 + 1;
  } while (bVar5);
  if (bVar5) {
    return 1;
  }
  uVar1 = FUN_004019a0(param_1,(byte *)"extinguisher");
  if ((char)uVar1 != '\0') {
    return 2;
  }
  uVar1 = FUN_004019a0(param_1,(byte *)0x4e1890);
  return -((char)uVar1 != '\0') & 3;
}
