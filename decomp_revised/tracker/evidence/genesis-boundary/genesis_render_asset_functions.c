/* Ghidra decompilation excerpts for the shipped FDNY Genesis.dll bitmap/world path.
 * Input SHA-256: 5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643
 * Captured 2026-10-13 from the active Ghidra MCP analysis.
 * See ../runtime/genesis_render_asset_path.md for interpretation and limits.
 * These are analysis outputs, not recovered original source.
 */
/* Ghidra entry 0x1001B930 */
/* __fastcall geBitmap_CreateFromFile,4 */

int __fastcall geBitmap_CreateFromFile(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  byte local_a;
  byte local_9;
  uint local_8;
  int local_4;

                    /* 0x1b930  75  @geBitmap_CreateFromFile@4 */
  iVar1 = geVFile_Read(param_1,&local_8,4);
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = FUN_10016960();
  if (iVar1 == 0) {
    return 0;
  }
  local_4 = iVar1;
  if (local_8 == 0x6d426547) {
    iVar2 = geVFile_Read(param_1,&local_9,1);
    if (iVar2 == 0) goto LAB_1001b9a4;
    if ((local_9 & 0xf0) != 0x40) {
      geErrorLog_AddExplicit
                (0xffffffff,&DAT_100511a8,"Bitmap\\bitmap.cpp",0xea3,
                 "CreateFromFile : incompatible GeBm version",0);
      goto LAB_1001b9a4;
    }
    iVar2 = FUN_10017870(iVar1,param_1);
    if (iVar2 == 0) goto LAB_1001b9a4;
    if (*(int *)(iVar1 + 0x24) != 0) {
      *(undefined4 *)(iVar1 + 0x24) = 0;
      iVar2 = geBitmap_Palette_CreateFromFile(param_1);
      *(int *)(iVar1 + 0x24) = iVar2;
      if (iVar2 == 0) goto LAB_1001b9a4;
    }
    if (*(int *)(iVar1 + 0x10) == 0x14) {
      geErrorLog_AddExplicit
                (0xffffffff,&DAT_100511a8,"Bitmap\\bitmap.cpp",0xeb3,
                 "Genesis3D 1.0 does not support Wavelet Images",0);
    }
    else {
      iVar2 = geVFile_Read(param_1,&local_a,1);
      if (iVar2 == 0) goto LAB_1001b9a4;
      while (uVar4 = local_a & 0xf, (int)uVar4 <= *(int *)(iVar1 + 0x18)) {
        iVar2 = FUN_100187e0(iVar1,uVar4);
        if (iVar2 == 0) goto LAB_1001b9a4;
        if ((local_a & 0x10) == 0) {
          uVar3 = geBitmap_MipBytes(iVar1,uVar4);
          iVar2 = geVFile_Read(param_1,*(undefined4 *)(iVar1 + 0x28 + uVar4 * 4),uVar3);
          if (iVar2 == 0) goto LAB_1001b9a4;
        }
        if ((local_a & 0x20) != 0) {
          geErrorLog_AddExplicit
                    (0xffffffff,&DAT_100511a8,"Bitmap\\bitmap.cpp",0xee9,
                     "Bitmap_CreateFromFile : Paeth Filter not supported in this version!",0);
          return 0;
        }
        *(undefined4 *)(iVar1 + 0x48 + uVar4 * 4) = 1;
        iVar2 = geVFile_Read(param_1,&local_a,1);
        if (iVar2 == 0) {
          geBitmap_Destroy(&local_4);
          return 0;
        }
      }
    }
    if (*(int *)(iVar1 + 0x68) == 0) {
      return iVar1;
    }
    iVar2 = geBitmap_CreateFromFile(param_1);
    *(int *)(iVar1 + 0x68) = iVar2;
  }
  else {
    iVar2 = geVFile_Seek(param_1,0xfffffffc,0);
    if ((iVar2 == 0) || ((local_8 & 0xffff) != 0x4d42)) goto LAB_1001b9a4;
    iVar2 = FUN_100195a0(param_1);
  }
  if (iVar2 != 0) {
    return iVar1;
  }
LAB_1001b9a4:
  geBitmap_Destroy(&local_4);
  return 0;
}

