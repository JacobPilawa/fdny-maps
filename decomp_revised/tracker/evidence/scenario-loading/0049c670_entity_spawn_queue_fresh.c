/* Fresh Ghidra MCP capture from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-13. */


/* Summary: Drain queued map-entity factory requests and run ordered initialization passes.
   
   Algorithm:
   - Remove each staged request, call the stored entity-class factory through descriptor vtable slot
   +0x08, register the returned object with the world entity manager, and collect the object pointer
   for batch initialization.
   - After the queue is empty, call root entity vtable slot +0x20 once for every collected entity.
   - Then call root entity vtable slot +0x24 once for every collected entity.
   - Release the temporary entity-pointer array.
   
   Parameters:
   - ECX points to the queue-processing context. The decompiler models stack parameters
   incompletely; the function ends with RET 8, so do not treat the current prototype as complete.
   
   Returns: No stable return value is established; the return value is not used by the mapped caller
   path.
   
   Evidence:
   - This is the world spawn-queue drain documented in
   tracker/evidence/scenario-loading/world_factory_resolution.md.
   - RCS_MiscEmitter root vtable slot +0x20 points to InitializeMiscEmitterEntity (004424D0), which
   registers the parsed A_Name and binds S_Script01-05.
   - Fresh code ends at 0049C800 with RET 8. */

void __fastcall ProcessEntitySpawnQueue(int param_1)

{
  undefined4 *_Memory;
  void *_Memory_00;
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int local_3c;
  int local_38;
  undefined4 local_34;
  undefined4 local_30;
  code *local_2c;
  undefined4 local_28;
  int local_24;
  char *pcStack_20;
  char *pcStack_1c;
  undefined4 uStack_18;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d4698;
  pvStack_c = ExceptionList;
  local_28 = 0x10;
  local_2c = FUN_0043f660;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_4 = 0;
  ExceptionList = &pvStack_c;
  while (iVar1 = local_38, _Memory = *(undefined4 **)(param_1 + -0x30), _Memory != (undefined4 *)0x0
        ) {
    iVar4 = _Memory[1];
    _Memory_00 = (void *)*_Memory;
    *(int *)(param_1 + -0x30) = iVar4;
    if (iVar4 == 0) {
      *(undefined4 *)(param_1 + -0x2c) = 0;
    }
    FUN_00429d20((void *)(param_1 + -0x40),(int)_Memory);
    _free(_Memory);
    *(int *)(param_1 + -0x28) = *(int *)(param_1 + -0x28) + -1;
    if (_Memory_00 == (void *)0x0) break;
    piVar2 = (int *)(**(code **)(**(int **)((int)_Memory_00 + 0xc) + 8))(_Memory_00);
    FUN_00429490(DAT_00502aac,piVar2);
    FUN_0043b6e0(&local_3c,iVar1 + 1);
    *(int **)(local_3c + iVar1 * 4) = piVar2;
    FUN_0043f660();
    _free(_Memory_00);
  }
  iVar4 = 0;
  if (0 < iVar1) {
    do {
      if ((iVar4 < 0) || (iVar1 <= iVar4)) {
        piVar2 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        local_24 = 1;
        pcStack_20 = "Array index out of bounds";
        pcStack_1c = "..\\BluelAPI/Container/SArray.h";
        uStack_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      (**(code **)(**(int **)(local_3c + iVar4 * 4) + 0x20))();
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar1);
  }
  iVar4 = 0;
  if (0 < iVar1) {
    do {
      if ((iVar4 < 0) || (iVar1 <= iVar4)) {
        piVar2 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        local_24 = 1;
        pcStack_20 = "Array index out of bounds";
        pcStack_1c = "..\\BluelAPI/Container/SArray.h";
        uStack_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      (**(code **)(**(int **)(local_3c + iVar4 * 4) + 0x24))();
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar1);
  }
  local_4 = 0xffffffff;
  if (iVar1 != 0) {
    FUN_0043b6e0(&local_3c,0);
  }
  ExceptionList = pvStack_c;
  return;
}


