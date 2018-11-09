;;;=============================================================================
;;; vram.inc
;;;=============================================================================
	.cseg
;;;-----------------------------------------------------------------------------
;;; vram.fromxy
;;; Calculate: X = (posx + posy * 32) * 3
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
	ldmw	[charp],0xc100
	ldmw	[attrp],0xc500
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
	inx
	stx	[srcp]
charp:
	.equ	$+1
	ldx	0x0000
	staa	[x]
	inx
	stx	[charp]
attrp:
	.equ	$+1
	ldx	0x0000
	stab	[x]
	inx
	stx	[attrp]
	bra	loop
done:
	rts
	.end

;;;-----------------------------------------------------------------------------
;;; VRAM Buffer
;;;-----------------------------------------------------------------------------
	.wseg
vram:
	.ds	32 * 24 * 3
vram.end:

	.include "oputil.inc"
