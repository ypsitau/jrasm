;;;=============================================================================
;;; xrnd-test.asm
;;;=============================================================================
	.org	0x1000

	bios.cls
	xrndseed {0x04}	// use the 0.1sec timer as random seed

	bios.color 0,7

	.scope
posx:	.equ	0
	bios.locate posx,23
	bios.puts "0-1\0"
	.end

	.scope
posx:	.equ	6
	bios.locate posx,23
	bios.puts "0-15\0"
	.end

	.scope
posx:	.equ	12
	bios.locate posx,23
	bios.puts "0-255\0"
	.end

	bios.color 7,0

loop:
	bios.scrollup 0xc3c0

	.scope
posx:	.equ	0
	bios.locate posx,22
	xrndn.mb 2
	bios.puthex.a
	.end

	.scope
posx:	.equ	6
	bios.locate posx,22
	xrndn.mb 16
	bios.puthex.a
	.end

	.scope
posx:	.equ	12
	bios.locate posx,22
	xrndn.mb
	bios.puthex.a
	.end

	bra 	loop

	.include "xrnd.inc"
	.include "bios.inc"
