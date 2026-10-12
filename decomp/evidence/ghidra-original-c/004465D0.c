/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 004465d0
 * Match: function entry
 * Entry: 004465d0 FUN_004465d0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_004465d0(void *this,undefined4 param_1,undefined4 param_2)

{
  int *this_00;
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  void *pvVar5;
  int iVar6;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf6bb;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x50) == 0) {
    ExceptionList = &local_c;
    *(undefined4 *)((int)this + 0x50) = param_1;
    *(undefined4 *)((int)this + 0x58) = param_2;
    iVar6 = 200;
    iVar3 = 2;
    pvVar5 = this;
    do {
      local_48 = 0x3f666666;
      local_44 = 0x3dcccccd;
      local_40 = 0;
      FUN_0047db10(pvVar5,iVar6,0x3f000000,0x3f800000,0x3e4ccccd,&local_48);
      local_3c = 0x3f666666;
      local_38 = 0x3f000000;
      local_34 = 0;
      FUN_0047db10(pvVar5,iVar6,0x3f000000,0x3f800000,0x3e4ccccd,&local_3c);
      local_30 = 0x3f000000;
      local_2c = 0x3f000000;
      local_28 = 0x3f000000;
      FUN_0047db10(pvVar5,iVar6,0x3f000000,0x3f800000,0x3e4ccccd,&local_30);
      local_24 = 0;
      local_20 = 0;
      local_1c = 0;
      FUN_0047db10(pvVar5,iVar6,0x3f800000,0x3f800000,0x3e4ccccd,&local_24);
      local_18 = 0;
      local_14 = 0;
      local_10 = 0;
      FUN_0047db10(pvVar5,iVar6,0x3f800000,0,0x3e4ccccd,&local_18);
      *(uint *)((int)pvVar5 + 0x18) = *(uint *)((int)pvVar5 + 0x18) & 0xfffffffe | 8;
      iVar6 = iVar6 + 1;
      pvVar5 = (void *)((int)pvVar5 + 0x1c);
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    uVar4 = 0;
    this_00 = (int *)((int)this + 0x38);
    do {
      pvVar5 = operator_new(0x16c);
      local_4 = 0;
      if (pvVar5 == (void *)0x0) {
        puVar1 = (undefined4 *)0x0;
      }
      else {
        puVar1 = FUN_004463f0(pvVar5,(int)((uVar4 & 1) * 0x1c + (int)this));
      }
      iVar3 = 0;
      local_4 = 0xffffffff;
      if (0 < *(int *)((int)this + 0x3c)) {
        piVar2 = (int *)*this_00;
        do {
          if (*piVar2 == 0) goto LAB_004467a8;
          iVar3 = iVar3 + 1;
          piVar2 = piVar2 + 1;
        } while (iVar3 < *(int *)((int)this + 0x3c));
      }
      FUN_0043b6e0(this_00,iVar3 + 1);
LAB_004467a8:
      uVar4 = uVar4 + 1;
      *(undefined4 **)(*this_00 + iVar3 * 4) = puVar1;
    } while ((int)uVar4 < 100);
    *(undefined4 *)((int)this + 0x54) = 0x3f800000;
  }
  ExceptionList = local_c;
  return;
}