/* Ghidra entry 0x1001C410 */
/* __fastcall geBitmap_CreateFromFileName,8 */

undefined4 __fastcall geBitmap_CreateFromFileName(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;

                    /* 0x1c410  76  @geBitmap_CreateFromFileName@8 */
  if (param_1 == 0) {
    iVar1 = geVFile_OpenNewSystem(0,1,param_2,0,1);
  }
  else {
    iVar1 = geVFile_Open(param_1,param_2,1);
  }
  if (iVar1 == 0) {
    return 0;
  }
  uVar2 = geBitmap_CreateFromFile(iVar1);
  geVFile_Close(iVar1);
  return uVar2;
}

/* Ghidra entry 0x100195A0 */
undefined4 FUN_100195a0(undefined4 param_1)

{
  int in_EAX;
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  char *pcVar7;
  undefined1 local_38 [10];
  int local_2e;
  uint local_28;
  int local_24;
  uint local_20;
  undefined2 local_1a;
  int local_18;
  int local_8;

  iVar1 = geVFile_Read(param_1,local_38,0xe);
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = geVFile_Read(param_1,&local_28,0x28);
  if (iVar1 == 0) {
    return 0;
  }
  if (local_28 < 0x29) {
    if (0x27 < local_28) goto LAB_100195ee;
    pcVar7 = "CreateFromFile : bmih size bad";
    uVar6 = 0xfea;
  }
  else {
    geVFile_Seek(param_1,local_28 - 0x28,0);
LAB_100195ee:
    if (local_18 == 0) {
      iVar1 = local_2e + -0x36;
      switch(local_1a) {
      case 8:
        if (local_8 == 0) {
          local_8 = 0x100;
        }
        iVar4 = geBitmap_Palette_Create(0xd);
        *(int *)(in_EAX + 0x24) = iVar4;
        if (iVar4 == 0) {
          return 0;
        }
        iVar4 = geVFile_Read(param_1,*(undefined4 *)(iVar4 + 0x1c),local_8 * 4);
        if (iVar4 == 0) {
          return 0;
        }
        iVar4 = 1;
        iVar1 = iVar1 + local_8 * -4;
        *(undefined4 *)(in_EAX + 0x10) = 1;
        break;
      default:
        goto switchD_10019647_caseD_9;
      case 0x10:
        *(undefined4 *)(in_EAX + 0x10) = 3;
        iVar4 = 2;
        break;
      case 0x18:
        *(undefined4 *)(in_EAX + 0x10) = 10;
        iVar4 = 3;
        break;
      case 0x20:
        *(undefined4 *)(in_EAX + 0x10) = 0xd;
        iVar4 = 4;
      }
      if (-1 < iVar1) {
        geVFile_Seek(param_1,iVar1,0);
        *(uint *)(in_EAX + 8) = (local_20 ^ (int)local_20 >> 0x1f) - ((int)local_20 >> 0x1f);
        *(int *)(in_EAX + 4) = local_24;
        uVar2 = local_24 + 3U & 0xfffffffc;
        *(uint *)(in_EAX + 0xc) = uVar2;
        iVar1 = uVar2 * iVar4;
        uVar2 = local_24 * iVar4 + 3U & 0xfffffffc;
        *(undefined4 *)(in_EAX + 0x1c) = 0;
        iVar4 = FUN_100187e0();
        if (iVar4 == 0) {
          return 0;
        }
        if ((int)local_20 < 1) {
          iVar4 = *(int *)(in_EAX + 0x28);
          iVar5 = *(int *)(in_EAX + 8);
          while (iVar5 != 0) {
            iVar5 = iVar5 + -1;
            iVar3 = geVFile_Read(param_1,iVar4,uVar2);
            if (iVar3 == 0) {
              return 0;
            }
            iVar4 = iVar4 + iVar1;
          }
        }
        else {
          iVar4 = *(int *)(in_EAX + 8);
          iVar5 = (iVar4 + -1) * iVar1 + *(int *)(in_EAX + 0x28);
          if (iVar4 != 0) {
            do {
              iVar4 = iVar4 + -1;
              iVar3 = geVFile_Read(param_1,iVar5,uVar2);
              if (iVar3 == 0) {
                return 0;
              }
              iVar5 = iVar5 - iVar1;
            } while (iVar4 != 0);
            return 1;
          }
        }
        return 1;
      }
      pcVar7 = "CreateFromFile : bPad bad";
      uVar6 = 0x1019;
    }
    else {
      pcVar7 = "CreateFromFile : only BI_RGB BMP compression supported";
      uVar6 = 0xff0;
    }
  }
  geErrorLog_AddExplicit(0xffffffff,&DAT_100511a8,"Bitmap\\bitmap.cpp",uVar6,pcVar7,0);
switchD_10019647_caseD_9:
  return 0;
}

