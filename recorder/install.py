#!/usr/bin/env python3
"""Install FDNYRecorder into a supplied native Windows or Wine game directory."""
import argparse
from datetime import datetime, timezone
import hashlib
import os
from pathlib import Path
import shutil
import struct

SUPPORTED_EXE_HASHES = {
    '4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e',
    'bf82c9c0abfa6e3e951875e8e266d63b2e8918bafc3277d09903dcea5517e5f7',
}


def validate_plugin(path):
    data = path.read_bytes()
    if len(data) < 64 or data[:2] != b'MZ':
        raise ValueError('Plugin is not a Windows PE file.')
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    if pe + 24 > len(data) or data[pe:pe + 4] != b'PE\0\0':
        raise ValueError('Invalid PE header.')
    if struct.unpack_from('<H', data, pe + 4)[0] != 0x14c or not struct.unpack_from('<H', data, pe + 22)[0] & 0x2000:
        raise ValueError('Expected a 32-bit x86 Windows DLL/ASI.')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('game_dir', type=Path, help='Folder containing FDNYFirefighter.exe')
    parser.add_argument('--plugin', type=Path, default=Path(__file__).resolve().parent / 'bin/FDNYRecorder.asi')
    parser.add_argument('--output-dir', type=Path, help='Host output folder; defaults to GAME_DIR/Recordings')
    parser.add_argument('--windows-output', help='Override the output path as seen by Windows/Wine')
    parser.add_argument('--no-victims', action='store_true')
    parser.add_argument('--check-only', action='store_true', help='Validate and show the installation without writing files')
    args = parser.parse_args()
    game = args.game_dir.expanduser().resolve()
    output = (args.output_dir.expanduser().resolve() if args.output_dir else game / 'Recordings')
    try:
        digest = hashlib.sha256((game / 'FDNYFirefighter.exe').read_bytes()).hexdigest()
        if digest not in SUPPORTED_EXE_HASHES:
            raise ValueError('Unrecognized game executable. See COMPATIBILITY.md before adapting to another build.')
        validate_plugin(args.plugin)
    except (OSError, ValueError) as error:
        parser.error(str(error))
    windows_output = args.windows_output or (str(output) if os.name == 'nt' else 'Z:' + output.as_posix().replace('/', '\\'))
    if any(c in windows_output for c in '\r\n'):
        parser.error('The output path must be a single INI value.')
    try:
        windows_output.encode('ascii')
    except UnicodeEncodeError:
        parser.error('This plugin uses Win32 ANSI file APIs; choose an ASCII output path with --windows-output.')
    print(f'Game: {game}\nExecutable SHA-256: {digest}\nRecordings (host): {output}\nRecordings (Windows/Wine): {windows_output}')
    if args.check_only:
        print('Checks passed; no files written.')
        return
    output.mkdir(parents=True, exist_ok=True)
    targets = [game / 'FDNYRecorder.asi', game / 'FDNYRecorder.ini']
    existing = [p for p in targets if p.exists()]
    if existing:
        stamp = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S.%fZ')
        backup = game / '.fdny-recorder-backups' / stamp
        backup.mkdir(parents=True)
        for path in existing:
            shutil.copy2(path, backup / path.name)
    shutil.copy2(args.plugin, targets[0])
    targets[1].write_text('[Recorder]\nEnabled=1\nRecordVictims=' + ('0' if args.no_victims else '1') +
                          '\nOutputDirectory=' + windows_output + '\n', encoding='ascii')
    print('Installed. An ASI loader is required; restart the game to load the recorder.')


if __name__ == '__main__':
    main()
