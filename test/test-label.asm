			.org	0x1000
label_89ab:	.equ	0x89ab
label_cdef:	.equ	0xcdef
label_1000:
			nop
label_1001:
			ldaa	0x12
label_1003:
label_1004:	.equ	$+1
label_1005:	.equ	$+2
			ldx		0x1234
label_1006:
label_100a:	.equ	$+4
			.db		1, 2, 3, 4, 5, 6, 7, 8
label_100e:
			.dw		label_89ab
			.dw		label_cdef
			.dw		label_1000
			.dw		label_1001
			.dw		label_1003
			.dw		label_1004
			.dw		label_1005
			.dw		label_1006
			.dw		label_100a
			.dw		label_100e
			.dw		Label_cdef
			.dw		LABEL_CDEF
