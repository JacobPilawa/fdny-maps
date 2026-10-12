#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
GHIDRA_HOME=${GHIDRA_HOME:-/Applications/ghidra}
mkdir -p "$ROOT_DIR/analysis/ghidra" "$ROOT_DIR/tracker/generated" "$ROOT_DIR/logs"
python3 "$ROOT_DIR/tools/build_input_manifest.py"
export JAVA_TOOL_OPTIONS="-Duser.home=$ROOT_DIR/.local/ghidra-user-home"

"$GHIDRA_HOME/support/analyzeHeadless" \
    "$ROOT_DIR/analysis/ghidra" FDNYPort \
    -import \
    "$ROOT_DIR/inputs/original/binaries/FDNYFirefighter.exe" \
    "$ROOT_DIR/inputs/original/binaries/Genesis.dll" \
    "$ROOT_DIR/inputs/original/binaries/D3DDrv.dll" \
    "$ROOT_DIR/inputs/original/binaries/fmod.dll" \
    -overwrite \
    -scriptPath "$ROOT_DIR/tools/ghidra-scripts" \
    -postScript ExportFunctionInventory.java "$ROOT_DIR/tracker/generated" \
    -log "$ROOT_DIR/logs/ghidra-analysis.log" \
    -scriptlog "$ROOT_DIR/logs/ghidra-scripts.log" \
    -analysisTimeoutPerFile 600 \
    -max-cpu 4

python3 "$ROOT_DIR/tools/inspect_binaries.py"
python3 "$ROOT_DIR/tools/build_tracker.py"
