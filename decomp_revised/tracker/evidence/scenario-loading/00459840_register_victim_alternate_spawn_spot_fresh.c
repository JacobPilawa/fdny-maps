
/* VictimAltSpot first-pass handler: finds the VictimRescue whose registered name matches A_Victim,
   appends the alternate XYZ and heading to that victim's spawn-spot list, then marks this spot
   handled. */

void __fastcall RegisterVictimAlternateSpawnSpot(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  void *this;
  int iStack_6c;
  undefined4 *puStack_68;
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;

  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d0e90;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  if (*(int *)(*(int *)(param_1 + 0x24) + 0x10) == 0) {
    ppuStack_64 = &PTR_FUN_004dc554;
    ExceptionList = &pvStack_c;
    FUN_004a3118();
    pcStack_60 = "rescue\\VictimAltSpotEnt.cpp";
    uStack_5c = 0x19;
    uStack_58 = 0;
    uStack_4 = 0;
    FUN_004a3a19(auStack_48,"TE_VicimtAltSpot: Not pointing to a Victim!");
    FUN_00401360(&ppuStack_64,0);
    uStack_4 = 0xffffffff;
    FUN_004012c0(&ppuStack_64);
  }
  FUN_00481c00(&iStack_6c);
  uStack_4 = 1;
  puVar2 = puStack_68;
  do {
    if ((iStack_6c == 0) || (puVar2 == (undefined4 *)0x0)) {
      this = (void *)0x0;
    }
    else {
      this = (void *)*puVar2;
    }
    if ((iStack_6c != 0) && (puVar2 != (undefined4 *)0x0)) {
      puVar2 = (undefined4 *)puVar2[1];
      puStack_68 = puVar2;
    }
    if (this == (void *)0x0) goto LAB_00459928;
  } while ((*(int *)((int)this + 0x18) != 2) ||
          (iVar1 = *(int *)(param_1 + 0x24), *(int *)((int)this + 0x150) != *(int *)(iVar1 + 0x10)))
  ;
  RegisterVictimSpawnSpot(this,(undefined4 *)(iVar1 + 4),*(undefined4 *)(iVar1 + 0x14));
LAB_00459928:
  *(undefined1 *)(param_1 + 0x10) = 1;
  uStack_4 = 0xffffffff;
  if (iStack_6c != 0) {
    FUN_00429fe0((void *)(iStack_6c + 0x28),(int)&iStack_6c);
  }
  ExceptionList = pvStack_c;
  return;
}
