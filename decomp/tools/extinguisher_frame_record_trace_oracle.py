"""Preserve intermediate original frame-record state for the 00482430 fixture."""
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import extinguisher_frame_record_oracle as fixture
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP

rotate_calls=[]
def record_rotate_args(engine,address,size,data):
    if address==0x47eb60:
        sp=engine.reg_read(UC_X86_REG_ESP)
        obj=engine.reg_read(UC_X86_REG_ECX)
        rotate_calls.append(dict(object=obj,args=[n.u32(sp+4),n.u32(sp+8),n.u32(sp+12)],corners=[n.u32(obj+off) for off in (0x10,0x14,0x34,0x38,0x58,0x5c,0x7c,0x80)]))
n.uc.hook_add(UC_HOOK_CODE,record_rotate_args)

def capture(width,height,center,rate,keys,progress):
    rotate_calls.clear()
    n.uc.mem_write(fixture.OBJECT,b'\0'*0x100000)
    n.write(0x4ffad8,width);n.write(0x4ffadc,height)
    n.write(0x4ffae0,0);n.write(0x4ffae4,0)
    n.write(0x503a78,fixture.MANAGER);n.write(fixture.MANAGER+4,fixture.MANAGER+0x100)
    n.write(fixture.FRAME,center[0]);n.write(fixture.FRAME+4,center[1]);n.write(fixture.FRAME+8,fixture.KEYS)
    n.write(fixture.FRAME+0x18,fixture.bits(rate))
    for i,value in enumerate(keys):n.write(fixture.KEYS+i*4,value)
    second=fixture.FRAME+0x140
    n.write(second+0x104,0);n.write(second+0x108,601)
    def words():return [n.u32(second+4*i) for i in range(0x120//4)]
    n.call(0x47edf0,[fixture.KEYS,0],this=second)
    after_init=words()
    angle=-rate*(0.5-progress)
    n.call(0x47eb60,[center[0],center[1],fixture.bits(angle)],this=second)
    after_rotate=words()
    n.call(0x482430,[fixture.bits(progress)],fixture.FRAME)
    return dict(input=dict(width=width,height=height,center=list(center),rate_bits=fixture.bits(rate),keys=list(keys),progress_bits=fixture.bits(progress)),after_rectangle_setup=after_init,after_rotation=after_rotate,after_update=words(),rotation_call_words=rotate_calls.copy(),rate_after_updates=n.u32(fixture.FRAME+0x18))

def main():
    samples=[capture(800,600,(320,240),180.0,(0,0,640,480),0.0),
             capture(1280,720,(410,255),90.0,(32,64,576,416),0.5),
             capture(1024,768,(512,384),-45.0,(80,48,560,432),1.0)]
    out=dict(provenance=dict(entry='00482430',exe_sha256=hashlib.sha256((ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()).hexdigest(),method='Original x86 calls to 0047EDF0, 0047EB60 and 00482430 with the same controlled viewport/record fixture as extinguisher_frame_record_oracle.py; captured intermediate state; not constructor state.'),samples=samples)
    (ROOT/'evidence/extinguisher-frame-record-trace-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
    print(f'{len(samples)} intermediate original frame-record captures saved; shared RNG draws: {n.draws}')

if __name__=='__main__':main()
