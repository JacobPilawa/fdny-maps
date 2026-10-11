#!/usr/bin/env python3
"""Attach to the user's running FDNY Wine game and record startup RNG states.

Uses WineDbg's documented breakpoint, register and memory commands. Captures
actual pre-call memory, not predicted random results. Never edits game files.
This debugger recorder changes timing; it is for startup-order verification.
"""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import pty
import re
import select
import subprocess
import termios
import time

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_APP = Path.home() / 'Applications/Sikarugir/FDNYFirefighterFOVFix.app'
EXPECTED_EXE = '4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e'
POINTS = {
    0x404930: 'seed_internal', 0x404B50: 'seed_public',
    0x404A70: 'raw_draw', 0x404AC0: 'float_draw',
    0x42AB00: 'level_load', 0x4A0D60: 'resume_sound',
}


def memory_words(output, count):
    values = []
    for line in output.splitlines():
        # Wine's x/Nx prints address: followed by four 32-bit words per line.
        match = re.match(r'\s*(?:0x)?[0-9a-fA-F]{8,16}:\s*(.*)', line)
        if match:
            values.extend(int(value, 16) for value in
                          re.findall(r'(?<![\w])(?:0x)?([0-9a-fA-F]{8})(?![\w])', match[1]))
    if len(values) != count:
        raise RuntimeError(f'Expected {count} memory words, got {len(values)}. See transcript.')
    return values


def register(output, name):
    match = re.search(r'\b' + name + r'\s*[:=]\s*(?:0x)?([0-9a-fA-F]+)', output, re.I)
    if not match:
        raise RuntimeError(f'Cannot read {name} from debugger registers. See transcript.')
    return int(match[1], 16)


