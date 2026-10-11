"""Original fire sprite opacity conversion (0047D6D0), flags 0x35."""
import json,struct,random
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP
OBJ=n.OBJECT;DESC=OBJ+0x400;ARRAY=OBJ+0x800;FRAMES=OBJ+0x1000;OUT=OBJ+0x2000
bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def hook(u,a,size,data):
 if a==0x48d490:
  sp=u.reg_read(UC_X86_REG_ESP);u.reg_write(UC_X86_REG_ESP,sp+8);u.reg_write(UC_X86_REG_EIP,n.u32(sp))
n.uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
rng=random.Random(4706)
for rate in [.5,.75,1,1.5]+[struct.unpack("<f",struct.pack("<f",rng.uniform(.75,1.25)))[0] for _ in range(20)]:
 for elapsed in [0,1,2,3,7,16,17,24,33,40,49,50,75,100,0x7fffffff,0xfffffff0]:
  for alpha in [0,.01,.1,.25,.5,.75,.999,1]:
   n.uc.mem_write(OBJ,b'\0'*0x4000)
   for off,val in [(4,0),(8,1),(0xc,DESC),(0x14,100),(0x18,0x35),(0x1c,bits(rate)),(0x20,bits(alpha))]:n.write(OBJ+off,val)
   n.write(DESC,ARRAY);n.write(DESC+4,2)
   for i in range(2):
    n.write(ARRAY+4*i,FRAMES+32*i);n.write(FRAMES+32*i+4,bits(.05));n.write(FRAMES+32*i+8,bits(1))
   n.write(0x502afc,(100+elapsed)&0xffffffff);n.call(0x47d6d0,[OUT,OUT+1],OBJ)
   samples.append(dict(alpha_bits=bits(alpha),rate_bits=bits(rate),elapsed=elapsed,current=n.uc.mem_read(OUT,1)[0],next=n.uc.mem_read(OUT+1,1)[0]))
(n.ROOT/'evidence/sprite-alpha-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='0047D6D0',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),scope='Original flame/glow descriptors: alpha 1, period .05, flags 0x35; valid array bounds fixture'),samples=samples),indent=2)+'\n');print(len(samples),'original fire sprite alpha calls')
