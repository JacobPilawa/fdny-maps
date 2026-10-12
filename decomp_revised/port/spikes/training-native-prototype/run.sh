#!/bin/sh
set -eu
cd "$(dirname "$0")"
cmake -S . -B build
cmake --build build -j 4
exec ./build/fdny-training "$@"
