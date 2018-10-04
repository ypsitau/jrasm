	.org	0x1000
label1:
	;; binary number
	.dw		0b0
	.dw		0b1
	.dw		0b111
	.dw		0b11111111
	.dw		0b10101010
	.dw		0b1010101001010101
	.dw		0b1111111111111111
	.dw		-0b0
	.dw		-0b1
	.dw		-0b111
	.dw		-0b11111111
	.dw		-0b10101010
	.dw		-0b1000000000000000
label2:
	;; octal number
	.dw		00
	.dw		01
	.dw		07
	.dw		011
	.dw		0377
	.dw		0177777
	.dw		-00
	.dw		-01
	.dw		-07
	.dw		-011
	.dw		-0377
	.dw		-0100000
label3:
	;; decimal number
	.dw		0
	.dw		1
	.dw		11
	.dw		255
	.dw		65535
	.dw		-0
	.dw		-1
	.dw		-11
	.dw		-255
	.dw		-32768
label4:
	;; hexadecimal number
	.dw		0x0
	.dw		0x1
	.dw		0xb
	.dw		0xff
	.dw		0xffff
	.dw		-0x0
	.dw		-0x1
	.dw		-0xb
	.dw		-0xff
	.dw		-0x8000
label5:
	;; character literal
	.db		' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/'
	.db		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?'
	.db		'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'
	.db		'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_'
	.db		'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o'
	.db		'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
	.db		'\r', '\n', '\0'
label6:
	;; string literal
	.db		" !\"#$%&'()*+,-./0123456789:;<=>?"
	.db		"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
	.db		"`abcdefghijklmnopqrstuvwxyz{|}~"
	.db		"\r\n\0"
label7:
	;; bit-pattern literal
	.db	b"........"
	.db	b".......#"
	.db	b"......#."
	.db	b".....#.."
	.db	b"....#..."
	.db	b"...#...."
	.db	b"..#....."
	.db	b".#......"
	.db	b"#......."
	.db	b"########"
	.db	b"#.#.#.#."
	.db	b"#.#......##...#."
	.db	b"#.#......##...#.#.#.#.#.########..#....."
label8:
