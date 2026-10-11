"""Capture the original PropModel ray/sphere interaction query."""
import hashlib, json, math, random, struct, sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools/runtime-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_FPCW

raw=(ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()
uc=Uc(UC_ARCH_X86,UC_MODE_32)
pe=struct.unpack_from('<I',raw,0x3c)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];size=struct.unpack_from('<I',raw,opt+56)[0]
uc.mem_map(base,(size+4095)&~4095);uc.mem_write(base,raw[:struct.unpack_from('<I',raw,opt+60)[0]])
section=opt+struct.unpack_from('<H',raw,pe+20)[0]
for i in range(struct.unpack_from('<H',raw,pe+6)[0]):
    _,rva,length,offset=struct.unpack_from('<IIII',raw,section+i*40+8);uc.mem_write(base+rva,raw[offset:offset+length])
uc.mem_map(0,0x1000);uc.mem_map(0x20000000,0x20000);uc.mem_map(0x30000000,0x100000);uc.reg_write(UC_X86_REG_FPCW,0x037f)
STACK=0x20010000;RETURN=0x20018000;OBJ=0x30000000
uc.mem_write(RETURN,b'\xf4')
u32=lambda a:struct.unpack('<I',uc.mem_read(a,4))[0]
def write(address,value):uc.mem_write(address,struct.pack('<I',value))
def call(address,args=(),this=None):
    uc.reg_write(UC_X86_REG_ESP,STACK)
    uc.mem_write(STACK,struct.pack('<I',RETURN)+b''.join(struct.pack('<I',a) for a in args))
    if this is not None:uc.reg_write(UC_X86_REG_ECX,this)
    uc.emu_start(address,RETURN,count=100000)
    if uc.reg_read(UC_X86_REG_EIP)!=RETURN:raise RuntimeError(f'Original routine did not return from {address:08x}')
START=OBJ+0x100
DIRECTION=OBJ+0x110
NEAR=OBJ+0x120
FAR=OBJ+0x124
POINT=OBJ+0x130
SCORE=OBJ+0x140
bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]

def put_vector(address, vector):
    for axis,value in enumerate(vector): write(address+axis*4,bits(value))

def capture(start,direction,center,radius):
    uc.mem_write(OBJ,b'\0'*0x1000)
    put_vector(START,start)
    put_vector(DIRECTION,direction)
    put_vector(OBJ+0x18,center)
    write(OBJ+0x24,bits(radius))
    write(NEAR,0x7fc00001); write(FAR,0x7fc00002)
    write(POINT,0x7fc00003); write(POINT+4,0x7fc00004); write(POINT+8,0x7fc00005)
    write(SCORE,0x7fc00006)
    args=[START,DIRECTION,OBJ+0x18,bits(radius),NEAR,FAR]
    call(0x4834a0,args)
    sphere_hit=uc.reg_read(UC_X86_REG_EAX)&0xffffffff
    near_bits,far_bits=u32(NEAR),u32(FAR)
    write(POINT,0x7fc00003); write(POINT+4,0x7fc00004); write(POINT+8,0x7fc00005)
    write(SCORE,0x7fc00006)
    call(0x489ce0,[START,DIRECTION,POINT,SCORE],this=OBJ)
    # This virtual callback is consumed as a char in 00459350; high EAX bits
    # are caller residue and are intentionally not part of its result.
    query_hit=uc.reg_read(UC_X86_REG_EAX)&0xff
    return dict(start=start,direction=direction,center=center,radius_bits=bits(radius),
                sphere_hit=sphere_hit,near_bits=near_bits,far_bits=far_bits,
                query_hit=query_hit,point_bits=[u32(POINT+4*i) for i in range(3)],
                score_bits=u32(SCORE))

def generate():
    cases=[]
    radii=[0.0,0.01,1.0,70.0,200.0]
    directions=[(0,0,-1),(0,0,1),(0,1,0),(1,0,0),(.3,.4,-.8660254),(2,0,0),(0,0,0)]
    for radius in radii:
        for direction in directions:
            length=math.sqrt(sum(x*x for x in direction))
            unit=tuple(x/length for x in direction) if length else (0,0,0)
            side=(1.0,0.0,0.0) if abs(unit[0])<.8 else (0.0,1.0,0.0)
            start=(13.25,-7.5,41.0)
            for depth in [-radius,0.0,radius,2*radius+1.0]:
                for lateral in [0.0,radius,radius+0.5]:
                    center=tuple(start[i]+unit[i]*depth+side[i]*lateral for i in range(3))
                    cases.append((start,direction,center,radius))
    rng=random.Random(59350)
    for _ in range(128):
        start=tuple(rng.uniform(-500,500) for _ in range(3))
        direction=tuple(rng.uniform(-1,1) for _ in range(3))
        norm=math.sqrt(sum(x*x for x in direction)) or 1.0
        direction=tuple(x/norm for x in direction)
        center=tuple(rng.uniform(-500,500) for _ in range(3))
        cases.append((start,direction,center,rng.choice(radii)))
    samples=[capture(*case) for case in cases]
    out={'provenance':{'exe_sha256':hashlib.sha256(raw).hexdigest(),
          'entries':['004834A0','00489CE0'],
          'method':'Original x86 sphere test and PropModel model-subobject query; x87 control 0x037f; fixture vectors, center, and radius; untouched output sentinels retained on misses'},
         'samples':samples}
    (ROOT/'evidence/prop-interaction-sphere-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
    print(len(samples),'original PropModel ray/sphere queries')

if __name__=='__main__':
    generate()
