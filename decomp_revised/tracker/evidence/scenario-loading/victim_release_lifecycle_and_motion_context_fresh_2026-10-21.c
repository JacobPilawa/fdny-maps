/* Fresh read-only Ghidra 12.0.4 decompilation of VictimRescue state, release,
   actor-animation, sound-stop, and render-flag helpers.
   Input: FDNYFirefighter.exe
   SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
   Saved project: .local/mission-lifecycle-ghidra/FDNYPort
   Opened with -readOnly -noanalysis; exported by DecompileTargetAndRefs.java.
   These are Ghidra outputs, not recovered original source. Runtime cadence and
   audiovisual parity remain unverified.
*/

/* FUNCTION 0x0045bea0 */

void __fastcall FUN_0045bea0(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x4c);
  if (iVar1 != 5) {
    if (iVar1 == 6) {
      FUN_004599b0(param_1 + -0xd4);
    }
    else if (iVar1 == 7) {
      UpdateVictimRoamMovement((int *)(param_1 + -0xd4));
    }
    if ((*(int *)(param_1 + 0x4c) != 6) && (*(int *)(param_1 + 0x80) != 0)) {
      FUN_0048d8f0(*(int *)(param_1 + 0x80));
    }
    FUN_00480d50(param_1 + 0x9c);
    return;
  }
  return;
}

/* FUNCTION 0x004599b0 */

void __fastcall FUN_004599b0(int param_1)

{
  int iVar1;
  
  if (*(uint *)(param_1 + 0x1f0) != 0xffffffff) {
    if ((*(uint *)(param_1 + 500) < DAT_00502afc) && (*(uint *)(param_1 + 0x1f0) < DAT_00502afc)) {
      if (*(uint *)(param_1 + 500) != 0) {
        if (*(void **)(param_1 + 0x154) != (void *)0x0) {
          FUN_0048d8e0(*(void **)(param_1 + 0x154));
        }
        *(undefined4 *)(param_1 + 500) = 0;
      }
      iVar1 = *(int *)(param_1 + 0x1f8) + -5;
      *(int *)(param_1 + 0x1f8) = iVar1;
      if (iVar1 == 0) {
        *(undefined1 *)(param_1 + 0x1fc) = 0;
        *(undefined4 *)(param_1 + 0x1f0) = 0xffffffff;
      }
      else {
        *(uint *)(param_1 + 0x1f0) = iVar1 + DAT_00502afc;
        *(bool *)(param_1 + 0x1fc) = *(char *)(param_1 + 0x1fc) == '\0';
      }
      FUN_00481140((void *)(param_1 + 0x170),*(char *)(param_1 + 0x1fc));
    }
  }
  return;
}

/* FUNCTION 0x0045c000 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0045c000(void *this,int *param_1)

{
  void *pvVar1;
  undefined4 *puVar2;
  float *pfVar3;
  int iVar4;
  float fVar5;
  float fVar6;
  int *piVar7;
  float *pfVar8;
  float local_30 [3];
  float local_24 [3];
  float afStack_18 [4];
  undefined1 *puStack_8;
  
  pfVar3 = local_30;
  pfVar8 = local_24;
  *(undefined4 *)((int)this + 0x120) = 6;
  pvVar1 = (void *)(**(code **)(*param_1 + 0x18))();
  pvVar1 = (void *)FUN_00446ee0(pvVar1,pfVar8);
  puVar2 = (undefined4 *)FUN_0042d2b0(pvVar1,pfVar3);
  *(undefined4 *)((int)this + 0x28) = *puVar2;
  *(undefined4 *)((int)this + 0x2c) = puVar2[1];
  *(undefined4 *)((int)this + 0x30) = puVar2[2];
  pfVar3 = local_24;
  pfVar8 = local_30;
  pvVar1 = (void *)(**(code **)(*param_1 + 0x18))();
  pvVar1 = (void *)FUN_0049e770(pvVar1,pfVar8);
  puVar2 = (undefined4 *)FUN_0042d2b0(pvVar1,pfVar3);
  *(undefined4 *)((int)this + 0x40) = *puVar2;
  *(undefined4 *)((int)this + 0x44) = puVar2[1];
  *(undefined4 *)((int)this + 0x48) = puVar2[2];
  (**(code **)(*(int *)((int)this + 0x24) + 8))();
  fVar6 = 24.0;
  fVar5 = 5.0;
  pfVar3 = (float *)FUN_0049e770((void *)((int)this + 0x24),local_30);
  pfVar3 = (float *)FUN_00446390((float *)&stack0xffffffc4,pfVar3,fVar5);
  pfVar3 = (float *)FUN_00446390(local_24,pfVar3,fVar6);
  iVar4 = (**(code **)(*param_1 + 0x18))();
  puVar2 = (undefined4 *)FUN_0043b2d0(afStack_18,(float *)(iVar4 + 0x28),pfVar3);
  *(undefined4 *)((int)this + 0x4c) = *puVar2;
  *(undefined4 *)((int)this + 0x50) = puVar2[1];
  *(undefined4 *)((int)this + 0x54) = puVar2[2];
  FUN_0049da00(this);
  FUN_00481140((void *)((int)this + 0x170),'\x01');
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar7 = &DAT_00501908;
  puStack_8 = &stack0xffffffb4;
  FUN_00401d80(&stack0xffffffb4,"rescue");
  DispatchScriptActorEvent((void *)((int)this + 0xd4),piVar7);
  *(undefined4 *)((int)this + 0x1f8) = 0x96;
  iVar4 = DAT_00502afc;
  *(int *)((int)this + 500) = DAT_00502afc + 2000;
  *(int *)((int)this + 0x1f0) = iVar4 + 0x866;
  *(undefined1 *)((int)this + 0x1fc) = 1;
  FUN_00480d40((int)this + 0x170);
  return;
}

/* FUNCTION 0x0045bf70 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0045bf70(int param_1)

{
  int *piVar1;
  
  *(undefined4 *)(param_1 + 0x120) = 6;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff4,"botrescue");
  DispatchScriptActorEvent((void *)(param_1 + 0xd4),piVar1);
  *(undefined4 *)(param_1 + 0x1f0) = 0xffffffff;
  FUN_00480d40(param_1 + 0x170);
  FUN_00481140((void *)(param_1 + 0x170),'\0');
  return;
}

/* FUNCTION 0x00480d00 */

