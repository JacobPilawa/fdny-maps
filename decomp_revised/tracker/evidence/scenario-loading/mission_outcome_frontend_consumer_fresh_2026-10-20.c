/*
 * Selected Ghidra 12.0.4 decompilation from the disposable copy of the
 * FDNYPort project, read-only with respect to the retained analysis.
 * Input: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 *
 * This records the frontend consumer of the win/loss manager state written
 * by 004295B0, plus the selected-ordinal profile mutation and child switch.
 * It is static control-flow evidence; it does not identify page-5 presentation
 * or establish runtime timing/parity.
 */

/* 0042AEA0: result state is consumed during the EverGame update. */
if (*(int *)(param_1 + 0x6ec) != 0) {
  if (*(int *)(param_1 + 0x6ec) == 1) {
    FUN_00424650(*(void **)(param_1 + 0x6e4),
                 *(int *)(*(int *)(param_1 + 0x688) + 0x18));
  }
  this = param_1 + 0x3c8;
  FUN_0042d480((int)this);
  FUN_0042cef0((int)this);
  FUN_0042cf10(this,0);
  FUN_0042cf10(this,1);
  if (*(int *)(param_1 + 0x6ec) == 2) {
    FUN_0042d3e0(this,2);
    return;
  }
  FUN_0042d3e0(this,5);
}

/* 00424650: win-only call above updates the record for the selected ordinal. */
void __thiscall FUN_00424650(void *this,int param_1)
{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  int *extraout_ECX;
  int *piVar5;

  iVar3 = FUN_004225a0((void *)(DAT_00502aac + 0x6ac),param_1);
  if (iVar3 != 0) {
    puVar1 = (undefined4 *)(iVar3 + 4);
    piVar5 = extraout_ECX;
    FUN_00401cb0(&stack0xffffffe8,puVar1);
    puVar4 = (undefined4 *)FUN_00423a70(this,piVar5);
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = FUN_00423fa0((int)this);
    }
    piVar5 = (int *)puVar4[0xb];
    puVar2 = puVar4 + 0xb;
    if (piVar5 != (int *)*puVar1) {
      if (piVar5 != &DAT_00501908) {
        *piVar5 = *piVar5 + -1;
        if (*(int *)*puVar2 < 1) {
          FUN_004011c0(puVar2);
        }
        else {
          *puVar2 = &DAT_00501908;
        }
      }
      piVar5 = (int *)*puVar1;
      *puVar2 = piVar5;
      if (piVar5 != &DAT_00501908) {
        *piVar5 = *piVar5 + 1;
      }
    }
    FUN_00488710(puVar4 + 2,(int *)((int)this + 0x30));
    *(undefined1 *)(puVar4 + 0xc) = 1;
    if (*(int *)((int)this + 4) < param_1) {
      *(int *)((int)this + 4) = param_1;
    }
    *(int *)((int)this + 8) = param_1;
    FUN_004243f0(this);
  }
  return;
}

/* 0042D3E0: current menu child is deactivated and the requested child enabled. */
void __thiscall FUN_0042d3e0(void *this,int param_1)
{
  int *this_00;
  undefined4 *puVar1;
  int iVar2;
  int *piVar3;

  if ((-1 < param_1) && (param_1 < *(int *)((int)this + 0x15c))) {
    this_00 = (int *)((int)this + 0x158);
    iVar2 = 0;
    if (0 < *(int *)((int)this + 0x15c)) {
      piVar3 = (int *)*this_00;
      do {
        if (*piVar3 == *(int *)((int)this + 0x10)) goto LAB_0042d41f;
        iVar2 = iVar2 + 1;
        piVar3 = piVar3 + 1;
      } while (iVar2 < *(int *)((int)this + 0x15c));
    }
    iVar2 = -1;
LAB_0042d41f:
    if (param_1 != iVar2) {
      if (*(undefined4 **)((int)this + 0x10) != (undefined4 *)0x0) {
        (**(code **)**(undefined4 **)((int)this + 0x10))(0);
      }
      FUN_0048d490(this_00,param_1);
      puVar1 = *(undefined4 **)(*this_00 + param_1 * 4);
      *(undefined4 **)((int)this + 0x10) = puVar1;
      (**(code **)*puVar1)(1);
      FUN_0042cf10(this,param_1);
    }
  }
  return;
}

/* Constructor audit: menu child 2 is FUN_0042F670; child 5 is FUN_00432BB0.
 * The former is the traced level selector. The latter creates two button
 * widgets and requests menu bitmap ID 54; its exact screen meaning is open.
 */
