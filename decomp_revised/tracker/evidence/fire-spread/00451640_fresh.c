/* Ghidra decompilation, manually reconciled with callee 0044BB00;
   FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: Return when Player state +0x18C is 6. Otherwise calculate a fire-relative angle and
   call 0044BB00, which writes one directional burn-indicator byte; it does not steer camera or
   movement heading. When unsigned game clock exceeds Player +0x390, request RC_Pain, set the next
   pain deadline using the shared RNG range 1000..2500 plus current clock, and increment Serious
   Burns. Independently accumulate 1.2 * frameDelta * exposureInfluence into Player +0x3538; a
   strict extended-precision comparison above 1.0 calls 0x00450FB0 with 1.
   
   Parameters: this is the Player; fireSource is the source position; exposureInfluence is the
   FireRescue message value.
   
   Returns: void; updates directional indicator state, pain deadline/stat, burn accumulator, and may
   trigger passout. The current-clock addition is visible in assembly but omitted by the C-like
   decompilation. */

void __thiscall ApplyPlayerFireExposure(void *this,float *fireSource,float exposureInfluence)

{
  float fVar1;
  float *pfVar2;
  float10 fVar3;
  float10 fVar4;
  ulonglong uVar5;
  int *piVar6;
  char cVar7;
  float local_74;
  float local_70;
  float local_6c;
  float local_68;
  float local_64;
  float local_60;
  undefined4 local_5c [3];
  undefined1 local_50 [68];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0318;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x18c) != 6) {
    ExceptionList = &local_c;
    FUN_0042d2d0(local_50,0);
    pfVar2 = (float *)FUN_0043b2d0(&local_74,fireSource,(float *)((int)this + 0x4c));
    local_68 = *pfVar2;
    local_64 = pfVar2[1];
    local_60 = pfVar2[2];
    pfVar2 = (float *)FUN_0049e770((void *)((int)this + 0x24),local_5c);
    local_74 = *pfVar2;
    local_6c = pfVar2[2];
    local_70 = pfVar2[1];
    fVar3 = (float10)fpatan((float10)local_6c,(float10)local_74);
    fVar4 = (float10)fpatan((float10)local_60,(float10)local_68);
    fireSource = (float *)(float)(fVar3 - fVar4);
    if ((float)_DAT_004e1840 < (float)fireSource) {
      fireSource = (float *)((float)fireSource - (float)_DAT_004e1b30);
    }
    if ((float)fireSource < (float)_DAT_004e1830) {
      fireSource = (float *)((float)fireSource + (float)_DAT_004e1b30);
    }
    FUN_0044bb00(this,(float)fireSource);
    if (*(int **)((int)this + 0x390) < DAT_00502afc) {
      cVar7 = '\0';
      piVar6 = DAT_00502afc;
      FUN_00402790(&stack0xffffff78,"RC_Pain");
      FUN_004897c0((void *)((int)this + 0x3540),piVar6,cVar7);
      FUN_00498b30(1000.0,2500.0);
      uVar5 = FUN_004b9400();
      *(int *)((int)this + 0x390) = (int)uVar5;
      FUN_00402790(&fireSource,"Serious Burns");
      local_4 = 0;
      FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&fireSource,1);
      local_4 = 0xffffffff;
      if (fireSource != (float *)&DAT_00501908) {
        *fireSource = (float)((int)*fireSource + -1);
        if ((int)*fireSource < 1) {
          FUN_004011c0(&fireSource);
        }
        else {
          fireSource = (float *)&DAT_00501908;
        }
      }
    }
    fVar1 = _DAT_004e1818 * DAT_00502b04 * exposureInfluence + *(float *)((int)this + 0x3538);
    *(float *)((int)this + 0x3538) = fVar1;
    if ((float)_DAT_004dd808 < fVar1) {
      FUN_00450fb0(this,1);
    }
  }
  ExceptionList = local_c;
  return;
}


