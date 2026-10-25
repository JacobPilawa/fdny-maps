"""Retail export checks; never installs into the real game."""
import copy,io,json,re,shutil,struct,subprocess,sys,tempfile,zipfile
from xml.etree import ElementTree as ET
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import level_edit as l

game=l.game_directory();count=0
for p in (game/'Data/Maps').iterdir():
    if p.suffix.lower()=='.bsp':assert l.Bsp(p.read_bytes()).write()==p.read_bytes();count+=1
print(f'{count} retail BSPs round-trip byte for byte')
def entry_from_readme(archive):
    readme=archive.read('README.txt').decode()
    return re.search(r'(?ms)^level: fdny_edit\n.*?(?=\n\n)',readme)[0]

def check_package(archive):
    names=set(archive.namelist());assert {'README.txt','FDNY_Custom_ILs.asl','custom-splits.lss'}<=names
    assert not any(name.endswith(('.py','.json','.lev')) for name in names)
    assert not any(name.startswith('timing/') for name in names)
    data={name for name in names if name.startswith('Data/')}
    assert names==data|{'README.txt','FDNY_Custom_ILs.asl','custom-splits.lss'}
    assert all(name.startswith(('Data/Maps/fdny_edit.','Data/Scripts/fdnyedit_')) for name in data)
    for name in data:
        assert name in archive.read('README.txt').decode()
        if name.startswith('Data/Scripts/'):
            original=name.removeprefix('Data/Scripts/fdnyedit_')
            assert archive.read(name)!=l.child(game/'Data/Scripts',original).read_bytes(),name
    return data

example=None
for mission in l.missions(game):
    info=l.describe(game,mission['key']);raw=l.child(game/'Data/Maps',mission['key']+'.bsp').read_bytes();before=l.Bsp(raw)
    triggers=[{'name':'custom_hint1','center':[v+123 for v in info['spawn']],'size':[128,192,96],'message':'Custom trigger works!','seconds':5},{'name':'custom_hint2','center':[-1320.25,218.5,364.75],'size':[320,72,680],'message':'Another custom box','seconds':3}]
    project={'schema':l.SCHEMA,'source':mission['key'],'source_sha256':info['source_sha256'],'triggers':triggers}
    archive=l.export_project(game,project);z=zipfile.ZipFile(io.BytesIO(archive));after=l.Bsp(z.read('Data/Maps/'+l.KEY+'.bsp'));check_package(z)
    if example is None:example=archive;example_project=copy.deepcopy(project)
    for typ in before.payload:
        if typ!=16:assert after.payload[typ].startswith(before.payload[typ]),(mission['key'],typ)
    assert len(after.entities)==len(before.entities)+4
    for n,t in enumerate(triggers):
        model=len(before.payload[1])//80+n;lo,hi=after.bounds(model)
        assert lo==[t['center'][a]-t['size'][a]/2 for a in range(3)]
        assert hi==[t['center'][a]+t['size'][a]/2 for a in range(3)]
        m=after.record(1,model,'<2i9f8iI');visited=set()
        def walk(i):
            if i<0:
                assert m[13]<=-i-1<m[13]+m[14];return
            assert i not in visited;visited.add(i);r=after.record(2,i,'<5i6f');assert 0<=r[4]<len(after.payload[10])//20
            assert r[2]==0 or m[11]<=r[3]<m[11]+m[12];walk(r[0]);walk(r[1])
        walk(m[0]);assert len(visited)==6
        for f in range(m[11],m[11]+m[12]):
            face=after.record(11,f,'<8i4B');plane=after.record(10,face[2],'<4fi')
            assert 0<=plane[4]<=5
            for vi in range(face[0],face[0]+face[1]):
                v=after.record(14,after.record(13,vi,'<i')[0],'<3f')
                assert abs(sum(v[a]*plane[a] for a in range(3))-plane[3])<.01
                assert all(lo[a]-.01<=v[a]<=hi[a]+.01 for a in range(3))
    original_catalog=(game/'Data/ResDefs/thegame.lev').read_bytes();catalog_text,old=l.catalog(original_catalog)
    slot=next(r for r in old if r['key'] in ('dock_h',l.KEY));changed=catalog_text[:slot['start']]+entry_from_readme(z)+'\n\n'+catalog_text[slot['end']:]
    _,new=l.catalog(changed.encode('latin1'));assert len(old)==len(new)
    for before_entry,after_entry in zip(old,new):
        if before_entry is slot:
            assert after_entry['key']==l.KEY and after_entry['fields']['rrf']=='rescue.rrf'
            assert after_entry['fields']['time']==next(r for r in old if r['key']==mission['key'])['fields']['time']
        else:assert before_entry['fields']==after_entry['fields']
    assert (game/'Data/ResDefs/thegame.lev').read_bytes()==original_catalog
    assert l.digest(raw)==info['source_sha256']
    # Checkpoints use real script markers, reset on load, stable IDs and ordered splits.
    for i,t in enumerate(triggers):t.update(checkpoint=True,split_label='Checkpoint <'+str(i+1)+'> & test')
    timed=zipfile.ZipFile(io.BytesIO(l.export_project(game,project)));check_package(timed)
    entities=[l.Bsp.pairs(e) for e in l.Bsp(timed.read('Data/Maps/'+l.KEY+'.bsp')).entities]
    for t in triggers:
        c={'variable':l.checkpoint_variable(t)}
        assert ('set '+c['variable']+' 0').encode() in timed.read('Data/Maps/'+l.KEY+'.evs')
        e=next(e for e in entities if e.get('A_Name')==t['name'])
        assert 'set '+c['variable']+' 1' in e['S_Script01'] and 'hint ' in e['S_Script01']
    segments=ET.fromstring(timed.read('custom-splits.lss')).findall('Segments/Segment/Name')
    assert [s.text for s in segments]==[t['split_label'] for t in triggers]+[s+' complete' for s in info['stage_finishes']]
    asl=timed.read('FDNY_Custom_ILs.asl').decode()
    assert 'vars.customVariables' in asl and all(l.checkpoint_variable(t) in asl for t in triggers)
    # Text-only edit preserves every geometry chunk.
    t=info['triggers'][0];edit={k:t[k] for k in ('name','center','size')};edit.update(existing_entity=t['entity'],message='Existing hint replaced',seconds=4)
    edit['checkpoint']=True;project['triggers']=[edit];text_only=zipfile.ZipFile(io.BytesIO(l.export_project(game,project)));b=l.Bsp(text_only.read('Data/Maps/'+l.KEY+'.bsp'))
    assert all(b.payload[typ]==before.payload[typ] for typ in before.payload if typ!=16)
    assert len(b.entities)==len(before.entities)
    print(mission['key']+': two independent boxes, topology, original chunks, catalog, and existing hint verified')
