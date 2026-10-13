/* Fresh Ghidra decompiler output; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __thiscall HandleFireRescueMotionCommand(void *this,int *param_1)

{
  int *this_00;
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int *local_3c;
  int *local_38;
  int *local_34;
  undefined4 local_30;
  void *local_2c;
  undefined4 local_28;
  undefined4 local_24;
  float local_20;
  undefined1 local_1c [4];
  int local_18;
  int local_14;
  undefined1 *local_10;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  
  this_00 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf9b0;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  local_2c = this;
  puVar2 = (undefined4 *)FUN_0040c330(param_1,&param_1);
  iVar3 = __stricmp((char *)(*(int *)*puVar2 + 0xc),"moveto");
  cVar1 = '\x01' - (iVar3 != 0);
  param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
  if (cVar1 == '\0') {
    puVar2 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar3 = __stricmp((char *)(*(int *)*puVar2 + 0xc),"stopmove");
    cVar1 = '\x01' - (iVar3 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 == '\0') {
      puVar2 = (undefined4 *)FUN_0040c330(this_00,&param_1);
      iVar3 = __stricmp((char *)(*(int *)*puVar2 + 0xc),"setsize");
      cVar1 = '\x01' - (iVar3 != 0);
      param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
      if (cVar1 == '\0') {
        uVar5 = -iVar3 & 0xffffff00;
      }
      else {
        iVar3 = this_00[7];
        iVar6 = 0;
        if (0 < iVar3) {
          do {
            FUN_00473d20(this_00,&param_1,iVar6,0);
            local_4 = 4;
            cVar1 = FUN_0040cd50(&param_1,&local_10);
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
              FUN_00473d20(this_00,&local_34,iVar6,1);
              local_4 = 5;
              cVar1 = FUN_0040cd50(&local_34,&local_14);
              local_4 = 0xffffffff;
              if (cVar1 == '\0') {
                if (local_34 != &DAT_00501908) {
                  *local_34 = *local_34 + -1;
                  if (*local_34 < 1) {
                    FUN_004011c0(&local_34);
                  }
                  else {
                    local_34 = &DAT_00501908;
                  }
                }
              }
              else {
                if (local_34 != &DAT_00501908) {
                  *local_34 = *local_34 + -1;
                  if (*local_34 < 1) {
                    FUN_004011c0(&local_34);
                  }
                  else {
                    local_34 = &DAT_00501908;
                  }
                }
                FUN_00473d20(this_00,&local_38,iVar6,2);
                local_4 = 6;
                cVar1 = FUN_0040cd50(&local_38,&local_18);
                local_4 = 0xffffffff;
                if (cVar1 == '\0') {
                  if (local_38 != &DAT_00501908) {
                    *local_38 = *local_38 + -1;
                    if (*local_38 < 1) {
                      FUN_004011c0(&local_38);
                    }
                    else {
                      local_38 = &DAT_00501908;
                    }
                  }
                }
                else {
                  if (local_38 != &DAT_00501908) {
                    *local_38 = *local_38 + -1;
                    if (*local_38 < 1) {
                      FUN_004011c0(&local_38);
                    }
                    else {
                      local_38 = &DAT_00501908;
                    }
                  }
                  FUN_00473d20(this_00,&local_30,iVar6,3);
                  uVar4 = FUN_0040cc50(&local_30,local_1c);
                  local_4 = 0xffffffff;
                  if ((char)uVar4 == '\0') {
                    FUN_00401c90(&local_30);
                  }
                  else {
                    FUN_00401c90(&local_30);
                    SetFireRescueSizeMotion
                              ((void *)((int)local_2c + -0x24),(int)local_10,local_14,local_18,
                               local_1c);
                  }
                }
              }
            }
            iVar3 = this_00[7];
            iVar6 = iVar6 + 1;
          } while (iVar6 < iVar3);
        }
        uVar5 = CONCAT31((int3)((uint)iVar3 >> 8),1);
      }
    }
    else {
      iVar3 = 0;
      if (0 < this_00[7]) {
        do {
          *(undefined4 *)((int)this + 0x4c) = 0;
          *(undefined4 *)((int)this + 0xdc) = 0;
          iVar3 = iVar3 + 1;
        } while (iVar3 < this_00[7]);
      }
      uVar5 = CONCAT31((int3)((uint)iVar3 >> 8),1);
    }
  }
  else {
    iVar3 = this_00[7];
    iVar6 = 0;
    if (0 < iVar3) {
      do {
        FUN_00473d20(this_00,&param_1,iVar6,0);
        local_4 = 0;
        FUN_00473d20(this_00,&local_3c,iVar6,1);
        uVar4 = FUN_0040cc50(&local_3c,&local_20);
        local_4 = local_4 & 0xffffff00;
        if ((char)uVar4 == '\0') {
          if (local_3c != &DAT_00501908) {
            *local_3c = *local_3c + -1;
            if (*local_3c < 1) {
              FUN_004011c0(&local_3c);
            }
            else {
              local_3c = &DAT_00501908;
            }
          }
LAB_00449ce6:
          local_4 = 0xffffffff;
          if (param_1 != &DAT_00501908) {
            *param_1 = *param_1 + -1;
            iVar3 = *param_1;
LAB_00449d00:
            local_4 = 0xffffffff;
            if (iVar3 < 1) {
              FUN_004011c0(&param_1);
            }
            else {
              param_1 = &DAT_00501908;
            }
          }
        }
        else {
          if (local_3c != &DAT_00501908) {
            *local_3c = *local_3c + -1;
            if (*local_3c < 1) {
              FUN_004011c0(&local_3c);
            }
            else {
              local_3c = &DAT_00501908;
            }
          }
          FUN_00473d20(this_00,&local_38,iVar6,2);
          uVar4 = FUN_0040cc50(&local_38,&local_24);
          local_4 = local_4 & 0xffffff00;
          if ((char)uVar4 == '\0') {
            if (local_38 != &DAT_00501908) {
              *local_38 = *local_38 + -1;
              if (*local_38 < 1) {
                FUN_004011c0(&local_38);
              }
              else {
                local_38 = &DAT_00501908;
              }
            }
            local_4 = 0xffffffff;
            if (param_1 != &DAT_00501908) {
              *param_1 = *param_1 + -1;
              iVar3 = *param_1;
              goto LAB_00449d00;
            }
          }
          else {
            if (local_38 != &DAT_00501908) {
              *local_38 = *local_38 + -1;
              if (*local_38 < 1) {
                FUN_004011c0(&local_38);
              }
              else {
                local_38 = &DAT_00501908;
              }
            }
            FUN_00473d20(this_00,&local_34,iVar6,3);
            uVar4 = FUN_0040cc50(&local_34,&local_28);
            local_4 = local_4 & 0xffffff00;
            if ((char)uVar4 != '\0') {
              if (local_34 != &DAT_00501908) {
                *local_34 = *local_34 + -1;
                if (*local_34 < 1) {
                  FUN_004011c0(&local_34);
                }
                else {
                  local_34 = &DAT_00501908;
                }
              }
              local_10 = &stack0xffffffa4;
              if (DAT_00501918 == '\0') {
                DAT_00501918 = '\x01';
                DAT_00501908 = 2;
                DAT_0050190c = 0;
                _DAT_00501910 = 1;
                DAT_00501914 = 0;
              }
              piVar7 = &DAT_00501908;
              if ((param_1 != &DAT_00501908) && (piVar7 = param_1, param_1 != &DAT_00501908)) {
                *param_1 = *param_1 + 1;
              }
              SetFireRescuePathPointMove
                        ((void *)((int)local_2c + -0x24),piVar7,local_20,local_24,local_28);
              goto LAB_00449ce6;
            }
            if (local_34 != &DAT_00501908) {
              *local_34 = *local_34 + -1;
              if (*local_34 < 1) {
                FUN_004011c0(&local_34);
              }
              else {
                local_34 = &DAT_00501908;
              }
            }
            local_4 = 0xffffffff;
            if (param_1 != &DAT_00501908) {
              *param_1 = *param_1 + -1;
              iVar3 = *param_1;
              goto LAB_00449d00;
            }
          }
        }
        iVar3 = this_00[7];
        iVar6 = iVar6 + 1;
      } while (iVar6 < iVar3);
    }
    uVar5 = CONCAT31((int3)((uint)iVar3 >> 8),1);
  }
  ExceptionList = local_c;
  return uVar5;
}