/* Ghidra entry 0x1001B890 */
/* __fastcall geBitmap_SetAlpha,8 */

undefined4 __fastcall geBitmap_SetAlpha(int param_1,int *param_2)

{
  int *piVar1;

                    /* 0x1b890  112  @geBitmap_SetAlpha@8 */
  if (*(int *)(param_1 + 0x70) != 0) {
    param_1 = *(int *)(param_1 + 0x70);
  }
  if ((*(int *)(param_1 + 0x6c) < 1) && (*(int *)(param_1 + 0x74) == 0)) {
    piVar1 = (int *)(param_1 + 0x68);
    if (param_2 != *(int **)(param_1 + 0x68)) {
      if (*(int *)(param_1 + 0xac) != 0) {
        FUN_1001b150(param_1);
      }
      if (*piVar1 != 0) {
        geBitmap_Destroy(piVar1);
      }
      *piVar1 = (int)param_2;
      if (param_2 != (int *)0x0) {
        *param_2 = *param_2 + 1;
      }
      if (*(int *)(param_1 + 0xac) != 0) {
        FUN_1001a6d0(param_1);
      }
    }
    return 1;
  }
  geErrorLog_AddExplicit
            (0xffffffff,&DAT_100511a8,"Bitmap\\bitmap.cpp",0xe06,"SetAlpha : not an original bitmap"
             ,0);
  return 0;
}

/* Ghidra entry 0x1003E810 */
undefined4 * geWorld_Create(int param_1)

