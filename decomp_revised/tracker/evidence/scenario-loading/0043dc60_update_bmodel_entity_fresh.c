/* Fresh Ghidra pseudocode export
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: UpdateBModelEntity at 0043DC60
 * Recovered body: 0043DC60-0043E297
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   - Virtual per-frame method in the BModel ScriptActor vtable at 004E0DF0 slot 0;
   BModelEntityConstructor (0043F340) installs that table on the embedded ScriptActor at owner
   +0x24.
   - Returns early when the BModel has no model children. Updates owner visibility/bounds and
   environment state through the linked helpers.
   - For command states 3 (opening) and 4 (closing), advances the current model frame at ScriptActor
   +0x1B4 by the global frame delta (00502B04) times speed (+0x1B8), then clamps at the authored
   endpoints (+0x1C4/+0x1C8). State 5 loops across that range.
   - Samples each child model's Genesis motion path and writes the resulting model transform.
   - Examines active timed-command records at +0x124 (count +0x128). It compares each key frame
   (+0x4) with the current and prior frame values, sends the selected event command/context through
   00475310 -> 00470150, and clears the record's active byte after dispatch.
   
   Parameters:
   - ECX: embedded ScriptActor subobject; the containing BModel owner is at this - 0x24.
   
   Returns:
   - void.
   
   Evidence:
   - Raw body: 0043DC60-0043E297; next function starts 0043E2A0.
   - Raw vtable bytes at 004E0DF0 begin with pointer 0043DC60, followed by 0043E680.
   BModelEntityConstructor references/installs 004E0DF0.
   - Ghidra pseudocode and raw x86 are archived in
   tracker/evidence/scenario-loading/update_bmodel_entity.*.
   
   Limits:
   - Field names beyond the command state, frame, speed, and endpoints are provisional offsets. No
   runtime trace has yet established callback cadence or visual timing. */

void __thiscall UpdateBModelEntity(void *this)

