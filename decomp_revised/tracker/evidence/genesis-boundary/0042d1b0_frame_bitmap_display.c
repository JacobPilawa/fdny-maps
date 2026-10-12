
void __fastcall FUN_0042d1b0(int param_1)

{
  int iVar1;
  
  geEngine_SetGamma(*(undefined4 *)(DAT_00503a70 + 0x74),DAT_004ff490);
  *(undefined1 *)(param_1 + 0x284) = 0;
  *(undefined1 *)(*(int *)(DAT_00503a78 + 4) + 0x160) = 0;
  geEngine_BeginFrame(*(undefined4 *)(DAT_00503a70 + 0x74),0,0);
  iVar1 = geBitmap_Height(*(undefined4 *)(param_1 + 0x44));
  geEngine_DrawBitmap(*(undefined4 *)(DAT_00503a70 + 0x74),*(undefined4 *)(param_1 + 0x44),0,
                      *(undefined4 *)(param_1 + 0x274),
                      (*(int *)(param_1 + 0x27c) + DAT_004ffadc) - iVar1);
  geEngine_EndFrame(*(undefined4 *)(DAT_00503a70 + 0x74));
  FUN_0048a8c0(&DAT_004ffad8,*(int **)(DAT_00503a78 + 4));
  FUN_0042c110(&DAT_00502ab8,'\0');
  FUN_0042a730(*(void **)(param_1 + 0x154),(undefined4 *)0x1,(int *)&DAT_00502ab4);
  *(undefined4 *)(*(int *)(param_1 + 0x154) + 0x6f0) = 1;
  return;
}

