/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
void __thiscall FUN_0042f500(void *this,int param_1)

{
  char cVar1;
  void *pvVar2;
  undefined4 uVar3;
  int *unaff_EDI;
  int *piVar4;
  
  if (param_1 == 3) {
    FUN_0042d460(*(void **)((int)this + 0x28));
    return;
  }
  if ((param_1 == 0) || (param_1 == 1)) {
    pvVar2 = (void *)FUN_004225a0((void *)((int)DAT_00502aac + 0x6ac),*(int *)((int)this + 0x2c));
    if (pvVar2 == (void *)0x0) {
      return;
    }
    FUN_0042d1b0(*(int *)((int)this + 0x28));
    *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)((int)DAT_00502aac + 0x6e4);
    FUN_0042ab00(DAT_00502aac,pvVar2);
  }
  if (param_1 != 2) {
    if ((param_1 == 4) || (param_1 == 1)) {
      FUN_0042d3e0(*(void **)((int)this + 0x28),3);
    }
    if (param_1 != 5) {
      if (param_1 == 6) {
        if (*(int *)((int)this + 0x2c) < *(int *)(*(int *)((int)DAT_00502aac + 0x6e4) + 4) + 1) {
          *(int *)((int)this + 0x2c) = *(int *)((int)this + 0x2c) + 1;
        }
        uVar3 = FUN_0042f280(this,*(int **)((int)this + 0x2c),unaff_EDI);
        cVar1 = (char)uVar3;
        while (cVar1 == '\0') {
          piVar4 = (int *)(*(int *)((int)this + 0x2c) + -1);
          *(int **)((int)this + 0x2c) = piVar4;
          uVar3 = FUN_0042f280(this,piVar4,unaff_EDI);
          cVar1 = (char)uVar3;
        }
      }
      return;
    }
    if (0 < *(int *)((int)this + 0x2c)) {
      *(int *)((int)this + 0x2c) = *(int *)((int)this + 0x2c) + -1;
    }
    FUN_0042f280(this,*(int **)((int)this + 0x2c),unaff_EDI);
    return;
  }
  FUN_0042d1b0(*(int *)((int)this + 0x28));
  return;
}

