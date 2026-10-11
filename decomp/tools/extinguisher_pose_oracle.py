"""Capture the original extinguisher local frame and parent-resolved pose."""
import hashlib,json,math,struct
import fire_effects_oracle as n

OBJ=n.OBJECT;TOOL=OBJ+0x2000;LOCAL=TOOL+0x58;PARENT=OBJ+0x7000;OUTPUT=OBJ+0x7100
POSITION=OBJ+0x7200;DIRECTION=OBJ+0x7210
bits=lambda value:struct.unpack('<I',struct.pack('<f',value))[0]
f32=lambda value:struct.unpack('<f',struct.pack('<f',value))[0]

def put_frame(address,axes,position,scale=(1,1,1),parent=0):
 n.write(address,0x4dfc30)
 for axis in range(3):
  for component in range(3):n.write(address+4+axis*12+component*4,bits(axes[axis][component]))
 for i,value in enumerate(position):n.write(address+0x28+4*i,bits(value))
 for i,value in enumerate(scale):n.write(address+0x34+4*i,bits(value))
 n.write(address+0x40,parent)

def capture(yaw,pitch,position):
 n.uc.mem_write(OBJ,b'\0'*0x100000);n.heap=OBJ+0x1000;n.write(0x502e2c,7)
 c,s=math.cos(yaw),math.sin(yaw);cp,sp=math.cos(pitch),math.sin(pitch)
 matrix=[c,s*sp,s*cp,0,cp,-sp,-s,c*sp,c*cp]
 axes=[[f32(matrix[i]),f32(matrix[i+3]),f32(matrix[i+6])] for i in range(3)]
 put_frame(PARENT,axes,[f32(x) for x in position])
 put_frame(LOCAL,[[1,0,0],[0,1,0],[0,0,1]],[0,0,0],parent=PARENT)
 for i,x in enumerate((-30.,-18.,40.)):n.write(POSITION+4*i,bits(x))
 for i,x in enumerate((.2,-1.,.1)):n.write(DIRECTION+4*i,bits(x))
 n.call(0x42d2d0,[PARENT],LOCAL)
 n.call(0x4612b0,[POSITION,DIRECTION],TOOL)
 n.write(TOOL+0x98,PARENT)
 n.call(0x4977b0,[OUTPUT,LOCAL])
 def words(address,count):return [n.u32(address+4*i) for i in range(count)]
 return dict(input=dict(yaw=f32(yaw),pitch=f32(pitch),parent_axes=[x for axis in axes for x in axis],parent_position=[f32(x) for x in position]),
  local_bits=words(LOCAL,16),world_bits=words(OUTPUT,16))

def generate():
 samples=[]
 for yaw in [0,.3,1.2,math.pi,-2.1]:
  for pitch in [0,.7,-.9]:
   position=[824+25*yaw,-120+13*pitch,-2092-31*yaw]
   samples.append(capture(yaw,pitch,position))
 out=dict(provenance=dict(entry='004612B0 -> 004977B0',constructor='00461830',parent_link='0045F660 stores tool follower at +0x98, following player follower +0x34A4',act='assets/training/actors/original/ext.act (static, no motions)',exe_sha256=hashlib.sha256(n.raw).hexdigest(),method='Original x86 local frame constructor and recursive parent frame resolver executed. Parent frame inputs are controlled transforms; no RNG calls occur.'),samples=samples)
 (n.ROOT/'evidence/extinguisher-pose-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(len(samples),'original extinguisher poses recorded; RNG draws:',n.draws)

if __name__=='__main__':generate()
