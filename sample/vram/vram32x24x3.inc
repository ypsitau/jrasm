;;;=============================================================================
;;; vram32x24x3.inc
;;;=============================================================================
	.cseg
;;;-----------------------------------------------------------------------------
;;; Constant
;;;-----------------------------------------------------------------------------
vram.margin@l: 		.equ	0
vram.margin@r: 		.equ	0
vram.width:		.equ	32
vram.height:		.equ	24
vram.bytes_per_char:	.equ	3

;;;-----------------------------------------------------------------------------
;;; vram.nextx
;;;-----------------------------------------------------------------------------
vram.nextx:
	.macro
	inx
	inx
	inx
	.end

;;;-----------------------------------------------------------------------------
;;; vram.fromxy
;;; Calculate: X = vram + (posx + posy * 32) * 3
;;;  Format: vram.fromxy posx,posy
;;;  Param: vram
;;;         posx [IMM, DIR, IDX, EXT] .. X-position
;;;         posy [IMM, DIR, IDX, EXT] .. Y-position
;;;  Output: X .. VRAM address
;;;  Broken: A, B
;;;-----------------------------------------------------------------------------
vram.fromxy:
	.macro	posx,posy
	ldaa	posx
	ldab	posy
	jsr	vram._fromxy
	.end

vram._fromxy:
	.scope
posx:	.equ	0x18
	staa	{posx}
	clra
	clc
	rorb
	rora
	rorb
	rora
	rorb
	rora
	adda	{posx}
	adcb	0
	staa	[result.l]
	stab	[result.h]
	clc
	rola
	rolb
	adda	[result.l]
	staa	[result.l]
	adcb	[result.h]
	stab	[result.h]
	ldaa	vram <+> [2,1]
	adda	[result.l]
	staa	[result.l]
	ldaa	vram <+> [2,0]
	adca	[result.h]
	staa	[result.h]
result:
	.equ	$+1
result.h:
	.equ	$+1
result.l:
	.equ	$+2
	ldx	0x0000
	rts
	.end

;;;-----------------------------------------------------------------------------
;;; vram.refresh
;;;-----------------------------------------------------------------------------
vram.refresh:
	.macro
	jsr	vram._refresh_without_margin
	.end

	.include "oputil.inc"
	.include "vramcmn.inc"
