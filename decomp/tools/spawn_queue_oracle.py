"""Execute the original entity batch dispatcher with controlled entity callbacks.

Original queue draining, batch arrays and both virtual-call passes execute.
Fixture entity callbacks record order and can append requests in any phase.
Allocator/free and global entity registration are fixture host services.
"""
import json
import fire_effects_oracle as n
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP

uc=n.uc; QUEUE=n.OBJECT; FACTORY=QUEUE+0x100; FV=QUEUE+0x200; EV=QUEUE+0x300
CREATE=n.RETURN+0x100; FIRST=CREATE+0x10; SECOND=FIRST+0x10
events=[]; allocations={}; additions={}; entity_ids={}

def alloc(size):
 p=n.heap;n.heap+=(size+15)&~15
 if n.heap>=0x30100000:raise RuntimeError('Fixture heap exhausted')
 allocations[p]=size
 return p

def enqueue(identity):
 request=0
 if identity is not None:
  request=alloc(0x14);n.write(request+8,identity);n.write(request+12,FACTORY)
 node=alloc(12);n.write(node,request);n.write(node+4,0)
 tail=n.u32(QUEUE+0x14)
 if tail:n.write(tail+4,node)
 else:n.write(QUEUE+0x10,node)
 n.write(QUEUE+0x14,node);n.write(QUEUE+0x18,n.u32(QUEUE+0x18)+1)

def ret(engine,value,pop):
 sp=engine.reg_read(UC_X86_REG_ESP);target=n.u32(sp)
 engine.reg_write(UC_X86_REG_EAX,value);engine.reg_write(UC_X86_REG_ESP,sp+4+pop)
 engine.reg_write(UC_X86_REG_EIP,target)

def hook(engine,address,size,data):
 sp=engine.reg_read(UC_X86_REG_ESP)
 if address==0x4b7b63:ret(engine,0,0) # fixture free
 elif address==0x4b7bfd:
  old,length=n.u32(sp+4),n.u32(sp+8);p=alloc(length) if length else 0
  if old and length:engine.mem_write(p,bytes(engine.mem_read(old,min(length,allocations[old]))))
  ret(engine,p,0)
 elif address==0x429490:
  entity=n.u32(sp+4);events.append(['register',entity_ids[entity]])
  ret(engine,0,4)
 elif address in [CREATE,FIRST,SECOND]:
  if address==CREATE:
   identity=n.u32(n.u32(sp+4)+8);entity=alloc(0x40)
   n.write(entity,EV);entity_ids[entity]=identity;phase='construct';pop=4;value=entity
  else:
   entity=engine.reg_read(UC_X86_REG_ECX);identity=entity_ids[entity]
   phase='first' if address==FIRST else 'second';pop=0;value=0
  events.append([phase,identity])
  for child in additions.get((phase,identity),[]):enqueue(child)
  ret(engine,value,pop)

uc.hook_add(UC_HOOK_CODE,hook)
samples=[]
plans=[{'initial':[1,2,3], 'additions':[['construct',1,4],['first',2,5],['second',3,6]]},
       {'initial':[1,None,2], 'additions':[['first',1,3]]},
       {'initial':[], 'additions':[]},
       {'initial':[1], 'additions':[['construct',1,2],['construct',2,3],['first',3,4],['second',4,5]]}]
for plan in plans:
 uc.mem_write(QUEUE,b'\0'*0x100000);n.heap=QUEUE+0x1000
 allocations.clear();entity_ids.clear();events=[];additions={}
 n.write(FACTORY,FV);n.write(FV+8,CREATE);n.write(EV+0x20,FIRST);n.write(EV+0x24,SECOND)
 for phase,identity,child in plan['additions']:additions.setdefault((phase,identity),[]).append(child)
 for identity in plan['initial']:enqueue(identity)
 batches=[]
 for iteration in range(4):
  before=len(events)
  n.call(0x49c670,[0,0],QUEUE+0x40)
  batches.append({'events':events[before:], 'pending':n.u32(QUEUE+0x18)})
 samples.append({**plan,'batches':batches})
(n.ROOT/'evidence/spawn-queue-oracle.json').write_text(json.dumps({
 'provenance':{'exe_sha256':n.hashlib.sha256(n.raw).hexdigest(), 'entry':'0049C670',
 'method':'Original batch dispatcher and batch container; fixture queue entries, entity callbacks, allocator/free and registration'},
 'samples':samples},indent=2)+'\n')
print(f'{len(samples)} original spawn-queue scenarios recorded')
