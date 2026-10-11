"""Run original world update; only the final script-loading host service is bound."""
import json
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
uc=n.uc;WORLD=n.OBJECT;MAIN=WORLD+0x100;FILENAME=WORLD+0x900;calls=[]
def hook(engine,address,size,data):
 if address==0x471310:
  sp=engine.reg_read(UC_X86_REG_ESP)
  calls.append({'filename_pointer':n.u32(sp+4),'mode':n.u32(sp+8)&255})
  engine.reg_write(UC_X86_REG_EAX,0);engine.reg_write(UC_X86_REG_ESP,sp+12);engine.reg_write(UC_X86_REG_EIP,n.u32(sp))
uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
for remaining in [0,1,2,4,8,0xffffffff]:
 for completed in [0,1]:
  uc.mem_write(WORLD,b'\0'*0x1000);n.write(0x502aac,MAIN);n.write(WORLD+0x24,FILENAME)
  uc.mem_write(FILENAME,b'Data\\Maps\\Training.bsp\0');n.write(WORLD+0x2c,remaining)
  uc.mem_write(WORLD+0x28,bytes([completed]));n.write(MAIN+0x6f0,2);calls=[];steps=[]
  for tick in range(10):
   n.call(0x49e2c0,(),WORLD)
   steps.append({'remaining':n.u32(WORLD+0x2c),'complete':uc.mem_read(WORLD+0x28,1)[0],
                 'main_state':n.u32(MAIN+0x6f0),'script_calls':len(calls)})
  samples.append({'initial_remaining':remaining,'initial_complete':completed,'steps':steps,'calls':calls[:]})
(n.ROOT/'evidence/world-startup-oracle.json').write_text(json.dumps({
 'provenance':{'exe_sha256':n.hashlib.sha256(n.raw).hexdigest(),'entry':'0049E2C0',
 'method':'Original world countdown/state update; script-loading host callback records filename and mode'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original world startup scenarios, {len(samples)*10} updates')
