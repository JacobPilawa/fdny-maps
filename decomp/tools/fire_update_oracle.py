"""Execute original Fire Think; fixture visual/audio/path services only."""
import json,struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
uc=n.uc;FIRE=n.OBJECT;SCRIPT=FIRE+0x24;PROPS=FIRE+0x500
calls=[];presentation=alpha=None
fbits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def ret(engine,pop=0):
 sp=engine.reg_read(UC_X86_REG_ESP);target=n.u32(sp);engine.reg_write(UC_X86_REG_EAX,0)
 engine.reg_write(UC_X86_REG_ESP,sp+4+pop);engine.reg_write(UC_X86_REG_EIP,target)
def hook(engine,address,size,data):
 global presentation,alpha
 sp=engine.reg_read(UC_X86_REG_ESP)
 if address in [0x44a6e0,0x44a6b0,0x44b060,0x449340,0x42d280,0x44a610]:
  calls.append(hex(address))
  if address==0x42d280:presentation=n.u32(sp+12)
  if address==0x44a610:alpha=n.u32(sp+4)
  ret(engine,{0x44a6b0:4,0x42d280:16,0x44a610:4}.get(address,0))
uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
for state in [0,4,5]:
 for strength in [0,.05,.1,.25,.49,.5,.9,.999,1]:
  for dt in [0,.001,.017,.1,1]:
   for sprayed in [0,1]:
    for divider in [0,1,2,3,255]:
     uc.mem_write(FIRE,b'\0'*0x1000);n.write(FIRE+0x48,PROPS);n.write(PROPS+4,1)
     n.write(FIRE+0x70,state);n.write(FIRE+0x128,fbits(strength));n.write(0x502b04,fbits(dt))
     n.write(FIRE+0xb8,4);uc.mem_write(FIRE+0x104,bytes([sprayed]));uc.mem_write(FIRE+0x11c,bytes([divider]))
     calls=[];presentation=alpha=None;n.draws=0;n.call(0x449fe0,this=SCRIPT)
     samples.append(dict(state=state,strength_bits=fbits(strength),dt_bits=fbits(dt),sprayed=sprayed,divider=divider,
      result_bits=n.u32(FIRE+0x128),result_sprayed=uc.mem_read(FIRE+0x104,1)[0],result_divider=uc.mem_read(FIRE+0x11c,1)[0],
      exposure_count=n.u32(FIRE+0xb8),presentation_bits=presentation,alpha_input_bits=alpha,calls=calls,draws=n.draws))
(n.ROOT/'evidence/fire-update-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='00449FE0',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original Think control flow/arithmetic; fixture visual calls, path advancement; named fire, absent sound handle. Path RNG excluded by fixture.'),samples=samples),indent=2)+'\n')
print(len(samples),'original fire Think calls')
