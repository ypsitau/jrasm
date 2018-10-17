	.org	0x1000
;;; -----------------------------
	.save	a
	ldaa	0x11
	ldab	0x22
	ldx	0x3333
	.end
	
	.save	b
	ldaa	0x11
	ldab	0x22
	ldx	0x3333
	.end
	
	.save	x
	ldaa	0x11
	ldab	0x22
	ldx	0x3333
	.end
	
	.save	a,b
	ldaa	0x11
	ldab	0x22
	ldx	0x3333
	.end
	
	ldaa	0x11
	ldab	0x22
	ldx	0x3333
	
	.save	a,b,x
	ldaa	0x44
	ldab	0x55
	ldx	0x6666
	
	.save	a,b,x
	ldaa	0x77
	ldab	0x88
	ldx	0x9999
	
	.save	a,b,x
	ldaa	0xaa
	ldab	0xbb
	ldx	0xcccc
	.end

	.end

	.end

	macro1
	macro1
	macro1
	macro2
	macro2
macro1:
	.macro
	.save	a
	ldaa	0xaa
	.end
	.end

macro2:
	.macro
	.save	a
	ldaa	0xaa
	.end
	.end

