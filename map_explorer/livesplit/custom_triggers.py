"""Optional custom trigger ASL generator; stock splitters stay unchanged.

Usage (from the map_explorer directory):
    python -I livesplit/custom_triggers.py timing/checkpoints.json --output custom.asl
Add --mode rescue for the rescue% base; the default is IL. Output must be a new file.
The editor may call generate(profile, base_text) to bundle a standalone ASL.

Open the editor's .lss in LiveSplit, select the generated ASL in Control ->
Scriptable Auto Splitter, and use Game Time. Use only one autosplitter. Rescue%
needs a run-wide split list when used across missions. Python, JSON and FDNYCore
are not needed at runtime; this tool never writes game memory or game files.

Profiles use fdny-checkpoints-v1: source_map, runtime_map and ordered checkpoints
with variable/label/from_value/to_value fields. The map must initialize each
FDNY_CP_... variable to 0 and set it to 1 on touch. Place checkpoints away from
spawn: the reader must observe all zeroes before the first touch. Missing or
out-of-order markers require a menu visit/restart; also restart after a manual
timer reset mid-stage. Simultaneous observations are queued in profile order,
not assigned exact contact timestamps. Cloned-map wins use the mission result.

Bundled templates: RichardFranksComputer/fdny-tas, livesplit/*.asl, v2.2,
commit 660b272efd921b8aa38b862fce1dd77c7031824b. Templates are copied unchanged.
Supports the current v2.2 stock ASL structure. Tests: source/check_livesplit.py.
Synthetic C# actions and real LiveSplit parser/compiler loading were checked;
live contact/finish timing, 32-marker performance and VM regressions remain pending.
"""
import argparse
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent
MAPS = {'training', 'bank', 'bar_r', 'gas_r', 'mbase', 'boat_a', 'boat_b',
        'subway_a', 'subway_b', 'oil_rig_a', 'oil_rig_b', 'oil_rig_c', 'oil_rig_d'}

# ScriptEngine's global variable AVL table, exe RVA 0x103044:
# +4 allocation base, +0x14 root offset; node +4/+8 child offsets,
# +0x14 inline key, +0x10 ref-counted value String*, chars at String+0xC.
# FindScriptVariableEntry 0x47CF70 -> 0x47C990; StoreScriptVariableValue 0x470FC0.
READER = '''
    vars.readCheckpoint = (Func<string, string>)(name => {
        IntPtr table = modules.First().BaseAddress + 0x103044;
        uint storage = memory.ReadValue<uint>(table + 4);
        uint offset = memory.ReadValue<uint>(table + 0x14);
        for (int depth = 0; storage >= 0x10000 && offset != 0 && depth < 64; depth++)
        {
            if (offset > 0x1000000) return null;
            IntPtr node = new IntPtr((long)storage + offset);
            string key = memory.ReadString(node + 0x14, 128);
            if (String.IsNullOrEmpty(key)) return null;
            int comparison = String.Compare(name, key, StringComparison.OrdinalIgnoreCase);
            if (comparison == 0)
            {
                uint value = memory.ReadValue<uint>(node + 0x10);
                if (value < 0x10000) return null;
                string text = memory.ReadString(new IntPtr((long)value + 0xC), 2);
                return text == "0" || text == "1" ? text : null;
            }
            offset = memory.ReadValue<uint>(node + (comparison < 0 ? 4 : 8));
        }
        return null;
    });
'''
RESET = '''
    vars.customStage = false;
    vars.customArmed = false;
    vars.customInvalid = false;
    vars.customIndex = 0;
    vars.customEmitted = 0;
    vars.customFinish = false;
'''
UPDATE = '''
    if (current.gameState == 0)
    {
        vars.customStage = false;
        vars.customArmed = false;
        vars.customInvalid = false;
        vars.customIndex = 0;
        vars.customEmitted = 0;
        vars.customFinish = false;
    }
    if (old.gameState == 3 && current.gameState == 1)
    {
        vars.customStage = vars.customMap;
        if (vars.customStage)
        {
            vars.customArmed = false;
            vars.customInvalid = false;
            vars.customIndex = 0;
            vars.customEmitted = 0;
            vars.customFinish = false;
        }
    }
    if (vars.customStage && current.gameState == 1 && !vars.customInvalid)
    {
        string[] names = vars.customVariables;
        string[] values = names.Select(name => (string)vars.readCheckpoint(name)).ToArray();
        if (!vars.customArmed)
        {
            // Never count stale 1s when attaching/resetting in the middle of a run.
            vars.customArmed = values.All(value => value == "0");
        }
        else if (values.Any(value => value == null))
        {
            vars.customInvalid = true;
            print("[FDNY CUSTOM] marker unavailable; restart the level");
        }
        else
        {
            int next = vars.customIndex;
            while (next < values.Length && values[next] == "1") next++;
            if (values.Skip(next).Any(value => value == "1"))
            {
                vars.customInvalid = true;
                print("[FDNY CUSTOM] checkpoint out of order; restart the level");
            }
            else vars.customIndex = next;
        }
    }
'''
SPLIT = '''
    if (vars.customInvalid) return false;
    bool finish = ((Func<bool>)(() => {
%s
    }))();
    if (finish && vars.customStage &&
        (!vars.customArmed || vars.customIndex != ((string[])vars.customVariables).Length))
    {
        vars.customInvalid = true;
        print("[FDNY CUSTOM] stage finished with missing checkpoints; restart the level");
        return false;
    }
    vars.customFinish = vars.customFinish || finish;
    // Drain observed checkpoints before a simultaneous stage/win event. No lost split.
    if (vars.customEmitted < vars.customIndex)
    {
        int index = vars.customEmitted;
        print(String.Format("[FDNY CUSTOM SPLIT] {0}", ((string[])vars.customLabels)[index]));
        vars.customEmitted = index + 1;
        return true;
    }
    if (vars.customFinish)
    {
        vars.customFinish = false;
        return true;
    }
    return false;
'''


