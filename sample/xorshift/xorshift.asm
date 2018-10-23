//==================================================================================
// 16bit Xorshift
// This code has been ported from a Z80 program released in the site:
// http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
//==================================================================================
	.org	0x1000
	ldx	0x2000
loop:
	jsr	xrnd
	ldaa	[xrnd.high]
	staa	[x]
	inx
	ldaa	[xrnd.high]
	staa	[x]
	inx
	cpx	0x2000+128*2
	bne	loop
	rts
	
xrnd:
	ror	[xrnd.high]
	ldaa	[xrnd.low]
	rora
	eora	[xrnd.high]
	staa	[xrnd.high]
	ror	[xrnd.low]
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
/*
xrnd:
  ld hl,1       ; seed must not be 0

  ld a,h
  rra
  ld a,l
  rra
  xor h
  ld h,a
  ld a,l
  rra
  ld a,h
  rra
  xor l
  ld l,a
	
  xor h
  ld h,a

  ld (xrnd+1),hl

  ret
*/
