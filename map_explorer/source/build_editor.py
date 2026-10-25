"""Build only the per-mission inputs needed by the browser's shared exporter."""
import io,json,re,sys,zipfile
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import level_edit as le


def build(game,out):
    out.mkdir(parents=True,exist_ok=True)
    text,records=le.catalog((game/'Data/ResDefs/thegame.lev').read_bytes())
    manifest=[]
    for mission in le.missions(game):
        key=mission['key'];info=le.describe(game,key)
        files={}
        for suffix in ['bsp','brf','bmp','evs']:
            try:p=le.child(game/'Data/Maps',key+'.'+suffix)
            except ValueError:
                if suffix=='evs':continue
                raise
            files['Data/Maps/'+p.name]=p.read_bytes()
        # Include only this mission's script dependency closure.
        bsp=le.Bsp(files['Data/Maps/'+le.child(game/'Data/Maps',key+'.bsp').name])
        pending=[v for e in bsp.entities for k,v in le.Bsp.pairs(e).items() if k.startswith('S_Script')]
        pending += [v.decode('latin1') for n,v in files.items() if n.endswith('.evs')]
        while pending:
            for name in re.findall(r'(?i)\bexec\s+([\w.-]+\.evs)',pending.pop()):
                path=le.child(game/'Data/Scripts',name);target='Data/Scripts/'+path.name
                if target not in files:files[target]=path.read_bytes();pending.append(files[target].decode('latin1'))
        files['Data/ResDefs/thegame.lev']='\n'.join(text[r['start']:r['end']] for r in records if r['key'] in {key,'dock_h'}).encode('latin1')
        data=io.BytesIO()
        with zipfile.ZipFile(data,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
            for name,value in sorted(files.items()):
                entry=zipfile.ZipInfo(name);entry.compress_type=zipfile.ZIP_DEFLATED;z.writestr(entry,value)
        (out/(key+'.zip')).write_bytes(data.getvalue())
        manifest.append(dict(**mission,**{'info':info}))
        print(key,len(data.getvalue()),'bytes')
    (out/'missions.json').write_text(json.dumps(manifest,separators=(',',':'))+'\n')

if __name__=='__main__':build(le.game_directory(),le.ROOT/'data/editor')
