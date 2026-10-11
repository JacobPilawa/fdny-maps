"""Read numeric constants at virtual addresses in the preserved executable."""
import struct,sys
from pathlib import Path
binary=(Path(__file__).resolve().parents[1]/'evidence/original/FDNYFirefighter.exe').read_bytes()
pe=struct.unpack_from('<I',binary,0x3c)[0];sections=struct.unpack_from('<H',binary,pe+6)[0];optional_size=struct.unpack_from('<H',binary,pe+20)[0];image_base=struct.unpack_from('<I',binary,pe+24+28)[0]
def offset(address):
 rva=address-image_base
 for i in range(sections):
  start=pe+24+optional_size+40*i;size,va,raw_size,raw=struct.unpack_from('<IIII',binary,start+8)
  if va<=rva<va+max(size,raw_size):return raw+rva-va
 raise ValueError(hex(address))
if __name__=='__main__':
 for value in sys.argv[1:]:
  address=int(value,16);o=offset(address);print(value,'float=',struct.unpack_from('<f',binary,o)[0],'double=',struct.unpack_from('<d',binary,o)[0],'hex=',binary[o:o+8].hex())
