;;;=============================================================================
;;; vram32x24x2.inc
;;;=============================================================================
	.cseg
;;;-----------------------------------------------------------------------------
;;; vram.fromxy
;;; Calculate: X = vram + (posx + posy * 32) * 2
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
	clc
	rola
	rolb
	staa	[result.l]
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
	ldx	vram
	clra
loop:
	staa	[x]
	inx
	cpx	vram.end
	bne	loop
	.end

;;;-----------------------------------------------------------------------------
;;; vram.scrollup
;;;  Format: vram.scrollup top=0,bottom=24
;;;-----------------------------------------------------------------------------
vram.scrollup:
	.macro	top=0,bottom=24
posy:	.equ	0x18
	ldaa	bottom
	deca
	staa	{posy}
	vram.fromxy 0,{posy}
	stx	[endaddr1]
	addx.mb	32*2
	stx	[endaddr2]
	vram.fromxy 0,top
loop1:
	ldaa	[x+32*2]
	staa	[x]
	inx
endaddr1:
	.equ	$+1
	cpx	0x0000
	bne	loop1
	clra
loop2:
	staa	[x]
	inx
endaddr2:
	.equ	$+1
	cpx	0x0000
	bne	loop2
	.end

;;;-----------------------------------------------------------------------------
;;; vram.scrolldown
;;;  Format: vram.scrolldown top=0,bottom=24
;;;-----------------------------------------------------------------------------
vram.scrolldown:
	.macro	top=0,bottom=24
posy:	.equ	0x18
	vram.fromxy 0,top
	dex
	stx	[endaddr1]
	subx.mb	32*2
	stx	[endaddr2]
	vram.fromxy 0,bottom
	dex
loop1:
	ldaa	[x]
	staa	[x+32*2]
	dex
endaddr1:
	.equ	$+1
	cpx	0x0000
	bne	loop1
	clra
loop2:
	staa	[x+32*2]
	inx
endaddr2:
	.equ	$+1
	cpx	0x0000
	bne	loop2
	.end

;;;-----------------------------------------------------------------------------
;;; vram.refresh
;;;-----------------------------------------------------------------------------
vram.refresh:
	.macro
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
	.end

;;;-----------------------------------------------------------------------------
;;; VRAM Buffer
;;;-----------------------------------------------------------------------------
	.wseg
vram.work:
	.ds	1
vram:
	.ds	32 * 24 * 2
vram.end:

	.include "oputil.inc"
