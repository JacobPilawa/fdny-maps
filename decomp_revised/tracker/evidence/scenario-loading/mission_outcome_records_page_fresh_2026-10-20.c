/*
 * Read-only Ghidra 12.0.4 decompilation of the Records page content updater.
 * Input: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Exported from the saved FDNYPort analysis with DecompileTargetAndRefs.java.
 * The catalog lookup and text construction are preserved as decompiled; field
 * names and final runtime presentation are not inferred from the C output.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004332e0(int param_1)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  undefined1 *puVar4;
  void *pvVar5;
  int *extraout_ECX;
  int iVar6;
  int **ppiVar7;
  int *piVar8;
  byte *pbVar9;
  int *piVar10;
  int iVar11;
  int *piVar12;
  int *piVar13;
  undefined4 *puVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  int *local_1e0;
  int *local_1dc;
  void *local_1d8;
  int *local_1d4;
  int **local_1d0;
  undefined1 *local_1cc;
  int *local_1c8;
  int local_1c4;
  int *local_1c0;
  int *local_1bc;
  int *local_1b8;
  int *local_1b4;
  int *local_1b0;
  int *local_1ac;
  int *local_1a8;
  int *local_1a4;
  int *local_1a0;
  int local_19c;
  int local_198;
  undefined4 local_194;
  undefined4 local_190;
  code *local_18c;
  undefined4 local_188;
  int *local_184;
  int *local_180;
  int local_17c;
  char *local_178;
  undefined4 local_174;
  int local_168;
  char *local_164;
  char *local_160;
  undefined4 local_15c;
  undefined1 *local_150;
  int local_14c;
  void *local_148;
  undefined4 local_144;
  undefined4 local_140;
  undefined4 local_13c;
  undefined4 local_138;
  int local_134;
  undefined1 local_130;
  int local_12c;
  char *local_128;
  char *local_124;
  undefined4 local_120;
  int local_114;
  char *local_110;
  char *local_10c;
  undefined4 local_108;
  int local_fc;
  char *local_f8;
  char *local_f4;
  undefined4 local_f0;
  int local_e4;
  char *local_e0;
  char *local_dc;
  undefined4 local_d8;
  int local_cc;
  char *local_c8;
  char *local_c4;
  undefined4 local_c0;
  int local_b4;
  char *local_b0;
  char *local_ac;
  undefined4 local_a8;
  int local_9c;
  char *local_98;
  char *local_94;
  undefined4 local_90;
  int local_84;
  char *local_80;
  char *local_7c;
  undefined4 local_78;
  int local_6c;
  char *local_68;
  char *local_64;
  undefined4 local_60;
  int local_54;
  char *local_50;
  char *local_4c;
  undefined4 local_48;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ce2f2;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  local_150 = (undefined1 *)param_1;
  iVar3 = FUN_004225a0((void *)(*(int *)(*(int *)(param_1 + 0x28) + 0x154) + 0x6ac),
                       *(int *)(param_1 + 0x354));
  if (iVar3 != 0) {
    uVar16 = 2;
    uVar15 = 2;
    puVar14 = (undefined4 *)&DAT_004ff8b0;
    local_1c8 = (int *)(iVar3 + 4);
    piVar8 = extraout_ECX;
    FUN_00401cb0(&stack0xfffffe00,local_1c8);
    FUN_0048c780((void *)(param_1 + 0x48),piVar8,puVar14,uVar15,uVar16);
    iVar11 = 0;
    local_188 = 0x10;
    local_18c = FUN_00432a40;
    local_19c = 0;
    local_198 = 0;
    local_194 = 0;
    local_190 = 0;
    local_148 = (void *)0x0;
    local_144 = 0;
    local_140 = 0;
    local_13c = 0;
    local_138 = 0;
    local_134 = 0;
    local_130 = 1;
    iVar3 = *(int *)(*(int *)(param_1 + 0x28) + 0x154);
    iVar6 = *(int *)(iVar3 + 0x6d0);
    piVar8 = (int *)(iVar3 + 0x6cc);
    local_4._0_1_ = 1;
    local_4._1_3_ = 0;
    local_1d4 = (int *)0x0;
    local_1c0 = piVar8;
    if (0 < iVar6) {
      do {
        piVar10 = local_1d4;
        if (((int)local_1d4 < 0) || (iVar6 <= (int)local_1d4)) {
          piVar13 = &local_12c;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar13 = 0;
            piVar13 = piVar13 + 1;
          }
          local_12c = 1;
          local_128 = "Array index out of bounds";
          local_124 = "..\\BluelAPI/Container/SArray.h";
          local_120 = 0x1cd;
          FUN_00422060(&local_12c);
          iVar11 = local_198;
        }
        local_1d8 = *(void **)(*piVar8 + (int)piVar10 * 4);
        local_1cc = &stack0xfffffe0c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar13 = (int *)*local_1c8;
        piVar12 = &DAT_00501908;
        if (piVar13 != &DAT_00501908) {
          *piVar13 = *piVar13 + 1;
          piVar12 = piVar13;
        }
        iVar3 = FUN_00423a70(local_1d8,piVar12);
        if (iVar3 != 0) {
          FUN_00432e20(&local_180,(undefined4 *)(iVar3 + 0xc));
          local_4._0_1_ = 2;
          while ((pcVar2 = local_178, local_180 != (int *)0x0 && (local_178 != (char *)0x0))) {
            iVar3 = *(int *)(local_178 + 0x20);
            pcVar1 = local_178 + 0xc;
            if (iVar3 < iVar11) {
              if ((iVar3 < 0) || (iVar11 <= iVar3)) {
                piVar8 = &local_b4;
                for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
                  *piVar8 = 0;
                  piVar8 = piVar8 + 1;
                }
                local_b4 = 1;
                local_b0 = "Array index out of bounds";
                local_ac = "..\\BluelAPI/Container/SArray.h";
                local_a8 = 0x1cd;
                FUN_00422060(&local_b4);
                iVar11 = local_198;
              }
              if (*(int *)(local_19c + iVar3 * 4) == 0) {
                puVar4 = operator_new(4);
                local_4._0_1_ = 3;
                local_1cc = puVar4;
                if (puVar4 == (undefined1 *)0x0) {
                  puVar4 = (undefined1 *)0x0;
                }
                else {
                  FUN_00401cb0(puVar4,(undefined4 *)(pcVar2 + 0x1c));
                }
                iVar3 = *(int *)(pcVar2 + 0x20);
                local_4._0_1_ = 2;
                if ((iVar3 < 0) || (local_198 <= iVar3)) {
                  piVar8 = &local_e4;
                  for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
                    *piVar8 = 0;
                    piVar8 = piVar8 + 1;
                  }
                  local_e4 = 1;
                  local_e0 = "Array index out of bounds";
                  local_dc = "..\\BluelAPI/Container/SArray.h";
                  local_d8 = 0x1cd;
                  FUN_00422060(&local_e4);
                }
                *(undefined1 **)(local_19c + iVar3 * 4) = puVar4;
              }
              else {
                iVar3 = *(int *)(pcVar2 + 0x20);
                if ((iVar3 < 0) || (iVar11 <= iVar3)) {
                  piVar8 = &local_fc;
                  for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
                    *piVar8 = 0;
                    piVar8 = piVar8 + 1;
                  }
                  local_fc = 1;
                  local_f8 = "Array index out of bounds";
                  local_f4 = "..\\BluelAPI/Container/SArray.h";
                  local_f0 = 0x1cd;
                  FUN_00422060(&local_fc);
                }
                puVar14 = *(undefined4 **)(local_19c + iVar3 * 4);
                piVar8 = (int *)*puVar14;
                if (piVar8 != *(int **)(pcVar2 + 0x1c)) {
                  if (piVar8 != &DAT_00501908) {
                    *piVar8 = *piVar8 + -1;
                    if (*(int *)*puVar14 < 1) {
                      FUN_004011c0(puVar14);
                    }
                    else {
                      *puVar14 = &DAT_00501908;
                    }
                  }
                  piVar8 = *(int **)(pcVar2 + 0x1c);
                  *puVar14 = piVar8;
                  if (piVar8 != &DAT_00501908) {
                    *piVar8 = *piVar8 + 1;
                  }
                }
              }
            }
            else {
              puVar14 = operator_new(4);
              if (puVar14 == (undefined4 *)0x0) {
                puVar14 = (undefined4 *)0x0;
              }
              else {
                if (DAT_00501918 == '\0') {
                  DAT_00501918 = '\x01';
                  DAT_00501908 = 2;
                  DAT_0050190c = 0;
                  _DAT_00501910 = 1;
                  DAT_00501914 = 0;
                }
                *puVar14 = &DAT_00501908;
                piVar8 = *(int **)(pcVar2 + 0x1c);
                if (piVar8 != &DAT_00501908) {
                  *puVar14 = piVar8;
                  *piVar8 = *piVar8 + 1;
                }
              }
              iVar3 = *(int *)(pcVar2 + 0x20);
              local_4._0_1_ = 2;
              if (local_198 <= iVar3) {
                FUN_0043b6e0(&local_19c,iVar3 + 1);
              }
              if ((iVar3 < 0) || (local_198 <= iVar3)) {
                piVar8 = &local_cc;
                for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
                  *piVar8 = 0;
                  piVar8 = piVar8 + 1;
                }
                local_cc = 1;
                local_c8 = "Array index out of bounds";
                local_c4 = "..\\BluelAPI/Container/SArray.h";
                local_c0 = 0x1cd;
                FUN_00422060(&local_cc);
              }
              iVar6 = *(int *)(local_19c + iVar3 * 4);
              *(undefined4 **)(local_19c + iVar3 * 4) = puVar14;
              if (iVar6 != 0) {
                iVar3 = 0;
                if (0 < local_198) {
                  do {
                    if (*(int *)(local_19c + iVar3 * 4) == 0) goto LAB_0043377c;
                    iVar3 = iVar3 + 1;
                  } while (iVar3 < local_198);
                }
                FUN_0043b6e0(&local_19c,iVar3 + 1);
LAB_0043377c:
                *(int *)(local_19c + iVar3 * 4) = iVar6;
              }
            }
            iVar3 = FUN_004326f0(&local_148,(byte *)(local_178 + 0x2c));
            if ((iVar3 == 0) || (piVar8 = (int *)(iVar3 + 0xc), piVar8 == (int *)0x0)) {
              pbVar9 = (byte *)(local_178 + 0x2c);
              if ((local_148 != (void *)0x0) && (*(int *)((int)local_148 + 4) != 0)) {
                FUN_0043b6e0(local_148,0);
              }
              puVar4 = FUN_00432f40(&local_148,&local_134,pbVar9);
              puVar14 = operator_new(8);
              if (puVar14 == (undefined4 *)0x0) {
                puVar14 = (undefined4 *)0x0;
              }
              else {
                *puVar14 = local_1d8;
                puVar14[1] = pcVar1;
              }
              iVar3 = *(int *)(puVar4 + 0x10);
              FUN_0043b6e0(puVar4 + 0xc,iVar3 + 1);
              *(undefined4 **)(*(int *)(puVar4 + 0xc) + iVar3 * 4) = puVar14;
            }
            else {
              iVar6 = *(int *)(iVar3 + 0x10);
              iVar11 = 0;
              if (0 < iVar6) {
                do {
                  if ((iVar11 < 0) || (iVar6 <= iVar11)) {
                    piVar10 = &local_114;
                    for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
                      *piVar10 = 0;
                      piVar10 = piVar10 + 1;
                    }
                    local_114 = 1;
                    local_110 = "Array index out of bounds";
                    local_10c = "..\\BluelAPI/Container/SArray.h";
                    local_108 = 0x1cd;
                    FUN_00422060(&local_114);
                  }
                  uVar15 = FUN_00486710(*(void **)(*(int *)(*piVar8 + iVar11 * 4) + 4),(int)pcVar1);
                  if ((char)uVar15 == '\0') {
                    puVar14 = operator_new(8);
                    if (puVar14 == (undefined4 *)0x0) {
                      FUN_0048f3c0(piVar8,0,iVar11);
                    }
                    else {
                      *puVar14 = local_1d8;
                      puVar14[1] = pcVar1;
                      FUN_0048f3c0(piVar8,puVar14,iVar11);
                    }
                    goto LAB_004338e7;
                  }
                  iVar6 = *(int *)(iVar3 + 0x10);
                  iVar11 = iVar11 + 1;
                } while (iVar11 < iVar6);
              }
              puVar14 = operator_new(8);
              if (puVar14 == (undefined4 *)0x0) {
                puVar14 = (undefined4 *)0x0;
              }
              else {
                *puVar14 = local_1d8;
                puVar14[1] = pcVar1;
              }
              iVar3 = *(int *)(iVar3 + 0x10);
              FUN_0043b6e0(piVar8,iVar3 + 1);
              *(undefined4 **)(*piVar8 + iVar3 * 4) = puVar14;
            }
LAB_004338e7:
            FUN_00432a60((int *)&local_180);
            piVar10 = local_1d4;
            piVar8 = local_1c0;
            iVar11 = local_198;
          }
          local_4 = CONCAT31(local_4._1_3_,5);
          if (1 < *(int *)local_17c) {
            FUN_0041aff0(&local_17c);
          }
          if (*(int *)(local_17c + 4) != 0) {
            FUN_00471760(&local_17c,0);
          }
          if (((int *)local_17c != (int *)0x0) &&
             (*(int *)local_17c = *(int *)local_17c + -1, *(int *)local_17c < 1)) {
            if (*(void **)(local_17c + 0x10) != (void *)0x0) {
              _free(*(void **)(local_17c + 0x10));
            }
            _free((void *)local_17c);
            local_17c = 0;
          }
          local_4._0_1_ = 1;
          if (local_180 != (int *)0x0) {
            FUN_00429fe0((void *)*local_180,(int)&local_180);
            local_180 = (int *)0x0;
          }
        }
        iVar6 = piVar8[1];
        local_1d4 = (int *)((int)piVar10 + 1);
      } while ((int)local_1d4 < iVar6);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar8 = &DAT_00501908;
    local_1c0 = &DAT_00501908;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar10 = &DAT_00501908;
    local_1c8 = &DAT_00501908;
    local_4._0_1_ = 7;
    local_14c = 0;
    local_1d8 = (void *)0x0;
    if (0 < local_198) {
      do {
        pvVar5 = local_1d8;
        if (((int)local_1d8 < 0) || (local_198 <= (int)local_1d8)) {
          piVar13 = &local_114;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar13 = 0;
            piVar13 = piVar13 + 1;
          }
          local_114 = 1;
          local_110 = "Array index out of bounds";
          local_10c = "..\\BluelAPI/Container/SArray.h";
          local_108 = 0x1cd;
          FUN_00422060(&local_114);
        }
        if (*(int *)(local_19c + (int)pvVar5 * 4) != 0) {
          if (((int)pvVar5 < 0) || (local_198 <= (int)pvVar5)) {
            piVar13 = &local_cc;
            for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
              *piVar13 = 0;
              piVar13 = piVar13 + 1;
            }
            local_cc = 1;
            local_c8 = "Array index out of bounds";
            local_c4 = "..\\BluelAPI/Container/SArray.h";
            local_c0 = 0x1cd;
            FUN_00422060(&local_cc);
          }
          iVar3 = FUN_004326f0(&local_148,(byte *)(**(int **)(local_19c + (int)pvVar5 * 4) + 0xc));
          if ((iVar3 != 0) &&
             (piVar13 = (int *)(iVar3 + 0xc), local_1d4 = piVar13, piVar13 != (int *)0x0)) {
            ppiVar7 = &local_1c0;
            if (5 < local_14c) {
              ppiVar7 = &local_1c8;
            }
            local_1d0 = ppiVar7;
            local_14c = local_14c + 1;
            FUN_00401d00(ppiVar7,"^(255,192,0)");
            if (((int)pvVar5 < 0) || (local_198 <= (int)pvVar5)) {
              piVar8 = &local_e4;
              for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                *piVar8 = 0;
                piVar8 = piVar8 + 1;
              }
              local_e4 = 1;
              local_e0 = "Array index out of bounds";
              local_dc = "..\\BluelAPI/Container/SArray.h";
              local_d8 = 0x1cd;
              FUN_00422060(&local_e4);
              piVar13 = local_1d4;
            }
            FUN_00401d00(ppiVar7,(char *)(**(int **)(local_19c + (int)pvVar5 * 4) + 0xc));
            FUN_00401d00(ppiVar7,"^(255,255,255)");
            FUN_00401d00(ppiVar7,"\n");
            iVar3 = piVar13[1];
            local_1cc = &DAT_00000036;
            local_1c4 = 0;
            if (iVar3 < 1) {
LAB_0043441b:
              iVar3 = 2 - local_1c4;
              do {
                FUN_00401d00(ppiVar7,"\n");
                iVar3 = iVar3 + -1;
              } while (iVar3 != 0);
            }
            else {
              do {
                iVar6 = local_1c4;
                pvVar5 = local_1d8;
                ppiVar7 = local_1d0;
                if (1 < local_1c4) goto LAB_00434449;
                if ((local_1c4 < 0) || (iVar3 <= local_1c4)) {
                  piVar8 = &local_fc;
                  for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                    *piVar8 = 0;
                    piVar8 = piVar8 + 1;
                  }
                  local_fc = 1;
                  local_f8 = "Array index out of bounds";
                  local_f4 = "..\\BluelAPI/Container/SArray.h";
                  local_f0 = 0x1cd;
                  FUN_00422060(&local_fc);
                }
                piVar8 = *(int **)(*local_1d4 + iVar6 * 4);
                if (DAT_00501918 == '\0') {
                  DAT_00501918 = '\x01';
                  DAT_00501908 = 2;
                  DAT_0050190c = 0;
                  _DAT_00501910 = 1;
                  DAT_00501914 = 0;
                }
                local_1e0 = &DAT_00501908;
                piVar10 = *(int **)(*piVar8 + 0xc);
                if (piVar10 != &DAT_00501908) {
                  *piVar10 = *piVar10 + 1;
                  local_1e0 = piVar10;
                }
                local_4._0_1_ = 8;
                FUN_0048b590((int *)&local_1e0);
                if (iVar6 == 1) {
                  puVar14 = FUN_00412340(&local_1a8,"^(192,192,192)",(int *)&local_1e0);
                  local_4._0_1_ = 9;
                  if (local_1e0 != (int *)*puVar14) {
                    if ((local_1e0 != &DAT_00501908) &&
                       (*local_1e0 = *local_1e0 + -1, *local_1e0 < 1)) {
                      FUN_004011c0(&local_1e0);
                    }
                    local_1e0 = (int *)*puVar14;
                    if (local_1e0 != &DAT_00501908) {
                      *local_1e0 = *local_1e0 + 1;
                    }
                  }
                  local_4._0_1_ = 8;
                  if (local_1a8 != &DAT_00501908) {
                    *local_1a8 = *local_1a8 + -1;
                    if (*local_1a8 < 1) {
                      FUN_004011c0(&local_1a8);
                    }
                    else {
                      local_1a8 = &DAT_00501908;
                    }
                  }
                  local_1cc = &DAT_00000044;
                }
                if (DAT_00501918 == '\0') {
                  DAT_00501918 = '\x01';
                  DAT_00501908 = 2;
                  DAT_0050190c = 0;
                  _DAT_00501910 = 1;
                  DAT_00501914 = 0;
                }
                local_1dc = &DAT_00501908;
                FUN_00401d80(&local_1dc,"^(192,192,255)");
                local_4._0_1_ = 10;
                piVar8 = FUN_00486ec0((void *)piVar8[1],&local_1a4,(int *)0x0,0,(int *)0x3f800000);
                local_4._0_1_ = 0xb;
                FUN_00401d00(&local_1dc,(char *)(*piVar8 + 0xc));
                piVar8 = local_1a4;
                local_4._0_1_ = 10;
                if (local_1a4 != &DAT_00501908) {
                  *local_1a4 = *local_1a4 + -1;
                  iVar3 = *local_1a4;
                  local_1a4 = &DAT_00501908;
                  if (iVar3 < 1) {
                    if (*piVar8 < 0) {
                      piVar10 = &local_b4;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar10 = 0;
                        piVar10 = piVar10 + 1;
                      }
                      local_b4 = 0;
                      local_b0 = "Invalid reference count";
                      local_ac = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_a8 = 0xd0;
                      FUN_00422060(&local_b4);
                    }
                    _free(piVar8);
                  }
                }
                FUN_00401d00(&local_1dc,"^(255,255,255)");
                if (DAT_00501918 == '\0') {
                  DAT_00501918 = '\x01';
                  DAT_00501908 = 2;
                  DAT_0050190c = 0;
                  _DAT_00501910 = 1;
                  DAT_00501914 = 0;
                }
                local_1b0 = &DAT_00501908;
                FUN_00401d80(&local_1b0," ");
                if (DAT_00501918 == '\0') {
                  DAT_00501918 = '\x01';
                  DAT_00501908 = 2;
                  DAT_0050190c = 0;
                  _DAT_00501910 = 1;
                  DAT_00501914 = 0;
                }
                local_184 = &DAT_00501908;
                local_4._0_1_ = 0xd;
                pvVar5 = AppendCStringToString(&local_1e0,&local_1a0,":");
                piVar8 = local_1dc;
                puVar4 = local_1cc + (-local_1e0[1] - local_1dc[1]) + -1;
                local_4._0_1_ = 0xe;
                piVar10 = &DAT_00501908;
                if (0 < (int)puVar4) {
                  piVar13 = local_1b0 + 3;
                  do {
                    FUN_00401d00(&local_184,(char *)piVar13);
                    puVar4 = puVar4 + -1;
                    piVar10 = local_184;
                  } while (puVar4 != (undefined1 *)0x0);
                }
                pvVar5 = AppendStringObject(pvVar5,&local_1ac,(int *)&local_184);
                local_4._0_1_ = 0xf;
                puVar14 = AppendStringObject(pvVar5,&local_1bc,(int *)&local_1dc);
                local_4._0_1_ = 0x10;
                if (piVar8 != (int *)*puVar14) {
                  if ((piVar8 != &DAT_00501908) &&
                     (iVar3 = *piVar8, *piVar8 = iVar3 + -1, iVar3 + -1 < 1)) {
                    FUN_004011c0(&local_1dc);
                  }
                  piVar8 = (int *)*puVar14;
                  local_1dc = piVar8;
                  if (piVar8 != &DAT_00501908) {
                    *piVar8 = *piVar8 + 1;
                  }
                }
                piVar13 = local_1bc;
                local_4._0_1_ = 0xf;
                if (local_1bc != &DAT_00501908) {
                  *local_1bc = *local_1bc + -1;
                  iVar3 = *local_1bc;
                  local_1bc = &DAT_00501908;
                  if (iVar3 < 1) {
                    if (*piVar13 < 0) {
                      piVar12 = &local_12c;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar12 = 0;
                        piVar12 = piVar12 + 1;
                      }
                      local_12c = 0;
                      local_128 = "Invalid reference count";
                      local_124 = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_120 = 0xd0;
                      FUN_00422060(&local_12c);
                    }
                    _free(piVar13);
                  }
                }
                piVar13 = local_1ac;
                local_4._0_1_ = 0xe;
                if (local_1ac != &DAT_00501908) {
                  *local_1ac = *local_1ac + -1;
                  iVar3 = *local_1ac;
                  local_1ac = &DAT_00501908;
                  if (iVar3 < 1) {
                    if (*piVar13 < 0) {
                      ppiVar7 = &local_180;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *ppiVar7 = (int *)0x0;
                        ppiVar7 = ppiVar7 + 1;
                      }
                      local_180 = (int *)0x0;
                      local_17c = 0x4dc53c;
                      local_178 = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_174 = 0xd0;
                      FUN_00422060((int *)&local_180);
                    }
                    _free(piVar13);
                  }
                }
                piVar13 = local_1a0;
                local_4._0_1_ = 0xd;
                if (local_1a0 != &DAT_00501908) {
                  *local_1a0 = *local_1a0 + -1;
                  iVar3 = *local_1a0;
                  local_1a0 = &DAT_00501908;
                  if (iVar3 < 1) {
                    if (*piVar13 < 0) {
                      piVar12 = &local_24;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar12 = 0;
                        piVar12 = piVar12 + 1;
                      }
                      local_24 = 0;
                      local_20 = "Invalid reference count";
                      local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_18 = 0xd0;
                      FUN_00422060(&local_24);
                    }
                    _free(piVar13);
                  }
                }
                local_4._0_1_ = 0xc;
                if (piVar10 != &DAT_00501908) {
                  iVar3 = *piVar10 + -1;
                  *piVar10 = iVar3;
                  if (iVar3 < 1) {
                    if (iVar3 < 0) {
                      piVar13 = &local_54;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar13 = 0;
                        piVar13 = piVar13 + 1;
                      }
                      local_54 = 0;
                      local_50 = "Invalid reference count";
                      local_4c = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_48 = 0xd0;
                      FUN_00422060(&local_54);
                    }
                    _free(piVar10);
                  }
                }
                local_4._0_1_ = 10;
                if (local_1b0 != &DAT_00501908) {
                  iVar3 = *local_1b0 + -1;
                  *local_1b0 = iVar3;
                  if (iVar3 < 1) {
                    if (iVar3 < 0) {
                      piVar10 = &local_9c;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar10 = 0;
                        piVar10 = piVar10 + 1;
                      }
                      local_9c = 0;
                      local_98 = "Invalid reference count";
                      local_94 = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_90 = 0xd0;
                      FUN_00422060(&local_9c);
                    }
                    _free(local_1b0);
                  }
                }
                pvVar5 = FUN_00412340(&local_1b4,"  ",(int *)&local_1dc);
                local_4._0_1_ = 0x11;
                piVar10 = AppendCStringToString(pvVar5,&local_1b8,"\n");
                local_4._0_1_ = 0x12;
                FUN_00401d00(local_1d0,(char *)(*piVar10 + 0xc));
                piVar10 = local_1b8;
                local_4._0_1_ = 0x11;
                if (local_1b8 != &DAT_00501908) {
                  *local_1b8 = *local_1b8 + -1;
                  iVar3 = *local_1b8;
                  local_1b8 = &DAT_00501908;
                  if (iVar3 < 1) {
                    if (*piVar10 < 0) {
                      piVar13 = &local_6c;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar13 = 0;
                        piVar13 = piVar13 + 1;
                      }
                      local_6c = 0;
                      local_68 = "Invalid reference count";
                      local_64 = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_60 = 0xd0;
                      FUN_00422060(&local_6c);
                    }
                    _free(piVar10);
                  }
                }
                piVar10 = local_1b4;
                local_4._0_1_ = 10;
                if (local_1b4 != &DAT_00501908) {
                  *local_1b4 = *local_1b4 + -1;
                  iVar3 = *local_1b4;
                  local_1b4 = &DAT_00501908;
                  if (iVar3 < 1) {
                    if (*piVar10 < 0) {
                      piVar13 = &local_3c;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar13 = 0;
                        piVar13 = piVar13 + 1;
                      }
                      local_3c = 0;
                      local_38 = "Invalid reference count";
                      local_34 = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_30 = 0xd0;
                      FUN_00422060(&local_3c);
                    }
                    _free(piVar10);
                  }
                }
                local_4._0_1_ = 8;
                if (piVar8 != &DAT_00501908) {
                  iVar3 = *piVar8 + -1;
                  *piVar8 = iVar3;
                  if (iVar3 < 1) {
                    if (iVar3 < 0) {
                      piVar10 = &local_84;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar10 = 0;
                        piVar10 = piVar10 + 1;
                      }
                      local_84 = 0;
                      local_80 = "Invalid reference count";
                      local_7c = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_78 = 0xd0;
                      FUN_00422060(&local_84);
                    }
                    _free(piVar8);
                  }
                }
                piVar8 = local_1e0;
                local_4._0_1_ = 7;
                if (local_1e0 != &DAT_00501908) {
                  *local_1e0 = *local_1e0 + -1;
                  iVar3 = *local_1e0;
                  local_1e0 = &DAT_00501908;
                  if (iVar3 < 1) {
                    if (*piVar8 < 0) {
                      piVar10 = &local_168;
                      for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                        *piVar10 = 0;
                        piVar10 = piVar10 + 1;
                      }
                      local_168 = 0;
                      local_164 = "Invalid reference count";
                      local_160 = "..\\BluelAPI/String/Rope_RatStr.h";
                      local_15c = 0xd0;
                      FUN_00422060(&local_168);
                    }
                    _free(piVar8);
                  }
                }
                iVar3 = local_1d4[1];
                local_1c4 = local_1c4 + 1;
              } while (local_1c4 < iVar3);
              pvVar5 = local_1d8;
              ppiVar7 = local_1d0;
              if (local_1c4 < 2) goto LAB_0043441b;
            }
LAB_00434449:
            FUN_00401d00(ppiVar7,"\n");
            piVar8 = local_1c0;
            piVar10 = local_1c8;
          }
        }
        local_1d8 = (void *)((int)pvVar5 + 1);
      } while ((int)local_1d8 < local_198);
    }
    puVar4 = local_150;
    local_1cc = &stack0xfffffe00;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar13 = &DAT_00501908;
    if (piVar8 != &DAT_00501908) {
      *piVar8 = *piVar8 + 1;
      piVar13 = piVar8;
    }
    FUN_0048c780((void *)((int)local_150 + 0x14c),piVar13,(undefined4 *)&DAT_004ff8c0,0,0);
    local_150 = &stack0xfffffe00;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar13 = &DAT_00501908;
    if (piVar10 != &DAT_00501908) {
      *piVar10 = *piVar10 + 1;
      piVar13 = piVar10;
    }
    FUN_0048c780((void *)((int)puVar4 + 0x250),piVar13,(undefined4 *)&DAT_004ff8d0,0,0);
    local_4._0_1_ = 6;
    if (piVar10 != &DAT_00501908) {
      iVar3 = *piVar10 + -1;
      *piVar10 = iVar3;
      if (iVar3 < 1) {
        if (iVar3 < 0) {
          piVar13 = &local_168;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar13 = 0;
            piVar13 = piVar13 + 1;
          }
          local_168 = 0;
          local_164 = "Invalid reference count";
          local_160 = "..\\BluelAPI/String/Rope_RatStr.h";
          local_15c = 0xd0;
          FUN_00422060(&local_168);
        }
        _free(piVar10);
      }
    }
    local_4._0_1_ = 1;
    if (piVar8 != &DAT_00501908) {
      iVar3 = *piVar8 + -1;
      *piVar8 = iVar3;
      if (iVar3 < 1) {
        if (iVar3 < 0) {
          piVar10 = &local_168;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar10 = 0;
            piVar10 = piVar10 + 1;
          }
          local_168 = 0;
          local_164 = "Invalid reference count";
          local_160 = "..\\BluelAPI/String/Rope_RatStr.h";
          local_15c = 0xd0;
          FUN_00422060(&local_168);
        }
        _free(piVar8);
      }
    }
    local_4._0_1_ = 0x13;
    FUN_004331f0(&local_148);
    pvVar5 = local_148;
    local_4 = (uint)local_4._1_3_ << 8;
    if (local_148 != (void *)0x0) {
      if (*(int *)((int)local_148 + 4) != 0) {
        FUN_0043b6e0(local_148,0);
      }
      _free(pvVar5);
      local_148 = (void *)0x0;
    }
    local_4 = 0xffffffff;
    if (local_198 != 0) {
      FUN_0043b6e0(&local_19c,0);
    }
  }
  ExceptionList = local_c;
  return;
}
