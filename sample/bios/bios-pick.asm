	.org	0x1000

	bios.keyclick 0
loop:
	bios.pick
	bios.puthex.acca
	bios.puts "  \0"
	bra	loop

	.include "bios.inc"
