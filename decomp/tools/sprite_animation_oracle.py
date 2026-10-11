"""Execute original frame deadline, advancement and looping routines."""
import json,struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP
uc=n.uc;OBJ=n.OBJECT;DESC=OBJ+0x400;ARRAY=OBJ+0x800;FRAMES=OBJ+0x1000
fbits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def hook(engine,address,size,data):
 if address==0x48d490:
  sp=engine.reg_read(UC_X86_REG_ESP);engine.reg_write(UC_X86_REG_ESP,sp+8);engine.reg_write(UC_X86_REG_EIP,n.u32(sp))
uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
for count in [1,2,19]:
 for rate in [.5,.75,1,1.25,1.5]:
  for period in [.016666667,.033333333,.05,.1]:
   for started in [0,100,0x7ffffff0,0xfffffff0]:
    for flags in [0,1,8,9]:
     for delta in [0,16,33,50,100,1000]:
      uc.mem_write(OBJ,b'\0'*0x4000);n.write(OBJ+4,count-1);n.write(OBJ+8,0);n.write(OBJ+0xc,DESC);n.write(OBJ+0x14,started);n.write(OBJ+0x18,flags);n.write(OBJ+0x1c,fbits(rate));uc.mem_write(OBJ+0x30,b'\1');n.write(DESC,ARRAY);n.write(DESC+4,count)
      for i in range(count):n.write(ARRAY+i*4,FRAMES+i*0x20);n.write(FRAMES+i*0x20+4,fbits(period))
      now=(started+delta)&0xffffffff;n.write(0x502afc,now);n.call(0x47d660,this=OBJ)
      samples.append(dict(frames=count,rate_bits=fbits(rate),period_bits=fbits(period),started=started,flags=flags,now=now,frame=n.u32(OBJ+4),next_frame=n.u32(OBJ+8),result_started=n.u32(OBJ+0x14),active=uc.mem_read(OBJ+0x30,1)[0]))
(n.ROOT/'evidence/sprite-animation-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='0047D660',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original deadline/advance/loop code; array bounds wrapper skipped for valid descriptor fixtures'),samples=samples),indent=2)+'\n');print(len(samples),'original sprite animation updates')
