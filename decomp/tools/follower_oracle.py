"""Original 00483720 follower states, with an actual unparented transform fixture."""
import json,struct,random
import fire_effects_oracle as n
OBJ=n.OBJECT;PARENT=OBJ+0x1000
bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
rng=random.Random(483720);samples=[]
for flags in [0,0x20,0x70,0xe,0x7e]:
 for held,allow,resume in [(0,0,0),(0,1,0),(1,1,0),(1,1,1)]:
  for i in range(20):
   dt=[.001,.017,.033,.1][i%4];target=[rng.uniform(-100,100) for _ in range(3)];position=[v+rng.choice([0,.0001,10,2000]) for v in target];previous=[v+rng.uniform(-5,5) for v in target]
   n.uc.mem_write(OBJ,b'\0'*0x2000);n.write(0x502b04,bits(dt));n.write(OBJ+0x70,PARENT);n.write(OBJ+0x6c,flags)
   for a in range(3):
    n.write(PARENT+4+16*a,bits(1));n.write(PARENT+0x28+4*a,bits(target[a]));n.write(OBJ+0x28+4*a,bits(position[a]));n.write(OBJ+0x74+4*a,bits(previous[a]));n.write(OBJ+0x44+4*a,bits([20,1000,20][a]));n.write(OBJ+0x50+4*a,bits([.05,0,.05][a]));n.write(OBJ+0x5c+4*a,bits([.006,.009,.006][a]))
   n.uc.mem_write(OBJ+0x80,bytes([held,allow,resume]));n.call(0x483720,this=OBJ)
   samples.append(dict(flags=flags,held=held,allow=allow,resume=resume,dt=dt,target=target,position=position,previous=previous,result_bits=[n.u32(OBJ+0x28+4*a) for a in range(3)],state=list(n.uc.mem_read(OBJ+0x80,3))))
(n.ROOT/'evidence/follower-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='00483720',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original follower and world transform copy, no arithmetic replacement'),samples=samples),indent=2)+'\n');print(len(samples),'original follower states')
