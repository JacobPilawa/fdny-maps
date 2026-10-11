"""Capture 004612B0's local-frame math for controlled pose vectors.

These are constructor-level inputs, not claims about live extinguisher
animation. The original x86 routine is executed directly; no RNG consumer is
called.
"""
import hashlib,json,struct
import extinguisher_pose_oracle as o

n=o.n
OBJ=o.OBJ;TOOL=o.TOOL;LOCAL=o.LOCAL
POSITION=o.POSITION;DIRECTION=o.DIRECTION
bits=o.bits;f32=o.f32

def words(address,count):
 return [n.u32(address+4*i) for i in range(count)]

def capture(position,direction):
 n.uc.mem_write(OBJ,b'\0'*0x100000);n.heap=OBJ+0x1000;n.write(0x502e2c,7)
 o.put_frame(LOCAL,[[1,0,0],[0,1,0],[0,0,1]],[0,0,0])
 position=[f32(x) for x in position];direction=[f32(x) for x in direction]
 for i,x in enumerate(position):n.write(POSITION+4*i,bits(x))
 for i,x in enumerate(direction):n.write(DIRECTION+4*i,bits(x))
 n.call(0x4612b0,[POSITION,DIRECTION],TOOL)
 return dict(input=dict(position=position,direction=direction),
  intermediates=dict(scaled_direction_bits=words(0x502e14,3),first_cross_bits=words(0x502e20,3),second_cross_bits=words(0x502e08,3)),
  local_bits=words(LOCAL,16))

def generate():
 directions=[
  (.2,-1,.1),(.5,-1,.2),(1,.3,-.7),(-.4,.8,.2),(.1,1,.2),
  (-1,-.2,.4),(.03,-.4,1),(.7,.01,-.3),(-.6,-.9,-.2),
  (.9,.45,.02),(-.12,.35,-.8),(.4,-.02,-.1),(2,-3,4),
  (-2,3,-4),(.0003,-1,.0007),(.8,-.001,.7),(-.7,.002,.3),
  (.45,-.6,.45),(-.45,-.6,.45),(.45,-.6,-.45),(-.45,-.6,-.45),
  (1.25,-.75,.125),(-.125,1.5,.625),(.0625,-2.5,-1.125),
 ]
 samples=[]
 for i,direction in enumerate(directions):
  position=(824+17*i,-120-3*i,-2092+11*i)
  samples.append(capture(position,direction))
 out=dict(provenance=dict(entry='004612B0',exe_sha256=hashlib.sha256(n.raw).hexdigest(),
  method='Direct original x86 execution of the local frame builder. Controlled finite nonparallel directions and positions; frame scale preinitialized to one. No animation caller, contact sampler, or RNG consumer is invoked.'),samples=samples)
 (n.ROOT/'evidence/extinguisher-frame-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} original 004612B0 local frames recorded; RNG draws: {n.draws}')

if __name__=='__main__':generate()
