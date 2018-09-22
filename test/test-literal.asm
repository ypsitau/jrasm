	.org	0x1000
	;; binary number
	ldx		0b0
	ldx		0b1
	ldx		0b111
	ldx		0b11111111
	ldx		0b10101010
	ldx		0b1010101001010101
	ldx		0b1111111111111111
	;; octal number
	ldx		00
	ldx		01
	ldx		07
	ldx		011
	ldx		0377
	ldx		0177777
	;; decimal number
	ldx		0
	ldx		1
	ldx		11
	ldx		255
	ldx		65535
	;; hexadecimal number
	ldx		0x0
	ldx		0x1
	ldx		0xb
	ldx		0xff
	ldx		0xffff
	;; character literal
	ldaa	'A'
	ldaa	'B'
	ldaa	'C'
	ldaa	'D'
	ldaa	'E'
