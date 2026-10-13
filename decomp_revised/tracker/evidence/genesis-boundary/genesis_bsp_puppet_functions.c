/* Ghidra decompilation excerpts for the shipped FDNY Genesis.dll GBSP/material path.
 * Input SHA-256: 5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643
 * Captured 2026-10-13 from the active Ghidra MCP analysis.
 * See ../runtime/genesis_render_asset_path.md for interpretation and limits.
 * These are analysis outputs, not recovered original source.
 */
/* Ghidra entry 0x100319D0 */
undefined4 FUN_100319d0(undefined4 param_1)

{
  int *in_EAX;
  int iVar1;
  undefined4 *puVar2;
  char *pcVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  char *unaff_EBX;
  char *pcVar7;
  bool bVar8;

  iVar1 = geVFile_Read(param_1);
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = *in_EAX;
  if (0xffff < iVar1) {
    return 0;
  }
  if (iVar1 == 0xffff) {
    return 1;
  }
  switch(iVar1) {
  case 0:
    if (in_EAX[1] != 0x1c) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x70,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = geVFile_Read(param_1);
    if (iVar1 == 0) {
      return 0;
    }
    iVar1 = 5;
    bVar8 = true;
    pcVar3 = unaff_EBX;
    pcVar7 = "GBSP";
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar8 = *pcVar3 == *pcVar7;
      pcVar3 = pcVar3 + 1;
      pcVar7 = pcVar7 + 1;
    } while (bVar8);
    if (!bVar8) {
      geErrorLog_AddExplicit
                (0x35,"GE_ERR_INVALID_BSP_TAG","World\\Gbspfile.cpp",0x77,&DAT_100511b7,0);
      return 0;
    }
    if (*(int *)(unaff_EBX + 8) == 0xf) {
      return 1;
    }
    geErrorLog_AddExplicit
              (0x36,"GE_ERR_INVALID_BSP_VERSION","World\\Gbspfile.cpp",0x7c,&DAT_100511b7,0);
    return 0;
  case 1:
    if (in_EAX[1] != 0x50) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x86,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xa4) = iVar1;
    uVar4 = geRam_Allocate(iVar1 * 0x50);
    *(undefined4 *)(unaff_EBX + 0x48) = uVar4;
    iVar1 = geVFile_Read(param_1,uVar4,in_EAX[2] * in_EAX[1]);
    if (iVar1 == 0) {
      return 0;
    }
    iVar1 = 0;
    if (*(int *)(unaff_EBX + 0xa4) < 1) {
      return 1;
    }
    iVar5 = 0;
    do {
      *(undefined4 *)(iVar5 + 0x4c + *(int *)(unaff_EBX + 0x48)) = 0;
      iVar1 = iVar1 + 1;
      iVar5 = iVar5 + 0x50;
    } while (iVar1 < *(int *)(unaff_EBX + 0xa4));
    return 1;
  case 2:
    if (in_EAX[1] != 0x2c) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x97,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xa8) = iVar1;
    uVar4 = geRam_Allocate(iVar1 * 0x2c);
    *(undefined4 *)(unaff_EBX + 0x4c) = uVar4;
    puVar2 = (undefined4 *)geRam_Allocate(*(int *)(unaff_EBX + 0xa8) << 2);
    *(undefined4 **)(unaff_EBX + 0x50) = puVar2;
    for (uVar6 = *(uint *)(unaff_EBX + 0xa8) & 0x3fffffff; uVar6 != 0; uVar6 = uVar6 - 1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
    for (iVar1 = 0; iVar1 != 0; iVar1 = iVar1 + -1) {
      *(undefined1 *)puVar2 = 0;
      puVar2 = (undefined4 *)((int)puVar2 + 1);
    }
    pcVar3 = *(char **)(unaff_EBX + 0x4c);
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 3:
    if (in_EAX[1] != 0xc) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0xa8,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xac) = iVar1;
    if (iVar1 == 0) {
      return 1;
    }
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0xc);
    *(char **)(unaff_EBX + 0x54) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 4:
    if (in_EAX[1] != 0x3c) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0xb9,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xb0) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0x3c);
    *(char **)(unaff_EBX + 0x58) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 5:
    if (in_EAX[1] != 4) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",199,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xb4) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 4);
    *(char **)(unaff_EBX + 0x5c) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 6:
    if (in_EAX[1] != 8) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0xd6,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xb8) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 8);
    *(char **)(unaff_EBX + 0x60) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 7:
    if (in_EAX[1] != 8) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0xe4,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xbc) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 8);
    *(char **)(unaff_EBX + 100) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 8:
    if (in_EAX[1] != 8) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x12a,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xcc) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 8);
    *(char **)(unaff_EBX + 0x74) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 9:
    if (in_EAX[1] != 0x10) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0xf2,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xf8) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 << 4);
    *(char **)(unaff_EBX + 0xa0) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 10:
    if (in_EAX[1] != 0x14) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x100,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xc0) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0x14);
    *(char **)(unaff_EBX + 0x68) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0xb:
    if (in_EAX[1] != 0x24) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x10e,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xc4) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0x24);
    *(char **)(unaff_EBX + 0x6c) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0xc:
    if (in_EAX[1] != 4) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x11c,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 200) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 4);
    *(char **)(unaff_EBX + 0x70) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0xd:
    if (in_EAX[1] != 4) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x146,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xd4) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 4);
    *(char **)(unaff_EBX + 0x7c) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0xe:
    if (in_EAX[1] != 0xc) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x138,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xd0) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0xc);
    *(char **)(unaff_EBX + 0x78) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0xf:
    if (in_EAX[1] != 0xc) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x156,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xd8) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0xc);
    *(char **)(unaff_EBX + 0x80) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x10:
    if (in_EAX[1] != 1) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x191,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xdc) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1);
    *(char **)(unaff_EBX + 0x84) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x11:
    if (in_EAX[1] != 0x40) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x165,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xe4) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 << 6);
    *(char **)(unaff_EBX + 0x8c) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x12:
    if (in_EAX[1] != 0x34) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x174,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xe0) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0x34);
    *(char **)(unaff_EBX + 0x88) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x13:
    if (in_EAX[1] != 1) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x183,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xe8) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1);
    *(char **)(unaff_EBX + 0x90) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x14:
    if (in_EAX[1] != 1) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x19f,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xf0) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1);
    *(char **)(unaff_EBX + 0x98) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x15:
    if (in_EAX[1] != 1) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x1ad,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xf4) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1);
    *(char **)(unaff_EBX + 0x9c) = pcVar3;
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x16:
    if (in_EAX[1] != 0x2c) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x1bc,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2] * 0x2c;
    pcVar3 = unaff_EBX + 0x1c;
    break;
  case 0x17:
    if (in_EAX[1] != 0x300) {
      geErrorLog_AddExplicit
                (0x76,"GE_ERR_BAD_BSP_FILE_CHUNK_SIZE","World\\Gbspfile.cpp",0x1c9,&DAT_100511b7,0);
      return 0;
    }
    iVar1 = in_EAX[2];
    *(int *)(unaff_EBX + 0xec) = iVar1;
    pcVar3 = (char *)geRam_Allocate(iVar1 * 0x300);
    *(char **)(unaff_EBX + 0x94) = pcVar3;
    if (pcVar3 == (char *)0x0) {
      return 0;
    }
    iVar1 = in_EAX[2] * in_EAX[1];
    break;
  case 0x18:
    uVar4 = FUN_100317d0();
    return uVar4;
  default:
    goto switchD_10031a0c_default;
  }
  iVar1 = geVFile_Read(param_1,pcVar3,iVar1);
  if (iVar1 != 0) {
    return 1;
  }
