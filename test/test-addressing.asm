	.org	0x1000
label8bit$1:	.equ	0x11
label8bit$2:	.equ	0x22
label8bit$3:	.equ	0x33
label16bit$1:	.equ	0x1234
label16bit$2:	.equ	0x2345
label16bit$3:	.equ	0x3456
;;; number
	adc		a,0x11
	adc		a,0x11 + 0x22
	adc		a,0x11 + 0x22 + 0x33
	adc		a,{0x11}
	adc		a,{0x11 + 0x22}
	adc		a,{0x11 + 0x22 + 0x33}
	adc		a,[0x11 + x]
	adc		a,[0x11 + 0x22 + x]
	adc		a,[0x11 + 0x22 + 0x33 + x]
	adc		a,[x + 0x11]
	adc		a,[x + 0x11 + 0x22]
	adc		a,[x + 0x11 + 0x22 + 0x33]
	adc		a,[0x1234]
	adc		a,[0x1234 + 0x2345]
	adc		a,[0x1234 + 0x2345 + 0x3456]
;;; label reference
	adc		a,label8bit$1
	adc		a,label8bit$1 + label8bit$2
	adc		a,label8bit$1 + label8bit$2 + label8bit$3
	adc		a,{label8bit$1}
	adc		a,{label8bit$1 + label8bit$2}
	adc		a,{label8bit$1 + label8bit$2 + label8bit$3}
	adc		a,[label8bit$1 + x]
	adc		a,[label8bit$1 + label8bit$2 + x]
	adc		a,[label8bit$1 + label8bit$2 + label8bit$3 + x]
	adc		a,[x + label8bit$1]
	adc		a,[x + label8bit$1 + label8bit$2]
	adc		a,[x + label8bit$1 + label8bit$2 + label8bit$3]
	adc		a,[label16bit$1]
	adc		a,[label16bit$1 + label16bit$2]
	adc		a,[label16bit$1 + label16bit$2 + label16bit$3]
