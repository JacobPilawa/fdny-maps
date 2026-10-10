#!/usr/bin/env python3
"""Build an explicit public file set for Cloudflare's free static assets hosting."""
from pathlib import Path
import hashlib,json,shutil

ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'dist'
files=[ROOT/name for name in ['app.js','style.css','collision-overlay.js','flight.js','textured-world.js','entity-volumes.js','animation.js','victim-models.js','script-info.js']]
for name in ['vendor','data','reference_pdfs','source/scripts']:
    files.extend(p for p in (ROOT/name).rglob('*') if p.is_file() and '__pycache__' not in p.parts)
manifest=json.loads((ROOT/'data/manifest.json').read_text())
for item in manifest:
    folder=ROOT/'source'/item['key']
    files.extend(p for p in folder.iterdir() if p.is_file() and p.suffix in {'.csv','.json','.txt'})
files=sorted(set(files),key=lambda p:p.relative_to(ROOT).as_posix())
assert len(files)+1<=20000,'Cloudflare Free static asset count exceeded'
for p in files:
    assert p.stat().st_size<=25*1024*1024,f'Cloudflare single-file limit exceeded: {p}'
    assert not p.is_symlink(),f'Unexpected symlink: {p}'
fingerprint=hashlib.sha256((ROOT/'index.html').read_bytes())
for p in files:
    fingerprint.update(p.relative_to(ROOT).as_posix().encode()+b'\0'+p.read_bytes())
version=fingerprint.hexdigest()[:16]
if OUT.exists():shutil.rmtree(OUT)
release=OUT/'releases'/version
release.mkdir(parents=True)
for p in files:
    target=release/p.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target)
html=(ROOT/'index.html').read_text().replace('<head>','<head><base href="/releases/'+version+'/">',1)
(OUT/'index.html').write_text(html)
(OUT/'_headers').write_text(f'''/
  Cache-Control: public, max-age=0, must-revalidate
/index.html
  Cache-Control: public, max-age=0, must-revalidate
/releases/{version}/*
  Cache-Control: public, max-age=31536000, immutable
/releases/{version}/source/scripts/*
  Content-Type: text/plain; charset=utf-8
''')
report={'release':version,'asset_files':len(files)+1,'maps':len(manifest),'total_bytes':sum(p.stat().st_size for p in OUT.rglob('*') if p.is_file()),'largest_file_bytes':max(p.stat().st_size for p in files),'static_only':True,'worker_script':False,'paid_storage_bindings':[],'public_files':[p.relative_to(ROOT).as_posix() for p in files]}
(ROOT/'deploy/build-report.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='public_files'},indent=2))
