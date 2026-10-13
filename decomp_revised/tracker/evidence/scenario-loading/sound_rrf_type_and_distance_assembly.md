# Sound definition type and distance field evidence

## Inputs

- `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- `Data/ResDefs/sound.rrf`, SHA-256 `433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`

## Type dispatch

At `LoadResourceDefinitionsFile` (`0049B3B0`), the parser compares the current
token against `ROC_SOUND_BUFFER2D` at `0x004E40A8` and
`ROC_SOUND_BUFFER3D` at `0x004E4094`. The corresponding callsites push a
one-byte type argument into `ParseSoundBufferDefinition` (`0049B140`):

```asm
0049b400: MOV EDI,0x4e40a8       ; ROC_SOUND_BUFFER2D
0049b40e: CMPSB.REPE ES:EDI,ESI
0049b410: JNZ 0x0049b41c
0049b412: PUSH 0x0                ; type = 2D
0049b413: MOV ECX,EBX
0049b415: CALL 0x0049b140

0049b41c: MOV EDI,0x4e4094       ; ROC_SOUND_BUFFER3D
0049b42a: CMPSB.REPE ES:EDI,ESI
0049b42c: JNZ 0x0049b439
0049b42e: PUSH 0x1                ; type = 3D
0049b430: MOV ECX,EBX
0049b432: CALL 0x0049b140
```

For a nonzero type, `ParseSoundBufferDefinition` consumes two numeric tokens
before the quoted path. Its caller disassembly shows each token converted and
stored separately; the first token is at `[ESP+0x1c]`, the second at
`[ESP+0x20]` before the final call setup.

## Resource flag and FMOD distance setter

`FindOrCreateSoundResourceByName` (`0049FDA0`) forwards the type byte and file
path to `InitializeFMODSoundResource` (`004A0210`). For non-MP3 resources,
`004A0210` stores the type byte at resource offset `+9` and calls
`FSOUND_Sample_Load`; the MP3 branch instead marks a stream and calls
`FSOUND_Stream_OpenFile`. The Subway source files are WAVs, so the 3D type byte
is retained at `+9` and later copied into the playing sound instance.

`ParseSoundBufferDefinition` forwards the first parsed numeric token as the
first distance argument and the second token as the second distance argument
to `SetFMODSampleMinMaxDistance` (`004A0040`). That wrapper calls
`FSOUND_Sample_SetMinMaxDistance(sample, first, second)`. Therefore
`ROC_SOUND_BUFFER3D SubDoor 200 1000` and the matching `Zapp3d` line configure
minimum distance `200` and maximum distance `1000`.

This establishes the parser fields and FMOD API arguments statically. It does
not capture the runtime attenuation curve or perceived sound on a target
platform.
