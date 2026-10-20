/* Fresh read-only Ghidra decompilation; names and types are provisional.
 * Retail input: FDNYFirefighter.exe, SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 *
 * Entry: 00438550
 */


uint __thiscall FUN_00438550(void *this,int *param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  int *piVar7;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar3 = FUN_0046e210((void *)((int)this + 0x18),(byte *)(*param_1 + 0xc));
  uVar4 = 0;
  if (iVar3 != 0) {
    uVar4 = 0;
    if ((int *)(iVar3 + 0xc) != (int *)0x0) {
      iVar3 = *(int *)(iVar3 + 0xc);
      uVar4 = FUN_0048d490(this,iVar3);
      piVar1 = *(int **)(*(int *)this + iVar3 * 4);
      if ((piVar1 != (int *)0x0) && (*piVar1 == 1)) {
        uVar4 = piVar1[2];
        iVar3 = 0;
        if (0 < (int)uVar4) {
          do {
            if ((iVar3 < 0) || (piVar1[2] <= iVar3)) {
              piVar7 = &local_18;
              for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
                *piVar7 = 0;
                piVar7 = piVar7 + 1;
              }
              local_18 = 1;
              local_14 = "Array index out of bounds";
              local_10 = "..\\BluelAPI/Container/SArray.h";
              local_c = 0x1cd;
              FUN_00422060(&local_18);
            }
            piVar7 = *(int **)(piVar1[1] + iVar3 * 4);
            piVar2 = (int *)*piVar7;
            if ((piVar2 != (int *)0x0) &&
               (uVar5 = (**(code **)(*piVar2 + 4))(piVar7[1]), (char)uVar5 != '\0')) {
              return CONCAT31((int3)((uint)uVar5 >> 8),1);
            }
            uVar4 = piVar1[2];
            iVar3 = iVar3 + 1;
          } while (iVar3 < (int)uVar4);
        }
      }
    }
  }
  return uVar4 & 0xffffff00;
}
