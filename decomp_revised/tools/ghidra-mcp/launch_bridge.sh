#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
export UV_CACHE_DIR="$ROOT_DIR/.local/uv-cache"
exec /Users/jacobpilawa/.local/bin/uv run --script "$ROOT_DIR/tools/ghidra-mcp/bridge_mcp_ghidra.py" --transport stdio
