"""Original placed-fire initialization with fixture registration/audio/floor services."""
import json,struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
uc=n.uc;FIRE=n.OBJECT;VISUAL=FIRE+0x12c;PROPS=FIRE+0x8000;MAIN=FIRE+0x9000;WORLD=FIRE+0xa000;choice=-1
fbits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def ret(engine,pop=0,result=0):
 sp=engine.reg_read(UC_X86_REG_ESP);target=n.u32(sp);engine.reg_write(UC_X86_REG_EAX,result);engine.reg_write(UC_X86_REG_ESP,sp+4+pop);engine.reg_write(UC_X86_REG_EIP,target)
def hook(engine,address,size,data):
 global choice
 sp=engine.reg_read(UC_X86_REG_ESP);this=engine.reg_read(UC_X86_REG_ECX)
 if address==0x49d350:
  out=n.u32(sp+4);n.write(out,0);n.write(out+4,0);ret(engine,4,out)
 elif address==0x402790:
  source=n.u32(sp+4)
  if source in [0x4e1748,0x4e173c,0x4e1730]:choice=[0x4e1748,0x4e173c,0x4e1730].index(source)
  n.write(this,0x501908);ret(engine,4,this)
 elif address==0x46e320:ret(engine,8)
 elif address==0x4a12f0:ret(engine,12)
 elif address==0x4a11d0:ret(engine)
 elif address==0x448630:ret(engine)
 elif address==0x43b6e0:
  count=n.u32(sp+4);buffer=n.heap;n.heap+=0x100;uc.mem_write(buffer,b'\0'*0x100);n.write(this,buffer);n.write(this+4,count);n.write(this+8,64);ret(engine,4,this)
uc.hook_add(UC_HOOK_CODE,hook)
from collections import deque
trace=deque(maxlen=20)
uc.hook_add(UC_HOOK_CODE,lambda e,a,z,d:trace.append(hex(a)))
samples=[]
for seed in [0,1,4357,2002,0xffffffff]:
 for radius,height,spray in [(150,250,0),(500,800,4),(350,800,4),(24,250,0),(288,800,0)]:
  for named in [0,1]:
   n.heap=FIRE+0x1000;uc.mem_write(FIRE,b'\0'*0x100000);n.write(0x503a78,MAIN);n.write(MAIN+4,WORLD);n.write(FIRE+0x78,PROPS);n.write(PROPS+4,0x4e1748 if named else 0)
   for off,value in [(0x10,radius),(0x14,height),(0x18,spray),(0x7c,824),(0x80,-120),(0x84,-2092)]:n.write(PROPS+off,fbits(value))
   n.write(PROPS+0x20,1)
   for off in [4,0x14,0x24,0x34,0x38,0x3c]:n.write(VISUAL+off,fbits(1))
   for desc in [0x502d08,0x502cd0]:n.write(desc+4,19);n.write(desc+0x18,0x35)
   n.write(0x502b8c,0);n.write(0x502afc,100);n.call(0x404b50,[seed]);n.draws=0;choice=-1;
   try:n.call(0x448d60,this=FIRE)
   except Exception:
    print(seed,radius,named,list(trace));raise
   key=n.u32(VISUAL+0x88)
   samples.append(dict(seed=seed,radius=radius,height=height,spray=spray,named=named,draws=n.draws,choice=choice,divider=uc.mem_read(FIRE+0x11c,1)[0],spray_bits=n.u32(PROPS+0x18),key_bits=[n.u32(key+8+4*a) for a in range(3)],position_bits=[n.u32(FIRE+0x7c+4*a) for a in range(3)],strength_bits=n.u32(FIRE+0x128)))
(n.ROOT/'evidence/fire-entity-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='00448D60',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original placed-fire initializer and effect constructors; fixture string/registration, paused sound setup, array allocation and floor callback. Floor callback leaves transform unchanged.'),samples=samples),indent=2)+'\n');print(len(samples),'original placed-fire initializations')
