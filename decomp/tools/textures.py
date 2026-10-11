"""Convert extracted PNG pixels to SDL-readable BMP without dependencies."""
import struct,zlib,sys
from pathlib import Path

def convert(path):
 data=path.read_bytes();pos=8;payload=b'';palette=None
 while pos<len(data):
  n=struct.unpack_from('>I',data,pos)[0];kind=data[pos+4:pos+8];chunk=data[pos+8:pos+8+n];pos+=12+n
  if kind==b'IHDR':w,h,depth,ctype,_,_,interlace=struct.unpack('>IIBBBBB',chunk)
  elif kind==b'PLTE':palette=[chunk[i:i+3] for i in range(0,len(chunk),3)]
  elif kind==b'IDAT':payload+=chunk
 if depth!=8 or interlace:raise ValueError((path,depth,interlace))
 bpp={0:1,2:3,3:1,4:2,6:4}[ctype];stride=w*bpp;raw=zlib.decompress(payload);prev=bytearray(stride);rows=[];offset=0
 for y in range(h):
  mode=raw[offset];offset+=1;row=bytearray(raw[offset:offset+stride]);offset+=stride
  for i in range(stride):
   a=row[i-bpp] if i>=bpp else 0;b=prev[i];c=prev[i-bpp] if i>=bpp else 0
   if mode==1:v=a
   elif mode==2:v=b
   elif mode==3:v=(a+b)//2
   elif mode==4:
    p=a+b-c;pa,pb,pc=abs(p-a),abs(p-b),abs(p-c);v=a if pa<=pb and pa<=pc else b if pb<=pc else c
   else:v=0
   row[i]=(row[i]+v)&255
  out=bytearray()
  for x in range(w):
   pixel=row[x*bpp:(x+1)*bpp]
   rgb=palette[pixel[0]] if ctype==3 else bytes([pixel[0]])*3 if ctype in (0,4) else pixel[:3]
   out.extend(rgb[::-1])
  out.extend(b'\0'*((-len(out))%4));rows.append(out);prev=row
 body=b''.join(reversed(rows));header=struct.pack('<2sIHHI',b'BM',54+len(body),0,0,54)+struct.pack('<IiiHHIIiiII',40,w,h,1,24,0,len(body),0,0,0,0)
 path.with_suffix('.bmp').write_bytes(header+body)
if __name__=='__main__':
 paths=[Path(arg) for arg in sys.argv[1:]] if len(sys.argv)>1 else list((Path(__file__).resolve().parents[1]/'assets/training/textures').glob('*.png'))
 for p in paths:convert(p)
