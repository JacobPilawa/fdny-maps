# Reuse in another viewer

`recording-data.js` has no Three.js, DOM, server, or build-system dependency. Import it as an ES module:

```js
import {parseRecording, frameAt, boundsFor} from './recording-data.js';

const trace = parseRecording(await file.text(), file.name);
// Load the BSP matching trace.stage, then render in original engine units.
const sample = frameAt(trace.samples, playbackMilliseconds);
playerMarker.position.set(...sample.position);

const bounds = boundsFor(sample);
const size = bounds.max.map((n, i) => n - bounds.min[i]);
const center = bounds.min.map((n, i) =>
  sample.position[i] + (n + bounds.max[i]) / 2);
// Set an axis-aligned box to `center` with dimensions `size`.
```

The marker/box above are your renderer's objects, not dependencies supplied by the parser. `trace.samples` contains the full Player path; `trace.victims` groups victim snapshots by lifetime ID. A rainbow track can map `sample.t_ms / trace.duration` to a hue; a scrubbed track renders only through the selected time. `timeColor`, `indexAt` and `canJoin` are exported for these uses.

For a victim, do not show a marker before its first snapshot or at/after a non-null `track.end`. Otherwise use `frameAt(track.samples, playbackMilliseconds)` for its last observed/interpolated state, and `boundsFor(sample, true)` for its box. The native event shape is described in [FORMAT.md](../FORMAT.md).

The helper caps input at 128 MB / 600,000 records, rejects invalid times/positions and out-of-order Player samples, and reports partial files/gaps through `trace.warnings`. Its duration uses the last Player sample. It interpolates positions/facing/pitch between nearby samples, holding state/bounds; it returns the previous sample across a long gap.

Run `read_recording.mjs` in Node for a complete minimal consumer. To create a separate Python consumer, read each JSON line, group by `type`/victim `id`, and use the same timestamp/coordinate conventions. `inspect_recordings.py` provides a Python reader and independent SVG plotter.
