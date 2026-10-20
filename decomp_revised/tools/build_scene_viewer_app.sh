#!/bin/sh
set -eu

PROJECT_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
SCENE_KEY=${1:-rescue-all}
BUILD_DIR="$PROJECT_ROOT/port/spikes/training-native-prototype/.local/build-scene-viewer"
ACTOR_ASSETS_DIR="$PROJECT_ROOT/.local/actor_exports"
APP_DIR="$PROJECT_ROOT/.local/FDNY Firefighters.app"

if [ "$SCENE_KEY" = "rescue-all" ]; then
    PACKAGE_KEYS="training mbase bar_r gas_r bank boat_a subway_a oil_rig_a boat_b subway_b oil_rig_b oil_rig_c oil_rig_d city_h const_h dock_h oil_rig_h"
    DEFAULT_SCENE_KEY="training"
else
    PACKAGE_KEYS="$SCENE_KEY"
    DEFAULT_SCENE_KEY="$SCENE_KEY"
fi

for map_key in $PACKAGE_KEYS; do
    package_dir="$PROJECT_ROOT/.local/exports/$map_key"
    if [ ! -d "$package_dir" ]; then
        printf 'Scene package not found: %s\n' "$package_dir" >&2
        printf 'Export it first with tools/export_gbsp_scene.py.\n' >&2
        exit 1
    fi
done

DEFAULT_PACKAGE_DIR="$PROJECT_ROOT/.local/exports/$DEFAULT_SCENE_KEY"
if [ ! -d "$DEFAULT_PACKAGE_DIR" ]; then
    printf 'Default scene package not found: %s\n' "$DEFAULT_PACKAGE_DIR" >&2
    exit 1
fi

if [ ! -f "$ACTOR_ASSETS_DIR/vic_man_0.bmp" ]; then
    python3 "$PROJECT_ROOT/tools/export_victim_actors.py"
fi
python3 "$PROJECT_ROOT/tools/export_equipment_actors.py"
python3 "$PROJECT_ROOT/tools/export_propmodel_actors.py"
python3 "$PROJECT_ROOT/tools/export_goggles_sprites.py"
python3 "$PROJECT_ROOT/tools/export_fire_direction_sprites.py"
python3 "$PROJECT_ROOT/tools/export_oxygen_sprites.py"
python3 "$PROJECT_ROOT/tools/export_victim_feedback_sprites.py"
python3 "$PROJECT_ROOT/tools/export_bonus_points_sprites.py"
python3 "$PROJECT_ROOT/tools/export_hint_font.py"
python3 "$PROJECT_ROOT/tools/export_fire_sprites.py"
python3 "$PROJECT_ROOT/tools/export_extinguisher_spray_sprites.py"
python3 "$PROJECT_ROOT/tools/export_playerhose_stream_sprites.py"

ORIGINAL_ROOT="$PROJECT_ROOT/inputs/original"
for map_key in $PACKAGE_KEYS; do
    package_dir="$PROJECT_ROOT/.local/exports/$map_key"
    map_evs="$ORIGINAL_ROOT/Data/Maps/$map_key.evs"
    if [ ! -f "$map_evs" ]; then
        map_evs=$(find "$ORIGINAL_ROOT/Data/Maps" -maxdepth 1 -type f -iname "$map_key.evs" -print -quit)
    fi
    if [ -n "$map_evs" ] && [ -f "$map_evs" ]; then
        python3 "$PROJECT_ROOT/tools/export_evs_bundle.py" \
            --map-key "$map_key" \
            --original-root "$ORIGINAL_ROOT" \
            --scene-package "$package_dir"
    elif [ "$map_key" = "city_h" ] || [ "$map_key" = "const_h" ] || \
         [ "$map_key" = "dock_h" ] || [ "$map_key" = "oil_rig_h" ]; then
        python3 "$PROJECT_ROOT/tools/export_evs_bundle.py" \
            --map-key "$map_key" \
            --original-root "$ORIGINAL_ROOT" \
            --scene-package "$package_dir"
    fi
    python3 "$PROJECT_ROOT/tools/export_level_music.py" \
        --map-key "$map_key" \
        --level-file "$ORIGINAL_ROOT/Data/ResDefs/thegame.lev" \
        --original-root "$ORIGINAL_ROOT" \
        --scene-package "$package_dir"
done
if [ ! -f "$ACTOR_ASSETS_DIR/vic_man_0.bmp" ]; then
    printf 'Decoded victim ACT assets not found under %s\n' "$ACTOR_ASSETS_DIR" >&2
    exit 1
fi

cmake -S "$PROJECT_ROOT/port/spikes/training-native-prototype" -B "$BUILD_DIR"
cmake --build "$BUILD_DIR" --target fdny-scene-viewer -j 4

rm -rf "$APP_DIR"
mkdir -p "$APP_DIR/Contents/MacOS" "$APP_DIR/Contents/Resources"
cp "$BUILD_DIR/fdny-scene-viewer" "$APP_DIR/Contents/MacOS/fdny-scene-viewer"
ditto "$DEFAULT_PACKAGE_DIR" "$APP_DIR/Contents/Resources/scene"
ditto "$ACTOR_ASSETS_DIR" "$APP_DIR/Contents/Resources/actor_exports"

if [ "$SCENE_KEY" = "rescue-all" ]; then
    mkdir -p "$APP_DIR/Contents/Resources/menu"
    cp "$ORIGINAL_ROOT/Data/menu/main.bmp" "$APP_DIR/Contents/Resources/menu/main.bmp"
    cp "$ORIGINAL_ROOT/Data/menu/mcontrols.bmp" "$APP_DIR/Contents/Resources/menu/mcontrols.bmp"
    cp "$ORIGINAL_ROOT/Data/menu/mcontrols_a.bmp" "$APP_DIR/Contents/Resources/menu/mcontrols_a.bmp"
    mkdir -p "$APP_DIR/Contents/Resources/scenes"
    for map_key in $PACKAGE_KEYS; do
        ditto "$PROJECT_ROOT/.local/exports/$map_key" \
            "$APP_DIR/Contents/Resources/scenes/$map_key"
    done
    python3 "$PROJECT_ROOT/tools/build_rescue_mission_catalog.py" \
        --level-file "$ORIGINAL_ROOT/Data/ResDefs/thegame.lev" \
        --scenes-dir "$APP_DIR/Contents/Resources/scenes" \
        --output "$APP_DIR/Contents/Resources/rescue_missions.json"
fi

if [ "$SCENE_KEY" = "rescue-all" ]; then
    python3 "$PROJECT_ROOT/level_editor/compiler.py"
fi

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
    <string>org.fdny.firefighters.americanhero</string>
    <key>CFBundleName</key>
    <string>FDNY Firefighters: American Hero</string>
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
if [ "$SCENE_KEY" = "rescue-all" ]; then
    printf 'Bundled rescue packages: %s\n' "$PACKAGE_KEYS"
    printf 'Default launch: title menu, then source-ordered rescue mission selection\n'
else
    printf 'Bundled scene: %s\n' "$SCENE_KEY"
fi
printf 'Bundled victim ACT assets: %s\n' "$ACTOR_ASSETS_DIR"
