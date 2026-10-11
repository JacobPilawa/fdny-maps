"""Execute the original victim beckon update and record draw order.
Only the final playback callback is replaced; selection and timer storage
execute from the original executable. Resource definitions are fixture data.
"""
import json
import fire_effects_oracle as original
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP
uc=original.uc;OBJECT=original.OBJECT;DEFINITION=OBJECT+0x200;ARRAY=OBJECT+0x300
plays=0
def playback(engine,address,size,data):
 global plays
 if address==0x4a11a0:
  plays+=1;sp=engine.reg_read(UC_X86_REG_ESP);ret=original.u32(sp)
  engine.reg_write(UC_X86_REG_ESP,sp+8);engine.reg_write(UC_X86_REG_EIP,ret)
uc.hook_add(UC_HOOK_CODE,playback)
samples=[]
for seed in [0,1,4357,2002,0xffffffff,0x12345678]:
 for count in [0,1,2,3,7]:
  for low,high in [(5.,10.),(15.,30.),(1.5,1.5)]:
   original.call(0x404b50,[seed]);uc.mem_write(OBJECT,b'\0'*0x400)
   original.write(OBJECT+0x24,ARRAY);original.write(OBJECT+0x28,count)
   original.write(OBJECT+0x3c,0xffffffff);original.write(OBJECT+0x40,500)
   original.write(OBJECT+0x44,DEFINITION)
   import struct
   uc.mem_write(DEFINITION+8,struct.pack('<ff',low,high))
   for i in range(count):original.write(ARRAY+4*i,OBJECT+0x400+32*i)
   for step in range(100):
    now=[0,499,500,1000,6000,12000,0xffffffff][step%7]
    previous=original.u32(OBJECT+0x3c);deadline=original.u32(OBJECT+0x40)
    original.write(0x502afc,now);before=original.draws;plays=0
    original.call(0x48d8f0,(),OBJECT)
    samples.append({'seed':seed,'count':count,'low':low,'high':high,'step':step,'now':now,'previous':previous,'deadline_before':deadline,'deadline_after':original.u32(OBJECT+0x40),'index':original.u32(OBJECT+0x3c),'draws':original.draws-before,'plays':plays})
(original.ROOT/'evidence/victim-sound-oracle.json').write_text(json.dumps({'provenance':{'entry':'0048D8F0','exe_sha256':original.hashlib.sha256(original.raw).hexdigest(),'method':'Original x86 routine; sound playback callback replaced; timing pairs from original victims.srf: 5..10, 15..30, 1.5..1.5 seconds; x87 0x037f'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original victim sound updates recorded')
