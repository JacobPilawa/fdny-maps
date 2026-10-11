"""Execute original sound constructor through the initial deadline store.

Stop before resource loading; this captures the native timer arithmetic and
its one shared RNG draw without replacing them or pretending to run loading.
"""
import json,struct
import fire_effects_oracle as n
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EIP
samples=[]
for seed in [0,1,4357,2002,0xffffffff,0x12345678]+[i*0x01010101 for i in range(32)]:
 for advance in [0,623,624]:
  n.uc.mem_write(n.OBJECT,b'\0'*0x1000);n.write(0,0)
  n.call(0x404b50,[seed])
  for _ in range(advance):n.call(0x404a70)
  n.draws=0;n.uc.reg_write(UC_X86_REG_ESP,n.STACK);n.uc.reg_write(UC_X86_REG_ECX,n.OBJECT)
  n.uc.mem_write(n.STACK,struct.pack('<3I',n.RETURN,n.OBJECT+0x100,0))
  n.uc.emu_start(0x48e080,0x48e0ef,count=100000)
  if n.uc.reg_read(UC_X86_REG_EIP)!=0x48e0ef:raise RuntimeError('Did not reach timer-store boundary')
  samples.append({'seed':seed,'advance':advance,'deadline':n.u32(n.OBJECT+0x40),
                  'draws':n.draws,'remaining':n.u32(0x4ff370)})
(n.ROOT/'evidence/victim-sound-init-oracle.json').write_text(json.dumps({
 'provenance':{'exe_sha256':n.hashlib.sha256(n.raw).hexdigest(),'entry':'0048E080',
 'stop_before':'0048E0EF','method':'Original sound constructor through timer store, with original TSound initialization and RNG; resource-loading remainder not executed'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original initial victim sound deadlines')
