#!/bin/zsh
PROJECT_ROOT="${0:A:h}"
cd "$PROJECT_ROOT" || exit 1
SCENE_KEY="${1:-mbase}"
BUILD_DIR="port/spikes/training-native-prototype/.local/build-scene-viewer"
PACKAGE_DIR=".local/exports/$SCENE_KEY"

if [[ ! -d "$PACKAGE_DIR" ]]; then
  print "Scene package not found: $PACKAGE_DIR"
  print "Export a BSP package first with tools/export_gbsp_scene.py."
  read -k 1 "REPLY?Press any key to close..."
  exit 1
fi

cmake -S port/spikes/training-native-prototype -B "$BUILD_DIR" || exit 1
cmake --build "$BUILD_DIR" --target fdny-scene-viewer -j 4 || exit 1
exec "$BUILD_DIR/fdny-scene-viewer" "$PACKAGE_DIR"
