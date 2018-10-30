	.org	0x1000

	bios.keyclick 0

	bios.color 7,0
	bios.cls
	
	bios.color 0,7
	bios.putc 'A'
	bios.color 1,0
	bios.putc 'B'
	bios.color 2,0
	bios.putc 'C'
	bios.color 3,0
	bios.putc 'D'
	bios.color 4,0
	bios.putc 'E'
	bios.color 5,0
	bios.putc 'F'
	bios.color 6,0
	bios.putc 'G'
	bios.color 7,0
	bios.putc 'H'

	bios.locate 10,2

	bios.color 0,7
	bios.putc 'A'
	bios.color 1,7
	bios.putc 'B'
	bios.color 2,7
	bios.putc 'C'
	bios.color 3,7
	bios.putc 'D'
	bios.color 4,7
	bios.putc 'E'
	bios.color 5,7
	bios.putc 'F'
	bios.color 6,7
	bios.putc 'G'
	bios.color 7,1
	bios.putc 'H'
	
	bios.color 0,7
	bios.locate 10,10
	bios.puts "Hello, World!\0"

	bios.locate 0,3
	bios.color 7,0
loop:
	bra	loop
	
	.include "bios.inc"
