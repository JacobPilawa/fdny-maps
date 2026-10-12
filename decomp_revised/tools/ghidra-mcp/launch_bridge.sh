#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
UV_BIN=${UV_BIN:-$(command -v uv || true)}
if [ -z "$UV_BIN" ]; then
    echo "uv is required; install it or set UV_BIN to its path." >&2
    exit 127
fi
exec "$UV_BIN" run --script "$ROOT_DIR/tools/ghidra-mcp/bridge_mcp_ghidra.py" --transport stdio
