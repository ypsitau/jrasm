	.org	0x1000
	
	.dw		0x1234 + 0x3456
	.dw		0x3456 - 0x1234
	.dw		0x1234 * 3
	.dw		0xabcd / 3
	.dw		0xabcd % 19
	.dw		0b1111000010101010 | 0b0011001101010111
	.dw		0b1111000010101010 & 0b0011001101010111
	.dw		0b1111000010101010 ^ 0b0011001101010111
	.dw		0x0234 << 4
	.dw		0x1234 >> 4
	.dw		(12 + 34) * 89
	.dw		89 * (12 + 34)
	.dw		12 * 34 + 89
	.dw		12 + 34 + 89
	.dw		12 + 34 * 89