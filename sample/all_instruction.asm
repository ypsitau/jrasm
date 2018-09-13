	aba
	adc		a,$12
	adc		a,($12)
	adc		a,[X+$12]
	adc		a,[$1234]
	adc		b,$12
	adc		b,($12)
	adc		b,[X+$12]
	adc		b,[$1234]
	adca	$12
	adca	($12)
	adca	[X+$12]
	adca	[$1234]
	adcb	$12
	adcb	($12)
	adcb	[X+$12]
	adcb	[$1234]
	add		a,$12
	add		a,($12)
	add		a,[X+$12]
	add		a,[$1234]
	add		b,$12
	add		b,($12)
	add		b,[X+$12]
	add		b,[$1234]
	adda	$12
	adda	($12)
	adda	[X+$12]
	adda	[$1234]
	addb	$12
	addb	($12)
	addb	[X+$12]
	addb	[$1234]
	and		a,$12
	and		a,($12)
	and		a,[X+$12]
	and		a,[$1234]
	and		b,$12
	and		b,($12)
	and		b,[X+$12]
	and		b,[$1234]
	anda	$12
	anda	($12)
	anda	[X+$12]
	anda	[$1234]
	andb	$12
	andb	($12)
	andb	[X+$12]
	andb	[$1234]
	asl		a
	asl		b
	asl		[X+$12]
	asl		[$1234]
	asla
	aslb
	asr		a
	asr		b
	asr		[X+$12]
	asr		[$1234]
	asra
	asrb
	bcc		$12
	bcs		$12
	beq		$12
	bge		$12
	bgt		$12
	bhi		$12
	bit		a,$12
	bit		a,($12)
	bit		a,[X+$12]
	bit		a,[$1234]
	bit		b,$12
	bit		b,($12)
	bit		b,[X+$12]
	bit		b,[$1234]
	bita	$12
	bita	($12)
	bita	[X+$12]
	bita	[$1234]
	bitb	$12
	bitb	($12)
	bitb	[X+$12]
	bitb	[$1234]
	ble		$12
	bls		$12
	blt		$12
	bmi		$12
	bne		$12
	bpl		$12
	bra		$12
	bsr		$12
	bvc		$12
	bvs		$12
	cba
	clc
	cli
	clr		a
	clr		b
	clr		[X+$12]
	clr		[$1234]
	clra
	clrb
	clv
	cmp		a,$12
	cmp		a,($12)
	cmp		a,[X+$12]
	cmp		a,[$1234]
	cmp		b,$12
	cmp		b,($12)
	cmp		b,[X+$12]
	cmp		b,[$1234]
	cmpa	$12
	cmpa	($12)
	cmpa	[X+$12]
	cmpa	[$1234]
	cmpb	$12
	cmpb	($12)
	cmpb	[X+$12]
	cmpb	[$1234]
	com		a
	com		b
	com		[X+$12]
	com		[$1234]
	coma
	comb
	cpx		($12)
	cpx		[X+$12]
	cpx		$1234
	cpx		[$1234]
	daa
	dec		a
	dec		b
	dec		[X+$12]
	dec		[$1234]
	deca
	decb
	des
	dex
	eor		a,$12
	eor		a,($12)
	eor		a,[X+$12]
	eor		a,[$1234]
	eor		b,$12
	eor		b,($12)
	eor		b,[X+$12]
	eor		b,[$1234]
	eora	$12
	eora	($12)
	eora	[X+$12]
	eora	[$1234]
	eorb	$12
	eorb	($12)
	eorb	[X+$12]
	eorb	[$1234]
	inc		a
	inc		b
	inc		[X+$12]
	inc		[$1234]
	inca
	incb
	ins
	inx
	jmp		X+$12
	jmp		$1234
	jsr		X+$12
	jsr		$1234
	lda		a,$12
	lda		a,($12)
	lda		a,[X+$12]
	lda		a,[$1234]
	lda		b,$12
	lda		b,($12)
	lda		b,[X+$12]
	lda		b,[$1234]
	ldaa	$12
	ldaa	($12)
	ldaa	[X+$12]
	ldaa	[$1234]
	ldab	$12
	ldab	($12)
	ldab	[X+$12]
	ldab	[$1234]
	lds		($12)
	lds		[X+$12]
	lds		$1234
	lds		[$1234]
	ldx		($12)
	ldx		[X+$12]
	ldx		$1234
	ldx		[$1234]
	lsr		a
	lsr		b
	lsr		[X+$12]
	lsr		[$1234]
	lsra
	lsrb
	neg		a
	neg		b
	neg		[X+$12]
	neg		[$1234]
	nega
	negb
	nop
	ora		a,$12
	ora		a,($12)
	ora		a,[X+$12]
	ora		a,[$1234]
	ora		b,$12
	ora		b,($12)
	ora		b,[X+$12]
	ora		b,[$1234]
	oraa	$12
	oraa	($12)
	oraa	[X+$12]
	oraa	[$1234]
	orab	$12
	orab	($12)
	orab	[X+$12]
	orab	[$1234]
	psh		a
	psh		b
	psha
	pshb
	pul		a
	pul		b
	pula
	pulb
	rol		a
	rol		b
	rol		[X+$12]
	rol		[$1234]
	rola
	rolb
	ror		a
	ror		b
	ror		[X+$12]
	ror		[$1234]
	rora
	rorb
	rti
	rts
	sba
	sbc		a,$12
	sbc		a,($12)
	sbc		a,[X+$12]
	sbc		a,[$1234]
	sbc		b,$12
	sbc		b,($12)
	sbc		b,[X+$12]
	sbc		b,[$1234]
	sbca	$12
	sbca	($12)
	sbca	[X+$12]
	sbca	[$1234]
	sbcb	$12
	sbcb	($12)
	sbcb	[X+$12]
	sbcb	[$1234]
	sec
	sei
	sev
	sta		a,($12)
	sta		a,[X+$12]
	sta		a,[$1234]
	sta		b,($12)
	sta		b,[X+$12]
	sta		b,[$1234]
	staa	($12)
	staa	[X+$12]
	staa	[$1234]
	stab	($12)
	stab	[X+$12]
	stab	[$1234]
	sts		($12)
	sts		[X+$12]
	sts		[$1234]
	stx		($12)
	stx		[X+$12]
	stx		[$1234]
	sub		a,$12
	sub		a,($12)
	sub		a,[X+$12]
	sub		a,[$1234]
	sub		b,$12
	sub		b,($12)
	sub		b,[X+$12]
	sub		b,[$1234]
	suba	$12
	suba	($12)
	suba	[X+$12]
	suba	[$1234]
	subb	$12
	subb	($12)
	subb	[X+$12]
	subb	[$1234]
	swi
	tab
	tap
	tpa
	tst		a
	tst		b
	tst		[X+$12]
	tst		[$1234]
	tsta
	tstb
	tsx
	txs
	wai
