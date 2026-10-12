# Local Training assets

The native scene expects `assets/training/`, which is intentionally excluded from the public mirror. Keep or regenerate it from a locally held copy of the game data. Set `FDNY_ASSETS_DIR` to that directory when it is outside this checkout.

The preserved `evidence/original/rescue.rrf` binds Training ash renderer resource IDs 200 and 201 to `data/sprites/ash1.bmp` and `data/sprites/ash2.bmp`. Exact original copies are in the local-only `assets/training/fire-sprites/` extraction, with their SHA-256 hashes in `evidence/asset-manifest.json`. The bitmaps are excluded from the public mirror with the rest of the proprietary game assets.
