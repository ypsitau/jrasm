;;;==================================================================================
;;; 16bit Xorshift
;;; This code has been ported from a Z80 program released in the site:
;;; http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
;;;==================================================================================

;;;------------------------------------------------------------------------------
;;; xrnd
;;; Generates an 8bit random number within a range from 0 to n-1.
;;;  Format: xrnd n
;;;  Param: n [IMM, DIR, IDX, EXT] .. Range of random number
;;;  Return: a
;;;  Broken: b
;;;------------------------------------------------------------------------------
xrndn:
	.macro	n
	ldaa	n
	jsr	_xrndn
	ldaa	[_xrndn.result]
	.end
	
;;;------------------------------------------------------------------------------
;;; _xrndn
;;;------------------------------------------------------------------------------
_xrndn:
	.scope
	clrb
	stab	[work.h]
	staa	[work.l]
	stab	[result.h]
	stab	[result.l]
	jsr	_xrnd
	ldab	[_xrnd.h]
loop:
	clc
	rorb
	bcc	no_addition
	ldaa	[work.l]
	adda	[result.l]
	staa	[result.l]
	ldaa	[work.h]
	adca	[result.h]
	staa	[result.h]
	clc
no_addition:
	rol	[work.l]
	rol	[work.h]
	tstb
	bne	loop
done:
	rts
work.h:
	.equ	$
work.l:
	.equ	$+1
	.dw	0x0000
result.h:
	.equ	$
result.l:
	.equ	$+1
	.end
_xrndn.result:
	.dw	0x0000

;;;------------------------------------------------------------------------------
;;; _xrnd
;;;------------------------------------------------------------------------------
_xrnd:
	clc
	ldaa	[_xrnd.h]
	rora
	ldaa	[_xrnd.l]
	rora
	eora	[_xrnd.h]
	staa	[_xrnd.h]
	ldaa	[_xrnd.l]
	rora
	ldaa	[_xrnd.h]
	rora
	eora	[_xrnd.l]
	staa	[_xrnd.l]
	eora	[_xrnd.h]
	staa	[_xrnd.h]
	rts
_xrnd.word:
	.equ	$
_xrnd.h:
	.equ	$
_xrnd.l:
	.equ	$+1
	.dw	0x0001
