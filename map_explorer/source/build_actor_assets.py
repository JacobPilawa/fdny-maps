"""Rebuild victim geometry/textures from the self-contained original ACT copies."""
from pathlib import Path
import json,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'source/tools'))
import extract_act as act
out=ROOT/'data/actors';out.mkdir(exist_ok=True)
for f in sorted((ROOT/'source/actors').glob('*.act')):
 stem=f.stem.lower();outer=act.read_vfs(f.read_bytes());vfs=act.read_vfs(outer.read('Body'));body=act.read_body(vfs.read('Geometry'))
 body['motions']=[act.read_motion(outer.data[e.offset:e.offset+e.size]) for e in sorted(outer.find('Motions').children,key=lambda e:int(e.name))]
 (out/f'{stem}.json').write_text(json.dumps(body,separators=(',',':')))
 for e in vfs.find('Bitmaps').children:
  w,h,rgba=act.decode_gebm(vfs.data[e.offset:e.offset+e.size]);act.write_png(out/f'{stem}_{e.name}.png',w,h,rgba)
 print(stem,len(body['vertices']))
