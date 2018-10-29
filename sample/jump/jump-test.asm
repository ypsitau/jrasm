	.org	0x1000

	test.cmp beq,"beq\0"
	test.cmp jeq,"jeq\0"
	test.cmp bne,"bne\0"
	test.cmp jne,"jne\0"
	test.cmp bgt,"bgt\0"
	test.cmp jgt,"jgt\0"
	test.cmp bge,"bge\0"
	test.cmp jge,"jge\0"
	test.cmp blt,"blt\0"
	test.cmp jlt,"jlt\0"
	test.cmp ble,"ble\0"
	test.cmp jle,"jle\0"
	test.cmp bhi,"bhi\0"
	test.cmp jhi,"jhi\0"
	test.cmp bls,"bls\0"
	test.cmp jls,"jls\0"
	test.cmp bmi,"bmi\0"
	test.cmp jmi,"jmi\0"
	test.cmp bpl,"bpl\0"
	test.cmp jpl,"jpl\0"
	rts

test.cmp:
	.macro	inst,title
	bios.puts title
	bios.puts " .. \0"
	bios.puts "4-3:\0"
	ldaa	4
	cmpa	3
	inst	rel1
	ldaa	' '
	bios.putc
	bra	rel2
rel1:
	ldaa	'X'
	bios.putc
rel2:	
	bios.puts " 4-4:\0"
	ldaa	4
	cmpa	4
	inst	rel3
	ldaa	' '
	bios.putc
	bra	rel4
rel3:	
	ldaa	'X'
	bios.putc
rel4:	
	bios.puts " 4-5:\0"
	ldaa	4
	cmpa	5
	inst	rel5
	ldaa	' '
	bios.putc
	bra	rel6
rel5:	
	ldaa	'X'
	bios.putc
rel6:	
	ldaa	'\r'
	bios.putc
	.end

	.include "jump.inc"
	.include "bios.inc"
