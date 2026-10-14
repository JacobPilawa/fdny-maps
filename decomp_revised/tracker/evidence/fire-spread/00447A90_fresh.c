/* Current Ghidra output; FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: After the pointer-at-root-+0x48 / pointed-to-+4 gate (embedded A_Name string length;
   see fire_name_gate.md), size, and deadline gates,
   compare a uniform [0,1) draw with intensity at +0x128. On success, sample radial distance
   0.5*min(sizeX,sizeZ)*(1+U*intensity) and angle [0,2*pi), sweep a +/-1 box downward 200 units from
   20 units above the origin, then factory-create and register an RCS_FireRescue child with timed
   size keys. A failed probability roll sets +0x124 to now+500; a geometry miss does not set this
   retry here.
   
   Parameters: param_1 is the parent FireRescue object.
   
   Returns: void; may enqueue a child, append its size keys, and update the parent's next-attempt
   time. */

void __fastcall ProcessFireRescueSpread(void *parentFire)

{
  float fVar1;
  float fVar2;
  float10 fVar3;
  float fVar4;
  float *pfVar5;
  int iVar6;
  undefined4 uVar7;
  int *piVar8;
  undefined4 *puVar9;
  int iVar10;
  void *pvVar11;
  float10 fVar12;
  float10 fVar13;
  float10 fVar14;
  float10 fVar15;
  ulonglong uVar16;
  float local_80;
  float local_7c;
  float local_78;
  int local_74 [4];
  int local_64 [4];
  void *local_54;
  undefined8 local_50;
  float local_48;
  double local_44;
  double local_3c;
  undefined4 local_34;
  float local_30;
  float local_2c;
  float local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf85e;
  local_c = ExceptionList;
  if (*(int *)(*(int *)((int)parentFire + 0x48) + 4) != 0) {
    pvVar11 = (void *)((int)parentFire + 300);
    ExceptionList = &local_c;
    local_54 = parentFire;
    pfVar5 = (float *)FUN_00447850(pvVar11,local_74);
    if (((float)_DAT_004dfc78 <= *pfVar5) && (*(uint *)((int)parentFire + 0x124) <= DAT_00502afc)) {
      fVar12 = FUN_00498b30(0.0,1.0);
      if (fVar12 <= (float10)*(float *)((int)parentFire + 0x128)) {
        pfVar5 = (float *)FUN_00447850(pvVar11,local_74);
        iVar6 = FUN_00447850(pvVar11,local_64);
        if (*(float *)(iVar6 + 8) <= *pfVar5) {
          iVar6 = FUN_00447850(pvVar11,local_18);
          fVar1 = *(float *)(iVar6 + 8);
        }
        else {
          pfVar5 = (float *)FUN_00447850(pvVar11,local_18);
          fVar1 = *pfVar5;
        }
        fVar1 = fVar1 * (float)_DAT_004de6f8;
        fVar12 = FUN_00498b30(0.0,1.0);
        fVar2 = (float)(fVar12 * (float10)*(float *)((int)parentFire + 0x128));
        fVar1 = ((fVar1 + fVar1) - fVar1) * fVar2 + fVar1;
        fVar12 = FUN_00498b30(0.0,6.2831855);
        iVar6 = FUN_0044a6a0((int)pvVar11);
        fVar13 = (float10)fcos((float10)(float)fVar12);
        local_30 = 0.0;
        local_2c = 0.0;
        local_28 = 0.0;
        local_24 = 0;
        local_20 = 0;
        local_1c = 0;
        local_3c = -13510798882111488.0;
        local_34 = 0;
        local_64[0] = 0x3f800000;
        local_64[1] = 0x3f800000;
        local_64[2] = 0x3f800000;
        local_74[0] = -0x40800000;
        local_74[1] = 0xbf800000;
        local_74[2] = 0xbf800000;
        local_44 = (double)fVar13;
        local_80 = fVar1 * (float)fVar13 + *(float *)(iVar6 + 0x28);
        fVar12 = (float10)fsin((float10)(float)fVar12);
        local_50 = (double)fVar12;
        local_78 = fVar1 * (float)fVar12 + *(float *)(iVar6 + 0x30);
        local_7c = *(float *)(iVar6 + 0x2c) + _DAT_004e1604;
        uVar7 = FUN_00492ce0(*(void **)(DAT_00503a78 + 4),&local_80,local_74,local_64,
                             (float *)&local_3c,&local_30,&local_24);
        if (((((char)uVar7 != '\0') && ((float)_DAT_004dfa28 <= ABS(local_2c - local_7c))) &&
            (ABS(local_30 - local_80) <= (float)_DAT_004df7b8)) &&
           (ABS(local_28 - local_78) <= (float)_DAT_004df7b8)) {
          fVar4 = _DAT_004dfc24 - fVar2;
          fVar12 = FUN_00498b30(0.9,1.1);
          fVar12 = ((float10)fVar4 * (float10)_DAT_004e110c + (float10)_DAT_004e169c) * fVar12;
          fVar1 = (float)fVar12;
          local_3c = (double)(fVar12 * (float10)_DAT_004e0508);
          fVar14 = FUN_00498b30(0.9,1.1);
          fVar3 = (float10)local_3c;
          fVar15 = FUN_00498b30(0.8,1.2);
          fVar13 = (float10)_DAT_004e1698;
          fVar12 = (float10)_DAT_004e169c;
          local_48 = (float)local_50;
          local_50._0_4_ = (float)local_44;
          local_50._4_4_ = 0;
          piVar8 = FUN_0049b760(&local_44,"RCS_FireRescue",100);
          local_4 = 0;
          puVar9 = (undefined4 *)FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar8);
          local_4 = 0xffffffff;
          FUN_0049b6f0((int)&local_44);
          iVar10 = (**(code **)*puVar9)();
          *(float *)(iVar10 + 0x7c) = local_80;
          *(float *)(iVar10 + 0x80) = local_7c;
          *(float *)(iVar10 + 0x84) = local_78;
          *(float *)(iVar10 + 0x70) = (float)local_50;
          *(undefined4 *)(iVar10 + 0x74) = local_50._4_4_;
          *(undefined4 *)(iVar10 + 0xc) = 0;
          *(undefined4 *)(iVar10 + 0x14) = 0;
          *(undefined4 *)(iVar10 + 0x10) = 0;
          *(undefined4 *)(iVar10 + 4) = 0;
          *(undefined4 *)(iVar10 + 8) = 0;
          *(float *)(iVar10 + 0x78) = local_48;
          *(undefined4 *)(iVar10 + 0x6c) = 1;
          *(undefined4 *)(iVar10 + 0x1c) = 1;
          *(undefined4 *)(iVar10 + 0x28) = 0;
          local_74[2] = iVar10 + 0x4c;
          *(undefined4 *)(iVar10 + 0x20) = 0;
          *(undefined4 *)(iVar10 + 0x2c) = 0;
          local_74[1] = iVar10 + 0x3c;
          local_64[0] = iVar10 + 0x38;
          local_74[0] = iVar10 + 0x2c;
          local_74[3] = iVar10 + 0x5c;
          local_64[2] = iVar10 + 0x58;
          *(undefined4 *)(local_74[*(int *)(iVar10 + 0x28)] + 4) = 0;
          local_64[1] = iVar10 + 0x48;
          local_64[3] = iVar10 + 0x68;
          *(undefined4 *)(local_74[*(int *)(iVar10 + 0x28)] + 8) = 0;
          *(undefined4 *)local_64[*(int *)(iVar10 + 0x28)] = 0;
          iVar6 = *(int *)(iVar10 + 0x28) + 1;
          *(int *)(iVar10 + 0x28) = iVar6;
          *(float *)local_74[iVar6] =
               (float)(((float10)fVar4 * fVar3 + (float10)fVar1 * (float10)fVar2) * fVar14);
          *(float *)(local_74[*(int *)(iVar10 + 0x28)] + 4) =
               (float)(((float10)fVar2 * fVar13 + fVar12) * fVar15);
          *(float *)(local_74[*(int *)(iVar10 + 0x28)] + 8) = fVar1;
          FUN_00498b30(750.0,3000.0);
          uVar16 = FUN_004b9400();
          *(int *)local_64[*(int *)(iVar10 + 0x28)] = (int)uVar16;
          iVar6 = *(int *)(iVar10 + 0x28) + 1;
          *(int *)(iVar10 + 0x28) = iVar6;
          *(undefined4 *)local_74[iVar6] = 0;
          *(undefined4 *)(local_74[*(int *)(iVar10 + 0x28)] + 4) = 0;
          *(float *)(local_74[*(int *)(iVar10 + 0x28)] + 8) = (float)local_3c;
          iVar6 = *(int *)(iVar10 + 0x28);
          FUN_00498b30(1000.0,6000.0);
          uVar16 = FUN_004b9400();
          *(int *)local_64[iVar6] = (int)uVar16;
          uVar16 = FUN_004b9400();
          *(int *)((int)local_54 + 0x124) = (int)uVar16;
          *(int *)(iVar10 + 0x28) = *(int *)(iVar10 + 0x28) + 1;
          puVar9 = operator_new(0x14);
          local_4 = 1;
          if (puVar9 == (undefined4 *)0x0) {
            pvVar11 = (void *)0x0;
          }
          else {
            pvVar11 = (void *)FUN_0049f230(puVar9);
          }
          local_4 = 0xffffffff;
          piVar8 = FUN_0049b760(local_18,"RCS_FireRescue",100);
          local_4 = 2;
          iVar6 = FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar8);
          local_4 = 0xffffffff;
          FUN_0049b6f0((int)local_18);
          if (iVar6 != 0) {
            *(int *)((int)pvVar11 + 0xc) = iVar6;
          }
          *(undefined4 *)((int)pvVar11 + 8) = 0;
          FUN_0049d130(pvVar11,iVar10);
          FUN_0049d4c0(pvVar11);
        }
      }
      else {
        *(uint *)((int)parentFire + 0x124) = DAT_00502afc + 500;
      }
    }
  }
  ExceptionList = local_c;
  return;
}


