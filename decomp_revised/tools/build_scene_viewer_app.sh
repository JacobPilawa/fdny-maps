#!/bin/sh
set -eu

PROJECT_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
SCENE_KEY=${1:-mbase}
BUILD_DIR="$PROJECT_ROOT/port/spikes/training-native-prototype/.local/build-scene-viewer"
PACKAGE_DIR="$PROJECT_ROOT/.local/exports/$SCENE_KEY"
ACTOR_ASSETS_DIR="$PROJECT_ROOT/.local/actor_exports"
APP_DIR="$PROJECT_ROOT/.local/FDNY Scene Preview.app"

if [ ! -d "$PACKAGE_DIR" ]; then
    printf 'Scene package not found: %s\n' "$PACKAGE_DIR" >&2
    printf 'Export it first with tools/export_gbsp_scene.py.\n' >&2
    exit 1
fi

if [ ! -f "$ACTOR_ASSETS_DIR/vic_man_0.bmp" ]; then
    python3 "$PROJECT_ROOT/tools/export_victim_actors.py"
fi
if [ ! -f "$ACTOR_ASSETS_DIR/vic_man_0.bmp" ]; then
    printf 'Decoded victim ACT assets not found under %s\n' "$ACTOR_ASSETS_DIR" >&2
    exit 1
fi

cmake -S "$PROJECT_ROOT/port/spikes/training-native-prototype" -B "$BUILD_DIR"
cmake --build "$BUILD_DIR" --target fdny-scene-viewer -j 4

rm -rf "$APP_DIR"
mkdir -p "$APP_DIR/Contents/MacOS" "$APP_DIR/Contents/Resources"
cp "$BUILD_DIR/fdny-scene-viewer" "$APP_DIR/Contents/MacOS/fdny-scene-viewer"
ditto "$PACKAGE_DIR" "$APP_DIR/Contents/Resources/scene"
ditto "$ACTOR_ASSETS_DIR" "$APP_DIR/Contents/Resources/actor_exports"

cat > "$APP_DIR/Contents/Info.plist" <<'PLIST'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleExecutable</key>
    <string>fdny-scene-viewer</string>
    <key>CFBundleIdentifier</key>
    <string>org.fdnydecomp.sceneviewer</string>
    <key>CFBundleName</key>
    <string>FDNY Scene Preview</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>0.1</string>
    <key>CFBundleVersion</key>
    <string>1</string>
    <key>LSMinimumSystemVersion</key>
    <string>13.0</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
PLIST

printf 'APPL????' > "$APP_DIR/Contents/PkgInfo"

plutil -lint "$APP_DIR/Contents/Info.plist"
printf '\nBuilt app: %s\n' "$APP_DIR"
printf 'Bundled scene: %s\n' "$SCENE_KEY"
printf 'Bundled victim ACT assets: %s\n' "$ACTOR_ASSETS_DIR"
