	.org	0x1000
	;; binary number
	.dw		0b0
	.dw		0b1
	.dw		0b111
	.dw		0b11111111
	.dw		0b10101010
	.dw		0b1010101001010101
	.dw		0b1111111111111111
	;; octal number
	.dw		00
	.dw		01
	.dw		07
	.dw		011
	.dw		0377
	.dw		0177777
	;; decimal number
	.dw		0
	.dw		1
	.dw		11
	.dw		255
	.dw		65535
	;; hexadecimal number
	.dw		0x0
	.dw		0x1
	.dw		0xb
	.dw		0xff
	.dw		0xffff
	;; character literal
	.db		'A'
	.db		'B'
	.db		'C'
	.db		'D'
	.db		'E'
	;; string literal
	.db		" !\"#$%&'()*+,-./0123456789:;<=>?"
	.db		"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
	.db		"`abcdefghijklmnopqrstuvwxyz{|}~"
	.db		"\r\n\0"
