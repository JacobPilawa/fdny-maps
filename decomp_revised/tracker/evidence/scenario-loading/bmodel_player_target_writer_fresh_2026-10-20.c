/* Hash-pinned raw-instruction evidence slice
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Source: read-only Ghidra 12.0.4, -process FDNYFirefighter.exe -readOnly -noanalysis
 * The Player update block at 0045387B has no function boundary in the saved project.
 * This is an instruction-backed reconstruction, not Ghidra-decompiler output.
 */

/* Constructor evidence at 00453D98..00453DA5 installs:
 *   [Player+0x00]  = vtable 0x004E1C48
 *   [Player+0x24]  = frame vtable 0x004DFC30
 *   [Player+0x140] = movement-subobject vtable 0x004E1C40
 * The Player main vtable's byte slot +0x18 resolves to 0049D760, which returns
 * this+0x24. The returned frame uses the known +0x10 and +0x1C basis columns.
 */

/* Player movement update slice, ESI = Player+0x140, EDI = Player root:

0045387b  CMP dword ptr [ESI + 0x3300],EBP
00453881  JZ  0x00453a38
00453887  MOV EDX,dword ptr [EDI]
00453889  PUSH 0x42b40000                 ; 90.0f, retained for ScaleVector
0045388e  LEA ECX,[ESP + 0x40]
00453892  PUSH ECX                          ; output for frame third basis copy
00453893  MOV ECX,EDI
00453895  LEA EBX,[ESI + 0x338c]           ; movement-subobject target basis
0045389b  CALL dword ptr [EDX + 0x18]      ; GetScriptActorSubobject -> Player+0x24
004538a0  CALL 0x0049e770                 ; copy frame +0x1c/+0x20/+0x24
004538ae  CALL 0x00446390                 ; scale copied vector by 90.0f

004538b8  PUSH EAX                          ; retain scaled third-column vector
004538b9  PUSH 0xc2700000                 ; -60.0f, retained for ScaleVector
004538be  LEA ECX,[ESP + 0x8c]
004538c5  PUSH ECX                          ; output for frame second basis copy
004538c6  MOV ECX,EDI
004538c8  CALL dword ptr [EDX + 0x18]      ; GetScriptActorSubobject -> Player+0x24
004538cd  CALL 0x00447830                 ; copy frame +0x10/+0x14/+0x18
004538db  CALL 0x00446390                 ; scale copied vector by -60.0f

004538e7  CALL 0x0042d050                 ; add movement [ESI+0x338c] and scaled vector
004538f5  CALL 0x0042d050                 ; add the other scaled basis vector
004538fa  MOV ECX,dword ptr [ESI + 0x3300] ; held BModel root = Player+0x3440
00453902  ADD ECX,0x224
00453908  MOV dword ptr [ECX],EDX
0045390d  MOV dword ptr [ECX + 0x4],EDX
00453913  MOV dword ptr [ECX + 0x8],EAX
0045391f  CALL 0x0043bd90                 ; apply held-child center correction

 * The source-backed vector composition is:
 *   heldBModel.target(+0x224) = movementSubobject(+0x338c)
 *       + 90.0f * frame(+0x1c..+0x24)
 *       - 60.0f * frame(+0x10..+0x18)
 * The two frame offsets are the third and second basis columns, respectively.
 * The arithmetic and target field are established. The semantic label and
 * renderer/camera relation of movementSubobject(+0x338c), retail callback
 * cadence, and final carried pose remain open. The smoothing formula is
 * separately resolved in bmodel_center_correction_smoothing_fresh_2026-10-20.asm.
 */
