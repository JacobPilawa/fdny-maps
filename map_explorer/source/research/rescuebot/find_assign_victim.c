
void __fastcall find_assign_victim(int param_1)

{
    int *piVar1;
    char cVar2;
    undefined4 *puVar3;
    float *pfVar4;
    int iVar5;
    int iVar6;
    int *piVar7;
    undefined4 *puStack_f4;
    int *piStack_f0;
    int iStack_ec;
    int *piStack_e8;
    void *local_e4;
    int iStack_e0;
    char *pcStack_dc;
    char *pcStack_d8;
    undefined4 uStack_d4;
    undefined **ppuStack_c8;
    char *pcStack_c4;
    undefined4 uStack_c0;
    undefined1 uStack_bc;
    undefined1 auStack_ac[60];
    undefined **local_70;
    char *local_6c;
    undefined4 local_68;
    undefined1 local_64;
    undefined1 local_54[60];
    float afStack_18[3];
    void *local_c;
    undefined1 *puStack_8;
    undefined4 local_4;
    struct_victim_entity *victim_entity;
    int *level_or_entity_manager;

    local_4 = 0xffffffff;
    puStack_8 = &LAB_004d0929;
    local_c = ExceptionList;
    level_or_entity_manager = *(int **)(DAT_00503a78 + 0x30);
    local_e4 = (void *)param_1;
    if (*(int *)(param_1 + 4) != 0)
    {
        local_70 = &PTR_FUN_004dc554;
        ExceptionList = &local_c;
        FUN_004a3118();
        local_6c = "rescue\\RescueBot.cpp";
        local_68 = 0x9c;
        local_64 = 0;
        local_4 = 0;
        FUN_004a3a19(local_54, "RESCUEBOT: Already rescuing someone");
        FUN_00401360(&local_70, 3);
        local_4 = 0xffffffff;
        FUN_004012c0(&local_70);
        ExceptionList = local_c;
        return;
    }
    ExceptionList = &local_c;
    get_entity_iterator(level_or_entity_manager + 0x176, &puStack_f4);
    local_4 = 1;
    while ((puStack_f4 != (undefined4 *)0x0 && (iStack_ec != 0)))
    {
        piStack_e8 = (int *)(iStack_ec + 0xc);
        if ((piStack_e8 != (int *)level_or_entity_manager[0x172]) &&
            (iVar6 = 0, 0 < *(int *)(iStack_ec + 0x30)))
        {
            do
            {
                piVar1 = piStack_e8;
                if ((iVar6 < 0) || (piStack_e8[9] <= iVar6))
                {
                    piVar7 = &iStack_e0;
                    for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1)
                    {
                        *piVar7 = 0;
                        piVar7 = piVar7 + 1;
                    }
                    iStack_e0 = 1;
                    pcStack_dc = "Array index out of bounds";
                    pcStack_d8 = "..\\BluelAPI/Container/SArray.h";
                    uStack_d4 = 0x1cd;
                    FUN_00422060(&iStack_e0);
                }
                victim_entity = *(struct_victim_entity **)(piVar1[8] + iVar6 * 4);
                if (((victim_entity->state == 4) || (victim_entity->state == 7)) &&
                    (victim_entity->is_being_rescued == '\0'))
                {
                    cVar2 = '\0';
                    iVar5 = (**(code **)(*level_or_entity_manager + 0x18))();
                    puVar3 = (undefined4 *)(iVar5 + 0x28);
                    iVar5 = (**(code **)(victim_entity->vtable + 0x18))();
                    cVar2 = check_line_of_sight((undefined4 *)(iVar5 + 0x28), puVar3, cVar2);
                    if (cVar2 == '\0')
                    {
                        iVar5 = (**(code **)(*level_or_entity_manager + 0x18))();
                        pfVar4 = (float *)(iVar5 + 0x28);
                        iVar5 = (**(code **)(victim_entity->vtable + 0x18))();
                        pfVar4 = (float *)FUN_0043b2d0(afStack_18, (float *)(iVar5 + 0x28), pfVar4);
                        if (600.0 <= SQRT(pfVar4[2] * pfVar4[2] + pfVar4[1] * pfVar4[1] + *pfVar4 * *pfVar4))
                        {
                            assign_victim_to_bot(local_e4, &victim_entity->vtable, piStack_e8);
                            local_4 = 0xffffffff;
                            FUN_00454630((int *)&puStack_f4);
                            ExceptionList = local_c;
                            return;
                        }
                    }
                }
                iVar6 = iVar6 + 1;
            } while (iVar6 < piStack_e8[9]);
        }
        FUN_00432a60((int *)&puStack_f4);
    }
    ppuStack_c8 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_c4 = "rescue\\RescueBot.cpp";
    uStack_c0 = 0xbf;
    uStack_bc = 0;
    local_4._0_1_ = 2;
    FUN_004a3a19(auStack_ac, "RESCUEBOT: No eligible victims");
    FUN_00401360(&ppuStack_c8, 3);
    local_4 = CONCAT31(local_4._1_3_, 1);
    FUN_004012c0(&ppuStack_c8);
    local_4 = 3;
    if (1 < *piStack_f0)
    {
        FUN_0041aff0((int *)&piStack_f0);
    }
    if (piStack_f0[1] != 0)
    {
        FUN_00471760(&piStack_f0, 0);
    }
    if ((piStack_f0 != (int *)0x0) && (*piStack_f0 = *piStack_f0 + -1, *piStack_f0 < 1))
    {
        if ((void *)piStack_f0[4] != (void *)0x0)
        {
            _free((void *)piStack_f0[4]);
        }
        _free(piStack_f0);
        piStack_f0 = (int *)0x0;
    }
    local_4 = 0xffffffff;
    if (puStack_f4 != (undefined4 *)0x0)
    {
        FUN_00429fe0((void *)*puStack_f4, (int)&puStack_f4);
    }
    ExceptionList = local_c;
    return;
}
