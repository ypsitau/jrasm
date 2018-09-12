	aba
	adc		a,$12
	adc		a,($12)
	adc		a,[X+$12]
	adc		a,[$1234]
	adc		b,$12
	adc		b,($12)
	adc		b,[X+$12]
	adc		b,[$1234]
	add		a,$12
	add		a,($12)
	add		a,[X+$12]
	add		a,[$1234]
	add		b,$12
	add		b,($12)
	add		b,[X+$12]
	add		b,[$1234]
	and		a,$12
	and		a,($12)
	and		a,[X+$12]
	and		a,[$1234]
	and		b,$12
	and		b,($12)
	and		b,[X+$12]
	and		b,[$1234]
	asl		a
	asl		b
	asl		[X+$12]
	asl		[$1234]
	asr		a
	asr		b
	asr		[X+$12]
	asr		[$1234]
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
	clv
	cmp		a,$12
	cmp		a,($12)
	cmp		a,[X+$12]
	cmp		a,[$1234]
	cmp		b,$12
	cmp		b,($12)
	cmp		b,[X+$12]
	cmp		b,[$1234]
	com		a
	com		b
	com		[X+$12]
	com		[$1234]
	cpx		($12)
	cpx		[X+$12]
	cpx		$1234
	cpx		[$1234]
	daa
	dec		a
	dec		b
	dec		[X+$12]
	dec		[$1234]
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
	inc		a
	inc		b
	inc		[X+$12]
	inc		[$1234]
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
	neg		a
	neg		b
	neg		[X+$12]
	neg		[$1234]
	nop
	ora		a,$12
	ora		a,($12)
	ora		a,[X+$12]
	ora		a,[$1234]
	ora		b,$12
	ora		b,($12)
	ora		b,[X+$12]
	ora		b,[$1234]
	psh		a
	psh		b
	pul		a
	pul		b
	rol		a
	rol		b
	rol		[X+$12]
	rol		[$1234]
	ror		a
	ror		b
	ror		[X+$12]
	ror		[$1234]
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
	sec
	sei
	sev
	sta		a,($12)
	sta		a,[X+$12]
	sta		a,[$1234]
	sta		b,($12)
	sta		b,[X+$12]
	sta		b,[$1234]
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
	swi
	tab
	tap
	tpa
	tst		a
	tst		b
	tst		[X+$12]
	tst		[$1234]
	tsx
	txs
	wai
