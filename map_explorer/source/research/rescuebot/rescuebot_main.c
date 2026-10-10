
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* iVar3 is used for TWO purposes:
   1. First: Game state from g_GameManager[0xc] + 0x18c (checks if state is 3, 4, or 5 - active
   gameplay states)
   2. Later: Maximum victim count from Math_RoundToInt()

   pvVar6 = g_GameManager[0xc] points to a game subsystem data structure
   Offset 0x18c contains the current game state integer */

void __fastcall RescueBot_Update(astruct *pRescueBot)

{
  undefined4 *puVar1;
  void *this;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  int pRescueBot_00;
  int pRescueBot_01;
  ulonglong uVar6;
  char *pcVar7;
  int iVar8;
  int *currentGameTime;
  int stringWorkBuffers[29];
  void *prevExceptionHandler;
  undefined1 *puStack_8;
  undefined4 exceptionCleanupState;
  void *gameLogicManager_local;

  exceptionCleanupState = 0xffffffff;
  puStack_8 = &LAB_004d097b;
  prevExceptionHandler = ExceptionList;
  if ((char)pRescueBot->field9_0x18 != '\0')
  {
    gameLogicManager_local = (void *)g_GameManager[0xc];
    currentGameTime = g_currentGameTime;
    if (*(int *)((int)gameLogicManager_local + 0x18c) != 6)
    {
      ExceptionList = &prevExceptionHandler;
      if ((pRescueBot->field1_0x4 != 0) &&
          (ExceptionList = &prevExceptionHandler, pRescueBot->field0_0x0 < g_currentGameTime))
      {
        ExceptionList = &prevExceptionHandler;
        RescueBot_UpdateVictimState(gameLogicManager_local);
        puVar1 = LogEntry_Construct(stringWorkBuffers + 7, "rescue\\RescueBot.cpp", 0x3c);
        exceptionCleanupState = 0;
        LogEntry_SetMessage(puVar1 + 7, "RESCUEBOT: Victim dropped off");
        LogEntry_Submit(puVar1, 3);
        exceptionCleanupState = 0xffffffff;
        LogEntry_Destruct(stringWorkBuffers + 7);
        pRescueBot->field1_0x4 = 0;
        RescueBot_ResetTimers((int)pRescueBot);
        RescueBot_SetNextRescueTime(pRescueBot_00);
      }
      if ((pRescueBot->field1_0x4 == 0) &&
          (((iVar3 = *(int *)((int)gameLogicManager_local + 0x18c), iVar3 == 3 || (iVar3 == 4)) ||
            (iVar3 == 5))))
      {
        if (DAT_00502a78 != '\0')
        {
          String_Construct(stringWorkBuffers, "DBG_ACTIVATE_RESCUEBOT");
          piVar5 = stringWorkBuffers;
          exceptionCleanupState = 1;
          this = (void *)GameState_GetConsoleSystem(g_GameManager);
          uVar2 = Console_CheckCommandActive(this, piVar5);
          exceptionCleanupState = 0xffffffff;
          String_Release(stringWorkBuffers);
          if ((char)uVar2 != '\0')
          {
            pRescueBot->field8_0x14 = (int *)((int)currentGameTime + 1);
          }
        }
        if (pRescueBot->field8_0x14 < currentGameTime)
        {
          stringWorkBuffers[1] = 0;
          stringWorkBuffers[2] = 0;
          stringWorkBuffers[3] = 0;
          stringWorkBuffers[4] = 0x3fd00000;
          stringWorkBuffers[5] = 0x9999999a;
          stringWorkBuffers[6] = 0x3fd99999;
          uVar6 = Math_RoundToInt();
          iVar3 = (int)uVar6;
          if (*(int *)((int)gameLogicManager_local + 0x3530) < iVar3)
          {
            RescueBot_FindVictimToRescue((int)pRescueBot);
          }
          else
          {
            puVar1 = LogEntry_Construct(stringWorkBuffers + 7, "rescue\\RescueBot.cpp", 100);
            iVar8 = 3;
            pcVar7 = " victims";
            exceptionCleanupState = 2;
            gameLogicManager_local =
                (void *)LogEntry_AppendString(puVar1, "RESCUEBOT: Exceeded maximum of ");
            gameLogicManager_local = (void *)LogEntry_AppendInt(gameLogicManager_local, iVar3);
            gameLogicManager_local = (void *)LogEntry_AppendString(gameLogicManager_local, pcVar7);
            LogEntry_Submit(gameLogicManager_local, iVar8);
            exceptionCleanupState = 0xffffffff;
            LogEntry_Destruct(stringWorkBuffers + 7);
          }
          RescueBot_SetNextAttemptTime((int)pRescueBot);
        }
        if (pRescueBot->field7_0x10 < currentGameTime)
        {
          uVar4 = GetRandomInt(*(uint *)(pRescueBot->field6_0xc + 4));
          puVar1 = (undefined4 *)HintArray_GetElementAt(&pRescueBot->field6_0xc, uVar4);
          if (DAT_00501918 == '\0')
          {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          currentGameTime = &DAT_00501908;
          piVar5 = (int *)*puVar1;
          if (piVar5 != &DAT_00501908)
          {
            *piVar5 = *piVar5 + 1;
            currentGameTime = piVar5;
          }
          exceptionCleanupState = 3;
          piVar5 = SelectRandomHintString(stringWorkBuffers + 5);
          exceptionCleanupState._0_1_ = 4;
          gameLogicManager_local =
              String_FormatHintWithColor(stringWorkBuffers + 3, "^(128,128,240)", piVar5);
          exceptionCleanupState._0_1_ = 5;
          String_AppendFormattedText(gameLogicManager_local, stringWorkBuffers + 1, ":\n^(255,255,255)");
          exceptionCleanupState._0_1_ = 8;
          String_Release(stringWorkBuffers + 3);
          exceptionCleanupState._0_1_ = 7;
          String_Release(stringWorkBuffers + 5);
          String_Append(stringWorkBuffers + 1, (char *)(currentGameTime + 3));
          DisplayMessageToPlayer((void *)(g_GameManager[0xc] + 0x5f4), &currentGameTime);
          RescueBot_ResetTimers((int)pRescueBot);
          RescueBot_SetNextRescueTime(pRescueBot_01);
          exceptionCleanupState = CONCAT31(exceptionCleanupState._1_3_, 3);
          String_Release(stringWorkBuffers + 1);
          exceptionCleanupState = 0xffffffff;
          String_Release(&currentGameTime);
        }
      }
    }
  }
  ExceptionList = prevExceptionHandler;
  return;
}
