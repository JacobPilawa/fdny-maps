# Retail FDNYFirefighter.exe disassembly via objdump
# SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
# This excerpt was captured from the project-local inputs/original binary.
# Ghidra had no running instance for this follow-up; see fire_name_gate.md.

# The UserData +4 pointer is A_Name. Null skips string setup.
00448dc2: MOV EAX,[ESI + 0x78]       # FireRescue UserData
00448dc8: CMP dword ptr [EAX + 4],0
00448dcb: JE 0x00448ea3

# Get the origin buffer, then construct a reference-counted string from A_Name
# in the temporary stack slot. 00402790 calls 00401D80 to assign the bytes.
00448dd1: LEA ECX,[ESP + 0x14]
00448dd5: PUSH ECX
00448dd6: MOV ECX,ESI
00448dd8: CALL 0x0049D350
00448ddd: MOV EDX,[ESI + 0x78]
00448de0: PUSH EAX                  # origin output buffer for setter
00448de1: MOV EAX,[EDX + 4]         # A_Name C string
00448de4: PUSH ECX                  # reserved stack slot becomes string object
00448de5: MOV ECX,ESP
00448deb: PUSH EAX
00448dec: CALL 0x00402790           # construct/assign local string

# Embedded visual subobject is root +0x24. Setter stores string at +0x24,
# i.e. FireRescue root +0x48, then copies the origin words.
00448df1: LEA ECX,[ESI + 0x24]
00448df4: CALL 0x0046E320

# Supporting layout: 00401D80 stores string byte length at implementation +4;
# 0046F930's shared empty string has length zero at 0050190C.
