/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00446510
 * Match: function entry
 * Entry: 00446510 FUN_00446510
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __fastcall FUN_00446510(int param_1)

{
  int iVar1;
  void *pvVar2;
  float10 fVar3;
  undefined4 *puVar4;
  float fVar5;
  undefined4 uVar6;
  float fVar7;
  float fVar8;
  undefined4 uVar9;
  undefined4 local_c [3];
  
  if ((*(char *)(param_1 + 0x30) != '\0') && (*(uint *)(param_1 + 0x168) <= DAT_00502afc)) {
    *(uint *)(param_1 + 0x168) = DAT_00502afc + 99999999;
    fVar3 = FUN_00498b30(-40.0,40.0);
    fVar8 = (float)fVar3;
    iVar1 = FUN_0047f1b0(param_1);
    fVar3 = FUN_00498b30(0.0,10.0);
    fVar7 = (float)(fVar3 + (float10)*(float *)(iVar1 + 4));
    fVar3 = FUN_00498b30(-40.0,40.0);
    fVar5 = (float)fVar3;
    puVar4 = local_c;
    pvVar2 = (void *)FUN_0047f1b0(param_1);
    FUN_0042d280(pvVar2,puVar4,fVar5,fVar7,fVar8);
    uVar9 = 0;
    fVar3 = FUN_00498b30(-30.0,-5.0);
    fVar8 = (float)fVar3;
    uVar6 = 0;
    puVar4 = local_c;
    pvVar2 = (void *)FUN_0047f1c0(param_1);
    FUN_0042d280(pvVar2,puVar4,uVar6,fVar8,uVar9);
  }
  return;
}


