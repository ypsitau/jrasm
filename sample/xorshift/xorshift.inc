;;;==================================================================================
;;; 16bit Xorshift
;;; This code has been ported from a Z80 program released in the site:
;;; http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
;;;==================================================================================

;;;------------------------------------------------------------------------------
;;; xrnd
;;; 
;;; Format: xrnd n
;;; Param: n [IMM, DIR, IDX, EXT] .. Range of random number
;;; Return: a
;;; Broken: b
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
	stab	[work]
	staa	[work+1]
	stab	[result]
	stab	[result+1]
	jsr	_xrnd
	ldab	[_xrnd.high]
loop:
	clc
	rorb
	bcc	no_addition
	ldaa	[work+1]
	adda	[result+1]
	staa	[result+1]
	ldaa	[work]
	adca	[result]
	staa	[result]
	clc
no_addition:
	rol	[work+1]
	rol	[work]
	tstb
	bne	loop
done:
	rts
work:	.dw	0x0000
result:
	.end
_xrndn.result:
	.dw	0x0000

;;;------------------------------------------------------------------------------
;;; _xrnd
;;;------------------------------------------------------------------------------
_xrnd:
	clc
	ldaa	[_xrnd.high]
	rora
	ldaa	[_xrnd.low]
	rora
	eora	[_xrnd.high]
	staa	[_xrnd.high]
	ldaa	[_xrnd.low]
	rora
	ldaa	[_xrnd.high]
	rora
	eora	[_xrnd.low]
	staa	[_xrnd.low]
	eora	[_xrnd.high]
	staa	[_xrnd.high]
	rts
_xrnd.word:
	.equ	$
_xrnd.high:
	.equ	$
_xrnd.low:
	.equ	$+1
	.dw	0x0001