class Debugger:
    def __init__(self, command, env, transcript):
        self.master, slave = pty.openpty()
        attrs = termios.tcgetattr(slave)
        # Wine's Windows console needs CR as Enter. Keep the pseudo-terminal
        # from converting CR to LF or holding it for canonical line input.
        attrs[0] &= ~(termios.ICRNL | getattr(termios, 'INLCR', 0) |
                      getattr(termios, 'IGNCR', 0))
        attrs[3] &= ~(termios.ECHO | termios.ICANON)
        termios.tcsetattr(slave, termios.TCSANOW, attrs)
        self.process = subprocess.Popen(command, env=env, stdin=slave, stdout=slave,
                                        stderr=slave, start_new_session=True)
        os.close(slave)
        self.buffer = b''
        self.transcript = transcript
        self.stopped = False

    def prompt(self, timeout=60):
        deadline = time.monotonic() + timeout
        while True:
            match = re.search(rb'Wine-dbg>\s*', self.buffer)
            if match:
                result = self.buffer[:match.end()].decode('utf-8', errors='replace')
                self.buffer = self.buffer[match.end():]
                self.stopped = True
                return result
            if time.monotonic() >= deadline:
                raise TimeoutError('Debugger did not reach a prompt. See transcript.')
            if select.select([self.master], [], [], 0.25)[0]:
                try:
                    chunk = os.read(self.master, 65536)
                except OSError as error:
                    raise RuntimeError('Wine debugger exited. See transcript.') from error
                if not chunk:
                    raise RuntimeError('Wine debugger closed its output. See transcript.')
                self.transcript.write(chunk.decode('utf-8', errors='replace'))
                self.transcript.flush()
                self.buffer += chunk

    def command(self, command, timeout=60):
        self.transcript.write('\n>>> ' + command + '\n')
        self.transcript.flush()
        self.stopped = False
        # WineDbg's Windows console on macOS displayed LF input as a literal
        # ^J and left the command pending; carriage return submits a command.
        os.write(self.master, (command + '\r').encode())
        return self.prompt(timeout)

    def words(self, address, count):
        # count=2 also supplies an address header; x/1x does not in WineDbg.
        text = self.command(f'x/{max(count, 2)}x 0x{address:x}')
        return memory_words(text, max(count, 2))[:count]

    def cleanup(self):
        # Never kill the game or its Wine server. Detach removes breakpoints.
        try:
            if not self.stopped:
                self.prompt(15)
            self.command('detach', 15)
            os.write(self.master, b'quit\n')
            self.process.wait(timeout=15)
        except Exception:
            print('Automatic detach could not be confirmed. Close the game normally; '
                  'the transcript contains the last debugger state.')
        finally:
            os.close(self.master)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app', type=Path, default=DEFAULT_APP)
    parser.add_argument('--pid', help='Wine Windows process ID, hexadecimal; usually auto-detected')
    parser.add_argument('--check-only', action='store_true', help='Check files without starting Wine')
    parser.add_argument('--max-draws', type=int, default=400, help='Safety limit after level-load entry')
    parser.add_argument('--after-resume', type=int, default=20, help='Draws to record after sound resumes')
    args = parser.parse_args()
    if args.max_draws < 1 or args.after_resume < 1:
        parser.error('Draw limits must be positive.')
    contents = args.app.resolve() / 'Contents'
    prefix = contents / 'SharedSupport/prefix'
    game = prefix / 'drive_c/Program Files/FireFighter American Hero'
    wine = contents / 'SharedSupport/wine/bin/wine'
    debugger = contents / 'SharedSupport/wine/lib/wine/x86_64-windows/winedbg.exe'
    for path in [wine, debugger, game / 'FDNYFirefighter.exe']:
        if not path.is_file():
            parser.error(f'Missing: {path}')
    hashes = {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
              for path in sorted(game.iterdir())
              if path.is_file() and path.suffix.lower() in ['.exe', '.dll', '.asi', '.ini']}
    if hashes['FDNYFirefighter.exe'] != EXPECTED_EXE:
        parser.error('Executable differs from the analyzed version; refusing fixed-address breakpoints.')
    print('Executable matches. Bundled Wine debugger found.')
    if args.check_only:
        print(json.dumps(hashes, indent=2))
        return
    env = os.environ.copy()
    env.update(WINEPREFIX=str(prefix), WINEDEBUG='-all', WINEMSYNC='1',
               CX_ROOT=str(contents / 'SharedSupport/wine'), WINELOADER=str(wine),
               DYLD_FALLBACK_LIBRARY_PATH=str(contents / 'Frameworks') + ':/usr/lib')
    if args.pid:
        pid = int(args.pid, 16)
    else:
        listing = subprocess.run([str(wine), str(debugger), '--command', 'info proc'],
                                 env=env, text=True, stdout=subprocess.PIPE,
                                 stderr=subprocess.STDOUT, timeout=30, check=True).stdout
        matches = re.findall(r'^\s*[=>]?([0-9a-fA-F]+)\s+\d+[^\n]*FDNYFirefighter\.exe',
                             listing, re.M | re.I)
        if len(matches) != 1:
            parser.error('Open your usual game launcher and leave it at the main menu first.\n' + listing)
        pid = int(matches[0], 16)
    output = ROOT / 'evidence/live' / datetime.datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    output.mkdir(parents=True)
    manifest = {'game_directory': str(game), 'file_sha256': hashes, 'windows_pid': pid,
                'method': 'WineDbg entry breakpoints; actual pre-call RNG memory and stack',
                'timing': 'Debugger perturbs timing. No frame/input replay in this startup recorder.',
                'scope': 'Attach at main menu; next level startup plus limited following draws.'}
    (output / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    with (output / 'debugger.txt').open('w') as transcript, (output / 'events.jsonl').open('w') as events:
        dbg = Debugger([str(wine), str(debugger), str(pid)], env, transcript)
        try:
            dbg.prompt()
            modules = dbg.command('info share')
            # Record module layout; disk hash guards addresses, live entry bytes below
            # additionally reject a runtime mod that replaced any traced entry.
            (output / 'modules.txt').write_text(modules)
            from pe_constants import offset, binary
            for address in POINTS:
                native = dbg.words(address, 2)
                expected = int.from_bytes(binary[offset(address):offset(address)+4], 'little')
                if native[0] != expected:
                    raise RuntimeError(f'Live code at {address:08x} differs from analyzed code.')
            # Snapshot before installing any breakpoints.
            def state():
                remaining = dbg.words(0x4FF370, 1)[0]
                pointer = dbg.words(0x5023EC, 1)[0]
                words = dbg.words(0x501A28, 624)
                if remaining > 624:
                    raise RuntimeError('Invalid RNG remaining count.')
                index = 624 - remaining
                if remaining and pointer != 0x501A28 + index * 4:
                    raise RuntimeError('RNG pointer and remaining count disagree.')
                return {'words': words, 'remaining': remaining, 'next_index': index,
                        'next_pointer': pointer}
            def save(record):
                events.write(json.dumps(record, separators=(',', ':')) + '\n')
                events.flush()
            save({'kind': 'attached', 'state': state()})
            for address in POINTS:
                result = dbg.command(f'break *0x{address:x}')
                if not re.search(r'Breakpoint\s+\d+', result, re.I):
                    raise RuntimeError('Could not confirm breakpoint installation. See transcript.')
            print('Recording. Now enter Training. Startup may be slow.\n'
                  'The recorder detaches automatically after startup.\nSaved to: ' + str(output))
            loaded = False
            resumed = False
            draws = after = sequence = 0
            while True:
                dbg.command('cont', 600)
                regs = dbg.command('info reg')
                pc = register(regs, 'EIP')
                if pc not in POINTS:
                    raise RuntimeError(f'Unexpected debugger stop at {pc:08x}; no automatic exception bypass.')
                stack = dbg.words(register(regs, 'ESP'), 16)
                kind = POINTS[pc]
                save({'sequence': sequence, 'kind': kind, 'pc': pc, 'caller': stack[0],
                      'stack': stack, 'registers': regs, 'state': state(),
                      'host_time_ns': time.monotonic_ns()})
                sequence += 1
                if kind == 'level_load':
                    loaded = True
                    print('Level loading captured.')
                elif kind == 'resume_sound' and loaded:
                    resumed = True
                    print('Queued sound resume captured.')
                elif kind in ['raw_draw', 'float_draw'] and loaded:
                    draws += 1
                    after += int(resumed)
                    if draws >= args.max_draws or after >= args.after_resume:
                        save({'kind': 'capture_end', 'reason': 'after_resume_limit' if after >= args.after_resume
                              else 'draw_safety_limit', 'draws_since_load': draws,
                              'draws_since_resume': after, 'state': state()})
                        break
            print(f'Saved {sequence} entry events. Detaching; continue playing normally.')
        finally:
            dbg.cleanup()


if __name__ == '__main__':
    main()
