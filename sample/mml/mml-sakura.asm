;;;=============================================================================
;;; mml-sakura.asm
;;;=============================================================================
	.org	0x1000
	ldx	mml1
	stx	{0x30}
	ldx	mml2
	stx	{0x32}
	ldaa	1
	staa	{0x2d}
	staa	{0x2e}
	ldaa	{0x2c}
	staa	[0xc811]  ; MN1271 TBWSR .. Time Counter B Write Status Register
	ldaa	(1 << 6) | (0 << 5) | (3 << 3) | (1 << 0)
	staa	[0xc810]  ; MN1271 TBCSR .. Time Counter B Control Status Register
	rts

mml1:
	.db	m">dde2 dde2 defe de8d8<b-2 afab-"
	.db	m"aa8f8e2 >defe de8d8<b-2 afab- aa8f8e2"
	.db	m">dde2 dde2 r8<a8b-2>e8d8 <b-1 a1;"

mml2:
	.db	m"afe2 afe2 d2.a f2e2 d2.f"
	.db	m"d1 a2de f2e2 d2.e d1"
	.db	m"afe2 afe2 feda f2.a8f8 e1;"
