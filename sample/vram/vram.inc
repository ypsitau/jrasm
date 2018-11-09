;;;=============================================================================
;;; vram.inc
;;;=============================================================================
	.cseg

;;;-----------------------------------------------------------------------------
;;; vram.fromxy
;;; Calculate: X = posx * 2 + posy * 64
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
	jsr	_vram.fromxy
	.end

_vram.fromxy:
	.scope
posx2:	.equ	0x18
	ldx	vram
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

;;;-----------------------------------------------------------------------------
;;; vram.clear
;;;-----------------------------------------------------------------------------
vram.clear:
	.macro
	jsr	vram._clear
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

;;;-----------------------------------------------------------------------------
;;; vram.refresh
;;;-----------------------------------------------------------------------------
vram.refresh:
	.macro
	jsr	vram._refresh
	.end

vram._refresh:
	.scope
	ldmw	[srcp],vram
	ldmw	[dstp_char],0xc100
	ldmw	[dstp_attr],0xc500
loop:	
srcp:
	.equ	$+1
	ldx	0x0000
	cpx	vram.end
	beq	done
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
	bra	loop
done:
	rts
	.end

;;;-----------------------------------------------------------------------------
;;; VRAM Buffer
;;;-----------------------------------------------------------------------------
	.wseg
vram:
	.ds	32 * 2 * 24
vram.end:

	.include "oputil.inc"
