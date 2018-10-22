#include <stdio.h>

/*
xrnd:
  ld hl,1       ; seed must not be 0

  ld a,h
  rra
  ld a,l
  rra
  xor h
  ld h,a
  ld a,l
  rra
  ld a,h
  rra
  xor l
  ld l,a
  xor h
  ld h,a

  ld (xrnd+1),hl

  ret
*/

typedef unsigned char UInt8;
typedef unsigned short UInt16;

UInt16 xrnd1();
UInt16 xrnd2();

int main()
{
	for (int i = 0; i < 1000; i++) {
		::printf("%04x %04x\n", xrnd1(), xrnd2());
	}
	return 0;
}

UInt16 xrnd1()
{
	static UInt16 xs = 1;
	xs ^= xs << 7;
	xs ^= xs >> 9;
	xs ^= xs << 8;
	return xs;	
}

UInt16 xrnd2()
{
	static UInt16 hl = 1;
	UInt8 h = static_cast<UInt8>(hl >> 8);
	UInt8 l = static_cast<UInt8>(hl);
	UInt8 a, carry, carryNext;
	a = h;
	carryNext = a & 1; a = (a >> 1) | (carry? 0x80 : 0); carry = carryNext;
	a = l;
	carryNext = a & 1; a = (a >> 1) | (carry? 0x80 : 0); carry = carryNext;
	a = a ^ h;
	h = a;
	a = l;
	carryNext = a & 1; a = (a >> 1) | (carry? 0x80 : 0); carry = carryNext;
	a = h;
	carryNext = a & 1; a = (a >> 1) | (carry? 0x80 : 0); carry = carryNext;
	a = a ^ l;
	l = a;
	a = a ^ h;
	h = a;
	hl = (static_cast<UInt16>(h) << 8) | l;
	return hl;
}
