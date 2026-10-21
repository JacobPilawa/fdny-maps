# Fresh profile difficulty default

## Provenance

Fresh read-only Ghidra 12.0.4 headless decompilation and instruction listing
from a disposable copy of the saved project. The primary project was not
modified. Input `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
No game runtime or saved-profile file was captured.

## Fresh object default

Profile manager initialization `00425090` allocates each profile record and
calls `00423DB0`. At `00423EAC`, the constructor calls its common initializer;
at `00423EB1` it sets the record's `+0x04` ordinal to `-1`, then the raw
instruction at `00423EBD` stores integer `1` at record `+0x2C`. The profile
serializer `00424DC0` names that field `DifficultyLevel` and writes its low
byte. This establishes `1` as the constructor default for a fresh profile.

After a level flush, `00471030` reads the profile object referenced by
EverGame `+0x6E4`, loads its `+0x2C` value, and passes it to `004443E0`; that
helper formats the integer using `%i` for the script variable `DIFFICULTY`.
The raw formatter anchors are in
[`004443e0_difficulty_format_fresh_2026-10-21.asm`](004443e0_difficulty_format_fresh_2026-10-21.asm).

## Limits and port implication

The manager first clears EverGame `+0x6E4` and then loads or creates profile
records into its list. The profile menu callback routes slot events 0 through 5
to `004239C0`, which writes the indexed record to the selected-profile field;
see [the selection pointer route](profile_selection_pointer_writer_2026-10-21.md).
The static assignment path is resolved, but no sample save or live menu capture
was present, so the selected slot's actual loaded `DifficultyLevel` remains
unknown.

The host's per-stage `Scripts` default `DIFFICULTY="1"` matches a freshly
constructed retail profile and the menu's profile-reset path. This does not
establish parity for an existing save, so no loaded-profile value is inferred
from the constructor default. See also
[`mission_outcome_score_profile_fresh_2026-10-20.md`](mission_outcome_score_profile_fresh_2026-10-20.md),
[`00425090_profile_manager_initializer_fresh_2026-10-21.c`](../runtime-loop/00425090_profile_manager_initializer_fresh_2026-10-21.c),
and [`selector_initial_ordinal_resolution.md`](selector_initial_ordinal_resolution.md).
