
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00423160(int *param_1)

{
  float *pfVar1;
  byte bVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  int iVar5;
  void *this;
  int *piVar6;
  int *piVar7;
  char *pcVar8;
  bool bVar9;
  int *local_e0;
  int *local_dc;
  int *local_d8;
  int *local_d4;
  int *local_d0;
  int *local_cc;
  int *local_c8;
  int *local_c4;
  undefined4 *local_c0;
  int local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  int local_a4;
  int iStack_a0;
  byte abStack_9c [4];
  undefined4 local_98 [13];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  int local_4;
  
  piVar7 = param_1;
  puStack_8 = &LAB_004ccf6a;
  pvStack_c = ExceptionList;
  this = (void *)0x0;
  local_4 = 0;
  ExceptionList = &pvStack_c;
  FUN_004a3bda();
  iVar5 = local_a4;
  local_4._0_1_ = 1;
  if (((abStack_9c[*(int *)(local_a4 + 4)] & 6) == 0) &&
     (*(int *)(*(int *)((int)&iStack_a0 + *(int *)(local_a4 + 4)) + 0x30) != -1)) {
    local_bc = 0;
    FUN_004054a0(&local_dc);
    FUN_004054a0(&local_e0);
    bVar2 = abStack_9c[*(int *)(iVar5 + 4)];
    local_4._0_1_ = 4;
    piVar6 = local_e0;
    while ((bVar2 & 1) == 0) {
      FUN_00422670(&local_dc,&local_a4);
      piVar6 = local_e0;
      if ((char)local_dc[3] != ';') {
        puVar3 = FUN_004172c0(&local_dc,&local_d0,(int *)0x0,(char *)0x0,(int *)0x0);
        local_4._0_1_ = 5;
        if (local_e0 != (int *)*puVar3) {
          if ((local_e0 != &DAT_00501908) && (*local_e0 = *local_e0 + -1, *local_e0 < 1)) {
            FUN_004011c0(&local_e0);
          }
          local_e0 = (int *)*puVar3;
          if (local_e0 != &DAT_00501908) {
            *local_e0 = *local_e0 + 1;
          }
        }
        local_4._0_1_ = 4;
        if (local_d0 != &DAT_00501908) {
          *local_d0 = *local_d0 + -1;
          if (*local_d0 < 1) {
            FUN_004011c0(&local_d0);
          }
          else {
            local_d0 = &DAT_00501908;
          }
        }
        piVar6 = local_e0;
        iVar5 = 0xb;
        bVar9 = true;
        piVar7 = local_e0 + 3;
        pcVar8 = "menumusic:";
        do {
          if (iVar5 == 0) break;
          iVar5 = iVar5 + -1;
          bVar9 = (char)*piVar7 == *pcVar8;
          piVar7 = (int *)((int)piVar7 + 1);
          pcVar8 = pcVar8 + 1;
        } while (bVar9);
        if (bVar9) {
          puVar3 = FUN_004172c0(&local_dc,&local_d8,(int *)0x0,(char *)0x0,(int *)0x0);
          local_4._0_1_ = 6;
          if (local_e0 != (int *)*puVar3) {
            if ((local_e0 != &DAT_00501908) && (*local_e0 = *local_e0 + -1, *local_e0 < 1)) {
              FUN_004011c0(&local_e0);
            }
            local_e0 = (int *)*puVar3;
            if (local_e0 != &DAT_00501908) {
              *local_e0 = *local_e0 + 1;
            }
          }
          local_4._0_1_ = 4;
          if (local_d8 != &DAT_00501908) {
            *local_d8 = *local_d8 + -1;
            if (*local_d8 < 1) {
              FUN_004011c0(&local_d8);
            }
            else {
              local_d8 = &DAT_00501908;
            }
          }
          puVar3 = local_c0;
          piVar7 = (int *)*local_c0;
          piVar6 = local_e0;
          if (piVar7 != local_e0) {
            if (piVar7 != &DAT_00501908) {
              *piVar7 = *piVar7 + -1;
              if (*(int *)*local_c0 < 1) {
                FUN_004011c0(local_c0);
              }
              else {
                *local_c0 = &DAT_00501908;
              }
            }
            *puVar3 = local_e0;
            piVar6 = local_e0;
            if (local_e0 != &DAT_00501908) {
              *local_e0 = *local_e0 + 1;
            }
          }
        }
        else {
          iVar5 = 7;
          bVar9 = true;
          piVar7 = local_e0 + 3;
          pcVar8 = "level:";
          do {
            if (iVar5 == 0) break;
            iVar5 = iVar5 + -1;
            bVar9 = (char)*piVar7 == *pcVar8;
            piVar7 = (int *)((int)piVar7 + 1);
            pcVar8 = pcVar8 + 1;
          } while (bVar9);
          if (bVar9) {
            puVar3 = operator_new(0x28);
            if (puVar3 == (undefined4 *)0x0) {
              this = (void *)0x0;
            }
            else {
              this = (void *)FUN_00422800(puVar3);
            }
            FUN_0048d9d0(local_c0 + 1,this);
            puVar3 = FUN_004172c0(&local_dc,&local_c8,(int *)0x0,(char *)0x0,(int *)0x0);
            local_4._0_1_ = 7;
            FUN_00403b00(this,puVar3);
            local_4._0_1_ = 4;
            if (local_c8 != &DAT_00501908) {
              *local_c8 = *local_c8 + -1;
              if (*local_c8 < 1) {
                FUN_004011c0(&local_c8);
              }
              else {
                local_c8 = &DAT_00501908;
              }
            }
            *(int *)((int)this + 0x18) = local_bc;
            local_bc = local_bc + 1;
            *(undefined1 *)((int)this + 0x14) = 1;
            piVar6 = local_e0;
          }
          else if (this != (void *)0x0) {
            uVar4 = FUN_004019a0(&local_e0,&DAT_004df250);
            if ((char)uVar4 == '\0') {
              uVar4 = FUN_004019a0(&local_e0,(byte *)"title:");
              if ((char)uVar4 == '\0') {
                uVar4 = FUN_004019a0(&local_e0,(byte *)"music:");
                if ((char)uVar4 != '\0') {
                  puVar3 = FUN_004172c0(&local_dc,&local_a8,(int *)0x0,(char *)0x0,(int *)0x0);
                  local_4._0_1_ = 0xc;
                  FUN_00403b00(&local_e0,puVar3);
                  local_4._0_1_ = 4;
                  FUN_00401c90(&local_a8);
                  FUN_00403b00((void *)((int)this + 0xc),&local_e0);
                  piVar6 = local_e0;
                }
                uVar4 = FUN_004019a0(&local_e0,(byte *)"time:");
                if ((char)uVar4 != '\0') {
                  puVar3 = FUN_004172c0(&local_dc,&local_b8,(int *)0x0,(char *)0x0,(int *)0x0);
                  local_4._0_1_ = 0xd;
                  FUN_00403b00(&local_e0,puVar3);
                  local_4._0_1_ = 4;
                  FUN_00401c90(&local_b8);
                  pfVar1 = (float *)((int)this + 0x10);
                  FUN_0040cc50(&local_e0,pfVar1);
                  *pfVar1 = *pfVar1 * _DAT_004df224;
                  piVar6 = local_e0;
                }
                uVar4 = FUN_004019a0(&local_e0,(byte *)"mapcoords:");
                if ((char)uVar4 != '\0') {
                  puVar3 = FUN_00422bb0(&local_dc,&local_b4,',');
                  local_4._0_1_ = 0xe;
                  FUN_00403b00(&local_e0,puVar3);
                  local_4._0_1_ = 4;
                  FUN_00401c90(&local_b4);
                  FUN_0040cd50(&local_e0,(int)this + 0x20);
                  puVar3 = FUN_004172c0(&local_dc,&local_b0,(int *)0x0,(char *)0x0,(int *)0x0);
                  local_4._0_1_ = 0xf;
                  FUN_00403b00(&local_e0,puVar3);
                  local_4._0_1_ = 4;
                  FUN_00401c90(&local_b0);
                  FUN_0040cd50(&local_e0,(int)this + 0x24);
                  piVar6 = local_e0;
                }
              }
              else {
                puVar3 = FUN_004172c0(&local_dc,&local_ac,(int *)0x1,"",(int *)0x0);
                local_4._0_1_ = 0xb;
                FUN_00403b00(&local_e0,puVar3);
                local_4._0_1_ = 4;
                FUN_00401c90(&local_ac);
                FUN_004064a0((int *)&local_e0,"\"",(int *)&DAT_004dd84c);
                FUN_00403b00((void *)((int)this + 4),&local_e0);
                piVar6 = local_e0;
              }
            }
            else {
              puVar3 = FUN_004172c0(&local_dc,&local_d4,(int *)0x1,"",(int *)0x0);
              local_4._0_1_ = 8;
              FUN_00403b00(&local_e0,puVar3);
              local_4._0_1_ = 4;
              if (local_d4 != &DAT_00501908) {
                *local_d4 = *local_d4 + -1;
                if (*local_d4 < 1) {
                  FUN_004011c0(&local_d4);
                }
                else {
                  local_d4 = &DAT_00501908;
                }
              }
              FUN_004064a0((int *)&local_e0,"\"",(int *)&DAT_004dd84c);
              puVar3 = FUN_00402790(&local_c4,"data\\resdefs\\");
              local_4._0_1_ = 9;
              puVar3 = FUN_004040e0(puVar3,&local_cc,(int *)&local_e0);
              local_4._0_1_ = 10;
              FUN_00403b00((void *)((int)this + 8),puVar3);
              local_4._0_1_ = 9;
              if (local_cc != &DAT_00501908) {
                *local_cc = *local_cc + -1;
                if (*local_cc < 1) {
                  FUN_004011c0(&local_cc);
                }
                else {
                  local_cc = &DAT_00501908;
                }
              }
              local_4._0_1_ = 4;
              piVar6 = local_e0;
              if (local_c4 != &DAT_00501908) {
                *local_c4 = *local_c4 + -1;
                if (*local_c4 < 1) {
                  FUN_004011c0(&local_c4);
                  piVar6 = local_e0;
                }
                else {
                  local_c4 = &DAT_00501908;
                }
              }
            }
          }
        }
      }
      piVar7 = param_1;
      bVar2 = abStack_9c[*(int *)(local_a4 + 4)];
    }
    local_4._0_1_ = 3;
    if (piVar6 != &DAT_00501908) {
      *piVar6 = *piVar6 + -1;
      if (*local_e0 < 1) {
        FUN_004011c0(&local_e0);
      }
      else {
        local_e0 = &DAT_00501908;
      }
    }
    local_4._0_1_ = 1;
    if ((local_dc != &DAT_00501908) && (iVar5 = *local_dc, *local_dc = iVar5 + -1, iVar5 + -1 < 1))
    {
      FUN_004011c0(&local_dc);
    }
    local_4 = (uint)local_4._1_3_ << 8;
    FUN_004a3da7((int)local_98);
    FUN_004a3426(local_98);
    if (piVar7 == &DAT_00501908) {
      ExceptionList = pvStack_c;
      return;
    }
    iVar5 = *piVar7 + -1;
    *piVar7 = iVar5;
  }
  else {
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "main\\LevelManager.cpp";
    local_5c = 0x57;
    local_58 = 0;
    local_4._0_1_ = 2;
    FUN_004a3a19(local_48,"ERROR: Couldn\'t load level definitions file.");
    FUN_00401360(&local_64,0);
    local_4._0_1_ = 1;
    FUN_004012c0(&local_64);
    local_4 = (uint)local_4._1_3_ << 8;
    FUN_004a3da7((int)local_98);
    FUN_004a3426(local_98);
    if (piVar7 == &DAT_00501908) {
      ExceptionList = pvStack_c;
      return;
    }
    iVar5 = *piVar7 + -1;
    *piVar7 = iVar5;
  }
  local_4 = 0xffffffff;
  if (iVar5 < 1) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = pvStack_c;
  return;
}

