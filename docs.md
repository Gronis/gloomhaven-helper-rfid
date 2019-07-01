MT 85

15 bytes per character (+1 för count lista??)
7 bytes per monster type
8 bytes per monster
116 bytes header (elements etc...)

byte 116 (115 - 0 indexed) is number of characters / monsters types

header (116 bytes):

message types:

s: state, payload: none (empty)
v: version, payload: version number

00  // 2 bytes length of string
01  // 2 bytes length of string
73  // 1 length string (java string starts with nr chars??, "s")
9b  // 1-5 byte int, length of message
01  // part of length of message (encoded as 2 bytes)
00 // 4 byte message nr
00 // 4 byte message nr
0a // 4 byte message nr
xx // 4 byte message nr
xx // round counter
05 // scenario number
03 // scenario level
01
00
01
01
00
01
01
00
00
03
0f
00
00
01
03
01
03
00
04
00
00
01
06
01
05
02
04
04
00 // fire element
00 // element
00 // element
00 // element
00 // element
00 // element
07
00
01
01
08
04
06
07
05
01
03
00
02
01
01
01
08
09
0e
0f
0b
0d
0a
0c
08
04
00
00
08
21
25
23
20
22
26
27
24
07
00
00
08
3c
38
39
3f
3e
3d
3a
3b
08
01
01
06
41
45
44
42
47
46
0e
00
00
07
77
76
75
74
73
71
70
12
00
00
05
93
01
97
01
95
01
96
01
94
01
xx // number of characters / monster types

character format (15 bytes):

01 // 00 == monster, 01 == character
80 // Character Name (string) 80 means "Null"
xx // class
xx // exp
xx // remaining health
xx // max health
0x // level
xx // coins picked
xx // initiative
0x // number of status tokens [followed by 1 byte per status, now]
00 // number of status tokens [followed by 1 byte per status, last round]
0x // number of status tokens [followed by 1 byte per status, planning]
0x // 00 == alive, 01 == dead
0x // 00 == hasn't moved, 01 == has moved
0x // nr summons [followed by 12 bytes per summon]

monster type format (7 bytes):
0x // 00 == monster, 01 == character
00 // monster type
03 // no idea (seem to be the same for every monster type)
00 // no idea
00 // no idea
0x // 00 == hasn't moved, 01 == has moved
0x // number of monsters [followed by 8 bytes per monster]

monster format (8 bytes):
0x // number
00 // monster type?
0x // no idea
xx // remaining health
xx // max health
0x // number of status tokens [followed by 1 byte per status]
0x // number of status tokens [followed by 1 byte per status, last round]
0x // number of status tokens [followed by 1 byte per status, planning]

summons format:
0x // number assigned
03 // no idea
0x // no idea, same as number assigned
0x // move
0x // attack
0x // range
01 // no idea
xx // remaining health
xx // max health
0x // number of status tokens [followed by 1 byte per status]
0x // number of status tokens [followed by 1 byte per status, last round]
0x // number of status tokens [followed by 1 byte per status, planning]


status tokens:

03 // stun
04 // immoblized
05 // disarmed
06 // wounded


classes:

00 -> escort
01 -> objective
02 -> brute
03 -> cragheart
04 -> mind theif