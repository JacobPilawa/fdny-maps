/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0043f810
 * Match: scratch-defined function boundary
 * Entry: 0043f810 FUN_0043f810
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0043f810(void *param_1,int param_2)

{
  undefined4 *puVar1;
  float *this;
  undefined4 *puVar2;
  undefined4 uVar3;
  float *pfVar4;
  float *pfVar5;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  float afStack_60 [3];
  float fStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  float afStack_3c [6];
  float afStack_24 [3];
  float afStack_18 [3];
  float afStack_c [3];
  
  *(undefined1 *)((int)param_1 + 0x13c) = 0;
  if ((*(char *)((int)param_1 + 0x130) == '\0') &&
     (*(float *)(param_2 + 0x10) <= (float)_DAT_004e0eb8)) {
    uStack_50 = *(undefined4 *)((int)param_1 + 0x138);
    puVar2 = (undefined4 *)((int)param_1 + 0x84);
    puVar1 = (undefined4 *)((int)param_1 + 0x78);
    uStack_6c = 0;
    uStack_68 = 0;
    uStack_64 = 0;
    uStack_48 = 0;
    uStack_44 = 0;
    uStack_40 = 0;
    fStack_54 = 0.0;
    uStack_4c = 0;
    this = (float *)((int)param_1 + 0x68);
    uVar3 = FUN_00492ce0(*(void **)(DAT_00503a78 + 4),this,puVar1,puVar2,&fStack_54,&uStack_6c,
                         &uStack_48);
    if ((char)uVar3 == '\0') {
      FUN_0042d050(afStack_3c + 3,this,&fStack_54);
      afStack_60[0] = *(float *)((int)param_1 + 0x90);
      afStack_60[2] = *(float *)((int)param_1 + 0x98);
      afStack_60[1] = 0.0;
      uVar3 = FUN_00492ce0(*(void **)(DAT_00503a78 + 4),afStack_3c + 3,puVar1,puVar2,afStack_60,
                           &uStack_6c,&uStack_48);
      if ((char)uVar3 != '\0') {
        FUN_0049e0a0(param_1,param_2);
        return;
      }
      FUN_0042d050(afStack_18,afStack_3c + 3,afStack_60);
      pfVar5 = afStack_3c;
      afStack_3c[0] = 0.0;
      afStack_3c[1] = -1.0;
      afStack_3c[2] = 0.0;
      pfVar4 = (float *)FUN_0042d2b0(&fStack_54,afStack_c);
      FUN_0042d050(afStack_24,pfVar4,pfVar5);
      uVar3 = FUN_00492ce0(*(void **)(DAT_00503a78 + 4),afStack_18,puVar1,puVar2,afStack_24,
                           &uStack_6c,&uStack_48);
      if ((char)uVar3 != '\0') {
        *(undefined1 *)((int)param_1 + 0x130) = 1;
        *(undefined1 *)((int)param_1 + 0x13c) = 1;
        *(undefined4 *)((int)param_1 + 0x4c) = uStack_6c;
        *(undefined4 *)((int)param_1 + 0x50) = uStack_68;
        *(undefined4 *)((int)param_1 + 0x54) = uStack_64;
        FUN_00441e90(this,(undefined4 *)((int)param_1 + 0x4c));
        return;
      }
    }
  }
  FUN_0049e0a0(param_1,param_2);
  return;
}


