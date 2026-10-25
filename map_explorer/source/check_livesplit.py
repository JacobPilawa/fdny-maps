"""Offline generator regressions; optionally emit a C# fixture for the actual ASL actions.

python -I source/check_livesplit.py [--emit-csharp output.cs]
Compile output.cs with csc /r:Microsoft.CSharp.dll, then run it (no game needed).
The fixture uses the generated C# bodies, not a Python model of their behavior.
"""
import copy
import re
import sys
import unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from livesplit import custom_triggers as custom

PROFILE = dict(schema='fdny-checkpoints-v1', source_map='training', runtime_map='fdny_edit', checkpoints=[
    dict(variable='FDNY_CP_A', label='First', from_value='0', to_value='1'),
    dict(variable='FDNY_CP_B', label='Second', from_value='0', to_value='1')])


def body(script, name):
    match = re.search(r'(?ms)^' + re.escape(name) + r'\n\{\n(.*?)^\}', script)
    return match[1] if match else 'return false;\n'


class GeneratorTests(unittest.TestCase):
    def test_stock_actions(self):
        for filename in ['FDNY_ILs.asl', 'FDNY_rescue%.asl']:
            base = (custom.ROOT / filename).read_text()
            output = custom.generate(PROFILE, base)
            for name in ['start', 'isLoading', 'reset']:
                self.assertEqual(body(base, name), body(output, name))
            self.assertIn('refreshRate = 200;', output)
            self.assertIn('new string[] { "FDNY_CP_A", "FDNY_CP_B" }', output)

    def test_invalid_profiles(self):
        base = (custom.ROOT / 'FDNY_ILs.asl').read_text()
        for key, value in [('runtime_map', 'training'), ('runtime_map', '../bad'), ('source_map', 'bad'), ('schema', 'bad'), ('checkpoints', [None])]:
            with self.subTest(key=key), self.assertRaises(ValueError):
                custom.generate(dict(PROFILE, **{key: value}), base)
        for update in [dict(variable='x"; bad()'), dict(from_value='1'), dict(label=''), dict(variable='FDNY_CP_A')]:
            profile = copy.deepcopy(PROFILE);profile['checkpoints'][1].update(update)
            with self.subTest(update=update), self.assertRaises(ValueError):custom.generate(profile, base)
        with self.assertRaises(ValueError):custom.generate(PROFILE, base.replace('v2.2', 'v9'))

    def test_empty_and_escaping(self):
        base = (custom.ROOT / 'FDNY_ILs.asl').read_text()
        self.assertIn('new string[] {  }', custom.generate(dict(PROFILE, checkpoints=[]), base))
        profile = copy.deepcopy(PROFILE);profile['checkpoints'][0]['label'] = 'Door "A" \\ west'
        self.assertIn('"Door \\"A\\" \\\\ west"', custom.generate(profile, base))


