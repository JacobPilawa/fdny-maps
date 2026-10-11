"""Capture Hose Spray's ACTION callback state writes from the original EXE.

The audio service entry points are intercepted so this fixture compares only
the callbacks' active byte and effect deadline. It does not model audio.
"""
import hashlib,json
import fire_effects_oracle as n
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP

OBJ=n.OBJECT
audio_calls=[]

def skip_audio(engine,address,size,data):
 if address not in (0x4a11d0,0x4a1080):return
 sp=engine.reg_read(UC_X86_REG_ESP)
 audio_calls.append(address)
 engine.reg_write(UC_X86_REG_EIP,n.u32(sp))
 engine.reg_write(UC_X86_REG_ESP,sp+4)

n.uc.hook_add(n.UC_HOOK_CODE,skip_audio)

def capture(action,active,deadline,now):
 n.uc.mem_write(OBJ,b'\0'*0x100000);n.heap=OBJ+0x1000
 n.write(OBJ+0xc58,int(active));n.write(OBJ+0x3b0,deadline);n.write(0x502afc,now)
 audio_calls.clear();n.draws=0
 n.call(0x4632d0 if action=='begin' else 0x463300,this=OBJ)
 return {'input':{'action':action,'active':active,'deadline':deadline,'now':now},
         'result':{'active':n.u32(OBJ+0xc58)&0xff,'deadline':n.u32(OBJ+0x3b0),
                   'audio_calls':[f'{value:08x}' for value in audio_calls],
                   'rng_draws':n.draws}}

def generate():
 cases=[('begin',False,0,1000),('begin',True,0xfffffff0,0xfffffff0),
        ('end',True,1100,1150),('end',False,0xdeadbeef,0x80000000),
        ('end',True,0xffffffff,0)]
 samples=[capture(*case) for case in cases]
 out={'provenance':{'entries':['004632D0','00463300'],
       'exe_sha256':hashlib.sha256(n.raw).hexdigest(),
       'method':'Direct original x86 callback execution. Only audio service entries 004A11D0 and 004A1080 are stubbed; callback byte/deadline writes and helper-call conditions are observed. No RNG call occurs in the callbacks.'},
       'samples':samples}
 (n.ROOT/'evidence/hose-spray/action-callback-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} original Hose Spray callback cases recorded; RNG draws: {sum(s["result"]["rng_draws"] for s in samples)}')

if __name__=='__main__':generate()
