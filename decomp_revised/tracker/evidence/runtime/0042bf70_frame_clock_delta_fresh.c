/* Selected Ghidra 12.0.4 pseudocode excerpts; names and types are provisional. */
/* Program: FDNYFirefighter.exe */
/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0042bf20 FUN_0042bf20: pause the selected timer. */
void __fastcall FUN_0042bf20(int param_1)
{
  DWORD now;
  if (*(char *)(param_1 + 0x1c) == '\0') {
    *(undefined1 *)(param_1 + 0x1c) = 1;
    now = timeGetTime();
    *(DWORD *)(param_1 + 0xc) = now;
  }
  return;
}

/* 0x0042bf40 FUN_0042bf40: resume and add paused milliseconds to the base. */
void __fastcall FUN_0042bf40(int param_1)
{
  DWORD now;
  if (*(char *)(param_1 + 0x1c) != '\0') {
    *(undefined1 *)(param_1 + 0x1c) = 0;
    now = timeGetTime();
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + (now - *(int *)(param_1 + 0xc));
  }
  return;
}

/* 0x0042bf70 FUN_0042bf70: update one timer record and its delta. */
void __fastcall FUN_0042bf70(int *param_1)
{
  float fVar1;
  float fVar2;
  DWORD now;
  int deltaMs;
  ulonglong adjusted;

  /* The int* Ghidra type aliases the binary32 delta stored at record +0x10. */
  if ((char)param_1[7] == '\0') {
    *param_1 = param_1[1];
    now = timeGetTime();
    param_1[1] = now - param_1[2];
    deltaMs = (now - param_1[2]) - *param_1;
    fVar1 = (float)deltaMs;
    if (deltaMs < 0) fVar1 += _DAT_004dfa30;
    fVar2 = (float)_DAT_004dfa28;
    param_1[4] = (int)(fVar1 * fVar2);
    if (fVar1 * fVar2 <= (float)_DAT_004dfa20) {
      do {
        Sleep(1);
        now = timeGetTime();
        param_1[1] = now - param_1[2];
        deltaMs = (now - param_1[2]) - *param_1;
        fVar1 = (float)deltaMs;
        if (deltaMs < 0) fVar1 += _DAT_004dfa30;
        fVar2 = (float)_DAT_004dfa28;
        param_1[4] = (int)(fVar1 * fVar2);
      } while (fVar1 * fVar2 <= (float)_DAT_004dfa20);
    }
    if ((float)_DAT_004df7b8 < (float)param_1[4]) {
      adjusted = ConvertX87FloatToIntegerTowardZero();
      param_1[1] = (int)adjusted;
      adjusted = ConvertX87FloatToIntegerTowardZero();
      param_1[2] = (int)adjusted;
      param_1[4] = 0x3dcccccd; /* binary32 0.1f */
    }
    fVar1 = (float)_DAT_004dd808;
    param_1[5] = (int)(fVar1 / (float)param_1[4]);
    param_1[6] = (int)((float)param_1[6] * (float)_DAT_004dfa08 +
                      (fVar1 / (float)param_1[4]) * (float)_DAT_004dfa10);
  }
  return;
}

/* 0x0042c080 FUN_0042c080: manager field +0x4c aliases global 00502b04. */
void __fastcall FUN_0042c080(int *param_1)
{
  int *timer;

  /* Pause accounting and active-timer selection omitted here. */
  timer = param_1 + 8;
  if ((char)param_1[0x10] == '\0') timer = param_1;
  FUN_0042bf70(timer);
  param_1[0x13] = timer[4]; /* (00502ab8 + 0x4c) = timer + 0x10 */
  param_1[0x14] = timer[5];
  param_1[0x15] = timer[6];
  param_1[0x11] = timer[1];
  return;
}

/* ProcessEverGameUpdate (0042aea0) calls FUN_0042c080(&DAT_00502ab8) at 0042aec0..0042aec5. */
