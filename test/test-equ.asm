label1:	
	.equ	0x1234
label2:	.equ	label1

label3:	.equ	0xabcd
label4:
	ldaa	0
label5:
	ldaa	0
label6:
	ldaa	0
label7:
	ldaa	0
label8:
	.equ	label6
	nop
