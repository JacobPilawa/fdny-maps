#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
GHIDRA_HOME=${GHIDRA_HOME:-/Applications/ghidra}
JAVA_HOME=${JAVA_HOME:-/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home}
export JAVA_HOME
export PATH="$JAVA_HOME/bin:$PATH"
export JAVA_TOOL_OPTIONS="-Duser.home=$ROOT_DIR/.local/ghidra-user-home"
mkdir -p "$ROOT_DIR/tracker/generated" "$ROOT_DIR/logs"

for PROGRAM in FDNYFirefighter.exe Genesis.dll D3DDrv.dll fmod.dll; do
    "$GHIDRA_HOME/support/analyzeHeadless" \
        "$ROOT_DIR/analysis/ghidra" FDNYPort \
        -process "$PROGRAM" -noanalysis \
        -scriptPath "$ROOT_DIR/tools/ghidra-scripts" \
        -postScript ExportFunctionInventory.java "$ROOT_DIR/tracker/generated" \
        -log "$ROOT_DIR/logs/ghidra-export-refresh.log" \
        -scriptlog "$ROOT_DIR/logs/ghidra-scripts.log"
done

python3 "$ROOT_DIR/tools/inspect_binaries.py"
python3 "$ROOT_DIR/tools/build_tracker.py"
