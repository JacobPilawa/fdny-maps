"""Original four extinguisher points; supplied tool pose, effects skipped."""
import json,struct,math
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP
OBJ=n.OBJECT;captured=[]
bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def hook(u,a,size,data):
 if a==0x481a50:
  captured.append([n.u32(0x502e30+4*k) for k in range(12)]);u.reg_write(UC_X86_REG_EIP,n.RETURN)
 elif a in [0x45f600,0x482430,0x480090]:
  sp=u.reg_read(UC_X86_REG_ESP);pop={0x45f600:0,0x482430:4,0x480090:8}[a];u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,n.u32(sp))
n.uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
for yaw,pitch in [(y,p) for y in [0,.3,1.2,math.pi] for p in [0,.7,-.9]]:
 for origin in [[0,0,0],[824,-120,-2092]]:
  n.uc.mem_write(OBJ,b'\0'*0x10000);n.uc.mem_write(OBJ+0x158,b'\1');n.uc.mem_write(OBJ+0x600,b'\1');n.write(0x502e60,1);n.write(0x502b04,bits(.017));n.call(0x404b50,[4357]);n.write(OBJ+0x58,1)
  c,s=math.cos(yaw),math.sin(yaw);cp,sp=math.cos(pitch),math.sin(pitch);matrix=[c,s*sp,s*cp,0,cp,-sp,-s,c*sp,c*cp]
  for col in range(3):
   for row in range(3):n.write(OBJ+0x5c+12*col+4*row,bits(matrix[row*3+col]))
  for a in range(3):n.write(OBJ+0x80+4*a,bits(origin[a]))
  before=len(captured);n.call(0x461f80,this=OBJ)
  if len(captured)!=before+1:raise RuntimeError('Sampler not reached')
  samples.append(dict(origin=origin,matrix=matrix,points_bits=captured[-1]))
(n.ROOT/'evidence/extinguisher-samples-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='00461F80 through sample generation',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original sampling/pose rotation and RNG; supplied tool pose; base Think, gauge, effect transforms skipped; stops before contact dispatch'),samples=samples),indent=2)+'\n');print(len(samples),'original sampling runs')
