#!/bin/zsh
PROJECT_ROOT="${0:A:h}"
cd "$PROJECT_ROOT" || exit 1

if (( $# > 0 )); then
  SCENE_KEY="$1"
  tools/build_scene_viewer_app.sh "$SCENE_KEY" || exit 1
  exec "$PROJECT_ROOT/.local/FDNY Firefighters.app/Contents/MacOS/fdny-scene-viewer" \
    "$PROJECT_ROOT/.local/FDNY Firefighters.app/Contents/Resources/scene"
fi

tools/build_scene_viewer_app.sh rescue-all || exit 1
exec "$PROJECT_ROOT/.local/FDNY Firefighters.app/Contents/MacOS/fdny-scene-viewer"
