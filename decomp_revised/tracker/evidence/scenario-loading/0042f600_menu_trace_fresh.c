/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __thiscall FUN_0042f600(void *this,char param_1)

{
  undefined4 uVar1;
  int *unaff_ESI;
  int *piVar2;
  int *unaff_EDI;
  
  FUN_00435df0(this,param_1);
  if (param_1 != '\0') {
    if ((*(int *)(DAT_00502aac + 0x6ec) == 0) && (*(int *)(DAT_00502aac + 0x688) != 0)) {
      UpdateSelectedLevelBriefing(this,*(int **)(*(int *)(DAT_00502aac + 0x688) + 0x18),unaff_EDI);
      return;
    }
    piVar2 = (int *)(*(int *)(*(int *)(DAT_00502aac + 0x6e4) + 4) + 1);
    while ((-1 < (int)piVar2 &&
           (uVar1 = UpdateSelectedLevelBriefing(this,piVar2,unaff_ESI), (char)uVar1 == '\0'))) {
      piVar2 = (int *)((int)piVar2 + -1);
    }
  }
  return;
}
