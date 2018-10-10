	.org	0x1000
	
	.dw	0x1234 + 0x3456
	.dw	0x3456 - 0x1234
	.dw	0x1234 * 3
	.dw	0xabcd / 3
	.dw	0xabcd % 19
	.dw	0b1111000010101010 | 0b0011001101010111
	.dw	0b1111000010101010 & 0b0011001101010111
	.dw	0b1111000010101010 ^ 0b0011001101010111
	.dw	0x0234 << 4
	.dw	0x1234 >> 4
	.dw	(12 + 34) * 89
	.dw	89 * (12 + 34)
	.dw	12 * 34 + 89
	.dw	12 + 34 + 89
	.dw	12 + 34 * 89
	
	.scope
num1:	.equ	0x200
num2:	.equ	0x300
num3:	.equ	0x30
	ldaa	[0x10] <+> 0x20
	ldaa	[x+0x10] <+> 0x20
	ldaa	[0x10+x] <+> 0x20
	ldaa	[num1] <+> 0x20
	ldaa	[num1+num2] <+> 0x20
	ldaa	[num2-num1] <+> 0x20
	ldaa	[num1+num2*3] <+> 0x20
	ldaa	[0x10] <+> num3
	ldaa	[x+0x10] <+> num3
	ldaa	[0x10+x] <+> num3
	ldaa	[num1] <+> num3
	ldaa	[num1+num2] <+> num3
	ldaa	[num2-num1] <+> num3
	ldaa	[num1+num2*3] <+> num3
	.end
