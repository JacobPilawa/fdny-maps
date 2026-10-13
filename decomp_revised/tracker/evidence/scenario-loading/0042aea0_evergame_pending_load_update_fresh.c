
/* Summary:
   EverGame update routine that consumes the pending-level flag and proceeds with the normal
   game-state update.
   
   Algorithm:
   - If byte this+0x68C is nonzero, clear it and call FUN_0042AB00(this, this+0x660).
   - FUN_0042AB00 logs the level load, constructs a World factory request, and drains the entity
   spawn queue; the established World factory path then loads the BSP through WorldEntity.
   - Continue other game-state updates.
   
   Parameters:
   - this: EverGame state object (layout inferred from shared fields and the TEverGame loading-level
   log).
   
   Returns:
   - No return value.
   
   Evidence:
   - Direct thunk at 0042BC60 adjusts ECX by +0xB4 then jumps here; data reference at 004DF9B8
   points to that thunk.
   - HandlePlayerFinalExitLoadMap (00450A70) sets byte this+0x68C through FUN_0042A260 after the
   requested map-key BSP path passes file-existence validation.
   - This establishes a static request-to-level-loader path. It does not establish successful
   runtime stage completion, exact frame cadence, or visual parity.
   - EXE SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __fastcall ProcessEverGameUpdate(char *param_1)

{
  char *this;
  
  if (param_1[0x68c] != '\0') {
    param_1[0x68c] = '\0';
    FUN_0042ab00(param_1,param_1 + 0x660);
  }
  FUN_0042c080((int *)&DAT_00502ab8);
  (**(code **)(*(int *)*DAT_00503a78 + 0x14))();
  if (param_1[0x64c] != '\0') {
    FUN_0042d8c0((int)(param_1 + 0x3c8));
    FUN_004a0660(*(int **)(param_1 + 0x3b0));
    return;
  }
  if (DAT_00502a78 != '\0') {
    FUN_0042a540(param_1);
  }
  FUN_00429520((int)param_1);
  FUN_00494670((void *)DAT_00503a78[1],(undefined *)0x0);
  if (*(int *)(param_1 + 0x6f0) == 3) {
    if (*(int *)(param_1 + 0x688) != 0) {
      FUN_0042a730(param_1,(undefined4 *)0x2,(int *)(*(int *)(param_1 + 0x688) + 0xc));
    }
    param_1[0x6f0] = '\x01';
    param_1[0x6f1] = '\0';
    param_1[0x6f2] = '\0';
    param_1[0x6f3] = '\0';
  }
  if (*(int *)(param_1 + 0x6ec) != 0) {
    if (*(int *)(param_1 + 0x6ec) == 1) {
      FUN_00424650(*(void **)(param_1 + 0x6e4),*(int *)(*(int *)(param_1 + 0x688) + 0x18));
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
  return;
}


