"""Execute original model floor placement with recorded world-trace fixtures.

Original entry, coordinate arithmetic, virtual transform access, hull and
trace arguments and result assignment execute. The world collision service
returns a controlled successful contact; geometry collision is checked by
the separate retail Genesis oracle.
"""
import json,struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP

uc=n.uc;ENTITY=n.OBJECT;VT=ENTITY+0x200;MAIN=ENTITY+0x300;WORLD=ENTITY+0x400
GET_TRANSFORM=n.RETURN+0x100;request={};contact=[]
def bits(values):return list(struct.unpack('<3I',struct.pack('<3f',*values)))
def readbits(address):return list(struct.unpack('<3I',uc.mem_read(address,12)))
def ret(engine,value,pop):
 sp=engine.reg_read(UC_X86_REG_ESP);target=n.u32(sp)
 engine.reg_write(UC_X86_REG_EAX,value);engine.reg_write(UC_X86_REG_ESP,sp+4+pop);engine.reg_write(UC_X86_REG_EIP,target)
def hook(engine,address,size,data):
 global request
 if address==GET_TRANSFORM:ret(engine,ENTITY+0x24,0)
 elif address==0x492ce0:
  sp=engine.reg_read(UC_X86_REG_ESP);args=[n.u32(sp+4+i*4) for i in range(6)]
  request={'start_bits':readbits(args[0]),'mins_bits':readbits(args[1]),
           'maxs_bits':readbits(args[2]),'delta_bits':readbits(args[3])}
  engine.mem_write(args[4],struct.pack('<3f',*contact));engine.mem_write(args[5],struct.pack('<3f',0,1,0))
  ret(engine,1,24)
uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
positions=[[1104,-60,-4976],[1300,-120,-4952],[1472,-120,-4972],
           [1656,-120,-4968],[1820,-120,-4960],[1972,-120,-4960]]
positions += [[0,y,0] for y in [0,.1,-.1,10000000,-10000000,16777216,-16777216,33554432]]
for p in positions:
 for floor in [-256.125,-120,-60,0,.1,256.125]:
  uc.mem_write(ENTITY,b'\0'*0x1000);n.write(ENTITY,VT);n.write(VT+0x18,GET_TRANSFORM)
  n.write(0x503a78,MAIN);n.write(MAIN+4,WORLD)
  mins=[-30,-30,-30];maxs=[30,70,30];contact=[p[0],floor,p[2]]
  uc.mem_write(ENTITY+0x4c,struct.pack('<3f',*p));uc.mem_write(ENTITY+0x78,struct.pack('<3f',*mins));uc.mem_write(ENTITY+0x84,struct.pack('<3f',*maxs))
  n.call(0x49da00,(),ENTITY)
  samples.append({'position_bits':bits(p),'mins_bits':bits(mins),'maxs_bits':bits(maxs),
                  'contact_bits':bits(contact),'request':request,'result_bits':readbits(ENTITY+0x4c)})
(n.ROOT/'evidence/model-floor-oracle.json').write_text(json.dumps({
 'provenance':{'exe_sha256':n.hashlib.sha256(n.raw).hexdigest(),'entry':'0049DA00',
 'method':'Original floor-placement arithmetic and argument construction; fixture virtual transform accessor and successful world collision callback'},
 'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original model-floor placements recorded')
