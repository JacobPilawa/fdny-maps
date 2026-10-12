
/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __thiscall FUN_00451870(void *this,int *param_1)

{
  int *this_00;
  char cVar1;
  bool bVar2;
  undefined4 *puVar3;
  int iVar4;
  void *pvVar5;
  undefined4 uVar6;
  uint3 extraout_var;
  int **ppiVar7;
  void *this_01;
  int *unaff_EDI;
  int iVar8;
  int *piVar9;
  int *piVar10;
  char *pcVar11;
  int *local_28;
  void *local_24;
  int *local_20;
  int local_1c;
  int local_18;
  float local_14;
  undefined1 *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  this_00 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d03a8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  local_24 = this;
  puVar3 = (undefined4 *)FUN_0040c330(param_1,&param_1);
  iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"jump");
  cVar1 = '\x01' - (iVar4 != 0);
  param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
  if (cVar1 == '\0') {
    puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"crouch");
    cVar1 = '\x01' - (iVar4 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 == '\0') {
      puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
      iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"uncrouch");
      cVar1 = '\x01' - (iVar4 != 0);
      param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
      if (cVar1 == '\0') {
        puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
        iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"push");
        cVar1 = '\x01' - (iVar4 != 0);
        param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
        if (cVar1 == '\0') {
          puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
          iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"climb");
          cVar1 = '\x01' - (iVar4 != 0);
          param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
          if (cVar1 == '\0') {
            pcVar11 = "unclimb";
            pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
            bVar2 = FUN_0040c360(pvVar5,pcVar11);
            if (bVar2) {
              iVar4 = 0;
              if (0 < this_00[7]) {
                do {
                  if (*(int *)((int)this + 0x4c) == 5) {
                    *(undefined4 *)((int)this + 0x4c) = 3;
                  }
                  iVar4 = iVar4 + 1;
                } while (iVar4 < this_00[7]);
              }
              iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
            }
            else {
              pcVar11 = "escape";
              pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
              bVar2 = FUN_0040c360(pvVar5,pcVar11);
              if (bVar2) {
                iVar4 = this_00[7];
                iVar8 = 0;
                if (0 < iVar4) {
                  do {
                    FUN_004513a0((int *)((int)this + -0x140));
                    iVar4 = this_00[7];
                    iVar8 = iVar8 + 1;
                  } while (iVar8 < iVar4);
                }
                iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
              }
              else {
                pcVar11 = "oxyon";
                pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                bVar2 = FUN_0040c360(pvVar5,pcVar11);
                if (bVar2) {
                  iVar4 = this_00[7];
                  iVar8 = 0;
                  if (0 < iVar4) {
                    do {
                      FUN_0044bd80((int)this + -0x140);
                      iVar4 = this_00[7];
                      iVar8 = iVar8 + 1;
                    } while (iVar8 < iVar4);
                  }
                  iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                }
                else {
                  pcVar11 = "oxyoff";
                  pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                  bVar2 = FUN_0040c360(pvVar5,pcVar11);
                  if (bVar2) {
                    iVar4 = this_00[7];
                    iVar8 = 0;
                    if (0 < iVar4) {
                      do {
                        FUN_0044bdb0((int)this + -0x140);
                        iVar4 = this_00[7];
                        iVar8 = iVar8 + 1;
                      } while (iVar8 < iVar4);
                    }
                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                  }
                  else {
                    pcVar11 = "hint";
                    pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                    bVar2 = FUN_0040c360(pvVar5,pcVar11);
                    if (bVar2) {
                      iVar4 = this_00[7];
                      iVar8 = 0;
                      if (0 < iVar4) {
                        do {
                          FUN_00473d20(this_00,&param_1,iVar8,0);
                          uVar6 = FUN_0040cc50(&param_1,&local_14);
                          ppiVar7 = &param_1;
                          local_4 = 0xffffffff;
                          if ((char)uVar6 != '\0') {
                            FUN_00401c90(ppiVar7);
                            FUN_00473d20(this_00,&local_20,iVar8,1);
                            local_10 = &stack0xffffffc4;
                            local_4 = 4;
                            if (DAT_00501918 == '\0') {
                              DAT_00501918 = '\x01';
                              DAT_00501908 = 2;
                              DAT_0050190c = 0;
                              _DAT_00501910 = 1;
                              DAT_00501914 = 0;
                            }
                            piVar10 = &DAT_00501908;
                            if ((local_20 != &DAT_00501908) &&
                               (piVar10 = local_20, local_20 != &DAT_00501908)) {
                              *local_20 = *local_20 + 1;
                            }
                            FUN_0044ee50((void *)((int)local_24 + -0x140),local_14,piVar10);
                            ppiVar7 = &local_20;
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
                      pcVar11 = "equip";
                      pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                      bVar2 = FUN_0040c360(pvVar5,pcVar11);
                      if (bVar2) {
                        iVar4 = this_00[7];
                        iVar8 = 0;
                        if (0 < iVar4) {
                          pvVar5 = (void *)((int)local_24 + -0x140);
                          do {
                            FUN_00473d20(this_00,&param_1,iVar8,0);
                            local_10 = &stack0xffffffc4;
                            local_4 = 5;
                            if (DAT_00501918 == '\0') {
                              DAT_00501918 = '\x01';
                              DAT_00501908 = 2;
                              DAT_0050190c = 0;
                              _DAT_00501910 = 1;
                              DAT_00501914 = 0;
                            }
                            piVar10 = &DAT_00501908;
                            if ((param_1 != &DAT_00501908) &&
                               (piVar10 = param_1, param_1 != &DAT_00501908)) {
                              *param_1 = *param_1 + 1;
                            }
                            FUN_0044d080(pvVar5,piVar10);
                            local_4 = 0xffffffff;
                            FUN_00401c90(&param_1);
                            iVar4 = this_00[7];
                            iVar8 = iVar8 + 1;
                          } while (iVar8 < iVar4);
                        }
                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                      }
                      else {
                        pcVar11 = "select";
                        pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                        bVar2 = FUN_0040c360(pvVar5,pcVar11);
                        if (bVar2) {
                          iVar4 = this_00[7];
                          iVar8 = 0;
                          if (0 < iVar4) {
                            pvVar5 = (void *)((int)local_24 + -0x140);
                            do {
                              FUN_00473d20(this_00,&param_1,iVar8,0);
                              local_10 = &stack0xffffffc4;
                              local_4 = 6;
                              if (DAT_00501918 == '\0') {
                                DAT_00501918 = '\x01';
                                DAT_00501908 = 2;
                                DAT_0050190c = 0;
                                _DAT_00501910 = 1;
                                DAT_00501914 = 0;
                              }
                              piVar10 = &DAT_00501908;
                              if ((param_1 != &DAT_00501908) &&
                                 (piVar10 = param_1, param_1 != &DAT_00501908)) {
                                *param_1 = *param_1 + 1;
                              }
                              FUN_0044d270(pvVar5,piVar10);
                              local_4 = 0xffffffff;
                              FUN_00401c90(&param_1);
                              iVar4 = this_00[7];
                              iVar8 = iVar8 + 1;
                            } while (iVar8 < iVar4);
                          }
                          iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                        }
                        else {
                          pcVar11 = "touchhose";
                          pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                          bVar2 = FUN_0040c360(pvVar5,pcVar11);
                          pvVar5 = local_24;
                          if (bVar2) {
                            iVar4 = this_00[7];
                            iVar8 = 0;
                            if (0 < iVar4) {
                              do {
                                FUN_00473d20(this_00,&param_1,iVar8,0);
                                local_4 = 7;
                                cVar1 = FUN_0040cd50(&param_1,&local_14);
                                local_4 = 0xffffffff;
                                if (cVar1 == '\0') {
                                  FUN_00401c90(&param_1);
                                }
                                else {
                                  FUN_00401c90(&param_1);
                                  FUN_0044cab0((void *)((int)pvVar5 + -0x140),(int)local_14);
                                }
                                iVar4 = this_00[7];
                                iVar8 = iVar8 + 1;
                              } while (iVar8 < iVar4);
                            }
                            iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                          }
                          else {
                            pcVar11 = "toolcmd";
                            pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                            bVar2 = FUN_0040c360(pvVar5,pcVar11);
                            if (bVar2) {
                              iVar4 = this_00[7];
                              iVar8 = 0;
                              if (0 < iVar4) {
                                do {
                                  FUN_00473d20(this_00,&local_20,iVar8,0);
                                  local_4 = 8;
                                  FUN_00473d20(this_00,&param_1,iVar8,1);
                                  if (DAT_00501918 == '\0') {
                                    DAT_00501918 = '\x01';
                                    DAT_00501908 = 2;
                                    DAT_0050190c = 0;
                                    _DAT_00501910 = 1;
                                    DAT_00501914 = 0;
                                  }
                                  piVar10 = &DAT_00501908;
                                  if ((param_1 != &DAT_00501908) &&
                                     (piVar10 = param_1, param_1 != &DAT_00501908)) {
                                    *param_1 = *param_1 + 1;
                                  }
                                  local_10 = &stack0xffffffc0;
                                  if (DAT_00501918 == '\0') {
                                    DAT_00501918 = '\x01';
                                    DAT_00501908 = 2;
                                    DAT_0050190c = 0;
                                    _DAT_00501910 = 1;
                                    DAT_00501914 = 0;
                                  }
                                  piVar9 = &DAT_00501908;
                                  if ((local_20 != &DAT_00501908) &&
                                     (piVar9 = local_20, local_20 != &DAT_00501908)) {
                                    *local_20 = *local_20 + 1;
                                  }
                                  local_4._0_1_ = 9;
                                  FUN_0044ecd0((void *)((int)this + -0x140),piVar9,piVar10);
                                  local_4 = CONCAT31(local_4._1_3_,8);
                                  FUN_00401c90(&param_1);
                                  local_4 = 0xffffffff;
                                  FUN_00401c90(&local_20);
                                  iVar4 = this_00[7];
                                  iVar8 = iVar8 + 1;
                                } while (iVar8 < iVar4);
                              }
                              iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                            }
                            else {
                              pcVar11 = "setashdensity";
                              pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                              bVar2 = FUN_0040c360(pvVar5,pcVar11);
                              if (bVar2) {
                                iVar4 = this_00[7];
                                iVar8 = 0;
                                if (0 < iVar4) {
                                  do {
                                    FUN_00473d20(this_00,&param_1,iVar8,0);
                                    uVar6 = FUN_0040cc50(&param_1,&local_14);
                                    local_4 = 0xffffffff;
                                    if ((char)uVar6 == '\0') {
                                      FUN_00401c90(&param_1);
                                    }
                                    else {
                                      FUN_00401c90(&param_1);
                                      FUN_0044bc90((void *)((int)this + -0x140),local_14);
                                    }
                                    iVar4 = this_00[7];
                                    iVar8 = iVar8 + 1;
                                  } while (iVar8 < iVar4);
                                }
                                iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                              }
                              else {
                                pcVar11 = "addbonus";
                                pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                                bVar2 = FUN_0040c360(pvVar5,pcVar11);
                                if (bVar2) {
                                  iVar4 = this_00[7];
                                  iVar8 = 0;
                                  if (0 < iVar4) {
                                    do {
                                      FUN_00473d20(this_00,&param_1,iVar8,0);
                                      local_4 = 0xc;
                                      cVar1 = FUN_0040cd50(&param_1,&local_14);
                                      local_4 = 0xffffffff;
                                      if (cVar1 == '\0') {
                                        FUN_00401c90(&param_1);
                                      }
                                      else {
                                        FUN_00401c90(&param_1);
                                        FUN_0044ef10((void *)((int)this + -0x140),(int)local_14);
                                      }
                                      iVar4 = this_00[7];
                                      iVar8 = iVar8 + 1;
                                    } while (iVar8 < iVar4);
                                  }
                                  iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                }
                                else {
                                  pcVar11 = "printstats";
                                  pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                                  bVar2 = FUN_0040c360(pvVar5,pcVar11);
                                  if (bVar2) {
                                    iVar4 = this_00[7];
                                    iVar8 = 0;
                                    if (0 < iVar4) {
                                      do {
                                        FUN_0044cfd0();
                                        iVar4 = this_00[7];
                                        iVar8 = iVar8 + 1;
                                      } while (iVar8 < iVar4);
                                    }
                                    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                  }
                                  else {
                                    pcVar11 = "finalexit";
                                    pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                                    bVar2 = FUN_0040c360(pvVar5,pcVar11);
                                    if (bVar2) {
                                      iVar4 = this_00[7];
                                      iVar8 = 0;
                                      if (0 < iVar4) {
                                        do {
                                          FUN_00450a00((void *)((int)this + -0x140));
                                          iVar4 = this_00[7];
                                          iVar8 = iVar8 + 1;
                                        } while (iVar8 < iVar4);
                                      }
                                      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                    }
                                    else {
                                      pcVar11 = "finalexitloadmap";
                                      pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                                      bVar2 = FUN_0040c360(pvVar5,pcVar11);
                                      if (bVar2) {
                                        iVar4 = this_00[7];
                                        iVar8 = 0;
                                        if (0 < iVar4) {
                                          pvVar5 = (void *)((int)local_24 + -0x140);
                                          do {
                                            FUN_00473d20(this_00,&param_1,iVar8,0);
                                            local_10 = &stack0xffffffc4;
                                            local_4 = 0xd;
                                            if (DAT_00501918 == '\0') {
                                              DAT_00501918 = '\x01';
                                              DAT_00501908 = 2;
                                              DAT_0050190c = 0;
                                              _DAT_00501910 = 1;
                                              DAT_00501914 = 0;
                                            }
                                            piVar10 = &DAT_00501908;
                                            if ((param_1 != &DAT_00501908) &&
                                               (piVar10 = param_1, param_1 != &DAT_00501908)) {
                                              *param_1 = *param_1 + 1;
                                            }
                                            FUN_00450a70(pvVar5,piVar10,unaff_EDI);
                                            local_4 = 0xffffffff;
                                            FUN_00401c90(&param_1);
                                            iVar4 = this_00[7];
                                            iVar8 = iVar8 + 1;
                                          } while (iVar8 < iVar4);
                                        }
                                        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                      }
                                      else {
                                        pcVar11 = "reset";
                                        pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                                        bVar2 = FUN_0040c360(pvVar5,pcVar11);
                                        pvVar5 = local_24;
                                        if (bVar2) {
                                          iVar4 = this_00[7];
                                          iVar8 = 0;
                                          if (0 < iVar4) {
                                            this_01 = (void *)((int)local_24 + -0x140);
                                            do {
                                              cVar1 = '\x01';
                                              param_1 = (int *)&stack0xffffffc0;
                                              if (DAT_00501918 == '\0') {
                                                DAT_00501918 = '\x01';
                                                DAT_00501908 = 2;
                                                DAT_0050190c = 0;
                                                _DAT_00501910 = 1;
                                                DAT_00501914 = 0;
                                              }
                                              piVar10 = &DAT_00501908;
                                              FUN_00401d80(&stack0xffffffc0,"RC_PassOut");
                                              FUN_004897c0((void *)((int)pvVar5 + 0x3400),piVar10,
                                                           cVar1);
                                              FUN_0044ff10(this_01,'\x01');
                                              iVar4 = this_00[7];
                                              iVar8 = iVar8 + 1;
                                            } while (iVar8 < iVar4);
                                          }
                                          iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
                                        }
                                        else {
                                          pcVar11 = "setrescuebot";
                                          pvVar5 = (void *)FUN_0040c330(this_00,&param_1);
                                          bVar2 = FUN_0040c360(pvVar5,pcVar11);
                                          pvVar5 = local_24;
                                          if (bVar2) {
                                            iVar4 = this_00[7];
                                            iVar8 = 0;
                                            if (0 < iVar4) {
                                              do {
                                                FUN_00473d20(this_00,&local_24,iVar8,0);
                                                local_4 = 0xe;
                                                cVar1 = FUN_0040cd50(&local_24,&local_10);
                                                local_4 = 0xffffffff;
                                                if (cVar1 == '\0') {
                                                  FUN_00401c90(&local_24);
                                                }
                                                else {
                                                  FUN_00401c90(&local_24);
                                                  *(bool *)((int)pvVar5 + 0x32e4) =
                                                       local_10 != (undefined1 *)0x0;
                                                }
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
          else {
            iVar4 = this_00[7];
            iVar8 = 0;
            if (0 < iVar4) {
              do {
                FUN_0044c0b0((int)this + -0x140);
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
              local_4 = 0;
              cVar1 = FUN_0040cd50(&param_1,&local_14);
              local_4 = 0xffffffff;
              if (cVar1 == '\0') {
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
                FUN_00473d20(this_00,&local_28,iVar8,1);
                local_4 = 1;
                cVar1 = FUN_0040cd50(&local_28,&local_18);
                local_4 = 0xffffffff;
                if (cVar1 == '\0') {
                  if (local_28 != &DAT_00501908) {
                    *local_28 = *local_28 + -1;
                    if (*local_28 < 1) {
                      FUN_004011c0(&local_28);
                    }
                    else {
                      local_28 = &DAT_00501908;
                    }
                  }
                }
                else {
                  if (local_28 != &DAT_00501908) {
                    *local_28 = *local_28 + -1;
                    if (*local_28 < 1) {
                      FUN_004011c0(&local_28);
                    }
                    else {
                      local_28 = &DAT_00501908;
                    }
                  }
                  FUN_00473d20(this_00,&local_20,iVar8,2);
                  local_4 = 2;
                  cVar1 = FUN_0040cd50(&local_20,&local_1c);
                  local_4 = 0xffffffff;
                  if (cVar1 == '\0') {
                    FUN_00401c90(&local_20);
                  }
                  else {
                    FUN_00401c90(&local_20);
                    FUN_0044c070((void *)((int)local_24 + -0x140),(int)local_14,local_18,local_1c);
                  }
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
            FUN_0044bce0((int)this + -0x140);
            iVar4 = this_00[7];
            iVar8 = iVar8 + 1;
          } while (iVar8 < iVar4);
        }
        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
      }
    }
    else {
      iVar4 = 0;
      if (0 < this_00[7]) {
        do {
          if ((*(int *)((int)local_24 + 0x4c) != 4) && (*(char *)((int)local_24 + -0x14) != '\0')) {
            *(undefined4 *)((int)local_24 + -0xb8) = DAT_004e1820;
            *(undefined4 *)((int)local_24 + 0x4c) = 4;
          }
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
        FUN_0044e990((int *)((int)this + -0x140));
        iVar4 = this_00[7];
        iVar8 = iVar8 + 1;
      } while (iVar8 < iVar4);
    }
    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
  }
  ExceptionList = local_c;
  return iVar4;
}

