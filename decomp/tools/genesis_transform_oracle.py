"""Execute shipped Genesis transform exports with original actor asset inputs."""
import json,struct,hashlib,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools/runtime-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EIP,UC_X86_REG_FPCW
raw=(ROOT/'evidence/original/Genesis.dll').read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32)
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0]
size=struct.unpack_from('<I',raw,opt+56)[0];uc.mem_map(base,(size+4095)&~4095)
uc.mem_write(base,raw[:struct.unpack_from('<I',raw,opt+60)[0]])
sections=opt+struct.unpack_from('<H',raw,pe+20)[0]
for i in range(struct.unpack_from('<H',raw,pe+6)[0]):
 _,rva,size,offset=struct.unpack_from('<4I',raw,sections+i*40+8);uc.mem_write(base+rva,raw[offset:offset+size])
MEM=0x20000000;STACK=MEM+0xf000;RETURN=MEM+0xff00;uc.mem_map(MEM,0x10000);uc.reg_write(UC_X86_REG_FPCW,0x037f)
u32=lambda address:struct.unpack('<I',uc.mem_read(address,4))[0]
directory=base+struct.unpack_from('<I',raw,opt+96)[0]
fields=struct.unpack('<IIHH7I',uc.mem_read(directory,40));exports={}
for i in range(fields[7]):
 name_address=base+u32(base+fields[9]+4*i);name=bytes(uc.mem_read(name_address,128)).split(b'\0')[0].decode()
 ordinal=struct.unpack('<H',uc.mem_read(base+fields[10]+2*i,2))[0];exports[name]=base+u32(base+fields[8]+4*ordinal)
def call(name,args):
 uc.reg_write(UC_X86_REG_ESP,STACK);uc.reg_write(UC_X86_REG_ECX,args[0]);uc.reg_write(UC_X86_REG_EDX,args[1])
 uc.mem_write(STACK,struct.pack('<I',RETURN)+b''.join(struct.pack('<I',a) for a in args[2:]))
 uc.emu_start(exports[name],RETURN,count=100000)
 if uc.reg_read(UC_X86_REG_EIP)!=RETURN:raise RuntimeError('Original transform routine did not return')
def put(address,values):uc.mem_write(address,struct.pack('<'+'f'*len(values),*values))
def floats(address,n):return list(struct.unpack('<'+'f'*n,uc.mem_read(address,n*4)))
def bits(address,n):return list(struct.unpack('<'+'I'*n,uc.mem_read(address,n*4)))
asset=json.loads((ROOT/'assets/training/actors/vic_man.json').read_text());samples=[]
for bone in asset['bones']:
 a=bone['attachment'];parent=bone['parent'];b=asset['bones'][parent]['attachment'] if parent>=0 else [1,0,0,0,1,0,0,0,1,0,0,0]
 put(MEM,b);put(MEM+0x100,a);call('@geXForm3d_Multiply@12',[MEM,MEM+0x100,MEM+0x200])
 samples.append({'kind':'multiply','a':b,'b':a,'result_bits':bits(MEM+0x200,12)})
for vertex in asset['vertices']:
 matrix=asset['bones'][vertex['bone']]['attachment'];put(MEM,matrix);put(MEM+0x100,vertex['position']);call('@geXForm3d_Transform@12',[MEM,MEM+0x100,MEM+0x200])
 samples.append({'kind':'point','matrix':matrix,'point':vertex['position'],'result_bits':bits(MEM+0x200,3)})
for motion in asset['motions']:
 for path in motion['paths']:
  track=path.get('rotation')
  if not track:continue
  for quaternion in [track['values'][0],track['values'][-1]]:
   put(MEM,quaternion);uc.mem_write(MEM+0x200,bytes(48));call('@geQuaternion_ToMatrix@8',[MEM,MEM+0x200])
   samples.append({'kind':'quaternion','quaternion':quaternion,'result_bits':bits(MEM+0x200,9)})
(ROOT/'evidence/genesis-transform-oracle.json').write_text(json.dumps({
 'provenance':{'genesis_sha256':hashlib.sha256(raw).hexdigest(),'exports':{k:hex(v) for k,v in exports.items() if k in ['@geXForm3d_Multiply@12','@geXForm3d_Transform@12','@geQuaternion_ToMatrix@8']},
 'method':'Unmodified shipped Genesis transform instructions, x87 control 0x037f; original victim bone, vertex and quaternion inputs; no external callbacks replaced'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original Genesis transform calls')
