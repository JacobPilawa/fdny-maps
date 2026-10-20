# PlayerHose stage chatter cue port (2026-10-20)

## Source mapping

The retail executable is pinned at SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The static lifecycle trace in
[`player_hose_stage_lifecycle_2026-10-20.md`](player_hose_stage_lifecycle_2026-10-20.md)
shows the first authored hotspot marker emits `RC_HoseLvlStart`; the terminal
stage handlers emit `RC_HoseLvlWin` and `RC_HoseLvlLose`. The chatter
registration/playback route resolves each key to a one-entry `TChatterSet`.
The source `sound.rrf` hash is
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333` and maps
the entries to:

| Chatter resource | WAV resource | SHA-256 |
| --- | --- | --- |
| `RC_HoseLvlStart0` | `start2_b.wav` | `a90183e1d5a1007bdb5dc00403e8587127e58de8c09f8b3a8e92d00f2b349fce` |
| `RC_HoseLvlWin0` | `hosewin2.wav` | `26f1a5029ffef5840079458fc021808b6756a04b785bb43a4039d85e62271517` |
| `RC_HoseLvlLose0` | `lose5_b.wav` | `4008719fb2623b800a240c2067ea9395c69df53642483d9d73e2d0d059fcd02e` |

The `sound.rrf` definitions are `ROC_SOUND_BUFFER2D`. The statically traced
request path establishes resource selection and a sound-manager request, not
audible playback or its retail channel category.

## Shared-host implementation

`PlayerHoseStage` raises one pending cue when the authored first-hotspot marker
is entered, when the modeled final target cycle wins, or when the modeled
hotspot timer expires. The shared viewer consumes it through the existing
`SceneAudio` path. Host-only invalid-anchor and unresolved-next-hotspot
diagnostics do not produce a retail loss cue. The package exporter includes
all three resources in scene bundles so they are available when the audio
device loads its samples. The build invokes the bundle exporter for all four
PlayerHose maps; maps without a map-level EVS file receive an empty source list
and the common authored sound resources, while `const_h` retains its authored
EVS source bundle.

The port uses local sound group 1, matching its existing one-shot Player
chatter route; the retail category mapping for these keys remains unknown.
The rail's anchor polling and stage update cadence remain provisional, and no
interactive audio or retail-parity comparison was made.

## Build checkpoint

`./tools/build_scene_viewer_app.sh rescue-all` compiled the shared macOS target
and packaged the rescue-all app. The standalone executable and the app's
`Contents/MacOS/fdny-scene-viewer` both hash to
`334fdd76a276719cb7582585a8651190643899b67ae3898dada2cfe5e6edf1ec`.
The app plist validated; the bundle reports eight rescue choices and contains
the four PlayerHose stages. Export output listed all three chatter resources
for dedicated hose maps. The existing json-c warning remains (library built
for macOS 27.0, target 13.3). No tests, interactive run, audible-output check,
or retail-parity comparison was performed.
