/* Current Ghidra output. ComputeFireVolumeFalloff's MCP prototype is normalized to float; raw x87 result behavior is preserved in its disassembly and old pre-normalization export. FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: FireRescue vtable slot +0x48 points here. Skip mode 5 or an exposure-record count
   outside [0,3], obtain candidate position via its interface slot +0x0C, compute maximum absolute
   axis separation and convert it toward zero, then call UpdateFireRescueProximityAndSpread. At separation <=500,
   calculate ComputeFireVolumeFalloff and weight it by intensity; attenuate by 0.1 if the world
   visibility query fails. Store the FireRescue pointer and influence, then send Player message
   0x7531.
   
   Parameters: this is the FireRescue object; contactCandidate is the candidate entity/interface.
   
   Returns: void; queues a bounded exposure record and sends the message when its gates pass.
   Dispatch timing is still unresolved. */

void __thiscall DispatchFireRescueExposureUpdate(void *this,int *contactCandidate)

{
  float fVar1;
  float fVar2;
  float fVar3;
  bool bVar4;
  int *piVar5;
  ulonglong uVar6;
  double dStack_8;
  
  if ((((*(int *)((int)this + 0x70) != 5) && (-1 < *(int *)((int)this + 0xb8))) &&
      (*(int *)((int)this + 0xb8) < 4)) &&
     (DAT_00502b94 = (float *)(**(code **)(*contactCandidate + 0xc))(), DAT_00502b94 != (float *)0x0
     )) {
    dStack_8 = (double)ABS(DAT_00502b94[2] - *(float *)((int)this + 0x84));
    uVar6 = FUN_004b9400();
    UpdateFireRescueProximityAndSpread(this,(int)uVar6);
    if (*(float *)((int)this + 0x120) <= _DAT_004e16c0) {
      fVar2 = _DAT_004dfc24 - *(float *)((int)this + 0x128);
      fVar3 = (float)_DAT_004e0508;
      fVar1 = *(float *)((int)this + 0x128);
      _DAT_004ff8e0 =
           ComputeFireVolumeFalloff((void *)((int)this + 300),DAT_00502b94,DAT_00502b94[3]);
      _DAT_004ff8e0 = _DAT_004ff8e0 * (fVar2 * fVar3 + fVar1);
      if ((float)_DAT_004e16b8 <= _DAT_004ff8e0) {
        bVar4 = FUN_0048fea0(*(int *)(DAT_00503a78 + 4),DAT_00502b94,
                             (undefined4 *)((int)this + 0x7c),'\0');
        if (!bVar4) {
          _DAT_004ff8e0 = _DAT_004ff8e0 * (float)_DAT_004df7b8;
        }
        fVar1 = _DAT_004ff8e0;
        *(void **)((int)this + *(int *)((int)this + 0xb8) * 8 + 0xbc) = this;
        *(float *)((int)this + *(int *)((int)this + 0xb8) * 8 + 0xc0) = fVar1;
        piVar5 = (int *)FUN_0049d350(contactCandidate,(undefined4 *)&dStack_8);
        FUN_0049d4e0(*piVar5,piVar5[1],0x7531,(int)this + *(int *)((int)this + 0xb8) * 8 + 0xbc);
        *(int *)((int)this + 0xb8) = *(int *)((int)this + 0xb8) + 1;
      }
    }
  }
  return;
}