void __thiscall FUN_00480d00(void *this,int param_1)

{
  undefined4 uVar1;
  
  *(int *)this = param_1;
  *(int *)((int)this + 4) = param_1 + 0x10;
  *(undefined1 *)(param_1 + 0x81) = 1;
  *(undefined4 *)((int)this + 8) = *(undefined4 *)(*(int *)this + 0x60);
  uVar1 = geActor_GetActorDef();
  *(undefined4 *)((int)this + 0xc) = uVar1;
  uVar1 = geActor_GetMotionCount(uVar1);
  *(undefined4 *)((int)this + 0x10) = uVar1;
  return;
}

/* FUNCTION 0x00459a50 */

/* Analyst note: the saved Ghidra project's inline comment below predates this
   trace and calls +0x180 unresolved. 00480D00 stores geActor_GetMotionCount
   at animation context +0x10 (VictimRescue +0x180), resolving that field. */

/* Called by StartVictimRoam after at least one initial candidate passes. Selects through the victim
   animation controller at +0x170; the +0x180==6 branch selects animations 3-5 and changes movement
   scalar +0x228. Meaning of the +0x180 discriminator is unresolved. */

void __thiscall SelectVictimRoamAnimation(void *this,int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    FUN_00480d40((int)this + 0x170);
    return;
  }
  if (*(uint *)((int)this + 0x180) != 6) {
    uVar2 = 0x3e4ccccd;
    uVar1 = FUN_00404b30(*(uint *)((int)this + 0x180));
    FUN_00480ea0((void *)((int)this + 0x170),uVar1,uVar2);
    *(undefined4 *)((int)this + 0x120) = 4;
    return;
  }
  if (param_1 == 1) {
    uVar2 = 0x3f800000;
    uVar1 = FUN_00404b30(3);
    FUN_00480ea0((void *)((int)this + 0x170),uVar1,uVar2);
    return;
  }
  uVar1 = FUN_00404b30(3);
  if (uVar1 == 0) {
    *(undefined4 *)((int)this + 0x228) = 0x4249999a;
  }
  else if ((uVar1 == 1) || (uVar1 == 2)) {
    *(undefined4 *)((int)this + 0x228) = 0x4199999a;
  }
  FUN_00480ea0((void *)((int)this + 0x170),uVar1 + 3,0x3f800000);
  return;
}

/* FUNCTION 0x00481140 */

void __thiscall FUN_00481140(void *this,char param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar1 + 0x160) == '\0') {
    iVar1 = iVar1 + 8;
  }
  else {
    iVar1 = iVar1 + 0xb4;
  }
  geWorld_SetActorFlags
            (*(undefined4 *)(iVar1 + 0x78),*(undefined4 *)((int)this + 8),
             -(uint)(param_1 != '\0') & 0x400);
  return;
}

/* FUNCTION 0x0048d8e0 */

void __fastcall FUN_0048d8e0(void *param_1)

{
  RequestSoundInstancePlayback(param_1,(undefined1 *)0x0);
  return;
}
