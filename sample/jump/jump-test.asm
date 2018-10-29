	.org	0x1000

	bios.puts "bgt: "
	ldaa	4
	bios.puts "(4,3) .. "
	cmpa	3
	bgt	rel1
	bios.puts "false"
	bra	rel2
rel1:
	bios.puts "true "
rel2:	
	bios.puts "; (4,4) .. "
	cmpa	4
	bgt	rel3
	bios.puts "false"
	bra	rel4
rel3:	
	bios.puts "true "
rel4:	
	bios.puts "; (4,5) .. "
	cmpa	5
	bgt	rel5
	bios.puts "false"
	bra	rel6
rel5:	
	bios.puts "true "
rel6:	
	rts
	
	.include "jump.inc"
	.include "bios.inc"
