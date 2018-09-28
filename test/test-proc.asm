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
	.proc
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
	.proc
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
	.proc
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
	
