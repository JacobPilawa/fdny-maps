
/* Algorithm:
   - Set the pending-level flag at this+0x68C.
   - Replace the reference-counted map-name string at this+0x660 from the first reference argument.
   - Replace the resource-definition path at this+0x668 from the second reference argument.
   - If an active level record is present at this+0x688, copy its label and timing metadata into
   adjacent pending fields and clear the spawned-level fallback flag at +0x674. If absent, seed the
   label as <Spawned Level>, use a 1200.0f duration, and set +0x674.
   
   Parameters:
   - this: EverGame state object.
   - param_1: reference to the requested map-name string stored at +0x660.
   - param_2: reference to a resource-definition path stored at +0x668; the Player finalexitloadmap
   caller passes data\\resdefs\\rescue.rrf.
   
   Returns:
   - No value.
   
   Evidence:
   - ProcessEverGameUpdate consumes +0x68C and passes the pending block at +0x660 to
   ProcessLevelLoadRequest.
   - HandlePlayerFinalExitLoadMap checks a BSP path built from the authored command key, then passes
   the original key reference through this setter. Raw stack offsets confirm the checked and stored
   keys match; runtime transition success remains unverified. */

void __thiscall SetPendingLevelRequest(void *this,undefined4 *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  *(undefined1 *)((int)this + 0x68c) = 1;
  piVar2 = *(int **)((int)this + 0x660);
  puVar1 = (undefined4 *)((int)this + 0x660);
  if (piVar2 != (int *)*param_1) {
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    piVar2 = (int *)*param_1;
    *puVar1 = piVar2;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + 1;
    }
  }
  piVar2 = *(int **)((int)this + 0x668);
  puVar1 = (undefined4 *)((int)this + 0x668);
  if (piVar2 != (int *)*param_2) {
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    piVar2 = (int *)*param_2;
    *puVar1 = piVar2;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + 1;
    }
  }
  iVar3 = *(int *)((int)this + 0x688);
  if (iVar3 == 0) {
    FUN_00401d80((void *)((int)this + 0x664),"<Spawned Level>");
    *(undefined4 *)((int)this + 0x670) = 0x44960000;
    *(undefined4 *)((int)this + 0x678) = 0;
    *(undefined1 *)((int)this + 0x674) = 1;
    return;
  }
  piVar2 = *(int **)((int)this + 0x66c);
  puVar1 = (undefined4 *)((int)this + 0x66c);
  if (piVar2 != *(int **)(iVar3 + 0xc)) {
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    piVar2 = *(int **)(iVar3 + 0xc);
    *puVar1 = piVar2;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + 1;
    }
  }
  iVar3 = *(int *)((int)this + 0x688);
  *(undefined4 *)((int)this + 0x670) = *(undefined4 *)(iVar3 + 0x10);
  *(undefined4 *)((int)this + 0x678) = *(undefined4 *)(iVar3 + 0x18);
  piVar2 = *(int **)((int)this + 0x664);
  puVar1 = (undefined4 *)((int)this + 0x664);
  if (piVar2 != *(int **)(iVar3 + 4)) {
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    piVar2 = *(int **)(iVar3 + 4);
    *puVar1 = piVar2;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + 1;
    }
  }
  *(undefined1 *)((int)this + 0x674) = 0;
  iVar3 = *(int *)(*(int *)((int)this + 0x688) + 0x1c);
  if (0 < iVar3) {
    *(int *)((int)this + 0x67c) = iVar3;
    return;
  }
  *(undefined4 *)((int)this + 0x67c) = *(undefined4 *)(*(int *)((int)this + 0x688) + 0x18);
  return;
}


