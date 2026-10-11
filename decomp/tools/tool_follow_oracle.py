"""Capture original tool-follow numeric core and frame subdivision; no arithmetic stubs."""
import json, struct, random
import fire_effects_oracle as n
OUT=n.OBJECT; WRAP=0x20019000
bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def capture(entry,args):
 count=len(args);size=count*4
 code=bytearray(bytes([0xff,0x74,0x24,size])*count+b'\xe8')
 code+=struct.pack('<i',entry-(WRAP+len(code)+4))
 code+=bytes([0x83,0xc4,size,0xd9,0x1d])+struct.pack('<I',OUT)+bytes([0xc2,size,0])
 n.uc.mem_write(WRAP,bytes(code));n.uc.ctl_remove_cache(WRAP,WRAP+64);n.call(WRAP,[bits(x) for x in args]);return n.u32(OUT)
rng=random.Random(483030);samples=[]
for dt in [.001,.01,.017,.033,.1]:
 n.write(0x502b04,bits(dt))
 for smooth in [0,.00001,.006,.009,.1,1e21]:
  for i in range(24):
   current=rng.uniform(-5000,5000);previous=rng.uniform(-5000,5000)
   target=previous if i%3==0 else rng.uniform(-5000,5000)
   args=[current,target,smooth,previous,dt]
   samples.append(dict(args=args,global_dt=dt,result_bits=capture(0x483030,args)))
steps=[]
for dt in [.001,.009999998,.01,.010000002,.017,.033,.1,.25,.999]:
 n.write(0x502b04,bits(dt))
 for smooth in [0,.006,.009,.1,1e21]:
  for i in range(24):
   c=rng.uniform(-5000,5000);p=rng.uniform(-5000,5000);t=p if i%3==0 else rng.uniform(-5000,5000)
   args=[c,t,smooth,p]
   steps.append(dict(args=args,global_dt=dt,result_bits=capture(0x483140,args)))
p=n.ROOT/'evidence/tool-follow-oracle.json'
p.write_text(json.dumps(dict(provenance=dict(entry='00483030',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original core, comparison and CRT power routine executed; FSTP float result'),samples=samples,steps=steps),indent=2)+'\n')
print(len(samples),'original core results;',len(steps),'original subdivision results')
