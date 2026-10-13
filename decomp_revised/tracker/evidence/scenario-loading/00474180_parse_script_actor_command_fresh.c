/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Parses command text into the ScriptActor's internal command/event representation.
   
   Algorithm: Expand global `*name` references first, normalize/trim the resulting text, then parse
   the command tokens, event markers, nested lists, and arguments into the output parser object. The
   completed object is returned to RouteParsedScriptActorCommand.
   
   Parameters: this is the output parser object; param_1 is the reference-counted source command
   string.
   
   Returns: this, populated with the parsed command data.
   
   Evidence: Fresh decompilation at 0x00474180; RouteParsedScriptActorCommand (0x00470150) calls it
   before common command dispatch. */

int * __thiscall ParseScriptActorCommandText(void *this,int *param_1)

{
  char cVar1;
  undefined4 *puVar2;
  void *this_00;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int *piVar7;
  bool bVar8;
  char *pcVar9;
  char local_5d;
  int *local_5c;
  int *local_58;
  int *local_54;
  int *local_50;
  int *local_4c;
  int *local_48;
  int *local_44;
  undefined1 *local_40;
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
  undefined4 local_4;
  
  puStack_8 = &LAB_004d2adb;
  local_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  *(undefined4 **)this = &DAT_00501908;
  *(undefined4 *)((int)this + 0x18) = 0x10;
  *(code **)((int)this + 0x14) = FUN_00474160;
  *(undefined4 *)((int)this + 4) = 0;
  *(undefined4 *)((int)this + 8) = 0;
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined4 *)((int)this + 0x10) = 0;
  local_4 = 2;
  local_58 = this;
  ExpandScriptVariableReferences(&param_1);
  FUN_004052b0((int *)&param_1);
  if ((((char)param_1[3] == '(') && (*(char *)(param_1[1] + 0xb + (int)param_1) == ')')) &&
     (FUN_00405af0(&param_1,(int *)0x1), (char)param_1[3] != '\0')) {
    if (param_1[1] < 2) {
      if (param_1 != &DAT_00501908) {
        iVar3 = *param_1;
        *param_1 = iVar3 + -1;
        if (iVar3 + -1 < 1) {
          FUN_004011c0(&param_1);
        }
        else {
          param_1 = &DAT_00501908;
        }
      }
    }
    else {
      FUN_00403b50(&param_1,param_1[1] + -1);
    }
  }
  puVar2 = FUN_004737a0(&local_48,(int *)&param_1,(int *)&local_5d);
  piVar5 = *(int **)this;
  local_4._0_1_ = 3;
  if (piVar5 != (int *)*puVar2) {
    if (piVar5 != &DAT_00501908) {
      *piVar5 = *piVar5 + -1;
      if (**(int **)this < 1) {
        FUN_004011c0(this);
      }
      else {
        *(undefined4 **)this = &DAT_00501908;
      }
    }
    piVar5 = (int *)*puVar2;
    *(int **)this = piVar5;
    if (piVar5 != &DAT_00501908) {
      *piVar5 = *piVar5 + 1;
    }
  }
  local_4._0_1_ = 2;
  if ((local_48 != &DAT_00501908) && (*local_48 = *local_48 + -1, *local_48 < 1)) {
    FUN_004011c0(&local_48);
  }
  iVar3 = *(int *)(*(int *)this + 4);
  if (iVar3 == 0) goto LAB_00474892;
  cVar1 = *(char *)(*(int *)this + 0xb + iVar3);
  piVar5 = (int *)0x0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_5c = &DAT_00501908;
  local_4._0_1_ = 4;
  if (cVar1 == ':') {
    pcVar9 = "onmsgappend";
LAB_0047434e:
    FUN_00401d80(&local_5c,pcVar9);
  }
  else if (cVar1 == ';') {
    pcVar9 = "onmsg";
    goto LAB_0047434e;
  }
  if (local_5c[1] != 0) {
    piVar5 = *(int **)this;
    local_44 = (int *)&stack0xffffff8c;
    FUN_00403cb0(this,(undefined4 *)&stack0xffffff8c,(int *)(piVar5[1] + -1));
    FUN_00474020(this,0,piVar5);
    FUN_00403b00(this,&local_5c);
    FUN_004052b0((int *)&param_1);
    if ((char)param_1[3] != '[') {
      this_00 = FUN_00412340(&local_48,"(",(int *)&param_1);
      local_4._0_1_ = 5;
      puVar2 = FUN_00403e40(this_00,&local_54,")");
      local_4._0_1_ = 6;
      FUN_00403b00(&param_1,puVar2);
      local_4._0_1_ = 5;
      if (local_54 != &DAT_00501908) {
        *local_54 = *local_54 + -1;
        if (*local_54 < 1) {
          FUN_004011c0(&local_54);
        }
        else {
          local_54 = &DAT_00501908;
        }
      }
      local_4._0_1_ = 4;
      if ((local_48 != &DAT_00501908) && (*local_48 = *local_48 + -1, *local_48 < 1)) {
        FUN_004011c0(&local_48);
      }
    }
    piVar5 = (int *)0x1;
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar6 = &DAT_00501908;
  local_4c = &DAT_00501908;
  iVar3 = param_1[1];
  local_4._0_1_ = 7;
  local_48 = piVar5;
  piVar5 = local_58;
  while (local_58 = piVar5, iVar3 != 0) {
    puVar2 = FUN_004737a0(&local_54,(int *)&param_1,(int *)&local_5d);
    local_4._0_1_ = 8;
    if (piVar6 != (int *)*puVar2) {
      if ((piVar6 != &DAT_00501908) && (iVar3 = *piVar6, *piVar6 = iVar3 + -1, iVar3 + -1 < 1)) {
        FUN_004011c0(&local_4c);
      }
      piVar6 = (int *)*puVar2;
      local_4c = piVar6;
      if (piVar6 != &DAT_00501908) {
        *piVar6 = *piVar6 + 1;
      }
    }
    local_4._0_1_ = 7;
    if (local_54 != &DAT_00501908) {
      *local_54 = *local_54 + -1;
      if (*local_54 < 1) {
        FUN_004011c0(&local_54);
      }
      else {
        local_54 = &DAT_00501908;
      }
    }
    if (local_5d == '[') {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar5 = &DAT_00501908;
      local_44 = &DAT_00501908;
      iVar3 = piVar6[1];
      local_4._0_1_ = 9;
      while (iVar3 != 0) {
        puVar2 = FUN_00473570(&local_50,(int *)&local_4c);
        local_4._0_1_ = 10;
        if (piVar5 != (int *)*puVar2) {
          if ((piVar5 != &DAT_00501908) && (iVar3 = *piVar5, *piVar5 = iVar3 + -1, iVar3 + -1 < 1))
          {
            FUN_004011c0(&local_44);
          }
          piVar5 = (int *)*puVar2;
          local_44 = piVar5;
          if (piVar5 != &DAT_00501908) {
            *piVar5 = *piVar5 + 1;
          }
        }
        piVar6 = local_50;
        local_4._0_1_ = 9;
        if (local_50 != &DAT_00501908) {
          *local_50 = *local_50 + -1;
          iVar3 = *local_50;
          local_50 = &DAT_00501908;
          if (iVar3 < 1) {
            if (*piVar6 < 0) {
              piVar7 = &local_3c;
              for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
                *piVar7 = 0;
                piVar7 = piVar7 + 1;
              }
              local_3c = 0;
              local_38 = "Invalid reference count";
              local_34 = "..\\BluelAPI/String/Rope_RatStr.h";
              local_30 = 0xd0;
              FUN_00422060(&local_3c);
            }
            _free(piVar6);
          }
        }
        iVar3 = 2;
        bVar8 = true;
        piVar6 = piVar5 + 3;
        pcVar9 = ",";
        do {
          if (iVar3 == 0) break;
          iVar3 = iVar3 + -1;
          bVar8 = (char)*piVar6 == *pcVar9;
          piVar6 = (int *)((int)piVar6 + 1);
          pcVar9 = pcVar9 + 1;
        } while (bVar8);
        if (!bVar8) {
          local_40 = &stack0xffffff8c;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar6 = &DAT_00501908;
          if (piVar5 != &DAT_00501908) {
            *piVar5 = *piVar5 + 1;
            piVar6 = piVar5;
          }
          FUN_00474020(local_58,(int)local_48,piVar6);
        }
        piVar6 = local_4c;
        iVar3 = local_4c[1];
      }
      local_4._0_1_ = 7;
      if (piVar5 != &DAT_00501908) {
        iVar3 = *piVar5 + -1;
        *piVar5 = iVar3;
        if (iVar3 < 1) {
          if (iVar3 < 0) {
            piVar7 = &local_24;
            for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
              *piVar7 = 0;
              piVar7 = piVar7 + 1;
            }
            local_24 = 0;
            local_20 = "Invalid reference count";
            local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
            local_18 = 0xd0;
            FUN_00422060(&local_24);
          }
          _free(piVar5);
        }
      }
    }
    else {
      local_40 = &stack0xffffff8c;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      if (piVar6 != &DAT_00501908) {
        *piVar6 = *piVar6 + 1;
        piVar7 = piVar6;
      }
      FUN_00474020(piVar5,(int)local_48,piVar7);
    }
    local_48 = (int *)((int)local_48 + 1);
    piVar5 = local_58;
    iVar3 = param_1[1];
  }
  iVar3 = 0;
  piVar5[7] = 1;
  if (0 < piVar5[2]) {
    do {
      if ((iVar3 < 0) || (piVar5[2] <= iVar3)) {
        piVar7 = &local_24;
        for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
          *piVar7 = 0;
          piVar7 = piVar7 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      piVar5[7] = piVar5[7] * *(int *)(*(int *)(*(int *)(piVar5[1] + iVar3 * 4) + 4) + 4);
      iVar3 = iVar3 + 1;
    } while (iVar3 < piVar5[2]);
  }
  piVar5[8] = piVar5[2];
  local_4._0_1_ = 4;
  if (piVar6 != &DAT_00501908) {
    iVar3 = *piVar6 + -1;
    *piVar6 = iVar3;
    if (iVar3 < 1) {
      if (iVar3 < 0) {
        piVar5 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_24 = 0;
        local_20 = "Invalid reference count";
        local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
        local_18 = 0xd0;
        FUN_00422060(&local_24);
      }
      _free(piVar6);
    }
  }
  local_4 = CONCAT31(local_4._1_3_,2);
  if (local_5c != &DAT_00501908) {
    iVar3 = *local_5c + -1;
    *local_5c = iVar3;
    if (iVar3 < 1) {
      if (iVar3 < 0) {
        piVar5 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_24 = 0;
        local_20 = "Invalid reference count";
        local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
        local_18 = 0xd0;
        FUN_00422060(&local_24);
      }
      _free(local_5c);
    }
  }
LAB_00474892:
  piVar5 = param_1;
  local_4 = 0xffffffff;
  if (param_1 != &DAT_00501908) {
    iVar3 = *param_1 + -1;
    *param_1 = iVar3;
    if (iVar3 < 1) {
      if (iVar3 < 0) {
        piVar6 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar6 = 0;
          piVar6 = piVar6 + 1;
        }
        local_24 = 0;
        local_20 = "Invalid reference count";
        local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
        local_18 = 0xd0;
        FUN_00422060(&local_24);
      }
      _free(piVar5);
    }
  }
  ExceptionList = local_c;
  return local_58;
}
