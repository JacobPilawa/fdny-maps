"""Run the original game clock; Windows clock/sleep APIs use supplied times."""
import struct,json,sys,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'tools/runtime-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_FPCW,UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EIP
raw=(ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32)
pe=struct.unpack_from('<I',raw,0x3c)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];size=struct.unpack_from('<I',raw,opt+56)[0]
uc.mem_map(base,(size+4095)&~4095);uc.mem_write(base,raw[:struct.unpack_from('<I',raw,opt+60)[0]])
sections=opt+struct.unpack_from('<H',raw,pe+20)[0]
for i in range(struct.unpack_from('<H',raw,pe+6)[0]):
 _,rva,length,offset=struct.unpack_from('<IIII',raw,sections+i*40+8);uc.mem_write(base+rva,raw[offset:offset+length])
uc.mem_map(0x20000000,0x20000);uc.mem_map(0x70000000,0x1000);uc.reg_write(UC_X86_REG_FPCW,0x037f)
OBJ=0x70000300;RET=0x70000100;TIME=0x70000800;SLEEP=TIME+16;STACK=0x20010000
uc.mem_write(0x4dc410,struct.pack('<I',TIME));uc.mem_write(0x4dc278,struct.pack('<I',SLEEP))
now=0;sleeps=0
def hook(engine,address,size,data):
 global now,sleeps
 if address not in (TIME,SLEEP):return
 sp=engine.reg_read(UC_X86_REG_ESP);ret=struct.unpack('<I',engine.mem_read(sp,4))[0]
 if address==TIME:engine.reg_write(UC_X86_REG_EAX,now);cleanup=4
 else:
  delay=struct.unpack('<I',engine.mem_read(sp+4,4))[0];now=(now+delay)&0xffffffff;sleeps+=1;cleanup=8
 engine.reg_write(UC_X86_REG_ESP,sp+cleanup);engine.reg_write(UC_X86_REG_EIP,ret)
uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
for initial in [0,500,0xffffffe0]:
 now=initial;uc.mem_write(OBJ,struct.pack('<IIIIffff',0,0,initial,0,0,0,60,0))
 for step in range(1000):
  delta=[17,16,1,0,33,100,101,250,500,18,17,17,17][step%13];now=(now+delta)&0xffffffff
  uc.mem_write(OBJ+28,struct.pack('<I',int(step%37==15)));before=list(struct.unpack('<8I',uc.mem_read(OBJ,32)));start=now;sleeps=0
  uc.reg_write(UC_X86_REG_ECX,OBJ);uc.reg_write(UC_X86_REG_ESP,STACK);uc.mem_write(STACK,struct.pack('<I',RET));uc.emu_start(0x42bf70,RET,count=100000)
  after=list(struct.unpack('<8I',uc.mem_read(OBJ,32)));samples.append({'before':before,'now':start,'after':after,'sleep_calls':sleeps,'end_now':now})
(ROOT/'evidence/clock-oracle.json').write_text(json.dumps({'provenance':{'exe_sha256':hashlib.sha256(raw).hexdigest(),'entry':'0042BF70','method':'Original x86 instructions; supplied Windows timeGetTime and Sleep; x87 0x037f'},'samples':samples}))
print('Recorded 3,000 original clock updates including sleep, pause, stalls and timer wrap')
