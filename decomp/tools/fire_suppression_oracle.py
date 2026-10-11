"""Execute original spray arithmetic and douse branch with fixture host services."""
import json,struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
uc=n.uc;FIRE=n.OBJECT;PROPS=FIRE+0x300;MAIN=FIRE+0x500;STATS=FIRE+0x1000;doused=stopped=0
def ret(engine,pop):
 sp=engine.reg_read(UC_X86_REG_ESP);target=n.u32(sp);engine.reg_write(UC_X86_REG_EAX,0)
 engine.reg_write(UC_X86_REG_ESP,sp+4+pop);engine.reg_write(UC_X86_REG_EIP,target)
def hook(engine,address,size,data):
 global doused,stopped
 sp=engine.reg_read(UC_X86_REG_ESP)
 if address==0x402790:n.write(engine.reg_read(UC_X86_REG_ECX),0x501908);ret(engine,4)
 elif address==0x486980:doused+=n.u32(sp+8);ret(engine,8)
 elif address==0x4a1080:stopped+=1;ret(engine,0)
uc.hook_add(UC_HOOK_CODE,hook)
fbits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
samples=[]
for step in range(720):
 strength=[0,.05,.1,.10000001,.5,1][step%6];dt=[0,.001,.017,.1,1][step//6%5]
 spray=[0,.5,1,4][step//30%4];influence=[0,.25,.5,.75,1,1.25][step//120%6]
 for terminal,state in [(0,0),(1,0),(1,5)]:
  uc.mem_write(FIRE,b'\0'*0x2000);n.write(FIRE+0x78,PROPS);n.write(0x502aac,MAIN);n.write(MAIN+0x6e4,STATS)
  n.write(FIRE+0x70,state);n.write(FIRE+0x128,fbits(strength));n.write(PROPS+0x18,fbits(spray));n.write(0x502b04,fbits(dt))
  n.write(FIRE+0x114,FIRE+0x1800);uc.mem_write(FIRE+0x1c8,b'\1');doused=stopped=0
  n.call(0x448970,[0,0,0,fbits(influence),terminal],FIRE)
  samples.append({'strength_bits':fbits(strength),'dt_bits':fbits(dt),'spray_time_bits':fbits(spray),
                  'influence_bits':fbits(influence),'terminal':terminal,'initial_state':state,
                  'result_bits':n.u32(FIRE+0x128),'state':n.u32(FIRE+0x70),
                  'sprayed':uc.mem_read(FIRE+0x104,1)[0],'flame_enabled':uc.mem_read(FIRE+0x1c8,1)[0],
                  'doused':doused,'stopped':stopped})
(n.ROOT/'evidence/fire-suppression-oracle.json').write_text(json.dumps({
 'provenance':{'exe_sha256':n.hashlib.sha256(n.raw).hexdigest(),'entry':'00448970',
 'method':'Original spray arithmetic, state and douse branches; fixture statistic increment, sound stop and temporary string assignment; no RNG routines replaced'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original fire spray/douse calls')
