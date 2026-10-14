#!/usr/bin/env python3
"""Mirror this workspace into fdny-maps/decomp_revised without deleting remote files."""
import argparse
import pathlib
import shutil
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
URL = 'https://github.com/JacobPilawa/fdny-maps.git'
PREFIX = 'decomp_revised'


def git(*args, cwd=ROOT, capture=False):
    return subprocess.run(['git', *args], cwd=cwd, check=True,
                          stdout=subprocess.PIPE if capture else None).stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--push', action='store_true', help='Commit and push to GitHub main; otherwise only compare.')
    parser.add_argument('--message', default='Sync local decompilation work')
    args = parser.parse_args()
    names = set(git('ls-files', '--cached', '--others', '--exclude-standard', '-z', capture=True).decode().split('\0')) - {''}
    paths = [ROOT / name for name in sorted(names)]
    missing = [p.relative_to(ROOT).as_posix() for p in paths if not p.exists()]
    if missing:
        raise SystemExit('Tracked files missing locally; resolve before syncing: ' + ', '.join(missing))
    with tempfile.TemporaryDirectory(prefix='fdny-github-sync-') as temporary:
        repo = pathlib.Path(temporary)
        git('clone', '--single-branch', '--branch', 'main', URL, str(repo))
        destination = repo / PREFIX
        remote_names = set(git('ls-files', '-z', '--', PREFIX, cwd=repo, capture=True).decode().split('\0')) - {''}
        remote_only = sorted(name for name in remote_names if name.removeprefix(PREFIX + '/') not in names)
        if remote_only:
            raise SystemExit('GitHub has files outside the local publishable set; reconcile before syncing: ' + ', '.join(remote_only))
        for source in paths:
            target = destination / source.relative_to(ROOT)
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, target)
        git('add', '--', PREFIX, cwd=repo)
        git('diff', '--cached', '--stat', cwd=repo)
        changed = git('diff', '--cached', '--name-only', cwd=repo, capture=True)
        if not changed:
            print('GitHub already matches the local publishable files.')
            return
        if not args.push:
            print('Comparison only. Run again with --push to publish these changes.')
            return
        git('commit', '-m', args.message, cwd=repo)
        git('push', 'origin', 'HEAD:main', cwd=repo)
        for source in paths:
            if source.read_bytes() != (destination / source.relative_to(ROOT)).read_bytes():
                raise SystemExit('Local files changed during sync. Run the sync again.')
        head = git('rev-parse', 'HEAD', cwd=repo, capture=True).decode().strip()
        remote = git('ls-remote', 'origin', 'refs/heads/main', cwd=repo, capture=True).decode().split()[0]
        if remote != head:
            raise SystemExit('GitHub main moved during verification. Run comparison again.')
        print(f'Verified GitHub main at {head} ({len(paths)} project files).')


if __name__ == '__main__':
    main()
