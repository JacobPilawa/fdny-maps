#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
GHIDRA_HOME=${GHIDRA_HOME:-/Applications/ghidra}
JAVA_HOME=${JAVA_HOME:-/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home}
export JAVA_HOME
export PATH="$JAVA_HOME/bin:$PATH"
export JAVA_TOOL_OPTIONS="-Duser.home=$ROOT_DIR/.local/ghidra-user-home -Djava.awt.headless=true"

exec "$GHIDRA_HOME/support/launch.sh" fg jdk FDNY-GhidraMCP '' '' \
    com.xebyte.headless.GhidraMCPHeadlessServer \
    --project "$ROOT_DIR/analysis/ghidra/FDNYPort.gpr" \
    --program /FDNYFirefighter.exe \
    --port 8089 --bind 127.0.0.1
