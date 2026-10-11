"""Run original animation selection and pose update with Genesis API fixtures."""
import json,struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP
uc=n.uc;OBJ=n.OBJECT;GET=n.RETURN+0x100;EXTENT=GET+16;POSE=EXTENT+16
extent=(0.,1.);pose_bits=None
def ret(engine,value,pop):
 sp=engine.reg_read(UC_X86_REG_ESP);target=n.u32(sp)
 engine.reg_write(UC_X86_REG_EAX,value);engine.reg_write(UC_X86_REG_ESP,sp+4+pop);engine.reg_write(UC_X86_REG_EIP,target)
def hook(engine,address,size,data):
 global pose_bits
 sp=engine.reg_read(UC_X86_REG_ESP)
 if address==GET:ret(engine,OBJ+0x500,0)
 elif address==EXTENT:
  engine.mem_write(engine.reg_read(UC_X86_REG_EDX),struct.pack('<f',extent[0]));engine.mem_write(n.u32(sp+4),struct.pack('<f',extent[1]));ret(engine,1,4)
 elif address==POSE:pose_bits=n.u32(sp+4);ret(engine,1,8)
uc.hook_add(UC_HOOK_CODE,hook)
n.write(0x4dc118,GET);n.write(0x4dc09c,EXTENT)
# Locate the actual SetPose import rather than assuming its IAT address.
pe=struct.unpack_from('<I',n.raw,0x3c)[0];descriptor=n.base+struct.unpack_from('<I',n.raw,pe+24+104)[0]
while any(uc.mem_read(descriptor,20)):
 lookup,_,_,_,iat=struct.unpack('<5I',uc.mem_read(descriptor,20));i=0
 while (value:=n.u32(n.base+(lookup or iat)+4*i)):
  if not value&0x80000000:
   label=bytes(uc.mem_read(n.base+value+2,128)).split(b'\0')[0]
   if b'geActor_SetPose' in label:n.write(n.base+iat+4*i,POSE)
  i+=1
 descriptor+=20
motions=json.loads((n.ROOT/'assets/training/actors/vic_man.json').read_text())['motions'];samples=[]
fbits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
for motion in motions:
 extent=(motion['start'],motion['end'])
 for speed in [1.,.2,2.]:
  for started in [0,5000,0xffffff00]:
   uc.mem_write(OBJ,b'\0'*0x1000);n.write(OBJ+0x10,6);n.write(OBJ+0x18,0xffffffff);n.write(0x502afc,started)
   n.call(0x480ea0,[0,fbits(speed)],OBJ)
   record={'begin_bits':fbits(extent[0]),'end_bits':fbits(extent[1]),'speed_bits':fbits(speed),
           'started':started,'duration_bits':n.u32(OBJ+0x20),'period':n.u32(OBJ+0x2c),'poses':[]}
   for elapsed in [0,1,17,999,1000,5000,60001,0xffffffff]:
    now=(started+elapsed)&0xffffffff;n.write(0x502afc,now);pose_bits=None;n.call(0x480d50,(),OBJ)
    if pose_bits is None:raise RuntimeError('Original SetPose callback missing')
    record['poses'].append({'now':now,'bits':pose_bits})
   samples.append(record)
(n.ROOT/'evidence/actor-animation-oracle.json').write_text(json.dumps({
 'provenance':{'exe_sha256':n.hashlib.sha256(n.raw).hexdigest(),'entries':['00480EA0','00480D50'],
 'method':'Original animation timing instructions; Genesis motion lookup/extents and final SetPose bound to fixtures; actual victim motion extents'},'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original animation selections; {sum(len(s["poses"]) for s in samples)} pose updates')
