/* Fresh Ghidra pseudocode export, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Entry: 004A0D60
 * Function: DrainQueuedSoundRequests
 * Analysis output, not recovered original source.
 */

/* Clear the manager busy flag, replay each deferred (instance, optional-parameter) pair through
   DispatchOrQueueSoundPlayback, then clear the deferred array. Fresh EXE pseudocode, 2026-10-13. */

void __fastcall DrainQueuedSoundRequests(void *param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int local_30;
  char *local_2c;
  char *local_28;
  undefined4 local_24;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;

  iVar2 = 0;
  *(undefined1 *)((int)param_1 + 0x38) = 0;
  if (0 < *(int *)((int)param_1 + 0x40)) {
    do {
      if ((iVar2 < 0) || (*(int *)((int)param_1 + 0x40) <= iVar2)) {
        piVar3 = &local_30;
        for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_30 = 1;
        local_2c = "Array index out of bounds";
        local_28 = "..\\BluelAPI/Container/SArray.h";
        local_24 = 0x1cd;
        FUN_00422060(&local_30);
      }
      if ((iVar2 < 0) || (*(int *)((int)param_1 + 0x40) <= iVar2)) {
        piVar3 = &local_18;
        for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_18 = 1;
        local_14 = "Array index out of bounds";
        local_10 = "..\\BluelAPI/Container/SArray.h";
        local_c = 0x1cd;
        FUN_00422060(&local_18);
      }
      DispatchOrQueueSoundPlayback
                (param_1,*(int **)(*(int *)(*(int *)((int)param_1 + 0x3c) + iVar2 * 4) + 4),
                 (undefined1 *)**(undefined4 **)(*(int *)((int)param_1 + 0x3c) + iVar2 * 4));
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)((int)param_1 + 0x40));
  }
  if (*(int *)((int)param_1 + 0x40) != 0) {
    FUN_0043b6e0((void *)((int)param_1 + 0x3c),0);
  }
  return;
}