switchD_10031a0c_default:
  return 0;
}

/* Ghidra entry 0x10032350 */
undefined4 FUN_10032350(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;

  iVar1 = *(int *)(param_2 * 0x2c + *(int *)(param_1 + 0x4c));
  iVar4 = param_2 * 0x2c + *(int *)(param_1 + 0x4c);
  if ((0 < iVar1) && (iVar1 = FUN_10032350(param_1,iVar1), iVar1 == 0)) {
    return 0;
  }
  if ((0 < *(int *)(iVar4 + 4)) && (iVar1 = FUN_10032350(param_1,*(int *)(iVar4 + 4)), iVar1 == 0))
  {
    return 0;
  }
  uVar3 = 0;
  iVar1 = 0;
  if (0 < *(int *)(iVar4 + 8)) {
    piVar2 = (int *)(*(int *)(param_1 + 0x6c) + 0x10 + *(int *)(iVar4 + 0xc) * 0x24);
    while ((*(byte *)(*piVar2 * 0x40 + 0x28 + *(int *)(param_1 + 0x8c)) & 4) == 0) {
      iVar1 = iVar1 + 1;
      piVar2 = piVar2 + 9;
      if (*(int *)(iVar4 + 8) <= iVar1) {
        *(undefined4 *)(*(int *)(param_1 + 0x50) + param_2 * 4) = 0;
        return 1;
      }
    }
    uVar3 = 1;
  }
  *(undefined4 *)(*(int *)(param_1 + 0x50) + param_2 * 4) = uVar3;
  return 1;
}

