/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __thiscall FUN_0047a1f0(void *this,int *param_1)

{
  int *this_00;
  char cVar1;
  bool bVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  void *pvVar6;
  uint3 extraout_var;
  int *extraout_ECX;
  int *extraout_ECX_00;
  int *extraout_ECX_01;
  int *extraout_ECX_02;
  int *extraout_ECX_03;
  int *extraout_ECX_04;
  int *extraout_ECX_05;
  int *extraout_ECX_06;
  int *extraout_ECX_07;
  int *extraout_ECX_08;
  int *extraout_ECX_09;
  int *extraout_ECX_10;
  int *extraout_ECX_11;
  int *extraout_ECX_12;
  int *extraout_ECX_13;
  int *extraout_ECX_14;
  int *extraout_ECX_15;
  int *extraout_ECX_16;
  int *extraout_ECX_17;
  int *extraout_ECX_18;
  int *extraout_ECX_19;
  int *extraout_ECX_20;
  int extraout_ECX_21;
  int extraout_ECX_22;
  int *extraout_ECX_23;
  int **ppiVar7;
  int *extraout_ECX_24;
  int *unaff_EDI;
  int iVar8;
  int *piVar9;
  int *piVar10;
  undefined1 *puVar11;
  char *pcVar12;
  int *piVar13;
  int *piVar14;
  void *local_34;
  int *local_30;
  int *local_2c;
  int *local_28;
  int *local_24;
  int local_20;
  undefined1 *local_1c;
  undefined1 *local_18;
  int *local_14;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  this_00 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3328;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  local_34 = this;
  puVar3 = (undefined4 *)FUN_0040c330(param_1,&param_1);
  iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"help");
  if ((iVar4 == 0) && (this_00[8] != 1)) {
    uVar5 = FUN_004727d0();
    iVar4 = CONCAT31((int3)((uint)uVar5 >> 8),1);
  }
  else {
    puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"help");
    cVar1 = '\x01' - (iVar4 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 == '\0') {
      puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
      iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"action");
      cVar1 = '\x01' - (iVar4 != 0);
      param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
      if (cVar1 == '\0') {
        puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
        iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"tell");
        cVar1 = '\x01' - (iVar4 != 0);
        param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
        if (cVar1 == '\0') {
          puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
          iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"break");
          cVar1 = '\x01' - (iVar4 != 0);
          param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
          if (cVar1 == '\0') {
            pcVar12 = "if";
            pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
            bVar2 = FUN_0040c360(pvVar6,pcVar12);
            if (bVar2) {
              iVar4 = this_00[7];
              iVar8 = 0;
              if (0 < iVar4) {
                do {
                  FUN_00473d20(this_00,&local_2c,iVar8,0);
                  local_4 = 7;
                  FUN_00473d20(this_00,&local_30,iVar8,1);
                  local_4._0_1_ = 8;
                  FUN_00473d20(this_00,&param_1,iVar8,2);
                  if (DAT_00501918 == '\0') {
                    DAT_00501918 = '\x01';
                    DAT_00501908 = 2;
                    DAT_0050190c = 0;
                    _DAT_00501910 = 1;
                    DAT_00501914 = 0;
                  }
                  piVar9 = &DAT_00501908;
                  if ((param_1 != &DAT_00501908) && (piVar9 = param_1, param_1 != &DAT_00501908)) {
                    *param_1 = *param_1 + 1;
                  }
                  if (DAT_00501918 == '\0') {
                    DAT_00501918 = '\x01';
                    DAT_00501908 = 2;
                    DAT_0050190c = 0;
                    _DAT_00501910 = 1;
                    DAT_00501914 = 0;
                  }
                  piVar13 = &DAT_00501908;
                  if ((local_30 != &DAT_00501908) && (piVar13 = local_30, local_30 != &DAT_00501908)
                     ) {
                    *local_30 = *local_30 + 1;
                  }
                  local_10 = (int *)&stack0xffffffb0;
                  if (DAT_00501918 == '\0') {
                    DAT_00501918 = '\x01';
                    DAT_00501908 = 2;
                    DAT_0050190c = 0;
                    _DAT_00501910 = 1;
                    DAT_00501914 = 0;
                  }
                  piVar10 = &DAT_00501908;
                  if ((local_2c != &DAT_00501908) && (piVar10 = local_2c, local_2c != &DAT_00501908)
                     ) {
                    *local_2c = *local_2c + 1;
                  }
                  local_4._0_1_ = 9;
                  FUN_004758f0(local_34,piVar10,piVar13,piVar9);
                  local_4._0_1_ = 8;
                  if (param_1 != &DAT_00501908) {
                    *param_1 = *param_1 + -1;
                    if (*param_1 < 1) {
                      FUN_004011c0(&param_1);
                    }
                    else {
                      param_1 = &DAT_00501908;
                    }
                  }
                  local_4 = CONCAT31(local_4._1_3_,7);
                  if (local_30 != &DAT_00501908) {
                    *local_30 = *local_30 + -1;
                    if (*local_30 < 1) {
                      FUN_004011c0(&local_30);
                    }
                    else {
                      local_30 = &DAT_00501908;
                    }
                  }
                  local_4 = 0xffffffff;
                  if (local_2c != &DAT_00501908) {
                    *local_2c = *local_2c + -1;
                    if (*local_2c < 1) {
                      FUN_004011c0(&local_2c);
                    }
                    else {
                      local_2c = &DAT_00501908;
                    }
                  }
                  iVar4 = this_00[7];
                  iVar8 = iVar8 + 1;
                } while (iVar8 < iVar4);
              }
              iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
            }
            else {
              pcVar12 = "set";
              pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
              bVar2 = FUN_0040c360(pvVar6,pcVar12);
              if (bVar2) {
                iVar4 = this_00[7];
                iVar8 = 0;
                if (0 < iVar4) {
                  do {
                    FUN_00473d20(this_00,&local_2c,iVar8,0);
                    local_4 = 0xc;
                    FUN_00473d20(this_00,&param_1,iVar8,1);
                    if (DAT_00501918 == '\0') {
                      DAT_00501918 = '\x01';
                      DAT_00501908 = 2;
                      DAT_0050190c = 0;
                      _DAT_00501910 = 1;
                      DAT_00501914 = 0;
                    }
                    piVar9 = &DAT_00501908;
                    if ((param_1 != &DAT_00501908) && (piVar9 = param_1, param_1 != &DAT_00501908))
                    {
                      *param_1 = *param_1 + 1;
                    }
                    local_10 = (int *)&stack0xffffffb4;
                    if (DAT_00501918 == '\0') {
                      DAT_00501918 = '\x01';
                      DAT_00501908 = 2;
                      DAT_0050190c = 0;
                      _DAT_00501910 = 1;
                      DAT_00501914 = 0;
                    }
                    piVar13 = &DAT_00501908;
                    if ((local_2c != &DAT_00501908) &&
                       (piVar13 = local_2c, local_2c != &DAT_00501908)) {
                      *local_2c = *local_2c + 1;
                    }
                    local_4._0_1_ = 0xd;
                    FUN_004750e0(piVar13,piVar9);
                    local_4 = CONCAT31(local_4._1_3_,0xc);
                    FUN_00401c90(&param_1);
                    local_4 = 0xffffffff;
                    FUN_00401c90(&local_2c);
                    iVar4 = this_00[7];
                    iVar8 = iVar8 + 1;
                  } while (iVar8 < iVar4);
                }
                iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
              }
              else {
                pcVar12 = "once";
                pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                bVar2 = FUN_0040c360(pvVar6,pcVar12);
                if (bVar2) {
                  iVar4 = this_00[7];
                  iVar8 = 0;
                  if (0 < iVar4) {
                    do {
                      FUN_00473d20(this_00,&param_1,iVar8,0);
                      local_10 = (int *)&stack0xffffffb8;
                      local_4 = 0xf;
                      if (DAT_00501918 == '\0') {
                        DAT_00501918 = '\x01';
                        DAT_00501908 = 2;
                        DAT_0050190c = 0;
                        _DAT_00501910 = 1;
                        DAT_00501914 = 0;
                      }
                      piVar9 = &DAT_00501908;
                      if ((param_1 != &DAT_00501908) && (piVar9 = param_1, param_1 != &DAT_00501908)
                         ) {
                        *param_1 = *param_1 + 1;
                      }
                      FUN_00475ce0(local_34,piVar9);
                      local_4 = 0xffffffff;
                      FUN_00401c90(&param_1);
                      iVar4 = this_00[7];
                      iVar8 = iVar8 + 1;
                    } while (iVar8 < iVar4);
                  }
                  iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                }
                else {
                  pcVar12 = "ignore";
                  pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                  if (bVar2) {
                    iVar4 = this_00[7];
                    iVar8 = 0;
                    if (0 < iVar4) {
                      do {
                        FUN_00473d20(this_00,&local_2c,iVar8,0);
                        local_4 = 0x10;
                        FUN_00473d20(this_00,&param_1,iVar8,1);
                        if (DAT_00501918 == '\0') {
                          DAT_00501918 = '\x01';
                          DAT_00501908 = 2;
                          DAT_0050190c = 0;
                          _DAT_00501910 = 1;
                          DAT_00501914 = 0;
                        }
                        piVar9 = &DAT_00501908;
                        if ((param_1 != &DAT_00501908) &&
                           (piVar9 = param_1, param_1 != &DAT_00501908)) {
                          *param_1 = *param_1 + 1;
                        }
                        local_10 = (int *)&stack0xffffffb4;
                        if (DAT_00501918 == '\0') {
                          DAT_00501918 = '\x01';
                          DAT_00501908 = 2;
                          DAT_0050190c = 0;
                          _DAT_00501910 = 1;
                          DAT_00501914 = 0;
                        }
                        piVar13 = &DAT_00501908;
                        if ((local_2c != &DAT_00501908) &&
                           (piVar13 = local_2c, local_2c != &DAT_00501908)) {
                          *local_2c = *local_2c + 1;
                        }
                        local_4._0_1_ = 0x11;
                        FUN_00475f10(local_34,piVar13,piVar9);
                        local_4 = CONCAT31(local_4._1_3_,0x10);
                        FUN_00401c90(&param_1);
                        local_4 = 0xffffffff;
                        FUN_00401c90(&local_2c);
                        iVar4 = this_00[7];
                        iVar8 = iVar8 + 1;
                      } while (iVar8 < iVar4);
                    }
                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                  }
                  else {
                    pcVar12 = "ifnot";
                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                    if (bVar2) {
                      iVar4 = this_00[7];
                      iVar8 = 0;
                      if (0 < iVar4) {
                        do {
                          FUN_00473d20(this_00,&local_30,iVar8,0);
                          local_4 = 0x13;
                          FUN_00473d20(this_00,&local_2c,iVar8,1);
                          local_4._0_1_ = 0x14;
                          FUN_00473d20(this_00,&param_1,iVar8,2);
                          if (DAT_00501918 == '\0') {
                            DAT_00501918 = '\x01';
                            DAT_00501908 = 2;
                            DAT_0050190c = 0;
                            _DAT_00501910 = 1;
                            DAT_00501914 = 0;
                          }
                          piVar9 = &DAT_00501908;
                          if ((param_1 != &DAT_00501908) &&
                             (piVar9 = param_1, param_1 != &DAT_00501908)) {
                            *param_1 = *param_1 + 1;
                          }
                          if (DAT_00501918 == '\0') {
                            DAT_00501918 = '\x01';
                            DAT_00501908 = 2;
                            DAT_0050190c = 0;
                            _DAT_00501910 = 1;
                            DAT_00501914 = 0;
                          }
                          piVar13 = &DAT_00501908;
                          if ((local_2c != &DAT_00501908) &&
                             (piVar13 = local_2c, local_2c != &DAT_00501908)) {
                            *local_2c = *local_2c + 1;
                          }
                          local_10 = (int *)&stack0xffffffb0;
                          if (DAT_00501918 == '\0') {
                            DAT_00501918 = '\x01';
                            DAT_00501908 = 2;
                            DAT_0050190c = 0;
                            _DAT_00501910 = 1;
                            DAT_00501914 = 0;
                          }
                          piVar10 = &DAT_00501908;
                          if ((local_30 != &DAT_00501908) &&
                             (piVar10 = local_30, local_30 != &DAT_00501908)) {
                            *local_30 = *local_30 + 1;
                          }
                          local_4._0_1_ = 0x15;
                          FUN_00475af0(local_34,piVar10,piVar13,piVar9);
                          local_4._0_1_ = 0x14;
                          FUN_00401c90(&param_1);
                          local_4 = CONCAT31(local_4._1_3_,0x13);
                          FUN_00401c90(&local_2c);
                          local_4 = 0xffffffff;
                          FUN_00401c90(&local_30);
                          iVar4 = this_00[7];
                          iVar8 = iVar8 + 1;
                        } while (iVar8 < iVar4);
                      }
                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                    }
                    else {
                      pcVar12 = "inc";
                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                      if (bVar2) {
                        iVar4 = this_00[7];
                        iVar8 = 0;
                        if (0 < iVar4) {
                          do {
                            FUN_00473d20(this_00,&param_1,iVar8,0);
                            local_10 = (int *)&stack0xffffffb8;
                            local_4 = 0x18;
                            if (DAT_00501918 == '\0') {
                              DAT_00501918 = '\x01';
                              DAT_00501908 = 2;
                              DAT_0050190c = 0;
                              _DAT_00501910 = 1;
                              DAT_00501914 = 0;
                            }
                            piVar9 = &DAT_00501908;
                            if ((param_1 != &DAT_00501908) &&
                               (piVar9 = param_1, param_1 != &DAT_00501908)) {
                              *param_1 = *param_1 + 1;
                            }
                            FUN_004762a0(piVar9);
                            local_4 = 0xffffffff;
                            FUN_00401c90(&param_1);
                            iVar4 = this_00[7];
                            iVar8 = iVar8 + 1;
                          } while (iVar8 < iVar4);
                        }
                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                      }
                      else {
                        pcVar12 = "dec";
                        pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                        bVar2 = FUN_0040c360(pvVar6,pcVar12);
                        if (bVar2) {
                          iVar4 = this_00[7];
                          iVar8 = 0;
                          if (0 < iVar4) {
                            do {
                              FUN_00473d20(this_00,&param_1,iVar8,0);
                              local_10 = (int *)&stack0xffffffb8;
                              local_4 = 0x19;
                              if (DAT_00501918 == '\0') {
                                DAT_00501918 = '\x01';
                                DAT_00501908 = 2;
                                DAT_0050190c = 0;
                                _DAT_00501910 = 1;
                                DAT_00501914 = 0;
                              }
                              piVar9 = &DAT_00501908;
                              if ((param_1 != &DAT_00501908) &&
                                 (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                *param_1 = *param_1 + 1;
                              }
                              FUN_004763c0(piVar9);
                              local_4 = 0xffffffff;
                              FUN_00401c90(&param_1);
                              iVar4 = this_00[7];
                              iVar8 = iVar8 + 1;
                            } while (iVar8 < iVar4);
                          }
                          iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                        }
                        else {
                          pcVar12 = "print";
                          pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                          bVar2 = FUN_0040c360(pvVar6,pcVar12);
                          if (bVar2) {
                            iVar4 = this_00[7];
                            iVar8 = 0;
                            if (0 < iVar4) {
                              do {
                                FUN_00473d20(this_00,&param_1,iVar8,0);
                                local_10 = (int *)&stack0xffffffb8;
                                local_4 = 0x1a;
                                if (DAT_00501918 == '\0') {
                                  DAT_00501918 = '\x01';
                                  DAT_00501908 = 2;
                                  DAT_0050190c = 0;
                                  _DAT_00501910 = 1;
                                  DAT_00501914 = 0;
                                }
                                piVar9 = &DAT_00501908;
                                if ((param_1 != &DAT_00501908) &&
                                   (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                  *param_1 = *param_1 + 1;
                                }
                                FUN_004756f0(piVar9);
                                local_4 = 0xffffffff;
                                FUN_00401c90(&param_1);
                                iVar4 = this_00[7];
                                iVar8 = iVar8 + 1;
                              } while (iVar8 < iVar4);
                            }
                            iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                          }
                          else {
                            pcVar12 = "exec";
                            pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                            bVar2 = FUN_0040c360(pvVar6,pcVar12);
                            if (bVar2) {
                              iVar4 = this_00[7];
                              iVar8 = 0;
                              if (0 < iVar4) {
                                do {
                                  FUN_00473d20(this_00,&param_1,iVar8,0);
                                  local_10 = (int *)&stack0xffffffb8;
                                  local_4 = 0x1b;
                                  if (DAT_00501918 == '\0') {
                                    DAT_00501918 = '\x01';
                                    DAT_00501908 = 2;
                                    DAT_0050190c = 0;
                                    _DAT_00501910 = 1;
                                    DAT_00501914 = 0;
                                  }
                                  piVar9 = &DAT_00501908;
                                  if ((param_1 != &DAT_00501908) &&
                                     (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                    *param_1 = *param_1 + 1;
                                  }
                                  FUN_0047a070(piVar9);
                                  local_4 = 0xffffffff;
                                  FUN_00401c90(&param_1);
                                  iVar4 = this_00[7];
                                  iVar8 = iVar8 + 1;
                                } while (iVar8 < iVar4);
                              }
                              iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                            }
                            else {
                              pcVar12 = "execsetsonly";
                              pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                              bVar2 = FUN_0040c360(pvVar6,pcVar12);
                              if (bVar2) {
                                iVar4 = this_00[7];
                                iVar8 = 0;
                                if (0 < iVar4) {
                                  do {
                                    FUN_00473d20(this_00,&param_1,iVar8,0);
                                    local_10 = (int *)&stack0xffffffb8;
                                    local_4 = 0x1c;
                                    if (DAT_00501918 == '\0') {
                                      DAT_00501918 = '\x01';
                                      DAT_00501908 = 2;
                                      DAT_0050190c = 0;
                                      _DAT_00501910 = 1;
                                      DAT_00501914 = 0;
                                    }
                                    piVar9 = &DAT_00501908;
                                    if ((param_1 != &DAT_00501908) &&
                                       (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                      *param_1 = *param_1 + 1;
                                    }
                                    FUN_0047a130(piVar9);
                                    local_4 = 0xffffffff;
                                    FUN_00401c90(&param_1);
                                    iVar4 = this_00[7];
                                    iVar8 = iVar8 + 1;
                                  } while (iVar8 < iVar4);
                                }
                                iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                              }
                              else {
                                pcVar12 = "sendmsg";
                                pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                if (bVar2) {
                                  iVar4 = this_00[7];
                                  iVar8 = 0;
                                  if (0 < iVar4) {
                                    do {
                                      FUN_00473d20(this_00,&local_2c,iVar8,0);
                                      local_4 = 0x1d;
                                      FUN_00473d20(this_00,&param_1,iVar8,1);
                                      if (DAT_00501918 == '\0') {
                                        DAT_00501918 = '\x01';
                                        DAT_00501908 = 2;
                                        DAT_0050190c = 0;
                                        _DAT_00501910 = 1;
                                        DAT_00501914 = 0;
                                      }
                                      piVar9 = &DAT_00501908;
                                      if ((param_1 != &DAT_00501908) &&
                                         (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                        *param_1 = *param_1 + 1;
                                      }
                                      local_10 = (int *)&stack0xffffffb4;
                                      if (DAT_00501918 == '\0') {
                                        DAT_00501918 = '\x01';
                                        DAT_00501908 = 2;
                                        DAT_0050190c = 0;
                                        _DAT_00501910 = 1;
                                        DAT_00501914 = 0;
                                      }
                                      piVar13 = &DAT_00501908;
                                      if ((local_2c != &DAT_00501908) &&
                                         (piVar13 = local_2c, local_2c != &DAT_00501908)) {
                                        *local_2c = *local_2c + 1;
                                      }
                                      local_4._0_1_ = 0x1e;
                                      FUN_004753d0(piVar13,piVar9);
                                      local_4 = CONCAT31(local_4._1_3_,0x1d);
                                      FUN_00401c90(&param_1);
                                      local_4 = 0xffffffff;
                                      FUN_00401c90(&local_2c);
                                      iVar4 = this_00[7];
                                      iVar8 = iVar8 + 1;
                                    } while (iVar8 < iVar4);
                                  }
                                  iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                }
                                else {
                                  pcVar12 = "fogenable";
                                  pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                  if (bVar2) {
                                    iVar4 = this_00[7];
                                    iVar8 = 0;
                                    if (0 < iVar4) {
                                      do {
                                        FUN_00473d20(this_00,&param_1,iVar8,0);
                                        local_4 = 0x20;
                                        cVar1 = FUN_0040cd50(&param_1,&local_10);
                                        local_4 = 0xffffffff;
                                        if (cVar1 == '\0') {
                                          FUN_00401c90(&param_1);
                                        }
                                        else {
                                          FUN_00401c90(&param_1);
                                          FUN_00473d20(this_00,&local_2c,iVar8,1);
                                          local_4 = 0x21;
                                          cVar1 = FUN_0040cd50(&local_2c,&local_14);
                                          local_4 = 0xffffffff;
                                          if (cVar1 == '\0') {
                                            FUN_00401c90(&local_2c);
                                          }
                                          else {
                                            FUN_00401c90(&local_2c);
                                            FUN_00473d20(this_00,&local_30,iVar8,2);
                                            local_4 = 0x22;
                                            cVar1 = FUN_0040cd50(&local_30,&local_18);
                                            local_4 = 0xffffffff;
                                            if (cVar1 == '\0') {
                                              FUN_00401c90(&local_30);
                                            }
                                            else {
                                              FUN_00401c90(&local_30);
                                              FUN_00473d20(this_00,&local_28,iVar8,3);
                                              local_4 = 0x23;
                                              cVar1 = FUN_0040cd50(&local_28,&local_1c);
                                              local_4 = 0xffffffff;
                                              if (cVar1 == '\0') {
                                                FUN_00401c90(&local_28);
                                              }
                                              else {
                                                FUN_00401c90(&local_28);
                                                FUN_00473d20(this_00,&local_24,iVar8,4);
                                                local_4 = 0x24;
                                                cVar1 = FUN_0040cd50(&local_24,&local_20);
                                                local_4 = 0xffffffff;
                                                if (cVar1 == '\0') {
                                                  FUN_00401c90(&local_24);
                                                }
                                                else {
                                                  FUN_00401c90(&local_24);
                                                  FUN_004789d0((int)local_10,(int)local_14,
                                                               (int)local_18,(int)local_1c,local_20)
                                                  ;
                                                }
                                              }
                                            }
                                          }
                                        }
                                        iVar4 = this_00[7];
                                        iVar8 = iVar8 + 1;
                                      } while (iVar8 < iVar4);
                                    }
                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                  }
                                  else {
                                    pcVar12 = "fogdisable";
                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                    if (bVar2) {
                                      iVar4 = this_00[7];
                                      iVar8 = 0;
                                      if (0 < iVar4) {
                                        do {
                                          FUN_00478ac0();
                                          iVar4 = this_00[7];
                                          iVar8 = iVar8 + 1;
                                        } while (iVar8 < iVar4);
                                      }
                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                    }
                                    else {
                                      pcVar12 = "playsndat";
                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                      if (bVar2) {
                                        iVar4 = this_00[7];
                                        iVar8 = 0;
                                        if (0 < iVar4) {
                                          do {
                                            FUN_00473d20(this_00,&local_24,iVar8,0);
                                            local_4 = 0x25;
                                            FUN_00473d20(this_00,&param_1,iVar8,1);
                                            if (DAT_00501918 == '\0') {
                                              DAT_00501918 = '\x01';
                                              DAT_00501908 = 2;
                                              DAT_0050190c = 0;
                                              _DAT_00501910 = 1;
                                              DAT_00501914 = 0;
                                            }
                                            piVar9 = &DAT_00501908;
                                            if ((param_1 != &DAT_00501908) &&
                                               (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                              *param_1 = *param_1 + 1;
                                            }
                                            local_10 = (int *)&stack0xffffffb4;
                                            if (DAT_00501918 == '\0') {
                                              DAT_00501918 = '\x01';
                                              DAT_00501908 = 2;
                                              DAT_0050190c = 0;
                                              _DAT_00501910 = 1;
                                              DAT_00501914 = 0;
                                            }
                                            piVar13 = &DAT_00501908;
                                            if ((local_24 != &DAT_00501908) &&
                                               (piVar13 = local_24, local_24 != &DAT_00501908)) {
                                              *local_24 = *local_24 + 1;
                                            }
                                            local_4._0_1_ = 0x26;
                                            FUN_00477bd0(local_34,piVar13,piVar9);
                                            local_4 = CONCAT31(local_4._1_3_,0x25);
                                            FUN_00401c90(&param_1);
                                            local_4 = 0xffffffff;
                                            FUN_00401c90(&local_24);
                                            iVar4 = this_00[7];
                                            iVar8 = iVar8 + 1;
                                          } while (iVar8 < iVar4);
                                        }
                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                      }
                                      else {
                                        pcVar12 = "playsnd";
                                        pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                        bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                        if (bVar2) {
                                          iVar4 = this_00[7];
                                          iVar8 = 0;
                                          if (0 < iVar4) {
                                            do {
                                              FUN_00473d20(this_00,&param_1,iVar8,0);
                                              local_10 = (int *)&stack0xffffffb8;
                                              local_4 = 0x28;
                                              if (DAT_00501918 == '\0') {
                                                DAT_00501918 = '\x01';
                                                DAT_00501908 = 2;
                                                DAT_0050190c = 0;
                                                _DAT_00501910 = 1;
                                                DAT_00501914 = 0;
                                              }
                                              piVar9 = &DAT_00501908;
                                              if ((param_1 != &DAT_00501908) &&
                                                 (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                                *param_1 = *param_1 + 1;
                                              }
                                              FUN_00477af0(local_34,piVar9);
                                              local_4 = 0xffffffff;
                                              FUN_00401c90(&param_1);
                                              iVar4 = this_00[7];
                                              iVar8 = iVar8 + 1;
                                            } while (iVar8 < iVar4);
                                          }
                                          iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                        }
                                        else {
                                          pcVar12 = "stopsnd";
                                          pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                          bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                          if (bVar2) {
                                            iVar4 = this_00[7];
                                            iVar8 = 0;
                                            if (0 < iVar4) {
                                              do {
                                                FUN_00473d20(this_00,&param_1,iVar8,0);
                                                local_10 = (int *)&stack0xffffffb8;
                                                local_4 = 0x29;
                                                if (DAT_00501918 == '\0') {
                                                  DAT_00501918 = '\x01';
                                                  DAT_00501908 = 2;
                                                  DAT_0050190c = 0;
                                                  _DAT_00501910 = 1;
                                                  DAT_00501914 = 0;
                                                }
                                                piVar9 = &DAT_00501908;
                                                if ((param_1 != &DAT_00501908) &&
                                                   (piVar9 = param_1, param_1 != &DAT_00501908)) {
                                                  *param_1 = *param_1 + 1;
                                                }
                                                FUN_00477f10(local_34,piVar9);
                                                local_4 = 0xffffffff;
                                                FUN_00401c90(&param_1);
                                                iVar4 = this_00[7];
                                                iVar8 = iVar8 + 1;
                                              } while (iVar8 < iVar4);
                                            }
                                            iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                          }
                                          else {
                                            pcVar12 = "loopsndat";
                                            pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                            bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                            if (bVar2) {
                                              iVar4 = this_00[7];
                                              iVar8 = 0;
                                              if (0 < iVar4) {
                                                do {
                                                  FUN_00473d20(this_00,&local_24,iVar8,0);
                                                  local_4 = 0x2a;
                                                  FUN_00473d20(this_00,&param_1,iVar8,1);
                                                  local_10 = (int *)&stack0xffffffb8;
                                                  local_4._0_1_ = 0x2b;
                                                  if (DAT_00501918 == '\0') {
                                                    DAT_00501918 = '\x01';
                                                    DAT_00501908 = 2;
                                                    DAT_0050190c = 0;
                                                    _DAT_00501910 = 1;
                                                    DAT_00501914 = 0;
                                                  }
                                                  piVar13 = &DAT_00501908;
                                                  FUN_00401a40(&stack0xffffffb8,&param_1);
                                                  local_14 = (int *)&stack0xffffffb4;
                                                  local_4._0_1_ = 0x2c;
                                                  piVar9 = extraout_ECX;
                                                  FUN_00402460(&stack0xffffffb4,&local_24);
                                                  local_4._0_1_ = 0x2b;
                                                  FUN_00477de0(local_34,piVar9,piVar13);
                                                  local_4 = CONCAT31(local_4._1_3_,0x2a);
                                                  FUN_00401c90(&param_1);
                                                  local_4 = 0xffffffff;
                                                  FUN_00401c90(&local_24);
                                                  iVar4 = this_00[7];
                                                  iVar8 = iVar8 + 1;
                                                } while (iVar8 < iVar4);
                                              }
                                              iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                            }
                                            else {
                                              pcVar12 = "loopsnd";
                                              pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                              bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                              if (bVar2) {
                                                iVar4 = this_00[7];
                                                iVar8 = 0;
                                                if (0 < iVar4) {
                                                  do {
                                                    FUN_00473d20(this_00,&param_1,iVar8,0);
                                                    local_10 = (int *)&stack0xffffffb8;
                                                    local_4 = 0x2d;
                                                    piVar9 = extraout_ECX_00;
                                                    FUN_00402460(&stack0xffffffb8,&param_1);
                                                    FUN_00477d00(local_34,piVar9);
                                                    local_4 = 0xffffffff;
                                                    FUN_00401c90(&param_1);
                                                    iVar4 = this_00[7];
                                                    iVar8 = iVar8 + 1;
                                                  } while (iVar8 < iVar4);
                                                }
                                                iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                              }
                                              else {
                                                pcVar12 = "loopsndatloc";
                                                pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                if (bVar2) {
                                                  iVar4 = this_00[7];
                                                  iVar8 = 0;
                                                  if (0 < iVar4) {
                                                    do {
                                                      FUN_00473d20(this_00,&param_1,iVar8,0);
                                                      local_4 = 0x2e;
                                                      FUN_00473d20(this_00,&local_24,iVar8,1);
                                                      local_4._0_1_ = 0x2f;
                                                      cVar1 = FUN_0040cd50(&local_24,&local_18);
                                                      local_4._0_1_ = 0x2e;
                                                      if (cVar1 == '\0') {
                                                        FUN_00401c90(&local_24);
                                                      }
                                                      else {
                                                        FUN_00401c90(&local_24);
                                                        FUN_00473d20(this_00,&local_28,iVar8,2);
                                                        local_4._0_1_ = 0x30;
                                                        cVar1 = FUN_0040cd50(&local_28,&local_14);
                                                        local_4._0_1_ = 0x2e;
                                                        if (cVar1 == '\0') {
                                                          FUN_00401c90(&local_28);
                                                        }
                                                        else {
                                                          FUN_00401c90(&local_28);
                                                          FUN_00473d20(this_00,&local_2c,iVar8,3);
                                                          local_4._0_1_ = 0x31;
                                                          cVar1 = FUN_0040cd50(&local_2c,&local_10);
                                                          local_4 = CONCAT31(local_4._1_3_,0x2e);
                                                          if (cVar1 == '\0') {
                                                            FUN_00401c90(&local_2c);
                                                          }
                                                          else {
                                                            FUN_00401c90(&local_2c);
                                                            local_1c = &stack0xffffffac;
                                                            piVar9 = local_14;
                                                            puVar11 = local_18;
                                                            piVar13 = local_14;
                                                            piVar10 = local_10;
                                                            FUN_00402460(&stack0xffffffac,&param_1);
                                                            FUN_00475180(piVar9,(int)puVar11,
                                                                         (int)piVar13,(int)piVar10);
                                                          }
                                                        }
                                                      }
                                                      local_4 = 0xffffffff;
                                                      FUN_00401c90(&param_1);
                                                      iVar4 = this_00[7];
                                                      iVar8 = iVar8 + 1;
                                                    } while (iVar8 < iVar4);
                                                  }
                                                  iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                }
                                                else {
                                                  pcVar12 = "testloopsnd";
                                                  pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    iVar8 = 0;
                                                    if (0 < iVar4) {
                                                      do {
                                                        FUN_00473d20(this_00,&param_1,iVar8,0);
                                                        local_10 = (int *)&stack0xffffffb8;
                                                        local_4 = 0x32;
                                                        piVar9 = extraout_ECX_01;
                                                        FUN_00402460(&stack0xffffffb8,&param_1);
                                                        FUN_00479200(local_34,piVar9);
                                                        local_4 = 0xffffffff;
                                                        FUN_00401c90(&param_1);
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
                                                    }
                                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                  }
                                                  else {
                                                    pcVar12 = "testambientsnd";
                                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                    if (bVar2) {
                                                      iVar4 = this_00[7];
                                                      iVar8 = 0;
                                                      if (0 < iVar4) {
                                                        do {
                                                          FUN_00473d20(this_00,&param_1,iVar8,0);
                                                          local_10 = (int *)&stack0xffffffb8;
                                                          local_4 = 0x33;
                                                          piVar9 = extraout_ECX_02;
                                                          FUN_00402460(&stack0xffffffb8,&param_1);
                                                          FUN_00479140(local_34,piVar9);
                                                          local_4 = 0xffffffff;
                                                          FUN_00401c90(&param_1);
                                                          iVar4 = this_00[7];
                                                          iVar8 = iVar8 + 1;
                                                        } while (iVar8 < iVar4);
                                                      }
                                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                    }
                                                    else {
                                                      pcVar12 = "ambientsnd";
                                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1
                                                                                   );
                                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                      if (bVar2) {
                                                        iVar4 = this_00[7];
                                                        iVar8 = 0;
                                                        if (0 < iVar4) {
                                                          do {
                                                            FUN_00473d20(this_00,&param_1,iVar8,0);
                                                            local_4 = 0x34;
                                                            FUN_00473d20(this_00,&local_24,iVar8,1);
                                                            local_4._0_1_ = 0x35;
                                                            cVar1 = FUN_0040cd50(&local_24,&local_18
                                                                                );
                                                            local_4._0_1_ = 0x34;
                                                            if (cVar1 == '\0') {
                                                              FUN_00401c90(&local_24);
                                                            }
                                                            else {
                                                              FUN_00401c90(&local_24);
                                                              FUN_00473d20(this_00,&local_28,iVar8,2
                                                                          );
                                                              local_4._0_1_ = 0x36;
                                                              cVar1 = FUN_0040cd50(&local_28,
                                                                                   &local_14);
                                                              local_4._0_1_ = 0x34;
                                                              if (cVar1 == '\0') {
                                                                FUN_00401c90(&local_28);
                                                              }
                                                              else {
                                                                FUN_00401c90(&local_28);
                                                                FUN_00473d20(this_00,&local_2c,iVar8
                                                                             ,3);
                                                                local_4._0_1_ = 0x37;
                                                                cVar1 = FUN_0040cd50(&local_2c,
                                                                                     &local_10);
                                                                local_4 = CONCAT31(local_4._1_3_,
                                                                                   0x34);
                                                                if (cVar1 == '\0') {
                                                                  FUN_00401c90(&local_2c);
                                                                }
                                                                else {
                                                                  FUN_00401c90(&local_2c);
                                                                  local_1c = &stack0xffffffac;
                                                                  piVar9 = local_14;
                                                                  puVar11 = local_18;
                                                                  piVar13 = local_14;
                                                                  piVar10 = local_10;
                                                                  FUN_00402460(&stack0xffffffac,
                                                                               &param_1);
                                                                  FUN_00475210(piVar9,(int)puVar11,
                                                                               (int)piVar13,
                                                                               (int)piVar10);
                                                                }
                                                              }
                                                            }
                                                            local_4 = 0xffffffff;
                                                            FUN_00401c90(&param_1);
                                                            iVar4 = this_00[7];
                                                            iVar8 = iVar8 + 1;
                                                          } while (iVar8 < iVar4);
                                                        }
                                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1)
                                                        ;
                                                      }
                                                      else {
                                                        pcVar12 = "hint";
                                                        pvVar6 = (void *)FUN_0040c330(this_00,&
                                                  param_1);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    iVar8 = 0;
                                                    if (0 < iVar4) {
                                                      do {
                                                        FUN_00473d20(this_00,&param_1,iVar8,0);
                                                        uVar5 = FUN_0040cc50(&param_1,&local_10);
                                                        ppiVar7 = &param_1;
                                                        local_4 = 0xffffffff;
                                                        if ((char)uVar5 != '\0') {
                                                          FUN_00401c90(ppiVar7);
                                                          FUN_00473d20(this_00,&local_24,iVar8,1);
                                                          local_14 = (int *)&stack0xffffffb8;
                                                          local_4 = 0x39;
                                                          piVar9 = extraout_ECX_03;
                                                          FUN_00402460(&stack0xffffffb8,&local_24);
                                                          FUN_00477fa0(local_10,piVar9);
                                                          ppiVar7 = &local_24;
                                                        }
                                                        local_4 = 0xffffffff;
                                                        FUN_00401c90(ppiVar7);
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
                                                    }
                                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                  }
                                                  else {
                                                    pcVar12 = "quake";
                                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                    if (bVar2) {
                                                      iVar4 = this_00[7];
                                                      iVar8 = 0;
                                                      if (0 < iVar4) {
                                                        do {
                                                          FUN_00473d20(this_00,&param_1,iVar8,0);
                                                          uVar5 = FUN_0040cc50(&param_1,&local_14);
                                                          local_4 = 0xffffffff;
                                                          if ((char)uVar5 == '\0') {
                                                            FUN_00401c90(&param_1);
                                                          }
                                                          else {
                                                            FUN_00401c90(&param_1);
                                                            FUN_00473d20(this_00,&local_24,iVar8,1);
                                                            uVar5 = FUN_0040cc50(&local_24,&local_10
                                                                                );
                                                            local_4 = 0xffffffff;
                                                            if ((char)uVar5 == '\0') {
                                                              FUN_00401c90(&local_24);
                                                            }
                                                            else {
                                                              FUN_00401c90(&local_24);
                                                              FUN_00478900(local_14,local_10);
                                                            }
                                                          }
                                                          iVar4 = this_00[7];
                                                          iVar8 = iVar8 + 1;
                                                        } while (iVar8 < iVar4);
                                                      }
                                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                    }
                                                    else {
                                                      pcVar12 = "if<=";
                                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1
                                                                                   );
                                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                      if (bVar2) {
                                                        iVar4 = this_00[7];
                                                        iVar8 = 0;
                                                        if (0 < iVar4) {
                                                          do {
                                                            FUN_00473d20(this_00,&local_24,iVar8,0);
                                                            local_4 = 0x3c;
                                                            FUN_00473d20(this_00,&param_1,iVar8,1);
                                                            local_4._0_1_ = 0x3d;
                                                            cVar1 = FUN_0040cd50(&param_1,&local_10)
                                                            ;
                                                            ppiVar7 = &param_1;
                                                            local_4._0_1_ = 0x3c;
                                                            if (cVar1 != '\0') {
                                                              FUN_00401c90(ppiVar7);
                                                              FUN_00473d20(this_00,&local_28,iVar8,2
                                                                          );
                                                              local_14 = (int *)&stack0xffffffb8;
                                                              local_4._0_1_ = 0x3e;
                                                              piVar10 = extraout_ECX_04;
                                                              FUN_00402460(&stack0xffffffb8,
                                                                           &local_28);
                                                              local_18 = &stack0xffffffb0;
                                                              local_4._0_1_ = 0x3f;
                                                              piVar9 = extraout_ECX_05;
                                                              piVar13 = local_10;
                                                              FUN_00402460(&stack0xffffffb0,
                                                                           &local_24);
                                                              local_4._0_1_ = 0x3e;
                                                              FUN_00476670(local_34,piVar9,
                                                                           (undefined1 *)piVar13,
                                                                           piVar10);
                                                              ppiVar7 = &local_28;
                                                            }
                                                            local_4._0_1_ = 0x3c;
                                                            FUN_00401c90(ppiVar7);
                                                            local_4 = 0xffffffff;
                                                            FUN_00401c90(&local_24);
                                                            iVar4 = this_00[7];
                                                            iVar8 = iVar8 + 1;
                                                          } while (iVar8 < iVar4);
                                                        }
                                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1)
                                                        ;
                                                      }
                                                      else {
                                                        pcVar12 = "if<";
                                                        pvVar6 = (void *)FUN_0040c330(this_00,&
                                                  param_1);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    iVar8 = 0;
                                                    if (0 < iVar4) {
                                                      do {
                                                        FUN_00473d20(this_00,&local_24,iVar8,0);
                                                        local_4 = 0x40;
                                                        FUN_00473d20(this_00,&param_1,iVar8,1);
                                                        local_4._0_1_ = 0x41;
                                                        cVar1 = FUN_0040cd50(&param_1,&local_10);
                                                        ppiVar7 = &param_1;
                                                        local_4._0_1_ = 0x40;
                                                        if (cVar1 != '\0') {
                                                          FUN_00401c90(ppiVar7);
                                                          FUN_00473d20(this_00,&local_28,iVar8,2);
                                                          local_14 = (int *)&stack0xffffffb8;
                                                          local_4._0_1_ = 0x42;
                                                          piVar10 = extraout_ECX_06;
                                                          FUN_00402460(&stack0xffffffb8,&local_28);
                                                          local_18 = &stack0xffffffb0;
                                                          local_4._0_1_ = 0x43;
                                                          piVar9 = extraout_ECX_07;
                                                          piVar13 = local_10;
                                                          FUN_00402460(&stack0xffffffb0,&local_24);
                                                          local_4._0_1_ = 0x42;
                                                          FUN_004764e0(local_34,piVar9,
                                                                       (undefined1 *)piVar13,piVar10
                                                                      );
                                                          ppiVar7 = &local_28;
                                                        }
                                                        local_4._0_1_ = 0x40;
                                                        FUN_00401c90(ppiVar7);
                                                        local_4 = 0xffffffff;
                                                        FUN_00401c90(&local_24);
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
                                                    }
                                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                  }
                                                  else {
                                                    pcVar12 = "if>=";
                                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                    if (bVar2) {
                                                      iVar4 = this_00[7];
                                                      iVar8 = 0;
                                                      if (0 < iVar4) {
                                                        do {
                                                          FUN_00473d20(this_00,&local_24,iVar8,0);
                                                          local_4 = 0x44;
                                                          FUN_00473d20(this_00,&param_1,iVar8,1);
                                                          local_4._0_1_ = 0x45;
                                                          cVar1 = FUN_0040cd50(&param_1,&local_10);
                                                          ppiVar7 = &param_1;
                                                          local_4._0_1_ = 0x44;
                                                          if (cVar1 != '\0') {
                                                            FUN_00401c90(ppiVar7);
                                                            FUN_00473d20(this_00,&local_28,iVar8,2);
                                                            local_14 = (int *)&stack0xffffffb8;
                                                            local_4._0_1_ = 0x46;
                                                            piVar10 = extraout_ECX_08;
                                                            FUN_00402460(&stack0xffffffb8,&local_28)
                                                            ;
                                                            local_18 = &stack0xffffffb0;
                                                            local_4._0_1_ = 0x47;
                                                            piVar9 = extraout_ECX_09;
                                                            piVar13 = local_10;
                                                            FUN_00402460(&stack0xffffffb0,&local_24)
                                                            ;
                                                            local_4._0_1_ = 0x46;
                                                            FUN_00476990(local_34,piVar9,
                                                                         (undefined1 *)piVar13,
                                                                         piVar10);
                                                            ppiVar7 = &local_28;
                                                          }
                                                          local_4._0_1_ = 0x44;
                                                          FUN_00401c90(ppiVar7);
                                                          local_4 = 0xffffffff;
                                                          FUN_00401c90(&local_24);
                                                          iVar4 = this_00[7];
                                                          iVar8 = iVar8 + 1;
                                                        } while (iVar8 < iVar4);
                                                      }
                                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                    }
                                                    else {
                                                      pcVar12 = "if>";
                                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1
                                                                                   );
                                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                      if (bVar2) {
                                                        iVar4 = this_00[7];
                                                        iVar8 = 0;
                                                        if (0 < iVar4) {
                                                          do {
                                                            FUN_00473d20(this_00,&local_24,iVar8,0);
                                                            local_4 = 0x48;
                                                            FUN_00473d20(this_00,&param_1,iVar8,1);
                                                            local_4._0_1_ = 0x49;
                                                            cVar1 = FUN_0040cd50(&param_1,&local_10)
                                                            ;
                                                            ppiVar7 = &param_1;
                                                            local_4._0_1_ = 0x48;
                                                            if (cVar1 != '\0') {
                                                              FUN_00401c90(ppiVar7);
                                                              FUN_00473d20(this_00,&local_28,iVar8,2
                                                                          );
                                                              local_14 = (int *)&stack0xffffffb8;
                                                              local_4._0_1_ = 0x4a;
                                                              piVar10 = extraout_ECX_10;
                                                              FUN_00402460(&stack0xffffffb8,
                                                                           &local_28);
                                                              local_18 = &stack0xffffffb0;
                                                              local_4._0_1_ = 0x4b;
                                                              piVar9 = extraout_ECX_11;
                                                              piVar13 = local_10;
                                                              FUN_00402460(&stack0xffffffb0,
                                                                           &local_24);
                                                              local_4._0_1_ = 0x4a;
                                                              FUN_00476800(local_34,piVar9,
                                                                           (undefined1 *)piVar13,
                                                                           piVar10);
                                                              ppiVar7 = &local_28;
                                                            }
                                                            local_4._0_1_ = 0x48;
                                                            FUN_00401c90(ppiVar7);
                                                            local_4 = 0xffffffff;
                                                            FUN_00401c90(&local_24);
                                                            iVar4 = this_00[7];
                                                            iVar8 = iVar8 + 1;
                                                          } while (iVar8 < iVar4);
                                                        }
                                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1)
                                                        ;
                                                      }
                                                      else {
                                                        pcVar12 = "iffacing";
                                                        pvVar6 = (void *)FUN_0040c330(this_00,&
                                                  param_1);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    iVar8 = 0;
                                                    if (0 < iVar4) {
                                                      do {
                                                        FUN_00473d20(this_00,&local_28,iVar8,0);
                                                        local_4 = 0x4c;
                                                        FUN_00473d20(this_00,&local_24,iVar8,1);
                                                        local_4._0_1_ = 0x4d;
                                                        FUN_00473d20(this_00,&param_1,iVar8,2);
                                                        local_4._0_1_ = 0x4e;
                                                        cVar1 = FUN_0040cd50(&param_1,&local_10);
                                                        ppiVar7 = &param_1;
                                                        local_4._0_1_ = 0x4d;
                                                        if (cVar1 != '\0') {
                                                          FUN_00401c90(ppiVar7);
                                                          FUN_00473d20(this_00,&local_2c,iVar8,3);
                                                          local_14 = (int *)&stack0xffffffb8;
                                                          local_4._0_1_ = 0x4f;
                                                          piVar14 = extraout_ECX_12;
                                                          FUN_00402460(&stack0xffffffb8,&local_2c);
                                                          local_18 = &stack0xffffffb0;
                                                          local_4._0_1_ = 0x50;
                                                          piVar13 = local_10;
                                                          piVar10 = local_10;
                                                          FUN_00402460(&stack0xffffffb0,&local_24);
                                                          local_1c = &stack0xffffffac;
                                                          local_4._0_1_ = 0x51;
                                                          piVar9 = extraout_ECX_13;
                                                          FUN_00402460(&stack0xffffffac,&local_28);
                                                          local_4._0_1_ = 0x4f;
                                                          FUN_00476e60(piVar9,piVar13,
                                                                       (undefined1 *)piVar10,piVar14
                                                                      );
                                                          ppiVar7 = &local_2c;
                                                        }
                                                        local_4._0_1_ = 0x4d;
                                                        FUN_00401c90(ppiVar7);
                                                        local_4 = CONCAT31(local_4._1_3_,0x4c);
                                                        FUN_00401c90(&local_24);
                                                        local_4 = 0xffffffff;
                                                        FUN_00401c90(&local_28);
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
                                                    }
                                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                  }
                                                  else {
                                                    pcVar12 = "ifnotfacing";
                                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                    if (bVar2) {
                                                      iVar4 = this_00[7];
                                                      iVar8 = 0;
                                                      if (0 < iVar4) {
                                                        do {
                                                          FUN_00473d20(this_00,&local_28,iVar8,0);
                                                          local_4 = 0x52;
                                                          FUN_00473d20(this_00,&local_24,iVar8,1);
                                                          local_4._0_1_ = 0x53;
                                                          FUN_00473d20(this_00,&param_1,iVar8,2);
                                                          local_4._0_1_ = 0x54;
                                                          cVar1 = FUN_0040cd50(&param_1,&local_10);
                                                          ppiVar7 = &param_1;
                                                          local_4._0_1_ = 0x53;
                                                          if (cVar1 != '\0') {
                                                            FUN_00401c90(ppiVar7);
                                                            FUN_00473d20(this_00,&local_2c,iVar8,3);
                                                            local_14 = (int *)&stack0xffffffb8;
                                                            local_4._0_1_ = 0x55;
                                                            piVar14 = extraout_ECX_14;
                                                            FUN_00402460(&stack0xffffffb8,&local_2c)
                                                            ;
                                                            local_18 = &stack0xffffffb0;
                                                            local_4._0_1_ = 0x56;
                                                            piVar13 = extraout_ECX_15;
                                                            piVar10 = local_10;
                                                            FUN_00402460(&stack0xffffffb0,&local_24)
                                                            ;
                                                            local_1c = &stack0xffffffac;
                                                            local_4._0_1_ = 0x57;
                                                            piVar9 = extraout_ECX_16;
                                                            FUN_00402460(&stack0xffffffac,&local_28)
                                                            ;
                                                            local_4._0_1_ = 0x55;
                                                            FUN_00477090(piVar9,piVar13,
                                                                         (undefined1 *)piVar10,
                                                                         piVar14);
                                                            ppiVar7 = &local_2c;
                                                          }
                                                          local_4._0_1_ = 0x53;
                                                          FUN_00401c90(ppiVar7);
                                                          local_4 = CONCAT31(local_4._1_3_,0x52);
                                                          FUN_00401c90(&local_24);
                                                          local_4 = 0xffffffff;
                                                          FUN_00401c90(&local_28);
                                                          iVar4 = this_00[7];
                                                          iVar8 = iVar8 + 1;
                                                        } while (iVar8 < iVar4);
                                                      }
                                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                    }
                                                    else {
                                                      pcVar12 = "ifclear";
                                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1
                                                                                   );
                                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                      if (bVar2) {
                                                        iVar4 = this_00[7];
                                                        if (0 < iVar4) {
                                                          iVar8 = 0;
                                                          do {
                                                            FUN_00473d20(this_00,&local_24,iVar8,0);
                                                            local_4 = 0x58;
                                                            FUN_00473d20(this_00,&param_1,iVar8,1);
                                                            local_10 = (int *)&stack0xffffffb8;
                                                            local_4._0_1_ = 0x59;
                                                            piVar13 = extraout_ECX_17;
                                                            FUN_00402460(&stack0xffffffb8,&param_1);
                                                            local_14 = (int *)&stack0xffffffb4;
                                                            local_4._0_1_ = 0x5a;
                                                            piVar9 = extraout_ECX_18;
                                                            FUN_00402460(&stack0xffffffb4,&local_24)
                                                            ;
                                                            local_4._0_1_ = 0x59;
                                                            FUN_004772c0(local_34,piVar9,piVar13);
                                                            local_4 = CONCAT31(local_4._1_3_,0x58);
                                                            FUN_00401c90(&param_1);
                                                            local_4 = 0xffffffff;
                                                            FUN_00401c90(&local_24);
                                                            iVar4 = this_00[7];
                                                            iVar8 = iVar8 + 1;
                                                          } while (iVar8 < iVar4);
                                                        }
                                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1)
                                                        ;
                                                      }
                                                      else {
                                                        pcVar12 = "ifnotclear";
                                                        pvVar6 = (void *)FUN_0040c330(this_00,&
                                                  param_1);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    if (0 < iVar4) {
                                                      iVar8 = 0;
                                                      do {
                                                        FUN_00473d20(this_00,&local_24,iVar8,0);
                                                        local_4 = 0x5b;
                                                        FUN_00473d20(this_00,&param_1,iVar8,1);
                                                        local_10 = (int *)&stack0xffffffb8;
                                                        local_4._0_1_ = 0x5c;
                                                        piVar13 = extraout_ECX_19;
                                                        FUN_00402460(&stack0xffffffb8,&param_1);
                                                        local_14 = (int *)&stack0xffffffb4;
                                                        local_4._0_1_ = 0x5d;
                                                        piVar9 = extraout_ECX_20;
                                                        FUN_00402460(&stack0xffffffb4,&local_24);
                                                        local_4._0_1_ = 0x5c;
                                                        FUN_00477500(local_34,piVar9,piVar13);
                                                        local_4 = CONCAT31(local_4._1_3_,0x5b);
                                                        FUN_00401c90(&param_1);
                                                        local_4 = 0xffffffff;
                                                        FUN_00401c90(&local_24);
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
                                                    }
                                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                  }
                                                  else {
                                                    pcVar12 = "listmaps";
                                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                    if (bVar2) {
                                                      iVar4 = this_00[7];
                                                      iVar8 = 0;
                                                      if (0 < iVar4) {
                                                        do {
                                                          FUN_00479670();
                                                          iVar4 = this_00[7];
                                                          iVar8 = iVar8 + 1;
                                                        } while (iVar8 < iVar4);
                                                      }
                                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                    }
                                                    else {
                                                      pcVar12 = "loadmap";
                                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1
                                                                                   );
                                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                      if (bVar2) {
                                                        iVar4 = this_00[7];
                                                        iVar8 = 0;
                                                        if (0 < iVar4) {
                                                          do {
                                                            FUN_00473d20(this_00,&param_1,iVar8,0);
                                                            local_10 = (int *)&stack0xffffffb8;
                                                            local_4 = 0x5e;
                                                            iVar4 = extraout_ECX_21;
                                                            FUN_00402460(&stack0xffffffb8,&param_1);
                                                            FUN_00478150(iVar4,unaff_EDI);
                                                            local_4 = 0xffffffff;
                                                            FUN_00401c90(&param_1);
                                                            iVar4 = this_00[7];
                                                            iVar8 = iVar8 + 1;
                                                          } while (iVar8 < iVar4);
                                                        }
                                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1)
                                                        ;
                                                      }
                                                      else {
                                                        pcVar12 = "loadhosemap";
                                                        pvVar6 = (void *)FUN_0040c330(this_00,&
                                                  param_1);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    iVar8 = 0;
                                                    if (0 < iVar4) {
                                                      do {
                                                        FUN_00473d20(this_00,&param_1,iVar8,0);
                                                        local_10 = (int *)&stack0xffffffb8;
                                                        local_4 = 0x5f;
                                                        iVar4 = extraout_ECX_22;
                                                        FUN_00402460(&stack0xffffffb8,&param_1);
                                                        FUN_004783d0(iVar4,unaff_EDI);
                                                        local_4 = 0xffffffff;
                                                        FUN_00401c90(&param_1);
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
                                                    }
                                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                  }
                                                  else {
                                                    pcVar12 = "setlighttable";
                                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                    if (bVar2) {
                                                      iVar4 = this_00[7];
                                                      if (0 < iVar4) {
                                                        iVar8 = 0;
                                                        do {
                                                          FUN_00473d20(this_00,&param_1,iVar8,0);
                                                          local_4 = 0x60;
                                                          cVar1 = FUN_0040cd50(&param_1,&local_10);
                                                          ppiVar7 = &param_1;
                                                          local_4 = 0xffffffff;
                                                          if (cVar1 != '\0') {
                                                            FUN_00401c90(ppiVar7);
                                                            FUN_00473d20(this_00,&local_24,iVar8,1);
                                                            local_14 = (int *)&stack0xffffffb8;
                                                            local_4 = 0x61;
                                                            piVar9 = extraout_ECX_23;
                                                            FUN_00402460(&stack0xffffffb8,&local_24)
                                                            ;
                                                            FUN_004786b0((int)local_10,piVar9);
                                                            ppiVar7 = &local_24;
                                                          }
                                                          local_4 = 0xffffffff;
                                                          FUN_00401c90(ppiVar7);
                                                          iVar4 = this_00[7];
                                                          iVar8 = iVar8 + 1;
                                                        } while (iVar8 < iVar4);
                                                      }
                                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                    }
                                                    else {
                                                      pcVar12 = "difficulty";
                                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1
                                                                                   );
                                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                      if (bVar2) {
                                                        iVar4 = this_00[7];
                                                        iVar8 = 0;
                                                        if (0 < iVar4) {
                                                          do {
                                                            FUN_00473d20(this_00,&param_1,iVar8,0);
                                                            local_10 = (int *)&stack0xffffffb8;
                                                            local_4 = 0x62;
                                                            piVar9 = extraout_ECX_24;
                                                            FUN_00402460(&stack0xffffffb8,&param_1);
                                                            FUN_004792c0(piVar9);
                                                            local_4 = 0xffffffff;
                                                            FUN_00401c90(&param_1);
                                                            iVar4 = this_00[7];
                                                            iVar8 = iVar8 + 1;
                                                          } while (iVar8 < iVar4);
                                                        }
                                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1)
                                                        ;
                                                      }
                                                      else {
                                                        pcVar12 = "quit";
                                                        pvVar6 = (void *)FUN_0040c330(this_00,&
                                                  param_1);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    iVar8 = 0;
                                                    if (0 < iVar4) {
                                                      do {
                                                        FUN_00474ed0();
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
                                                    }
                                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                  }
                                                  else {
                                                    pcVar12 = "debugvictims";
                                                    pvVar6 = (void *)FUN_0040c330(this_00,&param_1);
                                                    bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                    if (bVar2) {
                                                      iVar4 = this_00[7];
                                                      iVar8 = 0;
                                                      if (0 < iVar4) {
                                                        do {
                                                          FUN_00474f60();
                                                          iVar4 = this_00[7];
                                                          iVar8 = iVar8 + 1;
                                                        } while (iVar8 < iVar4);
                                                      }
                                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                                    }
                                                    else {
                                                      pcVar12 = "runrescuebot";
                                                      pvVar6 = (void *)FUN_0040c330(this_00,&param_1
                                                                                   );
                                                      bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                      if (bVar2) {
                                                        iVar4 = this_00[7];
                                                        iVar8 = 0;
                                                        if (0 < iVar4) {
                                                          do {
                                                            FUN_00475020();
                                                            iVar4 = this_00[7];
                                                            iVar8 = iVar8 + 1;
                                                          } while (iVar8 < iVar4);
                                                        }
                                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1)
                                                        ;
                                                      }
                                                      else {
                                                        pcVar12 = "listvars";
                                                        pvVar6 = (void *)FUN_0040c330(this_00,&
                                                  local_34);
                                                  bVar2 = FUN_0040c360(pvVar6,pcVar12);
                                                  if (bVar2) {
                                                    iVar4 = this_00[7];
                                                    iVar8 = 0;
                                                    if (0 < iVar4) {
                                                      do {
                                                        FUN_00478b80();
                                                        iVar4 = this_00[7];
                                                        iVar8 = iVar8 + 1;
                                                      } while (iVar8 < iVar4);
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
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else {
            iVar4 = 0;
            if (0 < this_00[7]) {
              do {
                *(undefined1 *)((int)this + 0x74) = 1;
                iVar4 = iVar4 + 1;
              } while (iVar4 < this_00[7]);
            }
            iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
          }
        }
        else {
          iVar4 = this_00[7];
          iVar8 = 0;
          if (0 < iVar4) {
            do {
              FUN_00473d20(this_00,&local_30,iVar8,0);
              local_4 = 4;
              FUN_00473d20(this_00,&param_1,iVar8,1);
              if (DAT_00501918 == '\0') {
                DAT_00501918 = '\x01';
                DAT_00501908 = 2;
                DAT_0050190c = 0;
                _DAT_00501910 = 1;
                DAT_00501914 = 0;
              }
              piVar9 = &DAT_00501908;
              if ((param_1 != &DAT_00501908) && (piVar9 = param_1, param_1 != &DAT_00501908)) {
                *param_1 = *param_1 + 1;
              }
              local_10 = (int *)&stack0xffffffb4;
              if (DAT_00501918 == '\0') {
                DAT_00501918 = '\x01';
                DAT_00501908 = 2;
                DAT_0050190c = 0;
                _DAT_00501910 = 1;
                DAT_00501914 = 0;
              }
              piVar13 = &DAT_00501908;
              if ((local_30 != &DAT_00501908) && (piVar13 = local_30, local_30 != &DAT_00501908)) {
                *local_30 = *local_30 + 1;
              }
              local_4._0_1_ = 5;
              FUN_00475560(local_34,piVar13,piVar9);
              local_4 = CONCAT31(local_4._1_3_,4);
              if (param_1 != &DAT_00501908) {
                *param_1 = *param_1 + -1;
                if (*param_1 < 1) {
                  FUN_004011c0(&param_1);
                }
                else {
                  param_1 = &DAT_00501908;
                }
              }
              local_4 = 0xffffffff;
              if (local_30 != &DAT_00501908) {
                *local_30 = *local_30 + -1;
                if (*local_30 < 1) {
                  FUN_004011c0(&local_30);
                }
                else {
                  local_30 = &DAT_00501908;
                }
              }
              iVar4 = this_00[7];
              iVar8 = iVar8 + 1;
            } while (iVar8 < iVar4);
          }
          iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
        }
      }
      else {
        iVar4 = this_00[7];
        iVar8 = 0;
        if (0 < iVar4) {
          do {
            FUN_00473d20(this_00,&local_30,iVar8,0);
            local_4 = 1;
            FUN_00473d20(this_00,&param_1,iVar8,1);
            if (DAT_00501918 == '\0') {
              DAT_00501918 = '\x01';
              DAT_00501908 = 2;
              DAT_0050190c = 0;
              _DAT_00501910 = 1;
              DAT_00501914 = 0;
            }
            piVar9 = &DAT_00501908;
            if ((param_1 != &DAT_00501908) && (piVar9 = param_1, param_1 != &DAT_00501908)) {
              *param_1 = *param_1 + 1;
            }
            local_10 = (int *)&stack0xffffffb4;
            if (DAT_00501918 == '\0') {
              DAT_00501918 = '\x01';
              DAT_00501908 = 2;
              DAT_0050190c = 0;
              _DAT_00501910 = 1;
              DAT_00501914 = 0;
            }
            piVar13 = &DAT_00501908;
            if ((local_30 != &DAT_00501908) && (piVar13 = local_30, local_30 != &DAT_00501908)) {
              *local_30 = *local_30 + 1;
            }
            local_4._0_1_ = 2;
            FUN_00475560(local_34,piVar13,piVar9);
            local_4 = CONCAT31(local_4._1_3_,1);
            if (param_1 != &DAT_00501908) {
              *param_1 = *param_1 + -1;
              if (*param_1 < 1) {
                FUN_004011c0(&param_1);
              }
              else {
                param_1 = &DAT_00501908;
              }
            }
            local_4 = 0xffffffff;
            if (local_30 != &DAT_00501908) {
              *local_30 = *local_30 + -1;
              if (*local_30 < 1) {
                FUN_004011c0(&local_30);
              }
              else {
                local_30 = &DAT_00501908;
              }
            }
            iVar4 = this_00[7];
            iVar8 = iVar8 + 1;
          } while (iVar8 < iVar4);
        }
        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
      }
    }
    else {
      iVar4 = this_00[7];
      iVar8 = 0;
      if (0 < iVar4) {
        do {
          FUN_00473d20(this_00,&param_1,iVar8,0);
          local_10 = (int *)&stack0xffffffb8;
          local_4 = 0;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar9 = &DAT_00501908;
          if ((param_1 != &DAT_00501908) && (piVar9 = param_1, param_1 != &DAT_00501908)) {
            *param_1 = *param_1 + 1;
          }
          FUN_004752a0(piVar9);
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
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar4);
      }
      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
    }
  }
  ExceptionList = local_c;
  return iVar4;
}

