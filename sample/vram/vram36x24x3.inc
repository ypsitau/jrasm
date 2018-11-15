;;;=============================================================================
;;; vram36x24x3.inc
;;;=============================================================================
	.cseg
;;;-----------------------------------------------------------------------------
;;; Constant
;;;-----------------------------------------------------------------------------
vram.margin@l: 		.equ	2
vram.margin@r: 		.equ	2
vram.width:		.equ	32 + vram.margin@l + vram.margin@r
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
;;; vram.zero_and_nextx
;;;-----------------------------------------------------------------------------
vram.zero_and_nextx:
	.macro
	clra
	staa	[x]
	staa	[x+1]
	staa	[x+2]
	inx
	inx
	inx
	.end

;;;-----------------------------------------------------------------------------
;;; vram.prevx
;;;-----------------------------------------------------------------------------
vram.prevx:
	.macro
	dex
	dex
	dex
	.end

;;;-----------------------------------------------------------------------------
;;; vram.zero_and_prevx
;;;-----------------------------------------------------------------------------
vram.zero_and_prevx:
	.macro
	clra
	staa	[x+1]
	staa	[x+2]
	staa	[x+3]
	dex
	dex
	dex
	.end

;;;-----------------------------------------------------------------------------
;;; vram.fromxy
;;; Calculate: X = vram + (posx + posy * 32 + posy * 4) * 3
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
	rolb
	rola
	rolb
	rola
	stab	[result.l]
	staa	[result.h]
	rolb
	rola
	rolb
	rola
	rolb
	rola
	addb	{posx}
	adca	0
	addb	[result.l]
	adca	[result.h]
	stab	[result.l]
	staa	[result.h]
	clc
	rolb
	rola
	addb	[result.l]
	stab	[result.l]
	adca	[result.h]
	staa	[result.h]
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
	jsr	vram._refresh_with_margin
	.end

	.include "oputil.inc"
	.include "vramcmn.inc"
