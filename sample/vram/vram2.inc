;;;=============================================================================
;;; vram2.inc
;;;=============================================================================
	.cseg

vram.margin@l: .equ	2
vram.margin@r: .equ	2

vram.clear:
	.macro
	jsr	vram._clear
	.end

vram.refresh:
	.macro
	jsr	vram._refresh
	.end

vram._clear:
	.scope
	ldx	vram
	clra
loop:
	staa	[x]
	inx
	cpx	vram.end
	bne	loop
	rts
	.end

vram._refresh:
	.scope
	ldmw	[srcp],vram + vram.margin@l * 2
	ldmw	[dstp_char],0xc100
	ldmw	[dstp_attr],0xc500
	ldmb	[cnty],24
loopy:	
	ldmb	[cntx],32
srcp:
	.equ	$+1
loopx:	
	ldx	0x0000
	ldaa	[x]
	inx
	ldab	[x]
	inx
	stx	[srcp]
dstp_char:
	.equ	$+1
	ldx	0x0000
	staa	[x]
	inx
	stx	[dstp_char]
dstp_attr:
	.equ	$+1
	ldx	0x0000
	stab	[x]
	inx
	stx	[dstp_attr]
	dec	[cntx]
	bne	loopx
	addx.mb	(vram.margin@r + vram.margin@l) * 2
	dec	[cnty]
	bne	loopy
done:
	rts

	.wseg
cntx:
	.ds	1
cnty:
	.ds	1
	.end

	.wseg
vram:
	.ds	(vram.margin@l + 32 + vram.margin@r) * 2 * 24
vram.end:

	.include "oputil.inc"
