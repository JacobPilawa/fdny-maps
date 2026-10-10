
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall
assign_victim_to_bot(void *this, int *get_bot_name, int *victim_container_or_location_info)

{
    int *piVar1;
    void *pvVar2;
    uint uVar3;
    int *piVar4;
    int extraout_ECX;
    int *extraout_ECX_00;
    int *extraout_ECX_01;
    int *piVar5;
    int *piVar6;
    int iVar7;
    int *local_88;
    undefined4 local_84;
    int *local_80;
    int *local_7c;
    int *local_78;
    int *local_74;
    int *local_70;
    int *local_6c;
    undefined **local_68;
    char *pcStack_64;
    undefined4 uStack_60;
    undefined1 uStack_5c;
    undefined1 auStack_4c[60];
    void *pvStack_10;
    void *pvStack_c;
    undefined1 *puStack_8;
    undefined4 local_4;

    local_4 = 0xffffffff;
    puStack_8 = &LAB_004d0869;
    pvStack_c = ExceptionList;
    ExceptionList = &pvStack_c;
    piVar1 = ::get_bot_name(&local_74);
    local_4 = 0;
    pvVar2 = FUN_00412340(&local_78, "^(128,128,240)", piVar1);
    local_4._0_1_ = 1;
    pvVar2 = FUN_00403e40(pvVar2, &local_7c, ", from ");
    local_4._0_1_ = 2;
    pvVar2 = FUN_004040e0(pvVar2, &local_80, victim_container_or_location_info);
    local_4._0_1_ = 3;
    FUN_00403e40(pvVar2, &local_84, ":\n^(255,255,255)");
    local_4._0_1_ = 8;
    if (local_80 != &difficulty_level_likely)
    {
        *local_80 = *local_80 + -1;
        if (*local_80 < 1)
        {
            FUN_004011c0(&local_80);
        }
        else
        {
            local_80 = &difficulty_level_likely;
        }
    }
    local_4._0_1_ = 7;
    if (local_7c != &difficulty_level_likely)
    {
        *local_7c = *local_7c + -1;
        if (*local_7c < 1)
        {
            FUN_004011c0(&local_7c);
        }
        else
        {
            local_7c = &difficulty_level_likely;
        }
    }
    local_4._0_1_ = 6;
    if (local_78 != &difficulty_level_likely)
    {
        *local_78 = *local_78 + -1;
        if (*local_78 < 1)
        {
            FUN_004011c0(&local_78);
        }
        else
        {
            local_78 = &difficulty_level_likely;
        }
    }
    local_4 = CONCAT31(local_4._1_3_, 5);
    if (local_74 != &difficulty_level_likely)
    {
        *local_74 = *local_74 + -1;
        if (*local_74 < 1)
        {
            FUN_004011c0(&local_74);
        }
        else
        {
            local_74 = &difficulty_level_likely;
        }
    }
    piVar1 = (int *)((int)this + 8);
    uVar3 = rescuebot_hash_function(*(uint *)(*(int *)((int)this + 8) + 4));
    FUN_00484800(piVar1, uVar3);
    if (1 < *(int *)*piVar1)
    {
        FUN_00405d00(piVar1);
    }
    if (difficulty_init_likely == '\0')
    {
        difficulty_init_likely = '\x01';
        difficulty_level_likely = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
    }
    piVar1 = *(int **)(*piVar1 + 0x14 + uVar3 * 4);
    local_6c = &difficulty_level_likely;
    if (piVar1 != &difficulty_level_likely)
    {
        *piVar1 = *piVar1 + 1;
        local_6c = piVar1;
    }
    piVar1 = local_6c;
    local_4._0_1_ = 9;
    get_victim_name(get_bot_name, &local_70);
    if (difficulty_init_likely == '\0')
    {
        difficulty_init_likely = '\x01';
        difficulty_level_likely = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
    }
    local_88 = &difficulty_level_likely;
    local_4._0_1_ = 0xb;
    piVar4 = (int *)FUN_00402770(&local_88, (byte *)(piVar1 + 3));
    FUN_00401d00(&local_84, (char *)(*piVar4 + 0xc));
    local_4._0_1_ = 10;
    if (local_88 != &difficulty_level_likely)
    {
        *local_88 = *local_88 + -1;
        if (*local_88 < 1)
        {
            FUN_004011c0(&local_88);
        }
        else
        {
            local_88 = &difficulty_level_likely;
        }
    }
    local_4._0_1_ = 9;
    if ((local_70 != &difficulty_level_likely) && (*local_70 = *local_70 + -1, *local_70 < 1))
    {
        FUN_004011c0(&local_70);
    }
    FUN_004574b0((void *)(*(int *)(DAT_00503a78 + 0x30) + 0x5f4), &local_84);
    local_70 = (int *)&stack0xffffff60;
    iVar7 = extraout_ECX;
    FUN_00401cb0(&stack0xffffff60, victim_container_or_location_info);
    local_88 = (int *)&stack0xffffff5c;
    local_4._0_1_ = 0xc;
    piVar6 = extraout_ECX_00;
    get_victim_id_string(get_bot_name, (undefined4 *)&stack0xffffff5c);
    local_68 = (undefined **)&stack0xffffff58;
    piVar5 = (int *)&stack0xffffff58;
    local_4 = CONCAT31(local_4._1_3_, 0xd);
    piVar4 = extraout_ECX_01;
    (**(code **)(*get_bot_name + 0x60))();
    puStack_8._0_1_ = 0xe;
    local_68 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_64 = "rescue\\RescueBot.cpp";
    uStack_60 = 0xcd;
    uStack_5c = 0;
    puStack_8._0_1_ = 0x10;
    FUN_004a3a19(auStack_4c, "RESCUEBOT: Rescuing ");
    pvVar2 = (void *)FUN_0040f020(&local_68, piVar5);
    puStack_8._0_1_ = 0x11;
    FUN_004a3a19((void *)((int)pvVar2 + 0x1c), " (");
    pvVar2 = (void *)FUN_0040f020(pvVar2, piVar4);
    puStack_8._0_1_ = 0x12;
    FUN_004a3a19((void *)((int)pvVar2 + 0x1c), ") in ");
    pvVar2 = (void *)FUN_0040f020(pvVar2, piVar6);
    FUN_00401360(pvVar2, iVar7);
    puStack_8._0_1_ = 9;
    FUN_004012c0(&local_68);
    *(int *)(*(int *)(DAT_00503a78 + 0x30) + 0x3530) =
        *(int *)(*(int *)(DAT_00503a78 + 0x30) + 0x3530) + 1;
    set_victim_rescue_state((int)get_bot_name);
    *(int **)((int)this + 4) = get_bot_name;
    iVar7 = DAT_00502afc;
    uVar3 = rescuebot_hash_function(4000);
    *(uint *)this = uVar3 + 4000 + iVar7;
    puStack_8 = (undefined1 *)CONCAT31(puStack_8._1_3_, 5);
    if ((piVar1 != &difficulty_level_likely) &&
        (iVar7 = *piVar1, *piVar1 = iVar7 + -1, iVar7 + -1 < 1))
    {
        FUN_004011c0(&local_70);
    }
    puStack_8 = (undefined1 *)0xffffffff;
    if ((local_88 != &difficulty_level_likely) && (*local_88 = *local_88 + -1, *local_88 < 1))
    {
        FUN_004011c0(&local_88);
    }
    ExceptionList = pvStack_10;
    return;
}
