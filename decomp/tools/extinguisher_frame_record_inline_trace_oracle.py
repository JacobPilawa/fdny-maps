"""Capture 00482430 input, rotation-helper output, and final derived record."""
import hashlib
import json
import sys
import struct
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import extinguisher_frame_record_inline_oracle as fixture
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP

FRAME=fixture.FRAME;SECOND=fixture.SECOND;MANAGER=fixture.MANAGER
helper_inputs=[]
pending_clips=[]
coordinate_calls=[]
pending_coordinates=[]
def record_helper_inputs(engine,address,size,data):
 for return_address,record in pending_coordinates:
  if address==return_address and 'result' not in record:
   record['result']=[n.u32(record['x_pointer']),n.u32(record['y_pointer'])]
 for return_address,record in pending_clips:
  if address==return_address and 'return_status' not in record:
   points=record['point_pointer'];uv=record['uv_pointer']
   record['return_status']=engine.reg_read(UC_X86_REG_EAX)
   record['return_coordinates']=[n.u32(points+off) for off in (0,4,0x24,0x28,0x48,0x4c,0x6c,0x70)]
   record['return_uv']=[n.u32(uv+4*i) for i in range(12)]
 if address==0x47e6d0:
  obj=engine.reg_read(UC_X86_REG_ECX)
  helper_inputs.append(dict(helper='0047E6D0',object=obj,coordinates=[n.u32(obj+off) for off in (0x10,0x14,0x34,0x38,0x58,0x5c,0x7c,0x80)],source_uv=[n.u32(obj+0xa0+4*i) for i in range(12)]))
 if address==0x48a650:
  sp=engine.reg_read(UC_X86_REG_ESP);viewport=engine.reg_read(UC_X86_REG_ECX);points=n.u32(sp+4);uv=n.u32(sp+8);clip=n.u32(sp+12)
  rec=dict(helper='0048A650',viewport=[n.u32(viewport+4*i) for i in range(4)],point_pointer=points,uv_pointer=uv,clip_pointer=clip,clip_rect=[n.u32(clip+4*i) for i in range(4)],coordinates=[n.u32(points+off) for off in (0,4,0x24,0x28,0x48,0x4c,0x6c,0x70)],uv=[n.u32(uv+4*i) for i in range(12)])
  helper_inputs.append(rec);pending_clips.append((n.u32(sp),rec))
 if address==0x48a910:
  sp=engine.reg_read(UC_X86_REG_ESP);xptr=n.u32(sp+4);yptr=n.u32(sp+8)
  rec=dict(viewport=[n.u32(engine.reg_read(UC_X86_REG_ECX)+4*i) for i in range(4)],x_pointer=xptr,y_pointer=yptr,input=[n.u32(xptr),n.u32(yptr)])
  coordinate_calls.append(rec);pending_coordinates.append((n.u32(sp),rec))
n.uc.hook_add(UC_HOOK_CODE,record_helper_inputs)
def words(address,size):return [n.u32(address+4*i) for i in range(size//4)]
def capture(inp):
 helper_inputs.clear()
 pending_clips.clear()
 coordinate_calls.clear();pending_coordinates.clear()
 n.uc.mem_write(fixture.OBJECT,b'\0'*0x100000)
 n.write(0x4ffad8,inp['width']);n.write(0x4ffadc,inp['height']);n.write(0x4ffae0,0);n.write(0x4ffae4,0)
 n.write(0x503a78,MANAGER);n.write(MANAGER+4,MANAGER+0x100)
 n.write(FRAME,inp['center'][0]);n.write(FRAME+4,inp['center'][1]);n.write(FRAME+0x18,inp['rate_bits'])
 for i,value in enumerate(inp['keys']):n.write(FRAME+8+i*4,value)
 n.write(SECOND+0x104,0);n.write(SECOND+0x108,601)
 n.call(0x47edf0,[FRAME+8,0],this=SECOND);setup=words(SECOND,0x120)
 rate=struct.unpack('<f',struct.pack('<I',inp['rate_bits']))[0];progress=struct.unpack('<f',struct.pack('<I',inp['progress_bits']))[0]
 angle=fixture.bits(-rate*(0.5-progress))
 n.call(0x47eb60,[inp['center'][0],inp['center'][1],angle],this=SECOND);rotated=words(SECOND,0x120)
 n.call(0x482430,[inp['progress_bits']],this=FRAME);final=words(SECOND,0x120)
 return dict(input=inp,rectangle_setup_words=setup,rotation_words=rotated,updated_record_words=final,viewport_helper_inputs=helper_inputs.copy(),coordinate_calls=coordinate_calls.copy())
def main():
 samples=[]
 for width,height,center,rate,keys in [(800,600,(320,240),180.0,(0,0,640,480)),(1280,720,(410,255),90.0,(32,64,576,416)),(1024,768,(512,384),-45.0,(80,48,560,432))]:
  for progress in (0.0,0.125,0.5,0.875,1.0):
   inp=dict(width=width,height=height,center=list(center),rate_bits=fixture.bits(rate),keys=list(keys),progress_bits=fixture.bits(progress))
   samples.append(capture(inp))
 out=dict(provenance=dict(entry='00482430 → 0047EDF0 → 0047EB60',exe_sha256=hashlib.sha256((ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()).hexdigest(),method='Original x86 routines with source-shaped inline keyframe words. Preserves state after rectangle setup, numeric rotation and full 00482430 call. This remains a controlled direct-call fixture, not an extinguisher constructor or live Think capture.'),samples=samples)
 (ROOT/'evidence/extinguisher-frame-record-inline-trace-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} original frame-record stage traces saved; shared RNG draws: {n.draws}')
if __name__=='__main__':main()
