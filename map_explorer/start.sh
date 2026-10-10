#!/bin/sh
set -eu
cd "$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
exec python3 serve.py "${FDNY_MAP_PORT:-4180}"
