"""Execute the original RNG and victim-index conversion instructions."""
import sys,struct,json,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools/runtime-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_FPCW,UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EIP
raw=(ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()
uc=Uc(UC_ARCH_X86,UC_MODE_32)
pe=struct.unpack_from('<I',raw,0x3c)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];size=struct.unpack_from('<I',raw,opt+56)[0]
uc.mem_map(base,(size+4095)&~4095)
uc.mem_write(base,raw[:struct.unpack_from('<I',raw,opt+60)[0]])
section=opt+struct.unpack_from('<H',raw,pe+20)[0]
for i in range(struct.unpack_from('<H',raw,pe+6)[0]):
 _,rva,length,offset=struct.unpack_from('<IIII',raw,section+i*40+8);uc.mem_write(base+rva,raw[offset:offset+length])
STACK=0x20010000;STUB=0x70000000;OUT=STUB+0x200;RETURN=STUB+0x100
uc.mem_map(0x20000000,0x20000);uc.mem_map(STUB,0x1000);uc.reg_write(UC_X86_REG_FPCW,0x037f)
def call(addr,args=()):
 uc.reg_write(UC_X86_REG_ESP,STACK);uc.mem_write(STACK,struct.pack('<I',RETURN)+b''.join(struct.pack('<I',a) for a in args));uc.emu_start(addr,RETURN,count=100000)
def wrapper(addr,convert=False):
 code=b'\xe8'+struct.pack('<i',addr-(STUB+5))
 if convert:code+=b'\xe8'+struct.pack('<i',0x4b9400-(STUB+len(code)+5))+b'\xa3'+struct.pack('<I',OUT)
 else:code+=b'\xd9\x1d'+struct.pack('<I',OUT)
 code+=b'\xc3';uc.mem_write(STUB,code);uc.ctl_remove_cache(STUB,STUB+0x100)
sequences=[]
for seed in [0,1,2,4357,2002,0xffffffff,0x12345678,0xf0000002]:
 call(0x404b50,[seed]);wrapper(0x404ac0);bits=[]
 for i in range(2500):call(STUB);bits.append(struct.unpack('<I',uc.mem_read(OUT,4))[0])
 # Range+integer helper as used by victim startup. STUB adds arguments before CALL.
 call(0x404b50,[seed]);indices=[]
 for count in [1,2,3,4,8]:
  upper=struct.unpack('<I',struct.pack('<f',count-.01))[0]
  code=b'\x68'+struct.pack('<I',upper)+b'\x6a\x00'
  code+=b'\xe8'+struct.pack('<i',0x498b30-(STUB+len(code)+5))+b'\x83\xc4\x08'
  code+=b'\xe8'+struct.pack('<i',0x4b9400-(STUB+len(code)+5))+b'\xa3'+struct.pack('<I',OUT)+b'\xc3';uc.mem_write(STUB,code);uc.ctl_remove_cache(STUB,STUB+0x100)
  for i in range(100):call(STUB);indices.append({'count':count,'index':struct.unpack('<I',uc.mem_read(OUT,4))[0]})
 call(0x404b50,[seed]);mixed=[]
 for i in range(2000):
  kind=i%4
  if kind==0:
   call(0x404a70);mixed.append({'kind':'word','value':uc.reg_read(UC_X86_REG_EAX)})
  elif kind==1:
   bound=[0,1,2,3,6,15,120000,0x7fffffff,0x80000000,0xffffffff][(i//4)%10]
   call(0x404b30,[bound]);mixed.append({'kind':'bounded','bound':bound,'value':uc.reg_read(UC_X86_REG_EAX)})
  elif kind==2:
   wrapper(0x404ac0);call(STUB);mixed.append({'kind':'unit','bits':struct.unpack('<I',uc.mem_read(OUT,4))[0]})
  else:
   lo,hi=[(0.,1.),(.5,1.5),(-80.,-10.),(-40.,40.),(.8,1.2),(750.,3000.),(1000.,6000.),(6.,18.),(0.,6.2831855)][(i//4)%9]
   lo32=struct.unpack('<I',struct.pack('<f',lo))[0];hi32=struct.unpack('<I',struct.pack('<f',hi))[0]
   code=b'\x68'+struct.pack('<I',hi32)+b'\x68'+struct.pack('<I',lo32)
   code+=b'\xe8'+struct.pack('<i',0x498b30-(STUB+len(code)+5))+b'\x83\xc4\x08'
   code+=b'\xd9\x1d'+struct.pack('<I',OUT)+b'\xc3';uc.mem_write(STUB,code);uc.ctl_remove_cache(STUB,STUB+0x100)
   call(STUB);mixed.append({'kind':'range','lo':lo32,'hi':hi32,'bits':struct.unpack('<I',uc.mem_read(OUT,4))[0]})
 sequences.append({'seed':seed,'float_bits':bits,'victim_indices':indices,'mixed':mixed})
# Run the complete original sound-choice routine. Only actual playback is
# replaced; RNG, repeat avoidance, bounds checks and index storage execute
# from the executable. Record generator entries to check conditional draws.
OBJECT=STUB+0x400;ARRAY=STUB+0x500
sound_samples=[];consumed=0;played=0
def hook_sound(engine,address,size,data):
 global consumed,played
 if address in (0x404ac0,0x404a70):consumed+=1
 elif address==0x4a11a0:
  played+=1;sp=engine.reg_read(UC_X86_REG_ESP);ret=struct.unpack('<I',engine.mem_read(sp,4))[0]
  engine.reg_write(UC_X86_REG_ESP,sp+8);engine.reg_write(UC_X86_REG_EIP,ret)
hook=uc.hook_add(UC_HOOK_CODE,hook_sound)
uc.ctl_remove_cache(0x404a70,0x404b20)
for seed in [0,1,4357,2002,0xffffffff,0x12345678]:
 for count in [0,1,2,3,7]:
  call(0x404b50,[seed]);uc.mem_write(OBJECT,struct.pack('<IIIIIII',ARRAY,count,count,0,0,0,0xffffffff))
  uc.mem_write(ARRAY,b''.join(struct.pack('<I',STUB+0x600+32*i) for i in range(count)))
  for i in range(100):
   before=struct.unpack('<i',uc.mem_read(OBJECT+24,4))[0];consumed=played=0
   uc.reg_write(UC_X86_REG_ECX,OBJECT);call(0x488d40)
   after=struct.unpack('<i',uc.mem_read(OBJECT+24,4))[0]
   sound_samples.append({'seed':seed,'count':count,'step':i,'previous':before,'index':after,'draws':consumed,'plays':played})
uc.hook_del(hook)
path=ROOT/'evidence/rng-oracle.json';path.write_text(json.dumps({'provenance':{'exe_sha256':hashlib.sha256(raw).hexdigest(),'float_entry':'00404AC0','seed_entry':'00404B50','range_entry':'00498B30','integer_entry':'004B9400','sound_choice_entry':'00488D40','method':'Unmodified original x86 instructions; x87 control 0x037f; sound playback callback stubbed'},'sequences':sequences,'sound_choices':sound_samples}))
print('Recorded 20,000 original RNG draws, 4,000 victim-index conversions, 16,000 mixed operations and 3,000 complete sound choices')
