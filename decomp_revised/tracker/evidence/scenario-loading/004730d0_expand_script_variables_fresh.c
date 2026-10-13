/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Expands global script-variable references written as `*name` inside a command string.
   
   Algorithm: Search the current text for `*`, extract a variable token using
   whitespace/comma/parenthesis/bracket delimiters, and look it up in DAT_00503044 without creating
   a missing entry. When found, rebuild the command text with the stored value in place of the
   reference and continue scanning; when absent, leave that token untouched and continue.
   
   Parameters: param_1 points to the reference-counted command string being rewritten.
   
   Returns: void; the supplied string reference is updated when substitutions occur.
   
   Evidence: Fresh decompilation at 0x004730D0; called at the start of ParseScriptActorCommandText
   (0x00474180). The expansion path uses AppendStringObject (0x004040E0). */

void ExpandScriptVariableReferences(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  void *this;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  int *local_64;
  int *local_60;
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
  int local_4;
  
  puStack_8 = &LAB_004d29a0;
  local_c = ExceptionList;
  local_58 = (int *)0x0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar5 = &DAT_00501908;
  local_60 = &DAT_00501908;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_5c = &DAT_00501908;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar4 = &DAT_00501908;
  local_44 = &DAT_00501908;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_64 = &DAT_00501908;
  local_4 = 3;
  ExceptionList = &local_c;
LAB_004731b1:
  do {
    while( true ) {
      if (piVar5 != (int *)*param_1) {
        if ((piVar5 != &DAT_00501908) && (iVar3 = *piVar5, *piVar5 = iVar3 + -1, iVar3 + -1 < 1)) {
          FUN_004011c0(&local_64);
        }
        piVar5 = (int *)*param_1;
        local_64 = piVar5;
        if (piVar5 != &DAT_00501908) {
          *piVar5 = *piVar5 + 1;
        }
      }
      local_58 = (int *)FUN_00404c80(&local_64,"*",(int)local_58,'\x01');
      if ((int)local_58 < 0) {
        local_4._0_1_ = 2;
        if ((piVar5 != &DAT_00501908) && (iVar3 = *piVar5, *piVar5 = iVar3 + -1, iVar3 + -1 < 1)) {
          FUN_004011c0(&local_64);
        }
        local_4._0_1_ = 1;
        if ((piVar4 != &DAT_00501908) && (iVar3 = *piVar4, *piVar4 = iVar3 + -1, iVar3 + -1 < 1)) {
          FUN_004011c0(&local_44);
        }
        local_4 = (uint)local_4._1_3_ << 8;
        if ((local_5c != &DAT_00501908) &&
           (iVar3 = *local_5c, *local_5c = iVar3 + -1, iVar3 + -1 < 1)) {
          FUN_004011c0(&local_5c);
        }
        local_4 = 0xffffffff;
        if ((local_60 != &DAT_00501908) &&
           (iVar3 = *local_60, *local_60 = iVar3 + -1, iVar3 + -1 < 1)) {
          FUN_004011c0(&local_60);
        }
        ExceptionList = local_c;
        return;
      }
      puVar1 = FUN_00404e50(&local_64,&local_54,local_58);
      local_4._0_1_ = 4;
      if (local_60 != (int *)*puVar1) {
        if ((local_60 != &DAT_00501908) &&
           (iVar3 = *local_60, *local_60 = iVar3 + -1, iVar3 + -1 < 1)) {
          FUN_004011c0(&local_60);
        }
        local_60 = (int *)*puVar1;
        if (local_60 != &DAT_00501908) {
          *local_60 = *local_60 + 1;
        }
      }
      piVar5 = local_54;
      local_4._0_1_ = 3;
      if (local_54 != &DAT_00501908) {
        *local_54 = *local_54 + -1;
        iVar3 = *local_54;
        local_54 = &DAT_00501908;
        if (iVar3 < 1) {
          if (*piVar5 < 0) {
            piVar6 = &local_3c;
            for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
              *piVar6 = 0;
              piVar6 = piVar6 + 1;
            }
            local_3c = 0;
            local_38 = "Invalid reference count";
            local_34 = "..\\BluelAPI/String/Rope_RatStr.h";
            local_30 = 0xd0;
            FUN_00422060(&local_3c);
          }
          _free(piVar5);
        }
      }
      FUN_00405af0(&local_64,(int *)0x1);
      puVar1 = FUN_004172c0(&local_64,&local_50,(int *)0x0," ,([])",(int *)0x0);
      local_4._0_1_ = 5;
      if (piVar4 != (int *)*puVar1) {
        if (piVar4 != &DAT_00501908) {
          iVar3 = *piVar4;
          *piVar4 = iVar3 + -1;
          if (iVar3 + -1 < 1) {
            FUN_004011c0(&local_44);
          }
        }
        piVar4 = (int *)*puVar1;
        local_44 = piVar4;
        if (piVar4 != &DAT_00501908) {
          *piVar4 = *piVar4 + 1;
        }
      }
      piVar5 = local_50;
      local_4 = CONCAT31(local_4._1_3_,3);
      if (local_50 != &DAT_00501908) {
        *local_50 = *local_50 + -1;
        iVar3 = *local_50;
        local_50 = &DAT_00501908;
        if (iVar3 < 1) {
          if (*piVar5 < 0) {
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
      piVar5 = local_64;
      if (local_5c != local_64) {
        if (local_5c != &DAT_00501908) {
          iVar3 = *local_5c;
          *local_5c = iVar3 + -1;
          if (iVar3 + -1 < 1) {
            FUN_004011c0(&local_5c);
          }
        }
        local_5c = piVar5;
        if (piVar5 != &DAT_00501908) {
          *piVar5 = *piVar5 + 1;
        }
      }
      local_40 = &stack0xffffff84;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      if (piVar4 != &DAT_00501908) {
        *piVar4 = *piVar4 + 1;
        piVar6 = piVar4;
      }
      puVar2 = FindScriptVariableEntry(&DAT_00503044,piVar6,(int *)0x0);
      if (puVar2 != (undefined1 *)0x0) break;
      local_58 = (int *)((int)local_58 + 1);
    }
    this = FUN_004040e0(&local_60,&local_48,(int *)(puVar2 + 4));
    local_4._0_1_ = 6;
    puVar1 = FUN_004040e0(this,&local_4c,(int *)&local_5c);
    piVar6 = (int *)*param_1;
    local_4._0_1_ = 7;
    if (piVar6 != (int *)*puVar1) {
      if (piVar6 != &DAT_00501908) {
        *piVar6 = *piVar6 + -1;
        if (*(int *)*param_1 < 1) {
          FUN_004011c0(param_1);
        }
        else {
          *param_1 = &DAT_00501908;
        }
      }
      piVar6 = (int *)*puVar1;
      *param_1 = piVar6;
      if (piVar6 != &DAT_00501908) {
        *piVar6 = *piVar6 + 1;
      }
    }
    local_4._0_1_ = 6;
    if (local_4c != &DAT_00501908) {
      *local_4c = *local_4c + -1;
      if (*local_4c < 1) {
        FUN_004011c0(&local_4c);
      }
      else {
        local_4c = &DAT_00501908;
      }
    }
    local_4 = CONCAT31(local_4._1_3_,3);
    if (local_48 != &DAT_00501908) {
      *local_48 = *local_48 + -1;
      if (0 < *local_48) {
        local_48 = &DAT_00501908;
        local_58 = (int *)0x0;
        goto LAB_004731b1;
      }
      FUN_004011c0(&local_48);
    }
    local_58 = (int *)0x0;
  } while( true );
}
