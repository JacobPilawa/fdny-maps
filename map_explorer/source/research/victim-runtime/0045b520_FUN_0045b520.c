/* Ghidra decompilation; names/types are provisional.
 * Entry: 0045b520
 * Function: FUN_0045b520
 */


void __thiscall FUN_0045b520(void *this,int param_1)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 != *(int *)(*(int *)this + 4)) {
    FUN_00474e20(param_1);
    if (1 < **(int **)this) {
      FUN_0045a6c0(this);
    }
    pvVar1 = *(void **)this;
    iVar2 = param_1;
    if ((0 < param_1) && (iVar3 = *(int *)((int)pvVar1 + 0xc), iVar3 != 1)) {
      iVar2 = param_1 + -1 + (iVar3 - (param_1 + -1) % iVar3);
    }
    if (iVar2 != *(int *)((int)pvVar1 + 8)) {
      pvVar1 = _realloc(pvVar1,iVar2 * 0x10 + 0x24);
      *(void **)this = pvVar1;
      *(int *)((int)pvVar1 + 8) = iVar2;
    }
    iVar2 = *(int *)(*(int *)this + 4);
    if (*(int *)(*(int *)this + 0x10) == 0) {
      if (iVar2 < param_1) {
        iVar3 = iVar2 << 4;
        iVar2 = param_1 - iVar2;
        do {
          FUN_0045a8e0((undefined4 *)(iVar3 + 0x14 + *(int *)this));
          iVar3 = iVar3 + 0x10;
          iVar2 = iVar2 + -1;
        } while (iVar2 != 0);
      }
    }
    else if (iVar2 < param_1) {
      iVar3 = iVar2 << 4;
      iVar2 = param_1 - iVar2;
      do {
        FUN_0045a5a0((undefined4 *)(iVar3 + 0x14 + *(int *)this),
                     *(undefined4 **)(*(int *)this + 0x10));
        iVar3 = iVar3 + 0x10;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      *(int *)(*(int *)this + 4) = param_1;
      return;
    }
    *(int *)(*(int *)this + 4) = param_1;
  }
  return;
}

