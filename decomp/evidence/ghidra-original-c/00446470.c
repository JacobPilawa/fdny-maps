/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00446470
 * Match: function entry
 * Entry: 00446470 FUN_00446470
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_00446470(void *this,undefined4 *param_1)

{
  int iVar1;
  void *this_00;
  float10 fVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  float fVar5;
  undefined4 uVar6;
  undefined4 local_c [3];
  
  iVar1 = FUN_0047f1a0((int)this);
  *(undefined4 *)(iVar1 + 0x28) = *param_1;
  *(undefined4 *)(iVar1 + 0x2c) = param_1[1];
  *(undefined4 *)(iVar1 + 0x30) = param_1[2];
  FUN_0047d300(this,0);
  fVar2 = FUN_00498b30(0.5,1.5);
  *(float *)((int)this + 0x1c) = (float)fVar2;
  *(undefined1 *)((int)this + 0x30) = 1;
  fVar2 = FUN_00498b30(0.5,1.5);
  *(float *)((int)this + 0xa8) = (float)fVar2;
  uVar6 = 0;
  *(undefined4 *)((int)this + 0x168) = 0;
  fVar2 = FUN_00498b30(-80.0,-10.0);
  fVar5 = (float)fVar2;
  uVar4 = 0;
  puVar3 = local_c;
  this_00 = (void *)FUN_0047f1b0((int)this);
  FUN_0042d280(this_00,puVar3,uVar4,fVar5,uVar6);
  return;
}


