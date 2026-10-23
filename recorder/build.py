#!/usr/bin/env python3
"""Cross-compile the 32-bit Windows ASI with Zig (tested with 0.14.1)."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--zig', default=os.environ.get('ZIG', 'zig'), help='Zig executable or path')
    parser.add_argument('--output-dir', type=Path)
    parser.add_argument('--selftest', action='store_true', help='Also build the synthetic test executable')
    args = parser.parse_args()
    root = Path(__file__).resolve().parent
    compiler = shutil.which(args.zig)
    if not compiler:
        parser.error('Zig not found. Install Zig 0.14.1 and put it on PATH, or pass --zig.')
    output = args.output_dir.expanduser().resolve() if args.output_dir else root / 'build'
    output.mkdir(parents=True, exist_ok=True)
    version = subprocess.check_output([compiler, 'version'], text=True).strip()
    print(f'Building with Zig {version}')
    if version != '0.14.1':
        print('Note: the reference build uses Zig 0.14.1.')
    flags = [compiler, 'cc', '-target', 'x86-windows-gnu', '-O2', '-g0', '-s',
             '-Wall', '-Wextra', '-ffp-contract=off']
    subprocess.run(flags + ['-shared', 'FDNYRecorder.c', '-o', str(output / 'FDNYRecorder.asi'),
                            '-luser32', '-lkernel32'], cwd=root, check=True)
    if args.selftest:
        subprocess.run(flags + ['-Wno-unused-function', 'selftest.c', '-o', str(output / 'recorder-selftest.exe'),
                                '-luser32', '-lkernel32'], cwd=root, check=True)
    print(f'Output: {output}')


if __name__ == '__main__':
    main()
