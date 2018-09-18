	.org	0x1000
label1:	
	.equ	0x1234
label2:	.equ	label1

label3:	.equ	0xabcd
label4:
	ldx		label1
label5:
	ldx		label2
label6:
	ldx		label3
label7:
	ldx		label4
label8:
	.equ	label6
	nop
	ldx		label5
	ldx		label6
	ldx		label7
