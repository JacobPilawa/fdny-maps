/* Fresh Ghidra pseudocode captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0049BD20 (DispatchEntityContactLists)
 * Analysis output, not recovered original source.
 */
/* Summary:
   Dispatches the shared owner's registered entities across nine type-specific contact lists.
   
   Algorithm:
   - Walks the manager's active linked list and removes entities whose virtual slot +0x04 reports
   inactive.
   - Walks active entities in list order.
   - For each entity, tests the nine category bits at root +0x20.
   - For each set bit, walks that category's contact list and calls the active entity's root vtable
   slot +0x48 on every listed candidate.
   
   Parameters:
   - ECX/this: shared entity contact manager at owner +0xE0.
   
   Returns:
   - No value; invokes class-specific contact callbacks for matching category pairs.
   
   Evidence:
   - Fresh Ghidra decompilation at 0x0049BD20.
   - Manager vtable word at 0x004E4124 points to this function.
   - Owner update path 0x0042AEA0 -> 0x00429520 reaches this manager pass.
   - BoundsTrigger and BModel root vtables place their candidate checks at slot +0x48.
   
   Limits:
   - This manager pass establishes callback traversal and list order for one invocation, not
   rendered-frame cadence or the order of the separate embedded ScriptActor updater. */

void __thiscall DispatchEntityContactLists(void *this)

{
  void *_Memory;
  undefined4 uVar1;
  int *piVar2;
  char cVar3;
  undefined4 *puVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int iStack_4;
  
  *(undefined4 *)((int)this + 8) = *(undefined4 *)((int)this + 0x14);
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined1 *)((int)this + 0x10) = 0;
  *(undefined1 *)((int)this + 0x11) = 1;
  while (puVar4 = *(undefined4 **)((int)this + 8), puVar4 != (undefined4 *)0x0) {
    if (*(char *)((int)this + 0x11) == '\0') {
      if (*(char *)((int)this + 0x10) == '\0') {
        *(undefined4 **)((int)this + 0xc) = puVar4;
        puVar4 = *(undefined4 **)(*(int *)((int)this + 8) + 4);
        *(undefined4 **)((int)this + 8) = puVar4;
        if (puVar4 == (undefined4 *)0x0) break;
        piVar6 = (int *)*puVar4;
      }
      else {
        *(undefined1 *)((int)this + 0x10) = 0;
        piVar6 = (int *)**(undefined4 **)((int)this + 8);
      }
    }
    else {
      *(undefined1 *)((int)this + 0x11) = 0;
      piVar6 = (int *)*puVar4;
    }
    if (piVar6 == (int *)0x0) break;
    cVar3 = (**(code **)(*piVar6 + 4))();
    if (cVar3 != '\0') {
      _Memory = *(void **)((int)this + 8);
      uVar1 = *(undefined4 *)((int)_Memory + 4);
      *(undefined4 *)((int)this + 8) = uVar1;
      if (*(int *)((int)this + 0xc) != 0) {
        *(undefined4 *)(*(int *)((int)this + 0xc) + 4) = uVar1;
      }
      if (*(void **)((int)this + 0x14) == _Memory) {
        *(undefined4 *)((int)this + 0x14) = uVar1;
      }
      if (*(void **)((int)this + 0x18) == _Memory) {
        *(undefined4 *)((int)this + 0x18) = *(undefined4 *)((int)this + 0xc);
      }
      if ((*(int *)((int)this + 0x18) == 0) && (*(int *)((int)this + 0x14) != 0)) {
        *(int *)((int)this + 0x18) = *(int *)((int)this + 0x14);
      }
      FUN_00429d20((void *)((int)this + 4),(int)_Memory);
      _free(_Memory);
      *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) + -1;
      *(undefined1 *)((int)this + 0x10) = 1;
      (*(code *)**(undefined4 **)((int)this + piVar6[6] * 0x48 + 0x4c))(piVar6);
    }
  }
  *(undefined4 *)((int)this + 8) = *(undefined4 *)((int)this + 0x14);
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined1 *)((int)this + 0x10) = 0;
  *(undefined1 *)((int)this + 0x11) = 1;
  do {
    puVar4 = *(undefined4 **)((int)this + 8);
    if (puVar4 == (undefined4 *)0x0) {
      return;
    }
    if (*(char *)((int)this + 0x11) == '\0') {
      if (*(char *)((int)this + 0x10) == '\0') {
        *(undefined4 **)((int)this + 0xc) = puVar4;
        puVar4 = *(undefined4 **)(*(int *)((int)this + 8) + 4);
        *(undefined4 **)((int)this + 8) = puVar4;
        if (puVar4 == (undefined4 *)0x0) {
          return;
        }
      }
      else {
        puVar4 = *(undefined4 **)((int)this + 8);
        *(undefined1 *)((int)this + 0x10) = 0;
      }
      piVar6 = (int *)*puVar4;
    }
    else {
      *(undefined1 *)((int)this + 0x11) = 0;
      piVar6 = (int *)*puVar4;
    }
    if (piVar6 == (int *)0x0) {
      return;
    }
    iStack_4 = 0;
    piVar7 = (int *)((int)this + 0x54);
    do {
      if ((piVar6[8] & 1 << ((byte)iStack_4 & 0x1f)) != 0) {
        piVar7[-1] = piVar7[2];
        *piVar7 = 0;
        *(undefined1 *)(piVar7 + 1) = 0;
        *(undefined1 *)((int)piVar7 + 5) = 1;
        while (piVar2 = (int *)piVar7[-1], piVar2 != (int *)0x0) {
          if (*(char *)((int)piVar7 + 5) == '\0') {
            if ((char)piVar7[1] == '\0') {
              *piVar7 = (int)piVar2;
              piVar2 = *(int **)(piVar7[-1] + 4);
              piVar7[-1] = (int)piVar2;
              if (piVar2 == (int *)0x0) break;
              iVar5 = *piVar2;
            }
            else {
              *(undefined1 *)(piVar7 + 1) = 0;
              iVar5 = *(int *)piVar7[-1];
            }
          }
          else {
            *(undefined1 *)((int)piVar7 + 5) = 0;
            iVar5 = *piVar2;
          }
          if (iVar5 == 0) break;
          (**(code **)(*piVar6 + 0x48))(iVar5);
        }
      }
      iStack_4 = iStack_4 + 1;
      piVar7 = piVar7 + 0x12;
    } while (iStack_4 < 9);
  } while( true );
}


