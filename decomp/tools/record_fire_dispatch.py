#!/usr/bin/env python3
"""Record original Fire Think caller, frame callbacks and shared RNG entries.

Attach to the matching game at its main menu, then enter Training manually.
WineDbg stops at fixed, hash-checked function entries and records their return
addresses, registers and stack words. It does not send input or alter game
memory. Breakpoint stops perturb frame timing; this capture establishes call
identity/order only, not normal-speed cadence.
"""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

from pe_constants import binary, offset
from record_startup import Debugger, EXPECTED_EXE, register

ROOT = Path(__file__).resolve().parents[1]
POINTS = {
    0x00449FE0: 'fire_think',
    0x004483A0: 'fire_player_influence',
    0x00429520: 'owner_update',
    0x0044F340: 'player_view',
    0x00461F80: 'extinguisher_post_view',
    0x0049B610: 'mapped_message_drain',
    0x00453A70: 'player_message',
    0x00451640: 'player_fire_exposure',
    0x00450FB0: 'player_fire_damage',
    0x00404AC0: 'rng_float',
    0x00404B30: 'rng_bounded',
    0x00498B30: 'rng_range',
    0x00404960: 'rng_reload',
}
OBSERVER_POINTS = {address: name for address, name in POINTS.items()
                   if address != 0x00449FE0}
FIRE_VTABLE = 0x004E1774
FIRE_PRIMARY_VTABLE = 0x004E1780


