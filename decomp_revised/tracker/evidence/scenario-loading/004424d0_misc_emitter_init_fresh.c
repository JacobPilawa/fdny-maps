/* Fresh Ghidra MCP capture from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-13. */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: First initialization-pass method for an RCS_MiscEmitter map entity.
   
   Algorithm:
   - The WorldEntity spawn queue invokes root vtable slot +0x20 for every constructed map entity in
   its first initialization pass.
   - If the contained emitter at owner +0x78 exists, initialize and register the embedded
   ScriptActor at owner +0x24 using the parsed A_Name; then initialize the contained emitter
   interface through slot +0x2C.
   - Set owner +0x7C to 1.0 and refresh the entity transform through 00441FC0.
   - Dispatch non-null S_Script01 through S_Script05 bindings from the parsed entity properties
   through ScriptActor dispatch 00470150 with contexts 0 through 4.
   
   Parameters:
   - this: root RCS_MiscEmitter entity.
   
   Returns: No return value.
   
   Evidence:
   - RCS_MiscEmitter primary vtable 004E1158 slot +0x20 points here.
   - World entity initialization pass in 0049C670 calls root slot +0x20 before the second +0x24
   pass.
   - Oil Rig C entities 346-348 have names steam01/02/03 and initial S_Script01=turnoff; their
   A_Name registry insertion is now statically proven. */

void __thiscall InitializeMiscEmitterEntity(void *this)

{
  void *this_00;
  char *pcVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  int *extraout_ECX;
  int *piVar4;
  int *piVar5;
  int iVar6;
  int *piStack_18;
  undefined1 *apuStack_14 [2];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004cf138;
  pvStack_c = ExceptionList;
  if (*(int *)((int)this + 0x78) != 0) {
    ExceptionList = &pvStack_c;
    puVar3 = (undefined4 *)FUN_0049d350(this,apuStack_14);
    piStack_18 = (int *)&stack0xffffffd0;
    piVar4 = extraout_ECX;
    FUN_00402790(&stack0xffffffd0,*(char **)(*(int *)((int)this + 0x84) + 4));
    this_00 = (void *)((int)this + 0x24);
    InitializeAndRegisterScriptActor(this_00,piVar4,puVar3);
    (**(code **)(**(int **)((int)this + 0x78) + 0x2c))();
    *(undefined4 *)((int)this + 0x7c) = 0x3f800000;
    FUN_00441fc0((int)this);
    pcVar1 = *(char **)(*(int *)((int)this + 0x84) + 0x14);
    if (pcVar1 != (char *)0x0) {
      FUN_00402790(&piStack_18,pcVar1);
      piVar4 = piStack_18;
      uStack_4 = 0;
      if (piStack_18[1] != 0) {
        iVar6 = 0;
        apuStack_14[0] = &stack0xffffffd0;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar5 = &DAT_00501908;
        puVar2 = &stack0xffffffd0;
        if (piStack_18 != &DAT_00501908) {
          piVar5 = piStack_18;
          *piStack_18 = *piStack_18 + 1;
          puVar2 = apuStack_14[0];
        }
        apuStack_14[0] = puVar2;
        FUN_00470150(this_00,piVar5,iVar6);
      }
      uStack_4 = 0xffffffff;
      if ((piVar4 != &DAT_00501908) && (iVar6 = *piVar4, *piVar4 = iVar6 + -1, iVar6 + -1 < 1)) {
        FUN_004011c0(&piStack_18);
      }
    }
    pcVar1 = *(char **)(*(int *)((int)this + 0x84) + 0x18);
    if (pcVar1 != (char *)0x0) {
      FUN_00402790(&piStack_18,pcVar1);
      piVar4 = piStack_18;
      uStack_4 = 1;
      if (piStack_18[1] != 0) {
        iVar6 = 0;
        apuStack_14[0] = &stack0xffffffd0;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar5 = &DAT_00501908;
        puVar2 = &stack0xffffffd0;
        if (piStack_18 != &DAT_00501908) {
          piVar5 = piStack_18;
          *piStack_18 = *piStack_18 + 1;
          puVar2 = apuStack_14[0];
        }
        apuStack_14[0] = puVar2;
        FUN_00470150(this_00,piVar5,iVar6);
      }
      uStack_4 = 0xffffffff;
      if ((piVar4 != &DAT_00501908) && (iVar6 = *piVar4, *piVar4 = iVar6 + -1, iVar6 + -1 < 1)) {
        FUN_004011c0(&piStack_18);
      }
    }
    pcVar1 = *(char **)(*(int *)((int)this + 0x84) + 0x1c);
    if (pcVar1 != (char *)0x0) {
      FUN_00402790(&piStack_18,pcVar1);
      piVar4 = piStack_18;
      uStack_4 = 2;
      if (piStack_18[1] != 0) {
        iVar6 = 0;
        apuStack_14[0] = &stack0xffffffd0;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar5 = &DAT_00501908;
        puVar2 = &stack0xffffffd0;
        if (piStack_18 != &DAT_00501908) {
          piVar5 = piStack_18;
          *piStack_18 = *piStack_18 + 1;
          puVar2 = apuStack_14[0];
        }
        apuStack_14[0] = puVar2;
        FUN_00470150(this_00,piVar5,iVar6);
      }
      uStack_4 = 0xffffffff;
      if ((piVar4 != &DAT_00501908) && (iVar6 = *piVar4, *piVar4 = iVar6 + -1, iVar6 + -1 < 1)) {
        FUN_004011c0(&piStack_18);
      }
    }
    pcVar1 = *(char **)(*(int *)((int)this + 0x84) + 0x20);
    if (pcVar1 != (char *)0x0) {
      FUN_00402790(&piStack_18,pcVar1);
      piVar4 = piStack_18;
      uStack_4 = 3;
      if (piStack_18[1] != 0) {
        iVar6 = 0;
        apuStack_14[0] = &stack0xffffffd0;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar5 = &DAT_00501908;
        puVar2 = &stack0xffffffd0;
        if (piStack_18 != &DAT_00501908) {
          piVar5 = piStack_18;
          *piStack_18 = *piStack_18 + 1;
          puVar2 = apuStack_14[0];
        }
        apuStack_14[0] = puVar2;
        FUN_00470150(this_00,piVar5,iVar6);
      }
      uStack_4 = 0xffffffff;
      if ((piVar4 != &DAT_00501908) && (iVar6 = *piVar4, *piVar4 = iVar6 + -1, iVar6 + -1 < 1)) {
        FUN_004011c0(&piStack_18);
      }
    }
    pcVar1 = *(char **)(*(int *)((int)this + 0x84) + 0x24);
    if (pcVar1 != (char *)0x0) {
      FUN_00402790(&piStack_18,pcVar1);
      piVar4 = piStack_18;
      uStack_4 = 4;
      if (piStack_18[1] != 0) {
        iVar6 = 0;
        apuStack_14[0] = &stack0xffffffd0;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar5 = &DAT_00501908;
        puVar2 = &stack0xffffffd0;
        if (piStack_18 != &DAT_00501908) {
          piVar5 = piStack_18;
          *piStack_18 = *piStack_18 + 1;
          puVar2 = apuStack_14[0];
        }
        apuStack_14[0] = puVar2;
        FUN_00470150(this_00,piVar5,iVar6);
      }
      uStack_4 = 0xffffffff;
      if ((piVar4 != &DAT_00501908) && (iVar6 = *piVar4, *piVar4 = iVar6 + -1, iVar6 + -1 < 1)) {
        FUN_004011c0(&piStack_18);
      }
    }
  }
  ExceptionList = pvStack_c;
  return;
}


