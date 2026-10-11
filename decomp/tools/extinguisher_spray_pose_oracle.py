"""Capture the active 00461F80 pose-phase branch up to its first frame call.

The tool follower update (0045F600) and animation-record update (00482430)
are return-stubbed; the original 00498B30 RNG range and following x87 phase,
sine and cosine instructions execute. Capture stops before 004977B0. This is
not a full tool Think or fire-contact replay.
"""
import hashlib,json,struct
import fire_effects_oracle as n

OBJ=n.OBJECT;TOOL=OBJ+0x2000;RETURN=n.RETURN
bits=lambda value:struct.unpack('<I',struct.pack('<f',value))[0]
f32=lambda value:struct.unpack('<f',struct.pack('<f',value))[0]

def words(address,count):
 return [n.u32(address+4*i) for i in range(count)]

def capture(seed,dt,phase,offsets):
 n.uc.mem_write(OBJ,b'\0'*0x100000);n.heap=OBJ+0x1000;n.write(0x502e2c,7)
 n.write(0x502b04,bits(dt));n.write(0x502afc,10000)
 n.write(TOOL+0x158,1);n.write(TOOL+0x600,1)
 n.write(TOOL+0x604,bits(phase));n.write(TOOL+0x8c8,bits(1.0))
 for i,value in enumerate(offsets):n.write(TOOL+0x610+4*i,bits(value))
 n.call(0x404b50,[seed]);n.draws=0
 reached=[]
 def hook(engine,address,size,data):
  if address in (0x45f600,0x82430):
   sp=engine.reg_read(n.UC_X86_REG_ESP);ret=n.u32(sp)
   engine.reg_write(n.UC_X86_REG_ESP,sp+(8 if address==0x82430 else 4))
   engine.reg_write(n.UC_X86_REG_EIP,ret)
  elif address==0x4977b0:
   reached.append(address);engine.emu_stop()
 h=n.uc.hook_add(n.UC_HOOK_CODE,hook)
 n.uc.reg_write(n.UC_X86_REG_ESP,n.STACK)
 n.uc.mem_write(n.STACK,struct.pack('<I',RETURN))
 n.uc.reg_write(n.UC_X86_REG_ECX,TOOL)
 try:n.uc.emu_start(0x461f80,RETURN,count=300000)
 except Exception as e:raise RuntimeError(f'{e}; instruction={n.uc.reg_read(n.UC_X86_REG_EIP):08x}') from e
 n.uc.hook_del(h)
 if not reached:raise RuntimeError(f'Pose branch did not reach 004977B0; stopped at {n.uc.reg_read(n.UC_X86_REG_EIP):08x}')
 return dict(input=dict(seed=seed,dt=f32(dt),initial_phase=f32(phase),position_offset=[f32(v) for v in offsets]),
  result=dict(draws=n.draws,phase_bits=n.u32(TOOL+0x604),direction_bits=words(TOOL+0x628,3),position_bits=words(TOOL+0x61c,3)))

def generate():
 seeds=[0,1,4357,2002,0x12345678,0xffffffff]
 dts=[0.0,.001,1/60,.0333333,.1,.5]
 phases=[0.0,.1234567,1.0,3.1415927,9.75]
 samples=[]
 for i,(seed,dt,phase) in enumerate(zip(seeds,dts,phases)):
  samples.append(capture(seed,dt,phase,(-20-0.25*i,-20+0.5*i,30+i)))
 # Additional timer wrap and multi-second updates use independently chosen
 # seeds and phases; values remain controlled inputs.
 for seed,dt,phase in [(91,.016,20.0),(1001,.25,-2.5),(0x89abcdef,1.25,123.75)]:
  i=len(samples);samples.append(capture(seed,dt,phase,(-20-0.25*i,-20+0.5*i,30+i)))
 out=dict(provenance=dict(entry='00461F80 active pose-phase branch through 004977B0 call site',
  exe_sha256=hashlib.sha256(n.raw).hexdigest(),method='Original x86 routine with 0045F600 follower update and 00482430 animation-record update return-stubbed. Original 00498B30/shared RNG and x87 phase, FSIN, FCOS, and vector writes execute. Capture stops before 004977B0. Each captured update consumes only the range draw observed in this prefix.'),samples=samples)
 (n.ROOT/'evidence/extinguisher-spray-pose-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(f'{len(samples)} original pose-phase prefixes recorded; draws={sum(s["result"]["draws"] for s in samples)}')

if __name__=='__main__':generate()
