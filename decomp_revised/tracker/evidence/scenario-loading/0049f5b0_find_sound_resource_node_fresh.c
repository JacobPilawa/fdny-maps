/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 0049F5B0
 * Function: FindSoundResourceNodeByName
 * Analysis output, not recovered original source.
 */

/* Look up a resource name in the sound-resource tree rooted at this+4. If this+0x18 is zero,
   compare case-insensitively; otherwise compare bytes exactly. Return the matched tree node or
   null. Fresh EXE pseudocode, 2026-10-13. */

int __thiscall FindSoundResourceNodeByName(void *this,byte *param_1)

{
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  byte *_Str2;
  int iVar4;
  bool bVar5;

  if (*(int *)((int)this + 0x14) != 0) {
    for (iVar4 = *(int *)((int)this + 4) + *(int *)((int)this + 0x14); iVar4 != 0;
        iVar4 = *(int *)((int)this + 4) + iVar4) {
      _Str2 = (byte *)(iVar4 + 0x18);
      pbVar2 = param_1;
      if (*(char *)((int)this + 0x18) == '\0') {
        iVar3 = __stricmp((char *)param_1,(char *)_Str2);
      }
      else {
        do {
          bVar1 = *pbVar2;
          bVar5 = bVar1 < *_Str2;
          if (bVar1 != *_Str2) {
LAB_0049f604:
            iVar3 = (1 - (uint)bVar5) - (uint)(bVar5 != 0);
            goto LAB_0049f615;
          }
          if (bVar1 == 0) break;
          bVar1 = pbVar2[1];
          bVar5 = bVar1 < _Str2[1];
          if (bVar1 != _Str2[1]) goto LAB_0049f604;
          _Str2 = _Str2 + 2;
          pbVar2 = pbVar2 + 2;
        } while (bVar1 != 0);
        iVar3 = 0;
      }
LAB_0049f615:
      if (iVar3 == 0) {
        return iVar4;
      }
      if (iVar3 < 0) {
        iVar4 = *(int *)(iVar4 + 4);
      }
      else {
        iVar4 = *(int *)(iVar4 + 8);
      }
      if (iVar4 == 0) {
        return 0;
      }
    }
  }
  return 0;
}
