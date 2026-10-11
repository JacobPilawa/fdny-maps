"""Capture 00491140 head-motion outputs from the original executable."""
import hashlib, json, struct
import fire_effects_oracle as n

OBJ=n.OBJECT
HEAD=OBJ+0x2000
OUTPUT=OBJ+0x3000
HISTORY=OBJ+0x4000
RECORDS=OBJ+0x5000
SLOT=OBJ+0x6000
bits=lambda value:struct.unpack('<I',struct.pack('<f',value))[0]

# Constructor 00491460 initializes five 0x3c-byte records. Unwritten words
# remain zero from its vector-constructor pass. Values here follow its exact
# field stores; phase (8,15,4) comes from its 0042D280 vector assignment.
def mode_row(values):return [int(v) if i in (12,13) else bits(v) for i,v in enumerate(values)]
MODE_WORDS=[
 mode_row((0,2,0,0,2,0,1,1,1,1,1,1,0,1,4)),
 mode_row((8,10,0,8,16,0,1,.5,1,1,1,1,0,1,16)),
 mode_row((0,0,0,0,0,0,0,0,0,0,0,0,0,1,10)),
 mode_row((10,10,0,12,16,0,1,.5,1,1,1,1,0,1,16)),
 mode_row((4,7,15,8,15,4,1,.5,1,1,1,1,0,1,6)),
]

def reset():
 n.uc.mem_write(OBJ,b'\0'*0x100000);n.heap=OBJ+0x1000
 n.write(HEAD+4,HISTORY);n.write(HEAD+8,1);n.write(HEAD+0x10,0x486640);n.write(HEAD+0x14,0x10)
 n.write(HEAD+0x1c,bits(.1));n.write(HEAD+0x20,bits(.001));n.write(HEAD+0x24,0xffffffff);n.write(HEAD+0x28,0)
 n.write(HEAD+0x3c,bits(8));n.write(HEAD+0x40,bits(15));n.write(HEAD+0x44,bits(4))
 n.write(HEAD+0x54,bits(.1));n.write(HEAD+0x64,0)
 for mode,row in enumerate(MODE_WORDS):
  for i,value in enumerate(row):n.write(HEAD+0x68+0x3c*mode+4*i,value)
 # The 0.1/0.001 history ring uses 100 entries in the original constructor.
 # One backing record suffices for this output-only capture; history is never
 # used to calculate phase or returned offsets.
 n.write(HISTORY,SLOT)
 n.write(SLOT,0);n.write(SLOT+4,0);n.write(SLOT+8,0);n.write(SLOT+12,0)

def call(mode,dt,clock,shake=None):
 n.write(0x502b04,bits(dt));n.write(0x502afc,clock)
 if shake is not None:n.call(0x490e60,[bits(shake)],HEAD)
 n.call(0x491140,[OUTPUT,mode],HEAD)
 return dict(output_bits=[n.u32(OUTPUT),n.u32(OUTPUT+4),n.u32(OUTPUT+8)],
  phase_bits=[n.u32(HEAD+0x3c),n.u32(HEAD+0x40),n.u32(HEAD+0x44)],
  mode=n.u32(HEAD+0x64),shake_active=n.uc.mem_read(HEAD+0x48,1)[0],
  shake_phase_bits=n.u32(HEAD+0x50),trend=n.uc.mem_read(HEAD,1)[0],
  previous_y_bits=[n.u32(HEAD+0x58),n.u32(HEAD+0x5c),n.u32(HEAD+0x60)])

def generate():
 sequences=[];draw_count_before=n.draws
 dt_values=[.016,.033,.001,.1,.25,0.0]
 for requested in range(5):
  reset();frames=[];clock=1000
  for frame in range(48):
   dt=dt_values[frame%len(dt_values)];clock=(clock+round(dt*1000))&0xffffffff
   frames.append(dict(mode=requested,dt_bits=bits(dt),shake_bits=None,result=call(requested,dt,clock)))
  sequences.append(dict(name=f'constructor-to-mode-{requested}',frames=frames))
 reset();frames=[];clock=5000
 for frame in range(60):
  mode=[4,1,3,2,0][(frame//12)%5];dt=dt_values[frame%len(dt_values)];clock=(clock+round(dt*1000))&0xffffffff
  shake=20 if frame==0 else None
  frames.append(dict(mode=mode,dt_bits=bits(dt),shake_bits=None if shake is None else bits(shake),result=call(mode,dt,clock,shake)))
 sequences.append(dict(name='mode-switches-and-shake',frames=frames))
 out=dict(provenance=dict(entry='00491140',constructor='00491460',constructor_caller='00453D50',shake_start='00490E60',exe_sha256=hashlib.sha256(n.raw).hexdigest(),method='Original x86 head-motion update and shake start executed. The five mode records and initial phases are initialized from the exact 00491460 constructor stores (including 0042D280 input (8,15,4)); a one-slot history buffer is a fixture because 0048D6B0 only records outputs and does not feed phase/output calculation. No RNG calls occur in this component.'),sequences=sequences,draws=n.draws-draw_count_before)
 (n.ROOT/'evidence/head-motion-oracle.json').write_text(json.dumps(out,indent=2)+'\n')
 print(sum(len(x['frames']) for x in sequences),'original 00491140 updates recorded; RNG draws:',out['draws'])

if __name__=='__main__':generate()
