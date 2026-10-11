"""Cross-check Ghidra's RNG references against every executable byte section."""
import hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
raw=(ROOT/'evidence/original/FDNYFirefighter.exe').read_bytes()
path=ROOT/'evidence/rng-audit.json';audit=json.loads(path.read_text())
targets={int(s,16) for s in ['00404930','00404960','00404a70','00404ac0','00404b20','00404b30','00404b50','00498b30','00498b50','00498c00']}
known={(int(r['site'],16),int(r['target'],16)) for r in audit['references']}
pe=struct.unpack_from('<I',raw,0x3c)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];sections=opt+struct.unpack_from('<H',raw,pe+20)[0];found=set()
for i in range(struct.unpack_from('<H',raw,pe+6)[0]):
 virtual,rva,length,offset=struct.unpack_from('<IIII',raw,sections+40*i+8)
 flags=struct.unpack_from('<I',raw,sections+40*i+36)[0]
 if not flags&0x20000000:continue
 code=raw[offset:offset+length]
 for k in range(length-4):
  if code[k] in (0xe8,0xe9):
   address=base+rva+k;target=address+5+struct.unpack_from('<i',code,k+1)[0]
   if target in targets:found.add((address,target))
missing=found-known;unattributed=[r for r in audit['references'] if r['function'] is None]
# Ghidra leaves this direct CALL as func_0x00498b50 rather than recording
# a reference. Both the original bytes and the separately exported routine
# identify it; retain that distinction in the inventory.
raw_references=[]
for a,t in sorted(missing):
 sources=sorted((ROOT/'evidence').glob(f'*/{a:08x}_*.c'))
 source=next((s for s in sources if f'{t:08x}' in s.read_text()),None)
 if source is not None:
  raw_references.append({'site':f'{a:08x}','target':f'{t:08x}','function':f'{a:08x}','source':str(source.relative_to(ROOT)),'method':'Executable relative CALL/JMP bytes and exported function entry'})
resolved={(int(r['site'],16),int(r['target'],16)) for r in raw_references}
unresolved=missing-resolved
audit['exe_sha256']=hashlib.sha256(raw).hexdigest()
audit['raw_references']=raw_references
audit['byte_scan']={'method':'All executable PE sections; direct x86 CALL/JMP relative destinations, cross-checked against disassembled instruction references','matches':len(found),'ghidra_missing_references':[{'site':f'{a:08x}','target':f'{t:08x}'} for a,t in sorted(missing)],'unresolved_references':[{'site':f'{a:08x}','target':f'{t:08x}'} for a,t in sorted(unresolved)],'unattributed_references':len(unattributed),'targets_without_references':[f'{t:08x}' for t in sorted(targets) if not any(x[1]==t for x in known|resolved)]}
path.write_text(json.dumps(audit,indent=2)+'\n')
print(f'{len(found)} executable CALL/JMP sites checked; {len(raw_references)} supplemented from bytes; {len(unresolved)} unresolved references; {len(unattributed)} unattributed references')
if unresolved or unattributed:raise SystemExit(1)
