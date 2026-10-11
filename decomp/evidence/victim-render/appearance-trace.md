# Victim actor setup and appearance trace

`evidence/startup-initializers/0045b690_FUN_0045b690.c` shows actor creation with model ID, a uniform scale of 2, a 90-degree basis adjustment, and subtraction of `_DAT_004E2358` from transform Y. `tools/pe_constants.py` reads that preserved executable constant as 25.0; the raw values are in `setup-constants.txt`.

`evidence/startup-initializers/0045d5b0_FUN_0045d5b0.c` shows the second-pass material selection. It looks up an appearance counter using the actor model ID, selects that index if it is below the Genesis actor's material count (otherwise resets the selected index to zero), copies the selected material into slot 0, then stores the selected index plus one in the model-keyed registry. The two bounded generic tree routines used by this path are retained in `appearance-registry-assembly.txt`; the caller decompilation supplies the material-count and slot semantics. The selected index is deterministic and this path has no random call.

For a fresh Training initialization where these are the first model-12 actors, the two victims select material indices 0 and 1 from the three-material `vic_man` ACT. The exact appearance sequence can depend on earlier model-12 constructions in the same process; live restart persistence is not established. Model-to-bitmap extraction order and triangle material index 0 are preserved in `assets/training/actors/vic_man.json`.

This resolves setup/material selection facts only. Native actor construction, applying the root transform and chosen texture to a live renderer, full hierarchy parity, AI and rescue remain open. No original process was attached.
