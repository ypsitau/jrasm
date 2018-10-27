	.org	0x1000
	ldx	mml1
	stx	{0x30}
	ldx	mml2
	stx	{0x32}
	ldaa	1
	staa	{0x2d}
	staa	{0x2e}
	ldaa	(1 << 6) | (0 << 5) | (3 << 3) | (1 << 0)
	staa	[0xc810]
	rts

mml1:
	.db	M">dde2 dde2 defe de8d8<b-2 afab-"
	.db	M"aa8f8e2 >defe de8d8<b-2 afab- aa8f8e2"
	.db	M">dde2 dde2 r8<a8b-2>e8d8 <b-1 a1;"

mml2:
	.db	M"afe2 afe2 d2.a f2e2 d2.f"
	.db	M"d1 a2de f2e2 d2.e d1"
	.db	M"afe2 afe2 feda f2.a8f8 e1;"
