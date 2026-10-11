"""Run original sound queuing, resume, sample playback and phase selection.

Original queue/container and playback instructions execute unchanged. The
fixture supplies a resource lookup, heap allocator and FMOD API bindings.
"""
import struct,json,re
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ECX
uc=n.uc;SYSTEM=n.OBJECT;MAIN=SYSTEM+0x100;RESOURCE=SYSTEM+0x200;SOUNDS=SYSTEM+0x400
backend={};allocations={};events=[];channels=0;sample_length=48000
# Bind the original FMOD IAT, as the Windows loader would bind a DLL.
pe=struct.unpack_from('<I',n.raw,0x3c)[0];opt=pe+24
rva=struct.unpack_from('<I',n.raw,opt+104)[0];descriptor=n.base+rva
while any(uc.mem_read(descriptor,20)):
 lookup,_,_,name,iat=struct.unpack('<5I',uc.mem_read(descriptor,20))
 dll=bytes(uc.mem_read(n.base+name,128)).split(b'\0')[0].lower()
 if b'fmod' in dll:
  i=0
  while (value:=n.u32(n.base+(lookup or iat)+i*4)):
   label=bytes(uc.mem_read(n.base+value+2,128)).split(b'\0')[0].decode();stub=n.RETURN+0x100+len(backend)*16
   backend[stub]=(label,int(label.split('@')[-1]));n.write(n.base+iat+i*4,stub);i+=1
 descriptor+=20
def return_from(engine,value,pop):
 sp=engine.reg_read(UC_X86_REG_ESP);ret=n.u32(sp)
 engine.reg_write(UC_X86_REG_EAX,value);engine.reg_write(UC_X86_REG_ESP,sp+4+pop);engine.reg_write(UC_X86_REG_EIP,ret)
def hook(engine,address,size,data):
 global channels
 sp=engine.reg_read(UC_X86_REG_ESP)
 if address==0x4b7bfd: # C runtime realloc; preserve existing container bytes.
  old,size=n.u32(sp+4),n.u32(sp+8);p=0
  if size:
   p=n.heap;n.heap+=(size+15)&~15
   if old:engine.mem_write(p,bytes(engine.mem_read(old,min(size,allocations[old]))))
   allocations[p]=size
  return_from(engine,p,0)
 elif address==0x49ffb0:return_from(engine,RESOURCE,0)
 elif address==0x49f850:return_from(engine,RESOURCE,4)
 elif address in backend:
  label,pop=backend[address];args=[n.u32(sp+4+i*4) for i in range(pop//4)];value=1
  if 'PlaySoundEx' in label:channels+=1;value=channels;events.append({'kind':'play','channel':channels})
  elif 'GetCurrentSample' in label:value=1
  elif 'Sample_GetLength' in label:value=sample_length
  elif 'SetCurrentPosition' in label:events.append({'kind':'phase','channel':args[0],'position':args[1]})
  elif 'IsPlaying' in label:value=0
  return_from(engine,value,pop)
uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
requests=[(0,1),(1,0),(2,1),(0,1),(3,0),(1,1),(4,1)]
for seed in [0,1,4357,2002,0xffffffff,0x12345678]:
 for sample_length in [1,7,48000,1234567]:
  n.heap=SYSTEM+0x1000;uc.mem_write(SYSTEM,b'\0'*0x100000);channels=0;events=[];allocations.clear()
  n.write(0x503a78,MAIN);n.write(MAIN+8,SYSTEM);n.write(RESOURCE+4,1)
  n.write(SYSTEM+0x3c+0x10,0x43f660);n.write(SYSTEM+0x3c+0x14,16)
  n.call(0x404b50,[seed]);n.call(0x4a0b60,(),SYSTEM);n.draws=0
  for identity,loop in requests:
   n.call(0x4a13a0,(),SOUNDS+identity*0x40)
   n.call(0x4a0be0,[SOUNDS+identity*0x40,loop],SYSTEM)
  queued=n.u32(SYSTEM+0x40);during_loading=n.draws
  n.call(0x4a0d60,(),SYSTEM)
  samples.append({'seed':seed,'sample_length':sample_length,'requests':[{'id':i,'loop':l} for i,l in requests],'queued':queued,'loading_draws':during_loading,'resume_draws':n.draws-during_loading,'after_queue_size':n.u32(SYSTEM+0x40),'events':events[:]})
(n.ROOT/'evidence/sound-queue-oracle.json').write_text(json.dumps({'provenance':{'exe_sha256':n.hashlib.sha256(n.raw).hexdigest(),'entries':['004A0B60','004A0BE0','004A0D60','0049F2D0'],'method':'Original queue/container/playback/phase instructions; fixture heap, resource lookup and FMOD DLL bindings; repeated requests retained'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original sound queue replays recorded')
