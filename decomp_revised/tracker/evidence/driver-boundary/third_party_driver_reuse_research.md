# Third-party Genesis3D driver reuse research

Checked 2026-10-13. This is a source discovery and reuse assessment, not a license opinion. It extends the existing [public driver comparison](public_driver_comparison.md); the already-established 100/3 versus FDNY 101/1 mismatch and FDNY table-slot observations are not repeated here.

## Findings

There are completed, public Genesis3D-era renderer sources that can reduce *reference* work. The strongest directly relevant lead is Advanced Genesis3D's **AG3D OGLDrv 0.1**: its SourceForge project announced an OpenGL driver source release in June 2002, and the file archive lists that driver package. It is a finished sibling-driver implementation, not a Direct3D driver and not evidence of FDNY lineage. Its project is marked “Other License”; the public listing does not establish the package's license or file-level provenance. Treat it as a research lead only until the archive and included notices are inspected.

The [RealityFactory Genesis3D repository](https://github.com/RealityFactory/Genesis3D) is a separate, more comprehensive Genesis3D 1.1-derived source tree. Its driver directory contains D3D, D3D7x, OpenGL and wireframe variants (as recorded by the existing comparison). This makes it useful for learning the shared driver architecture and locating candidate D3D code, but the repository's declared lineage and common API are not proof that any one driver matches the FDNY binary. Its checked-in [Genesis3D Public License 1.01](https://github.com/RealityFactory/Genesis3D/blob/master/g3dlicense.txt) permits modification/distribution subject to reciprocal source and branding provisions; therefore source reuse could impose material obligations. The repository's current files also include later community modifications, and exact per-file authorship/terms were not independently resolved here.

Official Genesis3D SDK documentation says the SDK included complete engine/tool source under its OpenSource directory and that the original SDK shipped software, D3D and Glide drivers. The driver documentation describes D3D6-era support. This corroborates availability of historical reference implementations, but documentation is not a source archive or a compatible replacement. The current Genesis3D website says 1.1 is available without licensing obligations, while the historical and community sources retain GPL-like Genesis3D Public License language; do not infer a blanket license clearance for third-party/community driver files from the current site statement.

## Compatibility and practical reuse

| Candidate | What it can help with | FDNY-compatible implementation? |
|---|---|---|
| AG3D OGLDrv 0.1 | Rendering-driver lifecycle, OpenGL state setup, texture/lightmap and draw-path concepts; useful as an independently authored behavioral reference | No: OpenGL backend, and the published source release's ABI/table correspondence to FDNY 101/1 has not been shown |
| RealityFactory Genesis3D `D3DDrv` | Likely closest source-level reference for the D3D6-era renderer and driver lifecycle; useful to formulate hypotheses and identify old API idioms | Unproven: shared Genesis3D ancestry does not establish matching table version, slot layout, callback conventions or binary provenance; the public 100/3 header mismatch already documented is a concrete incompatibility |
| RealityFactory `D3D7xDrv` | D3D7 renderer comparison only | No direct substitute: different driver generation/API |
| Official SDK driver source | Historical behavior and architecture if a verifiable archive is obtained | Unknown until source revision and ABI are compared |

No FDNY-specific compatible implementation was located in the primary sources searched. The useful reuse is **knowledge/reference**, not transplantable code at this evidence level. Continue fresh RE for the game-specific 101/1 dispatch contract, the FDNY-only table slots and callback meanings, initialization order, state behavior, and correspondence of any public function to the pinned local DLL. A candidate source can narrow questions, but each proposed mapping still needs corroboration against the local binary and runtime evidence.

## Provenance / evidence boundary

Sources: [official Genesis3D site](https://www.genesis3d.com/), [official SDK manifest](https://genesis3d.com/old/docs/Manifest.htm), [official driver documentation](https://genesis3d.com/old/docs/Drivers.htm), [RealityFactory source repository](https://github.com/RealityFactory/Genesis3D), its [license file](https://github.com/RealityFactory/Genesis3D/blob/master/g3dlicense.txt), [Advanced Genesis3D project](https://sourceforge.net/projects/advancedgenesis/), [OpenGL driver source announcement](https://sourceforge.net/p/advancedgenesis/news/), and [AG3D OGLDrv archive listing](https://sourceforge.net/projects/advancedgenesis/files/Advanced%20G3D%20Driver%20OpenGL/).

The Advanced Genesis3D archive package contents and exact license were not inspected; the RealityFactory comparison remains unpinned to an immutable source SHA and its nested D3D source was not confirmed against FDNY in this research. The official manifest describes the SDK structure but does not independently authenticate an obtainable archive's contents. No claim is made that any source produced the game DLL, that a source is drop-in ABI compatible, or that the FDNY binary's provenance is resolved. No external code was copied into the port.
