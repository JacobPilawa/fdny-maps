#!/usr/bin/env python3
"""Headless checks using the actual port's Collision and Player implementations."""
import copy
import json
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from compiler import ROOT, compile_level, validate

def run(args):
    return subprocess.run([str(a) for a in args],cwd=ROOT,check=True)

def main():
    doc=json.loads((ROOT/'level_editor/examples/empty_room.json').read_text())
    doc['key']='custom_validation_room'
    room=compile_level(doc)
    # Fixture is generated only; it is not saved into the user's mission list.
    fixture=copy.deepcopy(doc);fixture['key']='custom_editor_check';fixture['title']='Editor verification'
    fixture['items'] += [dict(kind=kind,position=pos,size=size,rotation=0) for kind,pos,size in [
        ('ramp',[0,0,0],[256,128,384]),('fire',[400,0,0],[64,96,64]),('survivor',[-400,40,0],[40,100,40])]]
    package=compile_level(fixture)
    for mutate in [lambda d:d.update(key='../training'),lambda d:d['items'][0].update(size=[0,32,256]),lambda d:d['items'].append(copy.deepcopy(next(i for i in d['items'] if i['kind']=='spawn')))]:
        invalid=copy.deepcopy(doc);mutate(invalid)
        try:validate(invalid)
        except ValueError:pass
        else:raise AssertionError('Invalid project was accepted')
    flags=shlex.split(subprocess.check_output(['pkg-config','--cflags','--libs','json-c'],text=True))
    with tempfile.TemporaryDirectory(prefix='fdny-editor-check-') as temp:
        binary=Path(temp)/'collision-check'
        run(['c++','-std=c++17','-ffp-contract=off','-I'+str(ROOT/'port/spikes/training-native-prototype/src'),ROOT/'level_editor/tests/collision_check.cpp',*flags,'-o',binary])
        run([binary,room/'collision.json']);run([binary,package/'collision.json','ramp'])
    native=ROOT/'port/spikes/training-native-prototype/.local/build-scene-viewer/fdny-scene-viewer'
    if native.is_file():run([native,room,'--inspect'])
    print('PASS editor validation and native geometry/movement checks')
if __name__=='__main__':main()
