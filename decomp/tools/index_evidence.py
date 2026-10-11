"""Index preserved native exports without modifying or deleting evidence."""
from pathlib import Path
import hashlib,json,re
ROOT=Path(__file__).resolve().parents[1]
functions={}
for path in sorted((ROOT/'evidence').rglob('*.c')):
 text=path.read_text(errors='replace');entry=re.search(r'Entry:\s*(?:0x)?([0-9a-fA-F]{8})',text)
 if not entry:continue
 address=entry.group(1).upper();digest=hashlib.sha256(path.read_bytes()).hexdigest()
 variants=functions.setdefault(address,{})
 variants.setdefault(digest,[]).append(str(path.relative_to(ROOT)))
ledger=json.loads((ROOT/'mechanics.json').read_text());systems={}
for system in ledger['systems']:
 for address in system.get('original_entries',[]):systems.setdefault(address.upper().removeprefix('0X'),[]).append(system['name'])
output=dict(purpose='Preserved decompiler exports for reuse beyond Training. Entries and provisional names do not establish correctness or complete function coverage.',functions=[dict(address=a,systems=systems.get(a,[]),variants=[dict(sha256=h,exports=paths) for h,paths in variants.items()]) for a,variants in sorted(functions.items())])
(ROOT/'evidence/function-index.json').write_text(json.dumps(output,indent=2)+'\n')
print(f'Indexed {len(functions)} addresses across {sum(len(p) for v in functions.values() for p in v.values())} preserved exports; no evidence removed')
