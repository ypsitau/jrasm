	.org	0x1000
label1:
	nop
label2:
	nop
label3:
	.equ	0x4321
label4:
	.equ	0x8765
	.dw		label1
	.dw		label2
	.dw		label3
	.dw		label4
	.dw		func1
	.dw		func1_inside
	.dw		func2
	.dw		func2_inside
	.dw		func3
	.dw		func3_inside
	
;;; -----------------------------
func1:
	.scope
label1:
	nop
label2:	
func1_inside::
	nop
label3:
	.equ	0x1234
	.dw		label1
	.dw		label2
	.dw		label3
	.dw		label4
	.end
	
;;; -----------------------------
func2:	
	.scope
label1:	
	nop
label2:	
func2_inside::
	nop
label3:
	.equ	0x5678
	.dw		label1
	.dw		label2
	.dw		label3
	.dw		label4
	.end

;;; -----------------------------
func3:	
	.scope
label1:	
	nop
label2:	
func3_inside::
	nop
label3:
	.equ	0xabcd
	.dw		label1
	.dw		label2
	.dw		label3
	.dw		label4
	.end
	
;;; -----------------------------
	.scope	a
	ldaa	0x11
	ldab	0x22
	ldx		0x3333
	.end
	
	.scope	b
	ldaa	0x11
	ldab	0x22
	ldx		0x3333
	.end
	
	.scope	x
	ldaa	0x11
	ldab	0x22
	ldx		0x3333
	.end
	
	.scope	a,b
	ldaa	0x11
	ldab	0x22
	ldx		0x3333
	.end
	
	ldaa	0x11
	ldab	0x22
	ldx		0x3333
	
	.scope	a,b,x
	ldaa	0x44
	ldab	0x55
	ldx		0x6666
	
	.scope	a,b,x
	ldaa	0x77
	ldab	0x88
	ldx		0x9999
	
	.scope	a,b,x
	ldaa	0xaa
	ldab	0xbb
	ldx		0xcccc
	.end

	.end
	
	.end