{
  char *pcVar1;
  float fVar2;
  float fVar3;
  char cVar4;
  void *pvVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  float *pfVar9;
  int *piVar10;
  float10 fVar11;
  int *piVar12;
  int **ppiVar13;
  int *piVar14;
  int *piStack_d8;
  float fStack_d4;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  int iStack_c8;
  char *pcStack_c4;
  char *pcStack_c0;
  undefined4 uStack_bc;
  float fStack_b0;
  char *pcStack_ac;
  char *pcStack_a8;
  undefined4 uStack_a4;
  int iStack_98;
  char *pcStack_94;
  char *pcStack_90;
  undefined4 uStack_8c;
  undefined1 auStack_80 [48];
  undefined1 auStack_50 [68];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004ceadb;
  pvStack_c = ExceptionList;
  if (*(int *)((int)this + 0x1a0) < 1) {
    return;
  }
  ExceptionList = &pvStack_c;
  *(undefined4 *)((int)this + 0x70) = 0;
  piVar10 = (int *)((int)this + -0x24);
  FUN_0043c1a0(piVar10,(float)_DAT_004dfa28 <
                       ABS(*(float *)((int)this + 0x1b4) - *(float *)((int)this + 0x1c4)));
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_d8 = &DAT_00501908;
  FUN_00401d80(&piStack_d8,"DBG_CLOSE_ALL_AREAS");
  ppiVar13 = &piStack_d8;
  uStack_4 = 0;
  pvVar5 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar6 = FUN_00438550(pvVar5,(int *)ppiVar13);
  uStack_4 = 0xffffffff;
  if ((piStack_d8 != &DAT_00501908) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 < 1)) {
    FUN_004011c0(&piStack_d8);
  }
  if ((char)uVar6 != '\0') {
    FUN_0043c1a0(piVar10,0);
  }
  piVar12 = (int *)((int)this + 0x13c);
  FUN_00470840((void *)(-(uint)(this != (void *)0x24) & (uint)this),piVar12);
  iVar7 = FUN_00490300(piVar12,0);
  if (iVar7 == 0) {
    iVar7 = FUN_004903a0(piVar12,0);
    if (iVar7 != 0) {
      *(undefined1 *)((int)this + 0x218) = 0;
    }
  }
  else {
    *(undefined1 *)((int)this + 0x218) = 1;
  }
  if (*(int *)((int)this + 0x234) != 0) {
    if (*(char *)((int)this + 0x218) == '\0') {
      FUN_00459090((int)this + 0x7c);
    }
    else {
      FUN_00459050((int)this + 0x7c);
    }
    FUN_0043d330(piVar10);
  }
  if ((((*(int *)(*(int *)((int)this + 200) + 0x34) != 0) &&
       (*(int *)(*(int *)((int)this + 200) + 0x38) == 0)) &&
      (((float)_DAT_004e0b68 <= *(float *)((int)this + 0x1c8) ||
       (*(float *)((int)this + 0x1c8) <= (float)_DAT_004e0b60)))) &&
     (*(char *)((int)this + 0x218) != '\0')) {
    piVar14 = &iStack_c8;
    pvVar5 = (void *)(**(code **)(*(int *)DAT_00503a78[0xc] + 0x18))();
    FUN_0049e770(pvVar5,piVar14);
    iVar7 = (**(code **)(*(int *)DAT_00503a78[0xc] + 0x18))();
    fStack_d4 = *(float *)(iVar7 + 0x28);
    uStack_d0 = *(undefined4 *)(iVar7 + 0x2c);
    uStack_cc = *(undefined4 *)(iVar7 + 0x30);
    iVar7 = (**(code **)(*piVar10 + 0x18))();
    FUN_004977b0(auStack_50,iVar7);
    pfVar9 = &fStack_d4;
    iVar7 = (**(code **)(*piVar10 + 0x18))();
    FUN_0043b2d0(&fStack_b0,(float *)(iVar7 + 0x28),pfVar9);
    fVar11 = FUN_00497aa0();
    if (fVar11 < (float10)_DAT_004df224) {
      cVar4 = '\x01';
      iVar7 = (**(code **)(*piVar10 + 0x18))();
      cVar4 = FUN_00490130(&fStack_d4,(undefined4 *)(iVar7 + 0x28),cVar4);
      if (cVar4 != '\0') {
        *(undefined1 *)(DAT_00503a78[0xc] + 0x5d4) = 1;
      }
    }
  }
  FUN_00490430(piVar12);
  FUN_0043aed0((int)piVar12);
  iVar7 = *(int *)((int)this + 0x4c);
  if (iVar7 == 0) {
    ExceptionList = pvStack_c;
    return;
  }
  piStack_d8 = *(int **)((int)this + 0x1b4);
  if (iVar7 == 5) {
    fVar2 = DAT_00502b04 * *(float *)((int)this + 0x1b8) + *(float *)((int)this + 0x1b4);
    *(float *)((int)this + 0x1b4) = fVar2;
    if (*(float *)((int)this + 0x1c8) <= fVar2) {
      do {
        *(float *)((int)this + 0x1b4) =
             *(float *)((int)this + 0x1b4) -
             (*(float *)((int)this + 0x1c8) - *(float *)((int)this + 0x1c4));
      } while (*(float *)((int)this + 0x1c8) <= *(float *)((int)this + 0x1b4));
    }
    if (*(float *)((int)this + 0x1b4) < *(float *)((int)this + 0x1c4) !=
        (*(float *)((int)this + 0x1b4) == *(float *)((int)this + 0x1c4))) {
      do {
        *(float *)((int)this + 0x1b4) =
             (*(float *)((int)this + 0x1c8) - *(float *)((int)this + 0x1c4)) +
             *(float *)((int)this + 0x1b4);
      } while (*(float *)((int)this + 0x1b4) < *(float *)((int)this + 0x1c4) !=
               (*(float *)((int)this + 0x1b4) == *(float *)((int)this + 0x1c4)));
    }
    goto LAB_0043e05c;
  }
  if (iVar7 == 3) {
    fVar2 = DAT_00502b04 * *(float *)((int)this + 0x1b8) + *(float *)((int)this + 0x1b4);
    *(float *)((int)this + 0x1b4) = fVar2;
    if (*(float *)((int)this + 0x1c8) <= fVar2) goto LAB_0043e04d;
    if (fVar2 < *(float *)((int)this + 0x1c4) == (fVar2 == *(float *)((int)this + 0x1c4)))
    goto LAB_0043e05c;
    *(undefined4 *)((int)this + 0x1b4) = *(undefined4 *)((int)this + 0x1c4);
  }
  else {
    if (iVar7 != 4) goto LAB_0043e05c;
    fVar2 = *(float *)((int)this + 0x1b4) - DAT_00502b04 * *(float *)((int)this + 0x1b8);
    *(float *)((int)this + 0x1b4) = fVar2;
    if (fVar2 < *(float *)((int)this + 0x1c4) != (fVar2 == *(float *)((int)this + 0x1c4))) {
      *(undefined4 *)((int)this + 0x1b4) = *(undefined4 *)((int)this + 0x1c4);
      *(undefined4 *)((int)this + 0x4c) = 0;
    }
    if (*(float *)((int)this + 0x1b4) < *(float *)((int)this + 0x1c8)) goto LAB_0043e05c;
LAB_0043e04d:
    *(undefined4 *)((int)this + 0x1b4) = *(undefined4 *)((int)this + 0x1c8);
  }
  *(undefined4 *)((int)this + 0x4c) = 0;
