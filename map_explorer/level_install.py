#!/usr/bin/env python3
"""Install or restore an exported retail trigger test, with verified backups."""
import argparse,hashlib,json,shutil
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest() if p.is_file() else None

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('game',type=Path);g=ap.add_mutually_exclusive_group();g.add_argument('--install',action='store_true');g.add_argument('--restore',action='store_true');a=ap.parse_args()
    game=a.game.expanduser().resolve()
    if game.suffix.lower()=='.app':game=game/'Contents/SharedSupport/prefix/drive_c/Program Files/FireFighter American Hero'
    if not (game/'Data/ResDefs/thegame.lev').is_file():raise ValueError('Not a supported game directory')
    backup=game/'.fdny-level-edit-backup';root=Path(__file__).resolve().parent
    if a.restore:
        state=json.loads((backup/'state.json').read_text());items=state['files']
        for name,meta in items.items():
            if sha(game/name)!=meta['sha256']:raise ValueError('Installed file changed; restore stopped: '+name)
            if meta['original_sha256'] is not None and sha(backup/name)!=meta['original_sha256']:raise ValueError('Backup is missing or changed: '+name)
        for name,meta in items.items():
            target=game/name
            if meta['original_sha256'] is None:target.unlink()
            else:shutil.copy2(backup/name,target)
        shutil.rmtree(backup);print('Original game files restored.');return
    state=json.loads((root/'manifest.json').read_text());items=state['files']
    if backup.exists():raise ValueError('An earlier install backup exists. Restore that install first.')
    for name,meta in items.items():
        p=Path(name)
        if p.is_absolute() or '..' in p.parts or not name.startswith('Data/'):raise ValueError('Unsafe export path')
        if sha(root/name)!=meta['sha256']:raise ValueError('Export is missing or changed: '+name)
        if sha(game/name)!=meta['original_sha256']:raise ValueError('Game file differs from export baseline: '+name)
    print('\n'.join(items));print(f'{len(items)} files verified.')
    if not a.install:print('Dry run only. Add --install to apply.');return
    backup.mkdir()
    for name,meta in items.items():
        if meta['original_sha256'] is not None:
            p=backup/name;p.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(game/name,p)
    (backup/'state.json').write_text(json.dumps(state,indent=2))
    try:
        for name in items:
            p=game/name;p.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(root/name,p)
    except OSError:
        for name,meta in items.items():
            p=game/name
            if meta['original_sha256'] is None:p.unlink(missing_ok=True)
            else:shutil.copy2(backup/name,p)
        shutil.rmtree(backup)
        raise
    print('Installed. Choose Custom trigger test in the first Hose slot.')

if __name__=='__main__':
    try:main()
    except (ValueError,OSError,KeyError) as e:raise SystemExit(str(e))
