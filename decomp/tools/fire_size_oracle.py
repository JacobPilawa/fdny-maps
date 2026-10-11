"""Original timed size-key update, including key promotion/pool scanning."""
import json,struct
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
uc=n.uc;OBJ=n.OBJECT;ARRAY=OBJ+0x1000;KEYS=OBJ+0x2000
fbits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def hook(engine,address,size,data):
 if address==0x48d490:
  sp=engine.reg_read(UC_X86_REG_ESP);engine.reg_write(UC_X86_REG_ESP,sp+8);engine.reg_write(UC_X86_REG_EIP,n.u32(sp))
uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
configs=[[(0,[0,0,0]),(1000,[225,125,225]),(3000,[0,0,0])],[(2000,[10,20,30]),(4000,[50,100,75])],[(0,[375,400,375])],[(0,[1,1,1]),(1000,[1.005,1,1])]]
for keys in configs:
 for scale in [[1,1,1],[1,.3,1],[.75,.5,.75]]:
  for now in [0,1,500,999,1000,1001,2000,2999,3000,3001,4001,5000,0xffffffff]:
   uc.mem_write(OBJ,b'\0'*0x4000);n.write(OBJ+0x70,ARRAY);n.write(OBJ+0x74,len(keys));n.write(OBJ+0x88,KEYS);n.write(OBJ+0x8c,KEYS+0x20 if len(keys)>1 else 0)
   for i,(time,value) in enumerate(keys):
    addr=KEYS+i*0x20;n.write(ARRAY+4*i,addr);uc.mem_write(addr,b'\1');n.write(addr+4,time)
    for a,f in enumerate(value):n.write(addr+8+4*a,fbits(f))
   for a,f in enumerate(scale):n.write(OBJ+0x90+4*a,fbits(f))
   n.write(0x502afc,now);n.call(0x44ae80,this=OBJ)
   idx=lambda p:(p-KEYS)//0x20 if p else -1
   samples.append(dict(keys=[dict(time=t,size=v) for t,v in keys],scale=scale,now=now,result_bits=[n.u32(OBJ+0x44+4*a) for a in range(3)],changed=uc.mem_read(OBJ+0x50,1)[0],first=idx(n.u32(OBJ+0x88)),next=idx(n.u32(OBJ+0x8c)),active=[uc.mem_read(KEYS+i*0x20,1)[0] for i in range(len(keys))]))
queue_samples=[]
def snapshot():
 idx=lambda p:(p-KEYS)//0x20 if p else -1
 return dict(keys=[dict(active=uc.mem_read(KEYS+i*0x20,1)[0],time=n.u32(KEYS+i*0x20+4),size_bits=[n.u32(KEYS+i*0x20+8+4*a) for a in range(3)]) for i in range(8)],first=idx(n.u32(OBJ+0x88)),next=idx(n.u32(OBJ+0x8c)),size_bits=[n.u32(OBJ+0x44+4*a) for a in range(3)],changed=uc.mem_read(OBJ+0x50,1)[0])
for offset in [0,10000,0x7ffffff0]:
 uc.mem_write(OBJ,b'\0'*0x4000);n.write(OBJ+0x70,ARRAY);n.write(OBJ+0x74,8)
 for i in range(8):n.write(ARRAY+4*i,KEYS+i*0x20)
 for a in range(3):n.write(OBJ+0x90+4*a,fbits(1))
 actions=[('add',0,0,[0,0,0]),('add',0,1000,[225,125,225]),('add',0,3000,[0,0,0]),('update',500,0,None),('add',500,750,[150,200,150]),('update',751,0,None),('update',1001,0,None),('add',1001,999,[75,50,75]),('update',1500,0,None),('add',1500,4000,[200,300,200]),('update',5000,0,None),('update',5001,0,None),('update',5002,0,None)]
 for kind,now,when,value in actions:
  now=(now+offset)&0xffffffff;when=(when+offset)&0xffffffff;n.write(0x502afc,now);before=snapshot()
  if kind=='add':
   for a,f in enumerate(value):n.write(OBJ+0x3000+4*a,fbits(f))
   n.call(0x44b620,[OBJ+0x3000,when],OBJ)
  else:n.call(0x44ae80,this=OBJ)
  queue_samples.append(dict(kind=kind,now=now,when=when,value=value,before=before,after=snapshot()))
(n.ROOT/'evidence/fire-size-oracle.json').write_text(json.dumps(dict(provenance=dict(entry='0044AE80',exe_sha256=n.hashlib.sha256(n.raw).hexdigest(),method='Original size interpolation and promotion; array bounds-check wrapper skipped for valid fixture arrays'),samples=samples,queue_samples=queue_samples),indent=2)+'\n');print(len(samples),'original fire-size updates;',len(queue_samples),'original queue/update transitions')