LAB_0043e05c:
  iVar7 = 0;
  if (0 < *(int *)((int)this + 0x1a0)) {
    do {
      if ((iVar7 < 0) || (*(int *)((int)this + 0x1a0) <= iVar7)) {
        pfVar9 = &fStack_b0;
        for (iVar8 = 6; iVar8 != 0; iVar8 = iVar8 + -1) {
          *pfVar9 = 0.0;
          pfVar9 = pfVar9 + 1;
        }
        fStack_b0 = 1.4013e-45;
        pcStack_ac = "Array index out of bounds";
        pcStack_a8 = "..\\BluelAPI/Container/SArray.h";
        uStack_a4 = 0x1cd;
        FUN_00422060((int *)&fStack_b0);
      }
      if ((iVar7 < 0) || (*(int *)((int)this + 0x1a0) <= iVar7)) {
        piVar10 = &iStack_c8;
        for (iVar8 = 6; iVar8 != 0; iVar8 = iVar8 + -1) {
          *piVar10 = 0;
          piVar10 = piVar10 + 1;
        }
        iStack_c8 = 1;
        pcStack_c4 = "Array index out of bounds";
        pcStack_c0 = "..\\BluelAPI/Container/SArray.h";
        uStack_bc = 0x1cd;
        FUN_00422060(&iStack_c8);
      }
      iVar8 = geWorld_ModelGetMotion();
      if (iVar8 != 0) {
        uVar6 = geMotion_GetPath(iVar8,0);
        gePath_Sample(uVar6,auStack_80);
        iVar8 = DAT_00503a78[1];
        if (*(char *)(iVar8 + 0x160) == '\0') {
          iVar8 = iVar8 + 8;
        }
        else {
          iVar8 = iVar8 + 0xb4;
        }
        geWorld_SetModelXForm(*(undefined4 *)(iVar8 + 0x78));
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)((int)this + 0x1a0));
  }
  if (*(int *)(*(int *)((int)this + 200) + 0x38) != 0) {
    FUN_0043b7a0((int)this + -0x24);
  }
  iVar7 = 0;
  if (0 < *(int *)((int)this + 0x128)) {
    do {
      if ((iVar7 < 0) || (*(int *)((int)this + 0x128) <= iVar7)) {
        piVar10 = &iStack_98;
        for (iVar8 = 6; iVar8 != 0; iVar8 = iVar8 + -1) {
          *piVar10 = 0;
          piVar10 = piVar10 + 1;
        }
        iStack_98 = 1;
        pcStack_94 = "Array index out of bounds";
        pcStack_90 = "..\\BluelAPI/Container/SArray.h";
        uStack_8c = 0x1cd;
        FUN_00422060(&iStack_98);
      }
      pcVar1 = *(char **)(*(int *)((int)this + 0x124) + iVar7 * 4);
      if ((*pcVar1 != '\0') &&
         (fVar2 = ABS(*(float *)((int)this + 0x1b4) - *(float *)(pcVar1 + 4)),
         fVar3 = ABS(*(float *)((int)this + 0x1b4) - (float)piStack_d8),
         fVar3 < fVar2 == (fVar3 == fVar2))) {
        pvVar5 = *(void **)(pcVar1 + 0xc);
        piStack_d8 = (int *)&stack0xffffff10;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar10 = *(int **)(pcVar1 + 8);
        piVar12 = &DAT_00501908;
        if (piVar10 != &DAT_00501908) {
          *piVar10 = *piVar10 + 1;
          piVar12 = piVar10;
        }
        DispatchScriptActorCommandWithContext(this,piVar12,pvVar5);
        *pcVar1 = '\0';
        ExceptionList = pvStack_c;
        return;
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)((int)this + 0x128));
  }
  ExceptionList = pvStack_c;
  return;
}