FIXTURE = r'''
using System;
using System.Linq;
using System.Dynamic;
using System.Collections.Generic;
class Module { public IntPtr BaseAddress = new IntPtr(0x400000); }
class Memory {
    public Dictionary<long,uint> Words = new Dictionary<long,uint>();
    public Dictionary<long,string> Strings = new Dictionary<long,string>();
    public T ReadValue<T>(IntPtr p) { uint value; Words.TryGetValue(p.ToInt64(), out value); return (T)(object)value; }
    public string ReadString(IntPtr p, int size) { string value; return Strings.TryGetValue(p.ToInt64(), out value) ? value : null; }
    public void Table() {
        Words[0x503048]=0x700000; Words[0x503058]=0x100;
        Words[0x700104]=0x300; Words[0x700108]=0x200;
        Strings[0x700114]="FDNY_CP_A"; Strings[0x700214]="fdny_cp_b"; Strings[0x700314]="DIFFICULTY";
        Words[0x700110]=0x800000; Words[0x700210]=0x800100;
        Strings[0x80000C]="0"; Strings[0x80010C]="0";
    }
    public void Mark(int n) { Strings[n==0 ? 0x80000C : 0x80010C]="1"; }
}
abstract class Script {
    public dynamic vars = new ExpandoObject(), current, old;
    public Memory memory = new Memory(); public List<Module> modules = new List<Module>{new Module()};
    public int refreshRate; public void print(string s) { }
    public abstract void Startup(); public abstract void Init(); public abstract void Update();
    public abstract bool Start(); public abstract bool Split(); public abstract bool Loading(); public abstract void ResetEvent();
    public dynamic State(int state, string map) {
        dynamic x = new ExpandoObject(); var d=(IDictionary<string,object>)x;
        foreach(string name in new string[]{"gameState","levelChange","customMissionResult","bankLevelEndSoundPlayed","barLevelEndSoundPlayed","gasStationLevelEndSoundPlayed","militaryBaseLevelEndSoundPlayed","trainingLevelEndSoundPlayed","boatLevelEndSoundPlayed","oilRigDLevelEndSoundPlayed","subwayLevelEndSoundPlayed"}) d[name]=0;
        x.gameState=state;x.mapName=map;x.pendingMap="";return x;
    }
    public void Frame(int before, int after, string map) { old=State(before,map);current=State(after,map);Update(); }
    public void Begin(string map) { Startup();Init();memory.Table();Frame(3,1,map); }
}
__CLASSES__
class Check {
    static int assertions;
    static void Assert(bool value, string name) { assertions++; if(!value)throw new Exception(name); }
    static void Run(Script s) {
        s.Begin("bank");Assert(s.Start(),"stock start");Assert(!s.Split(),"stock idle");
        s.current.bankLevelEndSoundPlayed=1;Assert(s.Split(),"stock win");Assert(s.Loading(),"win pause");
        s.Begin("fdny_edit");Assert(s.Start(),"custom start");Assert(s.vars.levelMap=="training","source alias");Assert(s.vars.customArmed,"zero arm");Assert(!s.Split(),"no idle split");
        s.memory.Mark(0);s.Frame(1,1,"fdny_edit");Assert(s.Split(),"first touch");Assert(!s.Split(),"no repeat");
        s.memory.Mark(1);s.Frame(1,1,"fdny_edit");s.current.customMissionResult=1;
        Assert(s.Split(),"checkpoint before simultaneous win");Assert(s.Split(),"latched win");Assert(!s.Split(),"no repeated win");
        s.Begin("fdny_edit");s.memory.Mark(1);s.Frame(1,1,"fdny_edit");Assert(s.vars.customInvalid&&!s.Split(),"out of order");
        s.Begin("fdny_edit");s.current.customMissionResult=1;Assert(!s.Split()&&s.vars.customInvalid,"missing checkpoint at win");
        s.Begin("fdny_edit");s.memory.Words[0x700110]=0;s.Frame(1,1,"fdny_edit");Assert(s.vars.customInvalid&&!s.Split(),"unreadable marker");
        s.Startup();s.Init();s.memory.Table();s.memory.Mark(0);s.Frame(3,1,"fdny_edit");Assert(!s.vars.customArmed&&!s.Split(),"stale marker not armed");
        s.Begin("fdny_edit");s.memory.Mark(0);s.Frame(1,1,"fdny_edit");Assert(s.Split(),"pre-restart touch");
        s.Frame(1,0,"fdny_edit");s.memory.Table();s.Frame(3,1,"fdny_edit");Assert(!s.Split(),"same-map restart clear");s.memory.Mark(0);s.Frame(1,1,"fdny_edit");Assert(s.Split(),"same-map restart touch");
        s.ResetEvent();Assert(!s.vars.customArmed&&s.vars.customIndex==0,"manual reset clear");
        s.Begin("fdny_edit");s.memory.Words[0x700108]=0x100;Assert(s.vars.readCheckpoint("FDNY_CP_B")==null,"cyclic table bounded");
        s.Begin("fdny_edit");s.memory.Words[0x503048]=0x900000;s.memory.Strings[0x900114]="FDNY_CP_A";s.memory.Words[0x900110]=0x800000;
        Assert(s.vars.readCheckpoint("FDNY_CP_A")=="0","moved table base");
    }
    static int Main() {
        Run(new IL());Run(new Rescue());Script s=new Boat();s.Begin("fdny_edit");
        s.memory.Mark(0);s.memory.Mark(1);s.Frame(1,1,"boat_b");s.current.pendingMap="boat_b";s.current.levelChange=1;s.Update();
        Assert(s.Split(),"first queued touch");Assert(s.Split(),"second queued touch");Assert(s.Split(),"stage follows queue");Assert(!s.Split(),"no repeated exit");Assert(s.Loading(),"request pause unchanged");
        s.Frame(3,1,"boat_b");Assert(!s.vars.customStage,"stock next stage");s.current.boatLevelEndSoundPlayed=1;Assert(s.Split(),"stock final");
        Console.WriteLine("PASS: "+assertions+" generated-C# assertions (IL, rescue%, staged clone)");return 0;
    }
}
'''


def emit(path):
    classes = []
    for name, mode, source in [('IL', 'FDNY_ILs.asl', 'training'), ('Rescue', 'FDNY_rescue%.asl', 'training'), ('Boat', 'FDNY_ILs.asl', 'boat_a')]:
        script = custom.generate(dict(PROFILE, source_map=source), (custom.ROOT / mode).read_text())
        methods = [('Startup','startup','void'),('Init','init','void'),('Update','update','void'),('Start','start','bool'),('Split','split','bool'),('Loading','isLoading','bool'),('ResetEvent','onReset','void')]
        classes.append('class '+name+' : Script {\n'+''.join('public override '+kind+' '+method+'() {\n'+body(script, action)+'}\n' for method, action, kind in methods)+'}\n')
        path.with_name(name+'.asl').write_text(script)
    path.write_text(FIXTURE.replace('__CLASSES__', '\n'.join(classes)))


if __name__ == '__main__':
    if len(sys.argv) == 3 and sys.argv[1] == '--emit-csharp':
        emit(Path(sys.argv[2]));sys.argv = [sys.argv[0]]
    unittest.main()
