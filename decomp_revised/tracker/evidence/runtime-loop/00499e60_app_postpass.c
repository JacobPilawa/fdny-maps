
void __fastcall FUN_00499e60(int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  
  *(undefined4 *)(param_1 + 0x70) = *(undefined4 *)(param_1 + 0x7c);
  *(undefined4 *)(param_1 + 0x74) = 0;
  *(undefined1 *)(param_1 + 0x78) = 0;
  *(undefined1 *)(param_1 + 0x79) = 1;
  while( true ) {
    puVar1 = *(undefined4 **)(param_1 + 0x70);
    if (puVar1 == (undefined4 *)0x0) {
      return;
    }
    if (*(char *)(param_1 + 0x79) == '\0') {
      if (*(char *)(param_1 + 0x78) == '\0') {
        *(undefined4 **)(param_1 + 0x74) = puVar1;
        puVar1 = *(undefined4 **)(*(int *)(param_1 + 0x70) + 4);
        *(undefined4 **)(param_1 + 0x70) = puVar1;
        if (puVar1 == (undefined4 *)0x0) {
          return;
        }
        piVar2 = (int *)*puVar1;
      }
      else {
        *(undefined1 *)(param_1 + 0x78) = 0;
        piVar2 = (int *)**(undefined4 **)(param_1 + 0x70);
      }
    }
    else {
      *(undefined1 *)(param_1 + 0x79) = 0;
      piVar2 = (int *)*puVar1;
    }
    if (piVar2 == (int *)0x0) break;
    (**(code **)(*piVar2 + 0x70))();
  }
  return;
}