with tempfile.TemporaryDirectory() as td:
    # Follow the README in a fake game directory, using only the new Data files.
    root=Path(td);z=zipfile.ZipFile(io.BytesIO(example));paths=check_package(z)
    for name in paths:
        target=root/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(z.read(name))
    target=root/'Data/ResDefs/thegame.lev';target.parent.mkdir(parents=True);original=(game/'Data/ResDefs/thegame.lev').read_bytes();target.write_bytes(original)
    text,entries=l.catalog(original);slot=next(r for r in entries if r['key'] in ('dock_h',l.KEY))
    target.write_bytes((text[:slot['start']]+entry_from_readme(z)+'\n\n'+text[slot['end']:]).encode('latin1'))
    assert next(r for r in l.catalog(target.read_bytes())[1] if r['key']==l.KEY)['fields']['rrf']=='rescue.rrf'
    target.write_bytes(original)
    for name in paths:(root/name).unlink()
    assert target.read_bytes()==original and not (root/'Data/Maps/fdny_edit.bsp').exists()
print('Minimal file set and manual catalog edit/undo passed in a temporary fake game')
with tempfile.TemporaryDirectory() as td:
    # Copy only the viewer's export code: no sibling fdny-tas checkout or imports.
    isolated=Path(td)/'viewer';isolated.mkdir()
    shutil.copy2(l.ROOT/'level_edit.py',isolated/'level_edit.py')
    shutil.copytree(l.ROOT/'livesplit',isolated/'livesplit',ignore=shutil.ignore_patterns('__pycache__'))
    project=copy.deepcopy(example_project);project['triggers'][0]['checkpoint']=True
    code="""import io,json,sys,zipfile
sys.path.insert(0,sys.argv[1])
import level_edit as l
from pathlib import Path
z=zipfile.ZipFile(io.BytesIO(l.export_project(Path(sys.argv[2]),json.load(sys.stdin))))
assert {'FDNY_Custom_ILs.asl','custom-splits.lss','README.txt','Data/Maps/fdny_edit.bsp'}<=set(z.namelist())
"""
    subprocess.run([sys.executable,'-I','-c',code,str(isolated),str(game)],input=json.dumps(project),text=True,check=True)
print('Standalone viewer export passed without an fdny-tas checkout')

with tempfile.TemporaryDirectory() as td:
    root=Path(td);scripts=root/'Data/Scripts';scripts.mkdir(parents=True)
    (scripts/'a.evs').write_text('exec b.evs\n');(scripts/'b.evs').write_text('exec a.evs\n')
    rewrite,files=l.script_rewriter(root,'training',['exec a.evs']);assert not files and rewrite('exec a.evs')=='exec a.evs'
    (scripts/'b.evs').write_text('exec a.evs\nif LEVEL training (set X 1)\n')
    rewrite,files=l.script_rewriter(root,'training',['exec a.evs']);assert set(files)=={'Data/Scripts/fdnyedit_a.evs','Data/Scripts/fdnyedit_b.evs'}
    assert rewrite('exec a.evs')=='exec fdnyedit_a.evs' and b'LEVEL fdny_edit' in files['Data/Scripts/fdnyedit_b.evs']
    try:l.script_rewriter(root,'training',['exec ../bad.evs']);raise AssertionError('Unsafe include accepted')
    except ValueError:pass
print('Unchanged script reuse, changed dependencies, cycles and unsafe include checks passed')
