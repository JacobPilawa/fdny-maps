"""Original D3DDrv render-state choices; device writes captured by a fixture."""
import json,hashlib,struct
import genesis_point_oracle as p
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX
p.DLL=p.ROOT/'evidence/original/D3DDrv.dll'
n=p.GenesisCollisionOracle(dict(models=[],nodes=[],leafs=[],leaf_sides=[],planes=[]))
states={}
def hook(u,a,size,data):
 if a in [0x100067e0,0x100069a0,0x100019b0]:
  sp=u.reg_read(UC_X86_REG_ESP)
  if a==0x100067e0:states[str(u.reg_read(UC_X86_REG_ECX))]=u.reg_read(UC_X86_REG_EDX)
  u.reg_write(UC_X86_REG_ESP,sp+(8 if a==0x100069a0 else 4));u.reg_write(UC_X86_REG_EIP,n._u32(sp))
n.uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
for flags in [0,4,8,16,32,64,128,160,164,255,256,512]:
 states={};n.uc.reg_write(UC_X86_REG_ESP,p.STACK);n.uc.mem_write(p.STACK,struct.pack('<I',p.RETURN));n.uc.reg_write(UC_X86_REG_EBX,flags);n.uc.emu_start(0x100076b0,p.RETURN,count=10000)
 samples.append(dict(flags=flags,states=states))
(p.ROOT/'evidence/driver-blend-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='D3DDrv.dll 100076B0',dll_sha256=hashlib.sha256(n.raw).hexdigest(),method='Original state-selection code and wrappers; device setter and texture-stage services captured/skipped'),samples=samples),indent=2)+'\n')
print(samples)
