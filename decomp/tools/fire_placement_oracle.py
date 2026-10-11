"""Execute original flame/glow placement with no replaced arithmetic."""
import json,struct,math
import fire_effects_oracle as n
uc=n.uc;OBJ=n.OBJECT;BOUNDS=OBJ+0x400;SPRITES=OBJ+0x1000
fbits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
f32=lambda f:struct.unpack('<f',struct.pack('<f',f))[0]
matrices=[[1,0,0,0,1,0,0,0,1]]
for angle in [math.pi/60,math.pi/4,math.pi/2]:
 c,s=f32(math.cos(angle)),f32(math.sin(angle));matrices.append([c,0,s,0,1,0,-s,0,c]);matrices.append([1,0,0,0,c,-s,0,s,c])
samples=[]
for size in [[225,62.5,225],[750,200,750],[525,200,525],[0,0,0]]:
 for matrix in matrices:
  for position in [[0,0,0],[824,-120,-2092]]:
   uc.mem_write(OBJ,b'\0'*0x4000);n.write(0x502c3c,1)
   for row in range(3):
    for col in range(3):n.write(OBJ+4+col*12+row*4,fbits(matrix[row*3+col]))
   for a,f in enumerate(position):n.write(OBJ+0x28+4*a,fbits(f))
   for a in range(3):n.write(OBJ+0x34+4*a,fbits(1));n.write(OBJ+0x44+4*a,fbits(size[a]))
   for i in range(4):n.write(OBJ+0x60+4*i,SPRITES+i*0x300)
   bounds=[f32(position[0]-size[0]*.5),position[1],f32(position[2]-size[2]*.5),f32(position[0]+size[0]*.5),f32(position[1]+size[1]),f32(position[2]+size[2]*.5)]
   for a,f in enumerate(bounds):n.write(BOUNDS+4*a,fbits(f))
   n.call(0x44a840,[BOUNDS],OBJ)
   sprites=[]
   for i in range(4):
    p=SPRITES+i*0x300
    sprites.append(dict(vertices_bits=[[n.u32(p+off+4*a) for a in range(3)] for off in [0xd8,0xfc,0x120,0x144]],width_bits=n.u32(p+0xb8),origin_y_bits=n.u32(p+0x60)))
   samples.append(dict(size=size,matrix=matrix,position=position,bounds=bounds,sprites=sprites))
(n.ROOT/'evidence/fire-placement-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='0044A840',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original placement and transform arithmetic; four fixture sprite objects; static temporary-vector registration marked initialized'),samples=samples),indent=2)+'\n');print(len(samples),'original flame/glow placements')
