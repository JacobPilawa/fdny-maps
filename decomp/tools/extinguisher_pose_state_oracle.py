"""Capture the 004614FE/004615BC pose-window callbacks directly."""
import hashlib,json,math,struct
import extinguisher_pose_oracle as o
from unicorn.x86_const import UC_X86_REG_ESI

n=o.n
OBJ=o.OBJ;TOOL=o.TOOL;LOCAL=o.LOCAL;RETURN=n.RETURN
bits=o.bits;f32=o.f32

def words(address,count):
 return [n.u32(address+4*i) for i in range(count)]

def capture(action,tick,cooldown,yaw,pitch,position):
 n.uc.mem_write(OBJ,b'\0'*0x100000);n.heap=OBJ+0x1000
 c,s=math.cos(yaw),math.sin(yaw);cp,sp=math.cos(pitch),math.sin(pitch)
 matrix=[c,s*sp,s*cp,0,cp,-sp,-s,c*sp,c*cp]
 axes=[[f32(matrix[i]),f32(matrix[i+3]),f32(matrix[i+6])] for i in range(3)]
 o.put_frame(LOCAL,axes,[f32(v) for v in position])
 n.write(TOOL+0x3a8,cooldown);n.write(0x502afc,tick)
 address=0x4614fe if action=='begin' else 0x4615bc
 # These entries are callback bodies inside a saved-ESI stack frame, not
 # ordinary C call targets. Recreate the caller frame their epilogues expect.
 n.uc.reg_write(n.UC_X86_REG_ESP,n.STACK);n.uc.mem_write(n.STACK,b'\0'*0x40)
 n.write(n.STACK+0x1c,RETURN)
 n.uc.reg_write(UC_X86_REG_ESI,TOOL)
 try:n.uc.emu_start(address,RETURN,count=100000)
 except Exception as e:raise RuntimeError(f'{e}; instruction={n.uc.reg_read(n.UC_X86_REG_EIP):08x}') from e
 if n.uc.reg_read(n.UC_X86_REG_EIP)!=RETURN:raise RuntimeError('Callback did not return')
 return dict(input=dict(action=action,tick=tick,old_cooldown=cooldown,frame_axes=[v for axis in axes for v in axis],frame_position=[f32(v) for v in position]),
  result=dict(active=n.u32(TOOL+0x600)&255,cooldown=n.u32(TOOL+0x3a8),window=[n.u32(TOOL+0x608),n.u32(TOOL+0x60c)],position_bits=words(TOOL+0x64c,3),direction_bits=words(TOOL+0x658,3)))

def generate():
 cases=[('begin',1000,77,0,.1,(824,-120,-2092)),('end',1300,123,0,.1,(824,-120,-2092)),
  ('begin',0xfffffff0,55,.9,-.8,(101.25,-32.5,407.75)),('end',0x20,99,.9,-.8,(101.25,-32.5,407.75)),
  ('begin',0x7fffffff,0xdeadbeef,-2.3,.7,(-500,12,900)),('end',0,0xcafebabe,-2.3,.7,(-500,12,900))]
 samples=[capture(*case) for case in cases]
 out=dict(provenance=dict(entries=['004614FE','004615BC'],exe_sha256=hashlib.sha256(n.raw).hexdigest(),
  method='Direct original x86 callback execution. Controlled current tool frames and global ticks; helper 00447830 copies frame axis 1 and 0042D2B0 negates it. No RNG calls or animation event dispatch.'),samples=samples)
 (n.ROOT/'evidence/extinguisher-pose-state-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} original pose-window callback captures recorded; RNG draws: {n.draws}')

if __name__=='__main__':generate()
