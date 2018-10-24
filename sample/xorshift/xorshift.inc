;;;==================================================================================
;;; 16bit Xorshift
;;; This code has been ported from a Z80 program released in the site:
;;; http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
;;;==================================================================================

;;;------------------------------------------------------------------------------
;;; xrndn8
;;; Generates an 8bit random number within a range from 0 to n-1.
;;;  Format: xrndn8 n
;;;  Param: n [IMM, DIR, IDX, EXT] .. Range of random number
;;;  Return: A
;;;  Broken: B
;;;------------------------------------------------------------------------------
xrndn8:
	.macro	n
	ldaa	n
	jsr	_xrndn8
	ldaa	[_xrndn8.result.h]
	.end

;;;------------------------------------------------------------------------------
;;; xrndseed
;;; Sets a random seed number.
;;;  Format: xrndseed n
;;;  Param: n [IMM, DIR, IDX, EXT] .. 16bit number for random seed
;;;  Broken: X
;;;------------------------------------------------------------------------------
xrndseed:
	.macro	n
	ldx	n
	cpx	0
	bne	skip
	inx
skip:	stx	[_xrnd.result]
	.end

;;;------------------------------------------------------------------------------
;;; _xrndn8
;;;------------------------------------------------------------------------------
_xrndn8:
	.scope
	clrb
	stab	[shifter.h]
	staa	[shifter.l]
	stab	[result.h]
	stab	[result.l]
	jsr	_xrnd
	ldab	[_xrnd.result.h]
loop:
	clc
	rorb
	bcc	no_addition
	ldaa	[shifter.l]
	adda	[result.l]
	staa	[result.l]
	ldaa	[shifter.h]
	adca	[result.h]
	staa	[result.h]
	clc
no_addition:
	rol	[shifter.l]
	rol	[shifter.h]
	tstb
	bne	loop
done:
	rts
shifter.h:
	.equ	$
shifter.l:
	.equ	$+1
	.dw	0x0000
result.h:
	.equ	$
result.l:
	.equ	$+1
	.end
_xrndn8.result:
	.equ	$
_xrndn8.result.h:
	.equ	$
_xrndn8.result.l:
	.equ	$+1
	.dw	0x0000

;;;------------------------------------------------------------------------------
;;; _xrnd
;;;------------------------------------------------------------------------------
_xrnd:
	.scope
	clc
	ldaa	[result.h]
	rora
	ldaa	[result.l]
	rora
	eora	[result.h]
	staa	[result.h]
	ldaa	[result.l]
	rora
	ldaa	[result.h]
	rora
	eora	[result.l]
	staa	[result.l]
	eora	[result.h]
	staa	[result.h]
	rts
result.h:
	.equ	$
result.l:
	.equ	$+1
	.end
_xrnd.result:
	.equ	$
_xrnd.result.h:
	.equ	$
_xrnd.result.l:
	.equ	$+1
	.dw	0x0001
