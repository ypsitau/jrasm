	.org	0x1000
	
	ldx	vram
	bios.puthex.x
	bios.putc '\r'

	bios.puts "0,0 .. \0"
	vram.fromxy 0,0
	bios.puthex.x
	bios.putc '\r'

	bios.puts "1,0 .. \0"
	vram.fromxy 1,0
	bios.puthex.x
	bios.putc '\r'

	bios.puts "31,0 .. \0"
	vram.fromxy 31,0
	bios.puthex.x
	bios.putc '\r'

	bios.puts "0,1 .. \0"
	vram.fromxy 0,1
	bios.puthex.x
	bios.putc '\r'

	bios.puts "0,24 .. \0"
	vram.fromxy 0,24
	bios.puthex.x
	bios.putc '\r'

	bios.puts "31,24 .. \0"
	vram.fromxy 31,24
	bios.puthex.x
	bios.putc '\r'

	rts
	
	.include "bios.inc"
	.include "vram32x24x2.inc"