def app_files(app):
    contents = app.resolve() / 'Contents'
    prefix = contents / 'SharedSupport/prefix'
    game = prefix / 'drive_c/Program Files/FireFighter American Hero'
    return contents, prefix, game, contents / 'SharedSupport/wine/bin/wine', \
        contents / 'SharedSupport/wine/lib/wine/x86_64-windows/winedbg.exe'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app', type=Path,
                        default=Path.home() / 'Applications/Sikarugir/FDNYFirefighterFOVFix.app')
    parser.add_argument('--pid', help='Wine Windows process ID in hexadecimal; auto-detected when unique')
    parser.add_argument('--max-think-calls', type=int, default=28,
                        help='Detach after this many Fire Think entries')
    parser.add_argument('--check-only', action='store_true', help='Check files and hashes without attaching')
    args = parser.parse_args()
    if args.max_think_calls < 1:
        parser.error('--max-think-calls must be positive')

    contents, prefix, game, wine, debugger = app_files(args.app)
    executable = game / 'FDNYFirefighter.exe'
    for path in (wine, debugger, executable):
        if not path.is_file():
            parser.error(f'Missing: {path}')
    hashed = {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
              for path in sorted(game.iterdir())
              if path.is_file() and path.suffix.lower() in ['.exe', '.dll', '.asi', '.ini']}
    if hashed.get('FDNYFirefighter.exe') != EXPECTED_EXE:
        parser.error('Executable differs from the analyzed version; refusing fixed-address breakpoints.')
    print('Executable matches. Bundled Wine debugger found.')
    if args.check_only:
        print(json.dumps(hashed, indent=2))
        return

    env = os.environ.copy()
    env.update(WINEPREFIX=str(prefix), WINEDEBUG='-all', WINEMSYNC='1',
               CX_ROOT=str(contents / 'SharedSupport/wine'), WINELOADER=str(wine),
               DYLD_FALLBACK_LIBRARY_PATH=str(contents / 'Frameworks') + ':/usr/lib')
    if args.pid:
        pid = int(args.pid, 16)
    else:
        listing_result = subprocess.run([str(wine), str(debugger), '--command', 'info proc'],
                                         env=env, text=True, stdout=subprocess.PIPE,
                                         stderr=subprocess.STDOUT, timeout=30, check=False)
        listing = listing_result.stdout
        if listing_result.returncode:
            parser.error('WineDbg could not list running game processes.\n' + listing)
        matches = re.findall(r'^\s*[=>]?([0-9a-fA-F]+)\s+\d+[^\n]*FDNYFirefighter\.exe',
                             listing, re.M | re.I)
        if len(matches) != 1:
            parser.error('Open the matching game at its main menu first, or pass --pid.\n' + listing)
        pid = int(matches[0], 16)

    output = ROOT / 'evidence/live' / datetime.datetime.now().strftime('fire-dispatch-%Y%m%d-%H%M%S-%f')
    output.mkdir(parents=True)
    manifest = {
        'game_directory': str(game), 'file_sha256': hashed, 'windows_pid': pid,
        'method': 'WineDbg entry breakpoints; EIP/ECX/ESP, return addresses and stack words',
        'timing': 'Breakpoints perturb timing; events establish call identity/order only.',
        'scope': ('Wait for Fire Think in Training, then record nearby owner/player/tool/queue '
                  'entries and shared RNG function entry/callsite order.'),
        'points': {f'0x{address:08x}': name for address, name in POINTS.items()},
        'fire_vtable_expected': f'0x{FIRE_VTABLE:08x}',
        'fire_primary_vtable_expected': f'0x{FIRE_PRIMARY_VTABLE:08x}',
    }
    (output / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    counts = {name: 0 for name in POINTS.values()}
    observers_installed = False
    sequence = 0
    with (output / 'debugger.txt').open('w') as transcript, \
            (output / 'events.jsonl').open('w') as events:
        dbg = Debugger([str(wine), str(debugger), str(pid)], env, transcript)
        try:
            dbg.prompt()
            modules = dbg.command('info share')
            (output / 'modules.txt').write_text(modules)
            for address in POINTS:
                actual = dbg.words(address, 2)
                expected = int.from_bytes(binary[offset(address):offset(address) + 4], 'little')
                if actual[0] != expected:
                    raise RuntimeError(f'Live code at {address:08x} differs from analyzed code.')
            result = dbg.command('break *0x449fe0')
            if not re.search(r'Breakpoint\s+\d+', result, re.I):
                raise RuntimeError('Could not confirm Fire Think breakpoint. See transcript.')
            print('Recording Fire Think calls. Enter Training in the game; the recorder detaches automatically.')
            while counts['fire_think'] < args.max_think_calls:
                dbg.command('cont', 600)
                regs = dbg.command('info reg')
                pc = register(regs, 'EIP')
                if pc not in POINTS:
                    raise RuntimeError(f'Unexpected debugger stop at {pc:08x}; no automatic exception bypass.')
                if pc != 0x00449FE0 and not observers_installed:
                    raise RuntimeError('Observer breakpoint fired before Fire Think initialization.')
                if pc == 0x00449FE0 and not observers_installed:
                    for address in OBSERVER_POINTS:
                        result = dbg.command(f'break *0x{address:x}')
                        if not re.search(r'Breakpoint\s+\d+', result, re.I):
                            raise RuntimeError(f'Could not install {OBSERVER_POINTS[address]} breakpoint.')
                    observers_installed = True
                esp = register(regs, 'ESP')
                ecx = register(regs, 'ECX')
                stack = dbg.words(esp, 12)
                receiver_vptr = dbg.words(ecx, 1)[0] if pc in {
                    0x00449FE0, 0x00429520, 0x0044F340, 0x00461F80,
                    0x00453A70, 0x00451640, 0x00450FB0, 0x004483A0
                } else None
                kind = POINTS[pc]
                record = {
                    'sequence': sequence, 'kind': kind, 'pc': f'0x{pc:08x}',
                    'caller': f'0x{stack[0]:08x}', 'ecx': f'0x{ecx:08x}',
                    'receiver_vptr': None if receiver_vptr is None else f'0x{receiver_vptr:08x}',
                    'stack': [f'0x{word:08x}' for word in stack],
                    'registers': regs, 'host_time_ns': time.monotonic_ns(),
                }
                if pc == 0x00453A70:
                    record['message_id'] = f'0x{stack[1]:08x}'
                    if stack[1] == 0x7531 and stack[2]:
                        record['fire_exposure_payload'] = [
                            f'0x{word:08x}' for word in dbg.words(stack[2], 2)
                        ]
                elif pc == 0x004483A0:
                    record['target'] = f'0x{stack[1]:08x}'
                    record['target_vptr'] = (
                        f'0x{dbg.words(stack[1], 1)[0]:08x}' if stack[1] else None
                    )
                    record['fire_exposure_slots_used'] = dbg.words(ecx + 0xb8, 1)[0]
                elif pc == 0x00451640:
                    record['origin_words'] = [
                        f'0x{word:08x}' for word in dbg.words(stack[1], 3)
                    ]
                    record['influence_bits'] = f'0x{stack[2]:08x}'
                    record['game_clock'] = f'0x{dbg.words(0x00502AFC, 1)[0]:08x}'
                    record['pain_deadline_before'] = f'0x{dbg.words(ecx + 0x390, 1)[0]:08x}'
                elif pc == 0x00450FB0:
                    record['damage_kind'] = stack[1]
                elif pc == 0x00449FE0:
                    record['fire_exposure_slots_before_reset'] = dbg.words(ecx + 0x94, 1)[0]
                events.write(json.dumps(record, separators=(',', ':')) + '\n')
                events.flush()
                sequence += 1
                counts[kind] += 1
                if kind == 'fire_think' and receiver_vptr != FIRE_VTABLE:
                    print(f'Fire Think receiver vptr was 0x{receiver_vptr:08x} (expected 0x{FIRE_VTABLE:08x}).')
                if kind == 'fire_player_influence' and receiver_vptr != FIRE_PRIMARY_VTABLE:
                    print(f'Fire influence receiver vptr was 0x{receiver_vptr:08x} '
                          f'(expected 0x{FIRE_PRIMARY_VTABLE:08x}).')
                if sequence % 25 == 0:
                    print(f"Recorded {counts['fire_think']} Fire Think entries and {sequence} total events.")
            events.write(json.dumps({'kind': 'capture_end', 'reason': 'think_limit',
                                     'counts': counts}, separators=(',', ':')) + '\n')
            events.flush()
            print(f'Saved {sequence} ordered events to {output}; detaching.')
        finally:
            dbg.cleanup()


if __name__ == '__main__':
    main()
