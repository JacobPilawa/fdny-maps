
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   When the HoseSpray item is attached and its spray action is active, walk eligible FireRescue
   objects. Compute the fire-volume influence at the current contact point with the item padding and
   shared ellipsoid helper. For influence >= 0.01, call ApplyFireSuppressionContact with the current
   contact xyz, influence, and finalize=1. If the fire remains above 0.01 and the effect timer is
   ready, update the timer and create the hose-hit effect path.
   Parameters:
   this is the HoseSpray item object; its contact position is stored at +0x614 and padding at
   +0x620.
   Returns:
   void. The caller is UpdateHoseSpray at 0x004640F0; global update cadence is not proven. */

void __thiscall ApplyHoseFireContact(void *this)

{
  float padding;
  undefined4 *puVar1;
  undefined4 *puVar2;
  void *pvVar3;
  float10 fVar4;
  ulonglong uVar5;
  float fVar6;
  undefined4 uVar7;
  float fVar8;
  undefined4 uVar9;
  float fVar10;
  undefined4 uVar11;
  int local_38;
  undefined4 *local_34;
  undefined4 local_30 [3];
  undefined4 local_24 [3];
  undefined4 local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1918;
  local_c = ExceptionList;
  if (*(char *)((int)this + 0x624) != '\0') {
    padding = *(float *)((int)this + 0x620);
    ExceptionList = &local_c;
    FUN_00481a50((undefined4 *)((int)this + 0x404));
    FUN_00481c00(&local_38);
    local_4 = 0;
    puVar1 = local_34;
    while( true ) {
      if ((local_38 == 0) || (puVar1 == (undefined4 *)0x0)) {
        pvVar3 = (void *)0x0;
      }
      else {
        pvVar3 = (void *)*puVar1;
      }
      if ((local_38 != 0) && (puVar1 != (undefined4 *)0x0)) {
        puVar1 = (undefined4 *)puVar1[1];
        local_34 = puVar1;
      }
      if (pvVar3 == (void *)0x0) break;
      if (*(int *)((int)pvVar3 + 0x18) == 6) {
        fVar6 = ComputeFireVolumeFalloff
                          ((void *)((int)pvVar3 + 300),(float *)((int)this + 0x614),padding);
        puVar1 = local_34;
        if ((((float)_DAT_004dfa28 <= fVar6) &&
            (ApplyFireSuppressionContact
                       (pvVar3,*(float *)((int)this + 0x614),*(float *)((int)this + 0x618),
                        *(float *)((int)this + 0x61c),fVar6,'\x01'), puVar1 = local_34,
            (float)_DAT_004dfa10 <= *(float *)((int)pvVar3 + 0x128))) &&
           (*(uint *)((int)this + 0x3b0) <= DAT_00502afc)) {
          FUN_00498b30(10.0,250.0);
          uVar5 = FUN_004b9400();
          *(int *)((int)this + 0x3b0) = (int)uVar5;
          puVar1 = (undefined4 *)FUN_00447870((void *)((int)pvVar3 + 300),local_30);
          puVar2 = FUN_0047ff30((void *)((int)this + 0x3d0),puVar1);
          puVar1 = local_34;
          if (puVar2 != (undefined4 *)0x0) {
            uVar11 = 0;
            uVar9 = 0;
            uVar7 = 0;
            puVar1 = local_24;
            pvVar3 = (void *)FUN_0047f1c0((int)puVar2);
            FUN_0042d280(pvVar3,puVar1,uVar7,uVar9,uVar11);
            fVar4 = FUN_00498b30(-50.0,50.0);
            fVar6 = (float)fVar4;
            fVar4 = FUN_00498b30(100.0,400.0);
            fVar10 = (float)fVar4;
            fVar4 = FUN_00498b30(-50.0,50.0);
            fVar8 = (float)fVar4;
            puVar1 = local_18;
            pvVar3 = (void *)FUN_0047f1b0((int)puVar2);
            FUN_0042d280(pvVar3,puVar1,fVar8,fVar10,fVar6);
            puVar2[0x29] = 0x20;
            puVar1 = local_34;
          }
        }
      }
    }
    local_4 = 0xffffffff;
    if (local_38 != 0) {
      FUN_00429fe0((void *)(local_38 + 0x28),(int)&local_38);
    }
  }
  ExceptionList = local_c;
  return;
}

