"""Run original fire visual initialization, including its real constructors.

Only operator new is replaced with a fixture heap. World registration is
avoided by passing a null world; no random-producing call is replaced.
"""
import sys,struct,json,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools/runtime-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_FPCW
raw=(ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()
uc=Uc(UC_ARCH_X86,UC_MODE_32)
pe=struct.unpack_from('<I',raw,0x3c)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];size=struct.unpack_from('<I',raw,opt+56)[0]
uc.mem_map(base,(size+4095)&~4095);uc.mem_write(base,raw[:struct.unpack_from('<I',raw,opt+60)[0]])
section=opt+struct.unpack_from('<H',raw,pe+20)[0]
for i in range(struct.unpack_from('<H',raw,pe+6)[0]):
 _,rva,length,offset=struct.unpack_from('<IIII',raw,section+i*40+8);uc.mem_write(base+rva,raw[offset:offset+length])
uc.mem_map(0,0x1000);uc.mem_map(0x20000000,0x20000);uc.mem_map(0x30000000,0x100000);uc.reg_write(UC_X86_REG_FPCW,0x037f)
STACK=0x20010000;RETURN=0x20018000;OBJECT=0x30000000;heap=OBJECT+0x1000;draws=0
u32=lambda a:struct.unpack('<I',uc.mem_read(a,4))[0]
def write(a,n):uc.mem_write(a,struct.pack('<I',n))
def hook(engine,address,size,data):
 global heap,draws
 if address==0x404ac0:draws+=1
 if address==0x4a5a84:
  sp=engine.reg_read(UC_X86_REG_ESP);n=u32(sp+4);p=heap;heap+=(n+15)&~15
  if heap>=0x30100000:raise RuntimeError('Fixture heap exhausted')
  engine.reg_write(UC_X86_REG_EAX,p);engine.reg_write(UC_X86_REG_ESP,sp+4);engine.reg_write(UC_X86_REG_EIP,u32(sp))
uc.hook_add(UC_HOOK_CODE,hook)
def call(address,args=(),this=None):
 uc.reg_write(UC_X86_REG_ESP,STACK);uc.mem_write(STACK,struct.pack('<I',RETURN)+b''.join(struct.pack('<I',a) for a in args))
 if this is not None:uc.reg_write(UC_X86_REG_ECX,this)
 try:uc.emu_start(address,RETURN,count=100000)
 except Exception as e:raise RuntimeError(f'{e}; instruction={uc.reg_read(UC_X86_REG_EIP):08x}') from e
 if uc.reg_read(UC_X86_REG_EIP)!=RETURN:raise RuntimeError('Original routine did not return')
def generate():
 global heap,draws
 samples=[]
 for seed in [0,1,4357,2002,0xffffffff,0x12345678]+[i*0x01010101 for i in range(32)]:
  for smoke in [0,1]:
   for frames in [1,2,8,16,19]:
    heap=OBJECT+0x1000;uc.mem_write(OBJECT,b'\0'*0x100000)
    # Original cached sprite descriptors: count, flags; the same frame-count
    # global supplies both the flame and glow's initial-frame upper bound.
    for descriptor in [0x502d08,0x502cd0]:write(descriptor+4,frames);write(descriptor+0x18,1)
    call(0x404b50,[seed]);draws=0;call(0x44b270,[0,smoke],OBJECT)
    def record(step,requested,before):
     sprites=[]
     for i in range(4):
      p=u32(OBJECT+0x60+i*4);sprites.append({'rate_bits':u32(p+0x1c),'frame':u32(p+4),'next_frame':u32(p+8)})
     samples.append({'seed':seed,'initial_smoke':smoke,'smoke':requested,'frames':frames,'step':step,'draws':draws-before,'smoke_interval_bits':u32(OBJECT+0x54),'sprites':sprites})
    record(0,smoke,0)
    # A non-null registered world means sprites already exist. Recreate
    # smoke if it was absent, then confirm a repeated init draws nothing.
    write(OBJECT+0x5c,1)
    for step in [1,2]:
     before=draws;call(0x44b270,[0,1],OBJECT);record(step,1,before)

 out={'provenance':{'exe_sha256':hashlib.sha256(raw).hexdigest(),'entry':'0044B270','method':'Original x86 initializer and constructors; only operator new replaced with a fixture heap; null world avoids external registration; x87 control 0x037f'},'samples':samples}
 (ROOT/'evidence/fire-effects-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} original fire-effect initializations recorded')

if __name__=="__main__":generate()