/* Ghidra entry 0x1003E6E0 */
undefined4 FUN_1003e6e0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  undefined1 local_24 [36];

  if (*(int *)(param_1 + 0xc1c) == 0) {
    iVar1 = FUN_10022bf0();
    *(int *)(param_1 + 0xc1c) = iVar1;
    if (iVar1 == 0) {
      geErrorLog_AddExplicit
                (0xffffffff,&DAT_100511a8,"World\\World.cpp",0xcd5,
                 "geWorld_BitmapListInit:  BitmapList_Create failed.",0);
      return 0;
    }
  }
  iVar1 = *(int *)(*(int *)(param_1 + 0x18) + 0x1cc);
  if (iVar1 != 0) {
    iVar1 = FUN_1003aba0(iVar1);
    if (0 < iVar1) {
      iVar5 = 0;
      do {
        uVar2 = FUN_1003abb0(*(undefined4 *)(*(int *)(param_1 + 0x18) + 0x1cc),iVar5);
        uVar3 = FUN_1003acc0(uVar2);
        FUN_1003acb0(uVar2);
        iVar4 = geBitmap_GetInfo(uVar3,local_24,0);
        if (iVar4 == 0) {
          geErrorLog_AddExplicit
                    (0xffffffff,&DAT_100511a8,"World\\World.cpp",0xcf1,
                     "geWorld_BitmapListInit:  geBitmap_GetInfo failed.",0);
          return 0;
        }
        iVar4 = geWorld_AddBitmap(param_1,uVar3);
        if (iVar4 == 0) {
          geErrorLog_AddExplicit
                    (0xffffffff,&DAT_100511a8,"World\\World.cpp",0xcf7,
                     "geWorld_BitmapListInit:  geWorld_AddBitmap failed.",0);
          return 0;
        }
        iVar5 = iVar5 + 1;
        *(undefined4 *)(param_1 + 0xc54) = 1;
      } while (iVar5 < iVar1);
    }
  }
  return 1;
}

/* Ghidra entry 0x10010D50 */
undefined4 FUN_10010d50(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int unaff_EDI;
  int local_8;
  undefined4 local_4;

  iVar1 = geBody_GetMaterialCount(param_1);
  *(int *)(unaff_EDI + 8) = iVar1;
  if (iVar1 < 1) {
    return 1;
  }
  iVar1 = geRam_Allocate(iVar1 << 5);
  *(int *)(unaff_EDI + 0xc) = iVar1;
  if (iVar1 == 0) {
    geErrorLog_AddExplicit(0x66,"ERR_PUPPET_ENOMEM","Actor\\puppet.cpp",0x9a,&DAT_100511b7,0);
    return 0;
  }
  iVar1 = 0;
  if (0 < *(int *)(unaff_EDI + 8)) {
    iVar3 = 0;
    do {
      iVar4 = *(int *)(unaff_EDI + 0xc) + iVar3;
      geBody_GetMaterial(param_1,iVar1,&local_4,&local_8,iVar4,iVar4 + 4,iVar4 + 8);
      if (local_8 == 0) {
        *(undefined4 *)(iVar4 + 0x10) = 0;
        *(undefined4 *)(iVar4 + 0xc) = 0;
      }
      else {
        *(undefined4 *)(iVar4 + 0xc) = 1;
        *(int *)(iVar4 + 0x10) = local_8;
        geBitmap_CreateRef(local_8);
        iVar2 = geWorld_AddBitmap(*(undefined4 *)(unaff_EDI + 0x70),local_8);
        if (iVar2 == 0) {
          geErrorLog_AddExplicit
                    (0xffffffff,&DAT_100511a8,"Actor\\puppet.cpp",0xb5,
                     "Puppet_FetchTextures : World_AddBitmap",0);
          geRam_Free_(*(void **)(unaff_EDI + 0xc));
          *(undefined4 *)(unaff_EDI + 0xc) = 0;
          *(undefined4 *)(unaff_EDI + 8) = 0;
          return 0;
        }
      }
      *(undefined4 *)(iVar4 + 0x18) = local_4;
      iVar1 = iVar1 + 1;
      iVar3 = iVar3 + 0x20;
    } while (iVar1 < *(int *)(unaff_EDI + 8));
  }
  return 1;
}

/* Ghidra entry 0x10010EC0 */
undefined4 __fastcall
FUN_10010ec0(int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
            undefined4 param_6)

{
  int iVar1;
  undefined4 *puVar2;
  int local_4;

  puVar2 = (undefined4 *)(param_2 * 0x20 + *(int *)(param_1 + 0xc));
  local_4 = puVar2[4];
  puVar2[4] = param_3;
  *puVar2 = param_4;
  puVar2[1] = param_5;
  puVar2[2] = param_6;
  if (local_4 != param_3) {
    if (local_4 != 0) {
      geWorld_RemoveBitmap(*(undefined4 *)(param_1 + 0x70),local_4);
      geBitmap_Destroy(&local_4);
    }
    puVar2[3] = 0;
    if (param_3 != 0) {
      geBitmap_CreateRef(param_3);
      puVar2[3] = 1;
      iVar1 = geWorld_AddBitmap(*(undefined4 *)(param_1 + 0x70),param_3);
      if (iVar1 == 0) {
        geErrorLog_AddExplicit
                  (0xffffffff,&DAT_100511a8,"Actor\\puppet.cpp",0x104,
                   "Puppet_SetMaterial : World_AddBitmap",0);
        return 0;
      }
    }
  }
  return 1;
}
