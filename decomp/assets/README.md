# Local Training assets

The native scene expects `assets/training/`, which is intentionally excluded from the public mirror. Keep or regenerate it from a locally held copy of the game data. Set `FDNY_ASSETS_DIR` to that directory when it is outside this checkout.

The preserved `evidence/original/rescue.rrf` binds Training ash renderer resource IDs 200 and 201 to `data/sprites/ash1.bmp` and `data/sprites/ash2.bmp`. Those original bitmap files are not present in the current `assets/training/` extraction or its manifest; include them from the original game data before implementing pixel-accurate ash rendering.