def action(script, name, transform):
    pattern = r'(?ms)^' + re.escape(name) + r'\n\{\n(.*?)^\}'
    found = list(re.finditer(pattern, script))
    if len(found) != 1:
        raise ValueError('Unsupported base ASL: expected one ' + name + ' block')
    match = found[0]
    return script[:match.start(1)] + transform(match[1]) + script[match.end(1):]


def generate(profile, base):
    if not isinstance(profile, dict) or profile.get('schema') != 'fdny-checkpoints-v1':
        raise ValueError('Expected fdny-checkpoints-v1 profile')
    source, runtime = profile.get('source_map'), profile.get('runtime_map')
    if source not in MAPS or not isinstance(runtime, str) or not re.fullmatch(r'[a-z][a-z0-9_]{0,15}', runtime) or runtime in MAPS:
        raise ValueError('Expected a rescue source map and a distinct custom runtime map')
    checkpoints = profile.get('checkpoints')
    if not isinstance(checkpoints, list) or len(checkpoints) > 32:
        raise ValueError('Expected up to 32 checkpoints')
    names, labels = [], []
    for checkpoint in checkpoints:
        if not isinstance(checkpoint, dict):
            raise ValueError('Invalid checkpoint')
        name, label = checkpoint.get('variable'), checkpoint.get('label')
        if not isinstance(name, str) or not re.fullmatch(r'FDNY_CP_[A-Za-z0-9_]{1,40}', name) or name.casefold() in {n.casefold() for n in names}:
            raise ValueError('Expected unique FDNY_CP_ variable identifiers')
        if checkpoint.get('from_value') != '0' or checkpoint.get('to_value') != '1':
            raise ValueError('Checkpoint markers must change from 0 to 1')
        if not isinstance(label, str) or not 1 <= len(label.strip()) <= 80 or any(ord(c) < 32 for c in label):
            raise ValueError('Expected a nonempty checkpoint label (80 characters max)')
        names.append(name)
        labels.append(label)
    if 'vars.customVariables' in base or 'v2.2' not in base:
        raise ValueError('Expected an unmodified v2.2 stock ASL')
    quote = lambda value: json.dumps(value, ensure_ascii=True)
    config = ('    vars.customVariables = new string[] { ' + ', '.join(map(quote, names)) + ' };\n'
              '    vars.customLabels = new string[] { ' + ', '.join(map(quote, labels)) + ' };\n')
    script = base.replace('    int gameState :', '    int customMissionResult : 0x00103A78, 0x30, 0x3438;\n    int gameState :', 1)
    script = action(script, 'startup', lambda body: config + RESET + body)
    script = action(script, 'init', lambda body: RESET + READER + body)
    map_line = '    vars.map = (current.mapName ?? "").ToLower().Trim();\n'
    alias = ('    vars.customMap = vars.map == ' + quote(runtime) + ';\n'
             '    if (vars.customMap) vars.map = ' + quote(source) + ';\n')
    if script.count(map_line) != 1:
        raise ValueError('Unsupported base ASL map handling')
    script = script.replace(map_line, map_line + alias, 1)
    script = action(script, 'update', lambda body: body + UPDATE)
    win_gate = '    if (current.gameState == 1 && vars.inLevel && !vars.levelDone && won)'
    if script.count(win_gate) != 1:
        raise ValueError('Unsupported base ASL win handling')
    script = script.replace(win_gate, '    // Cloned map names do not have stock end-sound slots.\n'
                            '    if (vars.customStage) won = old.customMissionResult == 0 && current.customMissionResult == 1;\n' + win_gate, 1)
    script = action(script, 'split', lambda body: SPLIT % body)
    script += '\nonReset\n{\n' + RESET + '}\n'
    return '// Generated by livesplit/custom_triggers.py; stock ASLs remain unchanged.\n' + script


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('profile', type=Path, help='exported timing/checkpoints.json')
    parser.add_argument('--mode', choices=['il', 'rescue'], default='il')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    base = ROOT / ('FDNY_ILs.asl' if args.mode == 'il' else 'FDNY_rescue%.asl')
    if args.output.resolve() in {base.resolve(), args.profile.resolve(), (ROOT / 'FDNY_ILs.asl').resolve(), (ROOT / 'FDNY_rescue%.asl').resolve()}:
        parser.error('Output must not overwrite the profile or a stock ASL')
    try:
        result = generate(json.loads(args.profile.read_text()), base.read_text())
        with args.output.open('x', encoding='utf-8', newline='\n') as output:
            output.write(result)
    except (OSError, ValueError, TypeError) as error:
        parser.exit(1, str(error) + '\n')
    print('Created ' + str(args.output) + '; select this ASL in LiveSplit and use Game Time.')


if __name__ == '__main__':
    main()
