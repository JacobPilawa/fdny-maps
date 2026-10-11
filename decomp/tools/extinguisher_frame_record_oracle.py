"""Capture original extinguisher bitmap-frame update with a fixture viewport."""
import hashlib
import json
import struct
from pathlib import Path
import fire_effects_oracle as n

ROOT=n.ROOT
OBJECT=n.OBJECT
FRAME=OBJECT+0x2000
KEYS=OBJECT+0x4000
MANAGER=OBJECT+0x8000
bits=lambda value:struct.unpack('<I',struct.pack('<f',value))[0]


def capture(width,height,center,rate,keys,progress):
 n.uc.mem_write(OBJECT,b'\0'*0x100000)
 # Original 0047E5B0 only needs the viewport manager for its 640x480 bitmap
 # validation. Other image/resource work is skipped at this non-reference size.
 n.write(0x4ffad8,width);n.write(0x4ffadc,height)
 n.write(0x4ffae0,0);n.write(0x4ffae4,0)
 n.write(0x503a78,MANAGER);n.write(MANAGER+4,MANAGER+0x100)
 n.write(FRAME,center[0]);n.write(FRAME+4,center[1]);n.write(FRAME+8,KEYS)
 n.write(FRAME+0x18,bits(rate))
 for i,value in enumerate(keys):n.write(KEYS+i*4,value)
 second=FRAME+0x140
 n.write(second+0x104,0);n.write(second+0x108,601)
 n.call(0x482430,[bits(progress)],FRAME)
 words=[n.u32(second+4*i) for i in range(0x120//4)]
 return dict(input=dict(width=width,height=height,center=list(center),rate_bits=bits(rate),keys=list(keys),progress_bits=bits(progress)),result_words=words)


def generate():
 samples=[]
 for width,height,center,rate,keys in [
  (800,600,(320,240),180.0,(0,0,640,480)),
  (1280,720,(410,255),90.0,(32,64,576,416)),
  (1024,768,(512,384),-45.0,(80,48,560,432)),
 ]:
  for progress in [0.0,0.125,0.5,0.875,1.0]:
   samples.append(capture(width,height,center,rate,keys,progress))
 out=dict(provenance=dict(entry='00482430',exe_sha256=hashlib.sha256((ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()).hexdigest(),method='Original x86 00482430 and numeric viewport/frame helpers; controlled 640x480-reference viewport scaling. A non-640x480 viewport avoids resource-manager bitmap lookup in 0047E5B0. This is a frame-record fixture, not a full 00461F80 Think capture.'),samples=samples)
 (ROOT/'evidence/extinguisher-frame-record-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} original frame-record updates recorded; shared RNG draws: {n.draws}')

if __name__=='__main__':generate()
