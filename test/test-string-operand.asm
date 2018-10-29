	.org	0x1000
	.dseg
dseg_top:
	.dw	dseg_top
	.cseg
	ldx	"ABCDEFG\0"
	ldx	"HIJKLMN\0"
	ldx	"OPQRSTU\0"

	macro1	"VWXYZ@@\0"
	macro1	"VWXYZ@@\0"

	macro2
	macro2
	
	.dseg
work1:	.db	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07

	.cseg
	ldx	"ABCDEFG\0"

macro1:	
	.macro str
	ldx	str
	.end

macro2:
	.macro
	ldx	"abcdefg\0"
	.end
