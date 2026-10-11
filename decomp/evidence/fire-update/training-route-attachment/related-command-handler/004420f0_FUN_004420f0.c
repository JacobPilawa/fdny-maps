/* Ghidra decompilation; names/types are provisional.
 * Entry: 004420f0
 * Function: FUN_004420f0
 */


int __thiscall FUN_004420f0(void *this,int *param_1)

{
  int *this_00;
  char cVar1;
  bool bVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  void *this_01;
  uint3 extraout_var;
  int iVar6;
  char *pcVar7;
  void *local_18;
  undefined4 local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  this_00 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf100;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  local_18 = this;
  puVar3 = (undefined4 *)FUN_0040c330(param_1,&param_1);
  iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"setsize");
  cVar1 = '\x01' - (iVar4 != 0);
  param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
  if (cVar1 == '\0') {
    puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"turnon");
    cVar1 = '\x01' - (iVar4 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 == '\0') {
      puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
      iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"turnoff");
      cVar1 = '\x01' - (iVar4 != 0);
      param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
      if (cVar1 == '\0') {
        puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
        iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"setrot");
        cVar1 = '\x01' - (iVar4 != 0);
        param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
        if (cVar1 == '\0') {
          puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
          iVar4 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"setpitch");
          cVar1 = '\x01' - (iVar4 != 0);
          param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
          if (cVar1 == '\0') {
            pcVar7 = "setstrength";
            this_01 = (void *)FUN_0040c330(this_00,&param_1);
            bVar2 = FUN_0040c360(this_01,pcVar7);
            if (bVar2) {
              iVar4 = this_00[7];
              iVar6 = 0;
              if (0 < iVar4) {
                do {
                  FUN_00473d20(this_00,&local_18,iVar6,0);
                  uVar5 = FUN_0040cc50(&local_18,&local_10);
                  local_4 = 0xffffffff;
                  if ((char)uVar5 == '\0') {
                    FUN_00401c90(&local_18);
                  }
                  else {
                    FUN_00401c90(&local_18);
                    (**(code **)(**(int **)((int)this + 0x54) + 0x28))(local_10);
                  }
                  iVar4 = this_00[7];
                  iVar6 = iVar6 + 1;
                } while (iVar6 < iVar4);
              }
              iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
            }
            else {
              iVar4 = (uint)extraout_var << 8;
            }
          }
          else {
            iVar4 = this_00[7];
            iVar6 = 0;
            if (0 < iVar4) {
              do {
                FUN_00473d20(this_00,&param_1,iVar6,0);
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
                  *(undefined4 *)(*(int *)((int)local_18 + 0x60) + 0x10) = local_14;
                  FUN_00441fc0((int)local_18 + -0x24);
                }
                iVar4 = this_00[7];
                iVar6 = iVar6 + 1;
              } while (iVar6 < iVar4);
            }
            iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
          }
        }
        else {
          iVar4 = this_00[7];
          iVar6 = 0;
          if (0 < iVar4) {
            do {
              FUN_00473d20(this_00,&param_1,iVar6,0);
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
                *(undefined4 *)(*(int *)((int)local_18 + 0x60) + 0xc) = local_14;
                FUN_00441fc0((int)local_18 + -0x24);
              }
              iVar4 = this_00[7];
              iVar6 = iVar6 + 1;
            } while (iVar6 < iVar4);
          }
          iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
        }
      }
      else {
        iVar4 = this_00[7];
        iVar6 = 0;
        if (0 < iVar4) {
          do {
            if (*(int **)((int)this + 0x54) != (int *)0x0) {
              (**(code **)(**(int **)((int)this + 0x54) + 0x1c))();
            }
            iVar4 = this_00[7];
            iVar6 = iVar6 + 1;
          } while (iVar6 < iVar4);
        }
        iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
      }
    }
    else {
      iVar4 = this_00[7];
      iVar6 = 0;
      if (0 < iVar4) {
        do {
          if (*(int **)((int)this + 0x54) != (int *)0x0) {
            (**(code **)(**(int **)((int)this + 0x54) + 0x18))();
          }
          iVar4 = this_00[7];
          iVar6 = iVar6 + 1;
        } while (iVar6 < iVar4);
      }
      iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
    }
  }
  else {
    iVar4 = this_00[7];
    iVar6 = 0;
    if (0 < iVar4) {
      do {
        FUN_00473d20(this_00,&param_1,iVar6,0);
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
          *(undefined4 *)((int)local_18 + 0x58) = local_14;
          FUN_00441fc0((int)local_18 + -0x24);
        }
        iVar4 = this_00[7];
        iVar6 = iVar6 + 1;
      } while (iVar6 < iVar4);
    }
    iVar4 = CONCAT31((int3)((uint)iVar4 >> 8),1);
  }
  ExceptionList = local_c;
  return iVar4;
}
