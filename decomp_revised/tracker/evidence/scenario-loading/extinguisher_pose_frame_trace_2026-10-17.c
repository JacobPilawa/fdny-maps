/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x004612b0 FUN_004612b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004612b0(void *this,undefined4 *param_1,float *param_2)

{
  undefined4 uVar1;
  float *pfVar2;
  undefined4 *puVar3;
  float fVar4;
  float local_18 [3];
  float local_c [3];
  
  *(undefined4 *)((int)this + 0x80) = *param_1;
  *(undefined4 *)((int)this + 0x84) = param_1[1];
  *(undefined4 *)((int)this + 0x88) = param_1[2];
  if ((DAT_00502e2c & 1) == 0) {
    DAT_00502e2c = DAT_00502e2c | 1;
    _DAT_00502e20 = 0;
    DAT_00502e24 = 0;
    DAT_00502e28 = 0;
    _atexit((_func_4879 *)&DAT_004db7b0);
  }
  if ((DAT_00502e2c & 2) == 0) {
    DAT_00502e2c = DAT_00502e2c | 2;
    DAT_00502e14 = 0;
    DAT_00502e18 = 0;
    DAT_00502e1c = 0;
    _atexit((_func_4879 *)&DAT_004db7a0);
  }
  if ((DAT_00502e2c & 4) == 0) {
    DAT_00502e2c = DAT_00502e2c | 4;
    DAT_00502e08 = 0;
    DAT_00502e0c = 0;
    DAT_00502e10 = 0;
    _atexit((_func_4879 *)&DAT_004db790);
  }
  FUN_0042d280(&DAT_00502e08,local_18,0,0x3f800000,0);
  fVar4 = (float)_DAT_004dd808 /
          SQRT(param_2[2] * param_2[2] + param_2[1] * param_2[1] + *param_2 * *param_2);
  pfVar2 = (float *)FUN_0042d2b0(param_2,local_18);
  puVar3 = (undefined4 *)FUN_00446390(local_c,pfVar2,fVar4);
  DAT_00502e14 = *puVar3;
  DAT_00502e18 = puVar3[1];
  DAT_00502e1c = puVar3[2];
  puVar3 = (undefined4 *)FUN_004476b0(local_c,(float *)&DAT_00502e08,(float *)&DAT_00502e14);
  uVar1 = *puVar3;
  DAT_00502e24 = puVar3[1];
  DAT_00502e28 = puVar3[2];
  _DAT_00502e20 = uVar1;
  puVar3 = (undefined4 *)FUN_004476b0(local_c,(float *)&DAT_00502e14,(float *)&DAT_00502e20);
  DAT_00502e08 = *puVar3;
  DAT_00502e0c = puVar3[1];
  DAT_00502e10 = puVar3[2];
  *(undefined4 *)((int)this + 0x5c) = uVar1;
  *(undefined4 *)((int)this + 0x60) = DAT_00502e24;
  *(undefined4 *)((int)this + 100) = DAT_00502e28;
  *(undefined4 *)((int)this + 0x68) = DAT_00502e14;
  *(undefined4 *)((int)this + 0x6c) = DAT_00502e18;
  *(undefined4 *)((int)this + 0x70) = DAT_00502e1c;
  *(undefined4 *)((int)this + 0x74) = DAT_00502e08;
  *(undefined4 *)((int)this + 0x78) = DAT_00502e0c;
  *(undefined4 *)((int)this + 0x7c) = DAT_00502e10;
  FUN_004964b0((int)this + 0x58);
  return;
}



