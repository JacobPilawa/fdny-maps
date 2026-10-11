"""Capture 00482430 with inline keyframe state at frame-record offsets +8..+14."""
import hashlib
import json
from pathlib import Path
import fire_effects_oracle as n

ROOT=n.ROOT
OBJECT=0x30000000
FRAME=OBJECT+0x4000
SECOND=FRAME+0x140
MANAGER=OBJECT+0x8000
bits=lambda value:__import__('struct').unpack('<I',__import__('struct').pack('<f',value))[0]

def capture(width,height,center,rate,keys,progress):
 n.uc.mem_write(OBJECT,b'\0'*0x100000)
 n.write(0x4ffad8,width);n.write(0x4ffadc,height);n.write(0x4ffae0,0);n.write(0x4ffae4,0)
 n.write(0x503a78,MANAGER);n.write(MANAGER+4,MANAGER+0x100)
 n.write(FRAME,center[0]);n.write(FRAME+4,center[1]);n.write(FRAME+0x18,bits(rate))
 for i,value in enumerate(keys):n.write(FRAME+8+i*4,value)
 n.write(SECOND+0x104,0);n.write(SECOND+0x108,601)
 n.call(0x482430,[bits(progress)],this=FRAME)
 return dict(input=dict(width=width,height=height,center=list(center),rate_bits=bits(rate),keys=list(keys),progress_bits=bits(progress)),result_words=[n.u32(SECOND+4*i) for i in range(0x120//4)])

def generate():
 samples=[]
 for width,height,center,rate,keys in [
  (800,600,(320,240),180.0,(0,0,640,480)),
  (1280,720,(410,255),90.0,(32,64,576,416)),
  (1024,768,(512,384),-45.0,(80,48,560,432))]:
  for progress in [0.0,0.125,0.5,0.875,1.0]:samples.append(capture(width,height,center,rate,keys,progress))
 out=dict(provenance=dict(entry='00482430',exe_sha256=hashlib.sha256((ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()).hexdigest(),method='Original x86 00482430 with four inline keyframe integers at receiver offsets +8..+14; controlled non-reference viewport avoids resource bitmap lookup. This direct helper fixture does not execute 004823D0/004824B0 constructor helpers or full extinguisher construction.'),samples=samples)
 (ROOT/'evidence/extinguisher-frame-record-inline-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} inline-state original updates recorded; shared RNG draws: {n.draws}')

if __name__=='__main__':generate()
