
/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __thiscall FUN_0046f9d0(void *this,int *param_1)

{
  int *this_00;
  char cVar1;
  bool bVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  void *pvVar6;
  uint3 extraout_var;
  int iVar7;
  ulonglong uVar8;
  int *piVar9;
  char *pcVar10;
  int *piVar11;
  int *local_14;
  undefined1 *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  this_00 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d2570;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar3 = (undefined4 *)FUN_0040c330(param_1,&param_1);
  iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"que");
  cVar1 = '\x01' - (iVar4 != 0);
  param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
  if (cVar1 == '\0') {
    puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"flush");
    cVar1 = '\x01' - (iVar4 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 == '\0') {
      puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
      iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"wait");
      cVar1 = '\x01' - (iVar4 != 0);
      param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
      if (cVar1 == '\0') {
        puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
        iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"idle");
        cVar1 = '\x01' - (iVar4 != 0);
        param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
        if (cVar1 == '\0') {
          puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
          iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"cmd");
          cVar1 = '\x01' - (iVar4 != 0);
          param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
          if (cVar1 == '\0') {
            pcVar10 = "loopinque";
            pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
            bVar2 = FUN_0040c360(pvVar6,pcVar10);
            if (bVar2) {
              iVar4 = this_00[7];
              iVar7 = 0;
              if (0 < iVar4) {
                do {
                  FUN_00473d20(this_00,&param_1,iVar7,0);
                  local_10 = &stack0xffffffd4;
                  local_4 = 3;
                  if (DAT_00501918 == '\0') {
                    DAT_00501918 = '\x01';
                    DAT_00501908 = 2;
                    DAT_0050190c = 0;
                    _DAT_00501910 = 1;
                    DAT_00501914 = 0;
                  }
                  piVar11 = &DAT_00501908;
                  if ((param_1 != &DAT_00501908) && (piVar11 = param_1, param_1 != &DAT_00501908)) {
                    *param_1 = *param_1 + 1;
                  }
                  FUN_0046ea90(this,piVar11);
                  local_4 = 0xffffffff;
                  if (param_1 != &DAT_00501908) {
                    *param_1 = *param_1 + -1;
                    if (*param_1 < 1) {
                      FUN_004011c0(&param_1);
                    }
                    else {
                      param_1 = &DAT_00501908;
                    }
                  }
                  iVar4 = this_00[7];
                  iVar7 = iVar7 + 1;
                } while (iVar7 < iVar4);
              }
              iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
            }
            else {
              pcVar10 = "onmsg";
              pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
              bVar2 = FUN_0040c360(pvVar6,pcVar10);
              if (bVar2) {
                iVar4 = this_00[7];
                iVar7 = 0;
                if (0 < iVar4) {
                  do {
                    FUN_00473d20(this_00,&local_14,iVar7,0);
                    local_4 = 4;
                    FUN_00473d20(this_00,&param_1,iVar7,1);
                    if (DAT_00501918 == '\0') {
                      DAT_00501918 = '\x01';
                      DAT_00501908 = 2;
                      DAT_0050190c = 0;
                      _DAT_00501910 = 1;
                      DAT_00501914 = 0;
                    }
                    piVar11 = &DAT_00501908;
                    if ((param_1 != &DAT_00501908) && (piVar11 = param_1, param_1 != &DAT_00501908))
                    {
                      *param_1 = *param_1 + 1;
                    }
                    local_10 = &stack0xffffffd0;
                    if (DAT_00501918 == '\0') {
                      DAT_00501918 = '\x01';
                      DAT_00501908 = 2;
                      DAT_0050190c = 0;
                      _DAT_00501910 = 1;
                      DAT_00501914 = 0;
                    }
                    piVar9 = &DAT_00501908;
                    if ((local_14 != &DAT_00501908) &&
                       (piVar9 = local_14, local_14 != &DAT_00501908)) {
                      *local_14 = *local_14 + 1;
                    }
                    local_4._0_1_ = 5;
                    FUN_0046f6f0(this,piVar9,piVar11);
                    local_4 = CONCAT31(local_4._1_3_,4);
                    FUN_00401c90(&param_1);
                    local_4 = 0xffffffff;
                    FUN_00401c90(&local_14);
                    iVar4 = this_00[7];
                    iVar7 = iVar7 + 1;
                  } while (iVar7 < iVar4);
                }
                iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
              }
              else {
                pcVar10 = "onmsgappend";
                pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                bVar2 = FUN_0040c360(pvVar6,pcVar10);
                if (bVar2) {
                  iVar4 = this_00[7];
                  iVar7 = 0;
                  if (0 < iVar4) {
                    do {
                      FUN_00473d20(this_00,&local_14,iVar7,0);
                      local_4 = 7;
                      FUN_00473d20(this_00,&param_1,iVar7,1);
                      if (DAT_00501918 == '\0') {
                        DAT_00501918 = '\x01';
                        DAT_00501908 = 2;
                        DAT_0050190c = 0;
                        _DAT_00501910 = 1;
                        DAT_00501914 = 0;
                      }
                      piVar11 = &DAT_00501908;
                      if ((param_1 != &DAT_00501908) &&
                         (piVar11 = param_1, param_1 != &DAT_00501908)) {
                        *param_1 = *param_1 + 1;
                      }
                      local_10 = &stack0xffffffd0;
                      if (DAT_00501918 == '\0') {
                        DAT_00501918 = '\x01';
                        DAT_00501908 = 2;
                        DAT_0050190c = 0;
                        _DAT_00501910 = 1;
                        DAT_00501914 = 0;
                      }
                      piVar9 = &DAT_00501908;
                      if ((local_14 != &DAT_00501908) &&
                         (piVar9 = local_14, local_14 != &DAT_00501908)) {
                        *local_14 = *local_14 + 1;
                      }
                      local_4._0_1_ = 8;
                      FUN_0046f820(this,piVar9,piVar11);
                      local_4 = CONCAT31(local_4._1_3_,7);
                      FUN_00401c90(&param_1);
                      local_4 = 0xffffffff;
                      FUN_00401c90(&local_14);
                      iVar4 = this_00[7];
                      iVar7 = iVar7 + 1;
                    } while (iVar7 < iVar4);
                  }
                  iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                }
                else {
                  pcVar10 = "printmsgs";
                  pvVar6 = (void *)FUN_0040c330(this_00,&local_10);
                  bVar2 = FUN_0040c360(pvVar6,pcVar10);
                  if (bVar2) {
                    iVar4 = this_00[7];
                    iVar7 = 0;
                    if (0 < iVar4) {
                      do {
                        FUN_0046ef60((int)this);
                        iVar4 = this_00[7];
                        iVar7 = iVar7 + 1;
                      } while (iVar7 < iVar4);
                    }
                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                  }
                  else {
                    iVar4 = (uint)extraout_var << 8;
                  }
                }
              }
            }
          }
          else {
            iVar4 = this_00[7];
            if (0 < iVar4) {
              iVar7 = 0;
              do {
                FUN_00473d20(this_00,&param_1,iVar7,0);
                local_10 = &stack0xffffffd4;
                local_4 = 2;
                if (DAT_00501918 == '\0') {
                  DAT_00501918 = '\x01';
                  DAT_00501908 = 2;
                  DAT_0050190c = 0;
                  _DAT_00501910 = 1;
                  DAT_00501914 = 0;
                }
                piVar11 = &DAT_00501908;
                if ((param_1 != &DAT_00501908) && (piVar11 = param_1, param_1 != &DAT_00501908)) {
                  *param_1 = *param_1 + 1;
                }
                FUN_0046e470(piVar11);
                local_4 = 0xffffffff;
                if (param_1 != &DAT_00501908) {
                  *param_1 = *param_1 + -1;
                  if (*param_1 < 1) {
                    FUN_004011c0(&param_1);
                  }
                  else {
                    param_1 = &DAT_00501908;
                  }
                }
                iVar4 = this_00[7];
                iVar7 = iVar7 + 1;
              } while (iVar7 < iVar4);
            }
            iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
          }
        }
        else {
          iVar4 = 0;
          if (0 < this_00[7]) {
            do {
              *(undefined4 *)((int)this + 0x4c) = 0;
              iVar4 = iVar4 + 1;
            } while (iVar4 < this_00[7]);
          }
          iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
        }
      }
      else {
        iVar4 = this_00[7];
        if (0 < iVar4) {
          iVar7 = 0;
          do {
            FUN_00473d20(this_00,&param_1,iVar7,0);
            uVar5 = FUN_0040cc50(&param_1,&local_14);
            local_4 = 0xffffffff;
            if ((char)uVar5 == '\0') {
              if (param_1 != &DAT_00501908) {
                *param_1 = *param_1 + -1;
                if (*param_1 < 1) {
                  FUN_004011c0(&param_1);
                }
                else {
                  param_1 = &DAT_00501908;
                }
              }
            }
            else {
              if (param_1 != &DAT_00501908) {
                *param_1 = *param_1 + -1;
                if (*param_1 < 1) {
                  FUN_004011c0(&param_1);
                }
                else {
                  param_1 = &DAT_00501908;
                }
              }
              uVar8 = FUN_004b9400();
              *(int *)((int)this + 0x48) = (int)uVar8;
              *(undefined4 *)((int)this + 0x4c) = 1;
            }
            iVar4 = this_00[7];
            iVar7 = iVar7 + 1;
          } while (iVar7 < iVar4);
        }
        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
      }
    }
    else {
      iVar4 = this_00[7];
      iVar7 = 0;
      if (0 < iVar4) {
        do {
          if (*(int *)((int)this + 0x34) != 0) {
            FUN_0043b6e0((void *)((int)this + 0x30),0);
          }
          *(undefined4 *)((int)this + 0x4c) = 0;
          iVar4 = this_00[7];
          iVar7 = iVar7 + 1;
        } while (iVar7 < iVar4);
      }
      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
    }
  }
  else {
    iVar4 = this_00[7];
    iVar7 = 0;
    if (0 < iVar4) {
      do {
        FUN_00473d20(this_00,&param_1,iVar7,0);
        local_14 = (int *)&stack0xffffffd4;
        local_4 = 0;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar11 = &DAT_00501908;
        if ((param_1 != &DAT_00501908) && (piVar11 = param_1, param_1 != &DAT_00501908)) {
          *param_1 = *param_1 + 1;
        }
        FUN_0046e9d0(this,piVar11);
        local_4 = 0xffffffff;
        if (param_1 != &DAT_00501908) {
          *param_1 = *param_1 + -1;
          if (*param_1 < 1) {
            FUN_004011c0(&param_1);
          }
          else {
            param_1 = &DAT_00501908;
          }
        }
        iVar4 = this_00[7];
        iVar7 = iVar7 + 1;
      } while (iVar7 < iVar4);
    }
    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
  }
  ExceptionList = local_c;
  return iVar4;
}

