
/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4
FUN_0049a5f0(FILE *param_1,char *param_2,undefined4 param_3,int *param_4,int param_5,char param_6)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int _C;
  uint unaff_retaddr;
  char local_1004 [4096];
  uint local_4;

  local_4 = DAT_005007e0 ^ unaff_retaddr;
  bVar1 = (byte)param_1->_flag;
  iVar5 = 0;
  uVar4 = 0;
  do {
    if ((bVar1 & 0x10) != 0) {
      return 0xffffffff;
    }
    iVar3 = FID_conflict__getc(param_1);
    cVar2 = (char)iVar3;
    switch(uVar4) {
    case 0:
      if (param_5 == 0) {
        _C = (int)cVar2;
        iVar3 = _isdigit(_C);
        if (iVar3 == 0) {
          iVar3 = _isalpha(_C);
          if ((iVar3 == 0) && (cVar2 != '_')) {
            switch(_C) {
            default:
              FUN_0049a540(*param_4," Unrecognized character.");
              break;
            case 10:
              *param_4 = *param_4 + 1;
              break;
            case 0x21:
              return 0xb;
            case 0x22:
              return 0x1a;
            case 0x23:
              FUN_0049a540(*param_4,"Preprocessor flag found.  Try running PRECOMP.");
              break;
            case 0x26:
              return 0xc;
            case 0x27:
              return 0x1b;
            case 0x28:
              return 0x14;
            case 0x29:
              return 0x15;
            case 0x2a:
              return 4;
            case 0x2b:
              return 6;
            case 0x2c:
              return 0x28;
            case 0x2d:
              uVar4 = 4;
              break;
            case 0x2e:
              return 0x2b;
            case 0x2f:
              return 3;
            case 0x3a:
              return 0x29;
            case 0x3b:
              return 0x2a;
            case 0x3c:
              return 9;
            case 0x3d:
              return 7;
            case 0x3e:
              return 8;
            case 0x5b:
              return 0x16;
            case 0x5c:
              return 0x1c;
            case 0x5d:
              return 0x17;
            case 0x5e:
              return 0xe;
            case 0x7b:
              return 0x18;
            case 0x7c:
              return 0xd;
            case 0x7d:
              return 0x19;
            case -1:
            case 0:
            case 9:
            case 0xd:
            case 0x20:
              break;
            }
            break;
          }
          *param_2 = cVar2;
          uVar4 = 2;
        }
        else {
          uVar4 = 3;
          local_1004[0] = cVar2;
        }
        iVar5 = 1;
      }
      else if (param_5 == 1) {
        if (cVar2 == param_6) {
          param_2[iVar5] = '\0';
          return 1;
        }
        param_2[iVar5] = cVar2;
        iVar5 = iVar5 + 1;
      }
      else if (param_5 == 2) {
        if (param_6 <= iVar5) {
          param_2[iVar5] = '\0';
          return 1;
        }
        param_2[iVar5] = cVar2;
        iVar5 = iVar5 + 1;
      }
      break;
    case 2:
      iVar3 = _isalpha((int)cVar2);
      if ((iVar3 == 0) && (iVar3 = _isdigit((int)cVar2), iVar3 == 0)) {
        if (cVar2 != '_') {
          param_2[iVar5] = '\0';
          param_1->_ptr = param_1->_ptr + -1;
          param_1->_cnt = param_1->_cnt + 1;
          return 1;
        }
        param_2[iVar5] = '_';
        iVar5 = iVar5 + 1;
      }
      else {
        param_2[iVar5] = cVar2;
        iVar5 = iVar5 + 1;
      }
      break;
    case 3:
      iVar3 = _isdigit((int)cVar2);
      if (iVar3 == 0) {
        if (cVar2 != '.') {
          local_1004[iVar5] = '\0';
          _sscanf(local_1004,"%f",param_3);
          param_1->_cnt = param_1->_cnt + 1;
          param_1->_ptr = param_1->_ptr + -1;
          return 2;
        }
        local_1004[iVar5] = '.';
        iVar5 = iVar5 + 1;
      }
      else {
        local_1004[iVar5] = cVar2;
        iVar5 = iVar5 + 1;
      }
      break;
    case 4:
      iVar5 = _isdigit((int)cVar2);
      if (iVar5 == 0) {
        param_1->_cnt = param_1->_cnt + 1;
        param_1->_ptr = param_1->_ptr + -1;
        return 5;
      }
      local_1004[0] = '-';
      iVar5 = 2;
      uVar4 = 3;
      local_1004[1] = cVar2;
    }
    bVar1 = (byte)param_1->_flag;
  } while( true );
}
