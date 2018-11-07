;;;=============================================================================
;;; utils.inc
;;;=============================================================================
	.cseg
;;;-----------------------------------------------------------------------------
;;; Format: xy2vram vram,posx,posy
;;;  Param: vram
;;;         posx [IMM, DIR, IDX, EXT] .. X position
;;;         posy [IMM, DIR, IDX, EXT] .. Y position
;;;  Output: x .. VRAM address
;;;  Broken: a, b
;;;-----------------------------------------------------------------------------
xy2vram:
	.macro	vram,posx,posy
	ldaa	posx
	ldab	posy
	ldx	vram
	jsr	_xy2vram
	.end

_xy2vram:
	.scope
posx2:	.equ	0x18
	stx	[result]
	asla
	staa	{posx2}
	clra
	clc
	rorb
	rora
	rorb
	rora
	adda	{posx2}
	adcb	0
	adda	[result.l]
	staa	[result.l]
	adcb	[result.h]
	stab	[result.h]
result:
	.equ	$+1
result.h:
	.equ	$+1
result.l:
	.equ	$+2
	ldx	0x0000
	rts
	.end
