#!/bin/zsh
PROJECT_ROOT="${0:A:h}"
cd "$PROJECT_ROOT" || exit 1
APP_DIR="$PROJECT_ROOT/.local/FDNY Scene Preview.app"

if (( $# > 0 )); then
  SCENE_KEY="$1"
else
  MAP_ITEMS=()
  for package_dir in "$PROJECT_ROOT"/.local/exports/*(/N); do
    map_key="${package_dir:t}"
    [[ "$map_key" == *'"'* ]] && continue
    MAP_ITEMS+=("\"$map_key\"")
  done
  if (( ${#MAP_ITEMS} == 0 )); then
    print "No exported BSP scene packages found under .local/exports."
    print "Export a map first with tools/export_gbsp_scene.py."
    read -k 1 "REPLY?Press any key to close..."
    exit 1
  fi
  APPLE_LIST="{${(j:, :)MAP_ITEMS}}"
  SCENE_KEY="$(/usr/bin/osascript \
    -e "set picked to choose from list $APPLE_LIST with title \"FDNY Scene Preview\" with prompt \"Choose a BSP level to load\"" \
    -e 'if picked is false then return ""' \
    -e 'return item 1 of picked')"
  [[ -n "$SCENE_KEY" ]] || exit 0
fi

PACKAGE_DIR=".local/exports/$SCENE_KEY"
if [[ ! -d "$PACKAGE_DIR" ]]; then
  print "Scene package not found: $PACKAGE_DIR"
  print "Export a BSP package first with tools/export_gbsp_scene.py."
  read -k 1 "REPLY?Press any key to close..."
  exit 1
fi

tools/build_scene_viewer_app.sh "$SCENE_KEY" || exit 1
exec "$APP_DIR/Contents/MacOS/fdny-scene-viewer" "$APP_DIR/Contents/Resources/scene"
