;;;=============================================================================
;;; utils.inc
;;;=============================================================================

;;;-----------------------------------------------------------------------------
;;; Format: jXX addr16
;;; Param: addr16 [EXT] .. Address
;;;-----------------------------------------------------------------------------
jcc:	.macro addr16
	bcs	skip
	jmp	addr16
skip:	.end

jcs:	.macro addr16
	bcc	skip
	jmp	addr16
skip:	.end

jeq:	.macro addr16
	bne	skip
	jmp	addr16
skip:	.end

jge:	.macro addr16
	blt	skip
	jmp	addr16
skip:	.end

jgt:	.macro addr16
	ble	skip
	jmp	addr16
skip:	.end

jhi:	.macro addr16
	bls	skip
	jmp	addr16
skip:	.end

jle:	.macro addr16
	bgt	skip
	jmp	addr16
skip:	.end

jls:	.macro addr16
	bhi	skip
	jmp	addr16
skip:	.end

jlt:	.macro addr16
	bge	skip
	jmp	addr16
skip:	.end

jmi:	.macro addr16
	bpl	skip
	jmp	addr16
skip:	.end

jne:	.macro addr16
	beq	skip
	jmp	addr16
skip:	.end

jpl:	.macro addr16
	bmi	skip
	jmp	addr16
skip:	.end

jvc:	.macro addr16
	bvs	skip
	jmp	addr16
skip:	.end

jvs:	.macro addr16
	bvc	skip
	jmp	addr16
skip:	.end

;;;-----------------------------------------------------------------------------
;;; Format: addx num
;;; Param: num [IMM, DIR, IDX, EXT] .. Added 8bit number
;;;-----------------------------------------------------------------------------
addx:
	.macro num
	ldaa	num
	jsr	_addx
	.end

_addx:
	.scope
	stx	[result]
	clrb
	adda	[result.l]
	adcb	[result.h]
	staa	[result.l]
	stab	[result.h]
result:
	.equ $+1
result.h:
	.equ $+1
result.l:
	.equ $+2
	ldx	0x0000
	rts
	.end
	
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
