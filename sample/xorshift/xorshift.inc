//==================================================================================
// 16bit Xorshift
// This code has been ported from a Z80 program released in the site:
// http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
//==================================================================================
xrndn:
	.scope
	clrb
	stab	[work]
	staa	[work+1]
	stab	[result]
	stab	[result+1]
	jsr	xrnd
	ldab	[xrnd.high]
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
	ldaa	[result]
	rts
work:	.dw	0x0000
result:	.dw	0x0000
	.end

xrnd:
	clc
	ldaa	[xrnd.high]
	rora
	ldaa	[xrnd.low]
	rora
	eora	[xrnd.high]
	staa	[xrnd.high]
	ldaa	[xrnd.low]
	rora
	ldaa	[xrnd.high]
	rora
	eora	[xrnd.low]
	staa	[xrnd.low]
	eora	[xrnd.high]
	staa	[xrnd.high]
	rts
xrnd.word:
	.equ	$
xrnd.high:
	.equ	$
xrnd.low:
	.equ	$+1
	.dw	0x0001