{
  void *_Memory;
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;

                    /* 0x3e810  505  geWorld_Create */
  puVar1 = (undefined4 *)geRam_Allocate(0xc58);
  iVar6 = 0;
  if (puVar1 == (undefined4 *)0x0) {
    geErrorLog_AddExplicit(0x34,"GE_ERR_OUT_OF_MEMORY","World\\World.cpp",0x17a,&DAT_100511b7,0);
    return (undefined4 *)0x0;
  }
  puVar7 = puVar1;
  for (iVar3 = 0x316; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  puVar1[0x314] = puVar1[0x314] + 1;
  iVar3 = FUN_10004b20();
  if (iVar3 != 0) {
    if (param_1 == 0) {
      local_c = 0xc47a0000;
      local_8 = 0xc47a0000;
      local_4 = 0xc47a0000;
      local_18 = 0x447a0000;
      local_14 = 0x447a0000;
      local_10 = 0x447a0000;
      iVar3 = FUN_1003b310(&local_c,&local_18);
    }
    else {
      iVar3 = FUN_1003b8c0(param_1);
    }
    puVar1[6] = iVar3;
    puVar1[0x315] = 1;
    if (iVar3 != 0) {
      uVar2 = geRam_Allocate(*(int *)(iVar3 + 0x178) * 0xc);
      *(undefined4 *)(puVar1[6] + 0x1d4) = uVar2;
      puVar7 = *(undefined4 **)(puVar1[6] + 0x1d4);
      if (puVar7 != (undefined4 *)0x0) {
        for (uVar4 = *(int *)(puVar1[6] + 0x178) * 3 & 0x3fffffff; uVar4 != 0; uVar4 = uVar4 - 1) {
          *puVar7 = 0;
          puVar7 = puVar7 + 1;
        }
        for (iVar3 = 0; iVar3 != 0; iVar3 = iVar3 + -1) {
          *(undefined1 *)puVar7 = 0;
          puVar7 = (undefined4 *)((int)puVar7 + 1);
        }
        iVar3 = FUN_10033990(puVar1);
        if ((((iVar3 != 0) && (iVar3 = FUN_10028a70(puVar1), iVar3 != 0)) &&
            (iVar3 = FUN_1003a770(puVar1), iVar3 != 0)) &&
           (iVar3 = FUN_10035110(puVar1), iVar3 != 0)) {
          uVar2 = FUN_1003b200(puVar1[6] + 200);
          *(undefined4 *)(puVar1[6] + 0x1cc) = uVar2;
          if (*(int *)(puVar1[6] + 0x1cc) != 0) {
            _Memory = *(void **)(puVar1[6] + 0x158);
            if (_Memory != (void *)0x0) {
              geRam_Free_(_Memory);
              *(undefined4 *)(puVar1[6] + 0x158) = 0;
              *(undefined4 *)(puVar1[6] + 0x158) = 0;
              *(undefined4 *)(puVar1[6] + 0x1b0) = 0;
            }
            iVar3 = FUN_1003e6e0(puVar1);
            if (iVar3 == 0) {
              geErrorLog_AddExplicit
                        (0xffffffff,&DAT_100511a8,"World\\World.cpp",0x1c5,
                         "geWorld_WorldCreate:  geWorld_BitmapListInit failed.",0);
              return (undefined4 *)0x0;
            }
            iVar3 = FUN_10039260(puVar1);
            if (iVar3 != 0) {
              if (0 < *(int *)(puVar1[6] + 0x16c)) {
                iVar3 = *(int *)(puVar1[6] + 0x1d0) + 0x44;
                do {
                  puVar7 = (undefined4 *)(iVar3 + -0x44);
                  for (iVar5 = 0x35; iVar5 != 0; iVar5 = iVar5 + -1) {
                    *puVar7 = 0;
                    puVar7 = puVar7 + 1;
                  }
                  *(undefined4 *)(iVar3 + 0x78) = 0xffffffff;
                  FUN_10006740(iVar3);
                  iVar6 = iVar6 + 1;
                  iVar3 = iVar3 + 0xd4;
                } while (iVar6 < *(int *)(puVar1[6] + 0x16c));
              }
              FUN_1003bba0(puVar1[6]);
              iVar6 = FUN_1003c690();
              if (iVar6 != 0) {
                puVar1[3] = 0xffffffff;
                puVar1[0xee] = 0;
                puVar1[0xef] = 0;
                iVar6 = FUN_1003e620(puVar1);
                if (iVar6 != 0) {
                  return puVar1;
                }
                geErrorLog_AddExplicit
                          (0xffffffff,&DAT_100511a8,"World\\World.cpp",0x1e5,
                           "Failed to create static FogList",0);
              }
            }
          }
        }
      }
    }
  }
  geWorld_Free(puVar1);
  return (undefined4 *)0x0;
}

/* Ghidra entry 0x1003B8C0 */
undefined4 * FUN_1003b8c0(undefined4 param_1)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;

  puVar1 = (undefined4 *)geRam_Allocate(0x101e8);
  if (puVar1 == (undefined4 *)0x0) {
    geErrorLog_AddExplicit(0x34,"GE_ERR_OUT_OF_MEMORY","World\\World.cpp",0x74c,&DAT_100511b7,0);
    return (undefined4 *)0x0;
  }
  puVar3 = puVar1;
  for (iVar2 = 0x407a; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  iVar2 = FUN_10032400(param_1,puVar1 + 0x32);
  if (iVar2 == 0) {
    geErrorLog_AddExplicit(0x12,"GE_ERR_GBSP_LOAD_FAILURE","World\\World.cpp",0x754,&DAT_100511b7,0)
    ;
    return (undefined4 *)0x0;
  }
  iVar2 = geRam_Allocate(puVar1[0x5b] * 0xd4);
  puVar1[0x74] = iVar2;
  if (iVar2 == 0) {
    geErrorLog_AddExplicit(0x34,"GE_ERR_OUT_OF_MEMORY","World\\World.cpp",0x75a,&DAT_100511b7,0);
    return (undefined4 *)0x0;
  }
  return puVar1;
}

/* Ghidra entry 0x10032400 */
undefined4 FUN_10032400(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  int local_c;

  iVar1 = FUN_100319d0(param_1);
  while( true ) {
    if (iVar1 == 0) {
      geErrorLog_AddExplicit
                (0x37,"GE_ERR_ERROR_READING_BSP_CHUNK","World\\Gbspfile.cpp",0x214,&DAT_100511b7,0);
      return 0;
    }
    if (local_c == 0xffff) break;
    iVar1 = FUN_100319d0(param_1);
  }
  if (0 < *(int *)(param_2 + 0xa8)) {
    uVar2 = FUN_10032350(param_2,0);
    return uVar2;
  }
  return 1;
}

/* Ghidra entry 0x1003CF10 */
undefined4 geWorld_AddBitmap(int param_1,undefined4 param_2)

{
  int iVar1;

                    /* 0x3cf10  498  geWorld_AddBitmap */
  if (*(int *)(param_1 + 0xc1c) == 0) {
    geErrorLog_AddExplicit
              (0xffffffff,&DAT_100511a8,"World\\World.cpp",0xd1e,
               "geWorld_AddBitmap:  AttachedBitmapList is NULL.",0);
    return 0;
  }
  FUN_100171e0();
  iVar1 = FUN_10022f20(*(undefined4 *)(param_1 + 0xc1c),param_2);
  if (iVar1 != 0) {
    *(undefined4 *)(param_1 + 0xc54) = 1;
  }
  return 1;
}

/* Ghidra entry 0x1003CF80 */
undefined4 geWorld_RemoveBitmap(int param_1,undefined4 param_2)

{
  int iVar1;

                    /* 0x3cf80  528  geWorld_RemoveBitmap */
  if (*(int *)(param_1 + 0xc1c) == 0) {
    return 0;
  }
  iVar1 = FUN_10022f80(*(int *)(param_1 + 0xc1c),param_2);
  if (iVar1 != 0) {
    *(undefined4 *)(param_1 + 0xc54) = 1;
  }
  return 1;
}

/* Ghidra entry 0x1003CFC0 */
void geWorld_GetBitmapByName(int param_1)

{
                    /* 0x3cfc0  508  geWorld_GetBitmapByName */
  if (*(int *)(param_1 + 0x18) == 0) {
    return;
  }
  FUN_1003ac10();
  return;
}

/* Ghidra entry 0x1003AC10 */
undefined4 FUN_1003ac10(int *param_1,char *param_2)

{
  int iVar1;
  int iVar2;
  char *_Str1;

  iVar2 = 0;
  _Str1 = (char *)param_1[1];
  if (0 < *param_1) {
    do {
      iVar1 = __stricmp(_Str1,param_2);
      if (iVar1 == 0) {
        return *(undefined4 *)(_Str1 + 0x40);
      }
      iVar2 = iVar2 + 1;
      _Str1 = _Str1 + 0x4c;
    } while (iVar2 < *param_1);
  }
  return 0;
}

/* Ghidra entry 0x10022F20 */
undefined4 FUN_10022f20(int param_1,undefined4 param_2)

{
  int iVar1;
  int local_4;

  geBitmap_CreateRef(param_2);
  *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
  iVar1 = FUN_10004a10(&local_4);
  if (iVar1 != 0) {
    FUN_10004ae0(iVar1,local_4 + 1);
    return 0;
  }
  *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  FUN_10004960(1);
  return 1;
}
