"""Original ellipsoidal fire contact strength (0044A7A0)."""
import json,struct,random
import fire_effects_oracle as n
OBJ=n.OBJECT;POINT=OBJ+0x1000;SPRITE=OBJ+0x2000;OUT=OBJ+0x3000;WRAP=0x20019000
bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
code=bytearray.fromhex('ff742408ff742408e8');code+=struct.pack('<i',0x44a7a0-(WRAP+13));code+=b'\xd9\x1d'+struct.pack('<I',OUT)+bytes.fromhex('c20800');n.uc.mem_write(WRAP,bytes(code))
rng=random.Random(4477);samples=[]
for size in [[0,0,0],[.001,1,.001],[.01,1,.01],[112.5,62.5,112.5],[375,200,375],[150,250,300]]:
 for padding in [0,60]:
  for alpha in [.1,.999,1]:
   points=[[0,0,0],[0,30,0],[60,0,0],[0,60,0],[1000,0,0]]+[[rng.uniform(-500,500),rng.uniform(-200,200),rng.uniform(-500,500)] for _ in range(20)]
   for point in points:
    n.uc.mem_write(OBJ,b'\0'*0x4000)
    for a in range(3):n.write(OBJ+0x44+4*a,bits(size[a]));n.write(POINT+4*a,bits(point[a]))
    n.write(OBJ+0x60,SPRITE);n.write(SPRITE+0xd0,bits(alpha));n.call(WRAP,[POINT,bits(padding)],OBJ)
    samples.append(dict(size=size,padding=padding,alpha=alpha,point=point,result_bits=n.u32(OUT)))
(n.ROOT/'evidence/fire-contact-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='0044A7A0',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original arithmetic and vector subtraction; result captured by FSTP float wrapper'),samples=samples),indent=2)+'\n');print(len(samples),'original fire contacts')
