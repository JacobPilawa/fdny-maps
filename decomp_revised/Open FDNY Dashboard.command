#!/bin/zsh
set -eu

ROOT_DIR=$(cd -- "$(dirname -- "$0")" && pwd)
URL="http://127.0.0.1:8765/dashboard/"
LOG_DIR="$ROOT_DIR/.local"
LOG_PATH="$LOG_DIR/dashboard-server.log"

if /usr/bin/curl --silent --fail --max-time 2 "$URL" >/dev/null; then
  /usr/bin/open "$URL"
  exit 0
fi

mkdir -p "$LOG_DIR"
cd "$ROOT_DIR"
nohup /usr/bin/python3 tools/serve_dashboard.py --no-open >"$LOG_PATH" 2>&1 </dev/null &

for attempt in {1..15}; do
  if /usr/bin/curl --silent --fail --max-time 2 "$URL" >/dev/null; then
    /usr/bin/open "$URL"
    exit 0
  fi
  sleep 1
done

print -u2 "Could not start the FDNY dashboard. See $LOG_PATH"
exit 1
