/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 0049F850
 * Function: ResolveSoundResourceByName
 * Analysis output, not recovered original source.
 */

/* Resolve the sound name stored at param+0x0c through the sound-resource manager; return the
   resolved payload at treeNode+0x0c, or null. Fresh EXE pseudocode, 2026-10-13. */

int __thiscall ResolveSoundResourceByName(void *this,int *param_1)

{
  int iVar1;

  iVar1 = FindSoundResourceNodeByName((void *)((int)this + 8),(byte *)(*param_1 + 0xc));
  if (iVar1 != 0) {
    return iVar1 + 0xc;
  }
  return 0;
}
