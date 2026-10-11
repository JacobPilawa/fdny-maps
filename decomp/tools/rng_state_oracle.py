"""Capture original generator state at seed/reload boundaries and mid-block.

The 624 words, remaining count and next-word pointer come directly from
original executable memory. Each capture includes 128 original outputs.
"""
import json,struct,hashlib
import fire_effects_oracle as original
from unicorn.x86_const import UC_X86_REG_EAX
samples=[]
for seed in [0,1,2,4357,2002,0xffffffff,0x12345678,0xf0000002]:
 for drawn in [0,1,2,622,623,624,625,1247,1248,1249,2500]:
  original.call(0x404b50,[seed])
  for _ in range(drawn):original.call(0x404a70)
  remaining=original.u32(0x4ff370);pointer=original.u32(0x5023ec)
  index=624-remaining
  if index!=624 and pointer!=0x501a28+index*4:raise RuntimeError('Unexpected original RNG cursor')
  words=list(struct.unpack('<624I',original.uc.mem_read(0x501a28,624*4)))
  following=[]
  for _ in range(128):
   original.call(0x404a70);following.append(original.uc.reg_read(UC_X86_REG_EAX))
  samples.append({'seed':seed,'drawn':drawn,'remaining':remaining,'next_index':index,'words':words,'following':following})
(original.ROOT/'evidence/rng-state-oracle.json').write_text(json.dumps({'provenance':{'exe_sha256':hashlib.sha256(original.raw).hexdigest(),'word_array':'00501A28','remaining_count':'004FF370','next_pointer':'005023EC','method':'Original x86 seed and raw-word routines; capture memory then execute 128 following draws'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original state captures; {len(samples)*128} following outputs')
