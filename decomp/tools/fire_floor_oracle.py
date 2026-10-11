"""Original fire floor alignment; world collision returns recorded fixture contacts."""
import json,struct,math
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
uc=n.uc;FIRE=n.OBJECT;VIS=FIRE+0x12c;MAIN=FIRE+0x1000;WORLD=FIRE+0x2000;STUB=0x20019000;contact=normal=None;request=None
fbits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def hook(engine,address,size,data):
 global request
 if address==STUB:
  sp=engine.reg_read(UC_X86_REG_ESP);start=n.u32(sp+16);end=n.u32(sp+20);out=n.u32(sp+44)
  request=dict(start_bits=[n.u32(start+4*a) for a in range(3)],end_bits=[n.u32(end+4*a) for a in range(3)],contents=n.u32(sp+24),flags=n.u32(sp+28))
  for a in range(3):n.write(out+0xc+4*a,fbits(contact[a]));n.write(out+0x1c+4*a,fbits(normal[a]))
  engine.reg_write(UC_X86_REG_EAX,1);engine.reg_write(UC_X86_REG_ESP,sp+4);engine.reg_write(UC_X86_REG_EIP,n.u32(sp))
uc.hook_add(UC_HOOK_CODE,hook);n.write(0x4dc0bc,STUB)
samples=[]
for origin in [[0,0,0],[824,-120,-2092],[616,-176,-2708]]:
 for normal in [[0,1,0],[.5,1,0],[0,1,.5],[.25,.75,.5]]:
  for yaw in [0,math.pi/3]:
   uc.mem_write(FIRE,b'\0'*0x4000);n.write(0x503a78,MAIN);n.write(MAIN+4,WORLD);n.write(0x502be8,3)
   c,s=math.cos(yaw),math.sin(yaw);matrix=[c,0,s,0,1,0,-s,0,c]
   for row in range(3):
    for col in range(3):n.write(VIS+4+12*col+4*row,fbits(matrix[row*3+col]))
   for a in range(3):n.write(VIS+0x28+4*a,fbits(origin[a]));n.write(VIS+0x34+4*a,fbits(1))
   contact=[origin[0]+.125,origin[1]-40,origin[2]-.125];request=None;n.call(0x448630,this=FIRE)
   samples.append(dict(origin=origin,matrix=matrix,contact=contact,normal=normal,request=request,result_matrix_bits=[n.u32(VIS+4+12*col+4*row) for row in range(3) for col in range(3)],result_origin_bits=[n.u32(VIS+0x28+4*a) for a in range(3)]))
(n.ROOT/'evidence/fire-floor-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='00448630',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original trace request and transform arithmetic; fixture successful world contacts; static temporaries marked initialized'),samples=samples),indent=2)+'\n');print(len(samples),'original fire floor alignments')
