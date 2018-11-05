;;;=============================================================================
;;; utils.inc
;;;=============================================================================

;;;-----------------------------------------------------------------------------
;;; Format: xy2codeaddr posx,posy
;;; Param: posx [IMM, DIR, IDX, EXT] .. X position
;;;        posy [IMM, DIR, IDX, EXT] .. Y position
;;; Output: x .. Code address that begins from 0xc100
;;; Broken: a, b
;;;-----------------------------------------------------------------------------
xy2codeaddr:
	.macro	posx,posy
	ldaa	posx
	ldab	posy
	jsr	_xy2codeaddr
	.end
	
_xy2codeaddr:
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
	adcb	0xc1
	stab	[result.h]
	staa	[result.l]
result.h:
	.equ $+1
result.l:
	.equ $+2
	ldx	0x0000
	rts
	.end

;;;-----------------------------------------------------------------------------
;;; Format: code2attraddr
;;; Input: x .. Code address that begins from 0xc100
;;; Output: x .. Attribute address that begins from 0xc500
;;; Broken: a
;;;-----------------------------------------------------------------------------
code2attraddr:
	.macro
	stx	[result]
	ldaa	0x04
	adda	[result.h]
	staa	[result.h]
result:
	.equ $+1
result.h:
	.equ $+1
	ldx	0x0000
	.end
