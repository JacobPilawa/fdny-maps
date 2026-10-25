"""Retail export checks; never installs into the real game."""
import io,json,shutil,struct,subprocess,sys,tempfile,zipfile
from xml.etree import ElementTree as ET
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import level_edit as l

game=l.game_directory();count=0
for p in (game/'Data/Maps').iterdir():
    if p.suffix.lower()=='.bsp':assert l.Bsp(p.read_bytes()).write()==p.read_bytes();count+=1
print(f'{count} retail BSPs round-trip byte for byte')
example=None
for mission in l.missions(game):
    info=l.describe(game,mission['key']);raw=l.child(game/'Data/Maps',mission['key']+'.bsp').read_bytes();before=l.Bsp(raw)
    triggers=[{'name':'custom_hint1','center':[v+123 for v in info['spawn']],'size':[128,192,96],'message':'Custom trigger works!','seconds':5},{'name':'custom_hint2','center':[-1320.25,218.5,364.75],'size':[320,72,680],'message':'Another custom box','seconds':3}]
    project={'schema':l.SCHEMA,'source':mission['key'],'source_sha256':info['source_sha256'],'triggers':triggers}
    archive=l.export_project(game,project);z=zipfile.ZipFile(io.BytesIO(archive));after=l.Bsp(z.read('Data/Maps/'+l.KEY+'.bsp'))
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
    _,old=l.catalog((game/'Data/ResDefs/thegame.lev').read_bytes());_,new=l.catalog(z.read('Data/ResDefs/thegame.lev'))
    assert len(old)==len(new);assert new[1]['key']==l.KEY and new[1]['fields']['rrf']=='rescue.rrf'
    for i in range(len(old)):
        if i!=1:assert old[i]['fields']==new[i]['fields']
    manifest=json.loads(z.read('manifest.json'))
    for name,meta in manifest['files'].items():assert l.digest(z.read(name))==meta['sha256']
    assert all(name.encode() in z.read('README.txt') for name in manifest['files'])
    assert l.digest(raw)==info['source_sha256']
    # Checkpoints use real script markers, reset on load, stable IDs and ordered splits.
    for i,t in enumerate(triggers):t.update(checkpoint=True,split_label='Checkpoint <'+str(i+1)+'> & test')
    timed=zipfile.ZipFile(io.BytesIO(l.export_project(game,project)));profile=json.loads(timed.read('timing/checkpoints.json'))
    assert profile['status']=='generated-asl-live-validation-pending' and profile['runtime_map']==l.KEY
    assert profile['default_stage_finishes']==info['stage_finishes']
    assert profile['map_sha256']==l.digest(timed.read('Data/Maps/'+l.KEY+'.bsp'))
    entities=[l.Bsp.pairs(e) for e in l.Bsp(timed.read('Data/Maps/'+l.KEY+'.bsp')).entities]
    for t,c in zip(triggers,profile['checkpoints']):
        assert c['trigger']==t['name'] and c['label']==t['split_label']
        assert ('set '+c['variable']+' 0').encode() in timed.read('Data/Maps/'+l.KEY+'.evs')
        e=next(e for e in entities if e.get('A_Name')==t['name'])
        assert 'set '+c['variable']+' 1' in e['S_Script01'] and 'hint ' in e['S_Script01']
    segments=ET.fromstring(timed.read('timing/custom-splits.lss')).findall('Segments/Segment/Name')
    assert [s.text for s in segments]==[t['split_label'] for t in triggers]+[s+' complete' for s in profile['default_stage_finishes']]
    assert l.checkpoint_variable(triggers[0])==profile['checkpoints'][0]['id']
    assert {'timing/FDNY_Custom_ILs.asl','timing/FDNY_Custom_rescue%.asl'}<=set(timed.namelist())
    assert 'vars.customVariables' in timed.read('timing/FDNY_Custom_ILs.asl').decode()
    # Text-only edit preserves every geometry chunk.
    t=info['triggers'][0];edit={k:t[k] for k in ('name','center','size')};edit.update(existing_entity=t['entity'],message='Existing hint replaced',seconds=4)
    edit['checkpoint']=True;project['triggers']=[edit];text_only=zipfile.ZipFile(io.BytesIO(l.export_project(game,project)));b=l.Bsp(text_only.read('Data/Maps/'+l.KEY+'.bsp'))
    assert all(b.payload[typ]==before.payload[typ] for typ in before.payload if typ!=16)
    assert len(b.entities)==len(before.entities)
    print(mission['key']+': two independent boxes, topology, original chunks, catalog, and existing hint verified')
    if example is None:example=archive
with tempfile.TemporaryDirectory() as td:
    root=Path(td);bundle=root/'export';bundle.mkdir();zipfile.ZipFile(io.BytesIO(example)).extractall(bundle)
    fake=root/'game';(fake/'Data/ResDefs').mkdir(parents=True);(fake/'Data/ResDefs/thegame.lev').write_bytes((game/'Data/ResDefs/thegame.lev').read_bytes())
    call=[sys.executable,str(bundle/'install.py'),str(fake)]
    subprocess.run(call,check=True,capture_output=True);assert not (fake/'.fdny-level-edit-backup').exists()
    subprocess.run(call+['--install'],check=True,capture_output=True);assert (fake/'Data/Maps/fdny_edit.bsp').is_file()
    catalog=fake/'Data/ResDefs/thegame.lev';changed=catalog.read_bytes();catalog.write_bytes(changed+b'changed')
    assert subprocess.run(call+['--restore'],capture_output=True).returncode!=0
    catalog.write_bytes(changed);subprocess.run(call+['--restore'],check=True,capture_output=True)
    assert catalog.read_bytes()==(game/'Data/ResDefs/thegame.lev').read_bytes()
    assert not (fake/'Data/Maps/fdny_edit.bsp').exists();assert not (fake/'.fdny-level-edit-backup').exists()
print('Installer dry run, install, changed-file refusal, and restore passed in a temporary fake game')
with tempfile.TemporaryDirectory() as td:
    # Copy only the viewer's export code: no sibling fdny-tas checkout or imports.
    isolated=Path(td)/'viewer';isolated.mkdir()
    for name in ['level_edit.py','level_install.py']:shutil.copy2(l.ROOT/name,isolated/name)
    shutil.copytree(l.ROOT/'livesplit',isolated/'livesplit',ignore=shutil.ignore_patterns('__pycache__'))
    project=json.loads(zipfile.ZipFile(io.BytesIO(example)).read('project.json'));project['triggers'][0]['checkpoint']=True
    code="""import io,json,sys,zipfile
sys.path.insert(0,sys.argv[1])
import level_edit as l
from pathlib import Path
z=zipfile.ZipFile(io.BytesIO(l.export_project(Path(sys.argv[2]),json.load(sys.stdin))))
assert {'timing/FDNY_Custom_ILs.asl','timing/FDNY_Custom_rescue%.asl','timing/custom-splits.lss','Data/Maps/fdny_edit.bsp'}<=set(z.namelist())
"""
    subprocess.run([sys.executable,'-I','-c',code,str(isolated),str(game)],input=json.dumps(project),text=True,check=True)
print('Standalone viewer export passed without an fdny-tas checkout')
