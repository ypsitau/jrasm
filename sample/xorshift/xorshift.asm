	.org	0x1000
	jsr	xrnd
	
	

xrnd:
	.scope
	ror	[high]
	ldaa	[low]
	rora
	eora	[high]
	staa	[high]
	ror	[low]
	ldaa	[high]
	rora
	eora	[low]
	staa	[low]
	eora	[high]
	staa	[high]
high:	.equ	$+1
low:	.equ	$+2
	ldx	0x0001
	rts
	.end
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
