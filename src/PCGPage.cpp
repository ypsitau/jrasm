//=============================================================================
// PCGPage.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
PCGChar *PCGPage::CreatePCGChar(const Binary &buff)
{
	const PCGChar *pPCGCharFound = _pcgCharOwner.FindSamePattern(buff);
	if (pPCGCharFound != nullptr) return pPCGCharFound->Reference();
	AutoPtr<PCGChar> pPCGChar(new PCGChar(_pcgType, GetCharCodeCur(), buff));
	_pcgCharOwner.push_back(pPCGChar->Reference());
	return pPCGChar.release();
}

Expr *PCGPage::ComposeExpr() const
{
	char asmCode[1024];
	const char *asmCodeTmpl = "";
	UInt16 dstAddrStart = 0x0000;
	UInt16 dstAddrEnd = 0x0000;
	if (_pcgType == PCGTYPE_CRAM) {
		asmCodeTmpl = _asmCodeTmpl1;
		dstAddrStart = 0xd000 + _charCodeStart * 8;
		dstAddrEnd = 0xd000 + GetCharCodeCur() * 8;
	} else if (_charCodeStart < 64 && GetCharCodeCur() <= 64) { // _pcgType == PCGTYPE_User
		asmCodeTmpl = _asmCodeTmpl1;
		dstAddrStart = 0xc000 + (_charCodeStart - 32) * 8;
		dstAddrEnd = 0xc000 + (GetCharCodeCur() - 32) * 8;
	} else if (_charCodeStart < 64 && GetCharCodeCur() > 64) { // _pcgType == PCGTYPE_User
		asmCodeTmpl = _asmCodeTmpl2;
		dstAddrStart = 0xc000 + (_charCodeStart - 32) * 8;
		dstAddrEnd = 0xc400 + (GetCharCodeCur() - 64) * 8;
	} else { // _charCodeStart >= 64 && _pcgType == PCGTYPE_User
		asmCodeTmpl = _asmCodeTmpl1;
		dstAddrStart = 0xc400 + (_charCodeStart - 64) * 8;
		dstAddrEnd = 0xc400 + (GetCharCodeCur() - 64) * 8;
	}
	::sprintf_s(asmCode, asmCodeTmpl, GetSymbol(), GetSymbol(), GetSymbol(), dstAddrStart, dstAddrEnd);
	Parser parser("***PCGPage.cpp***");
	if (!parser.ParseString(asmCode)) return nullptr;
	return parser.GetRoot()->Reference();
}

const char *PCGPage::_asmCodeTmpl1 = R"**(pcgpage.%s.src:
pcgpage.%s.store:
        .macro
        ldx     pcgpage.%s.src
        stx     [ptrsrc]
        ldx     0x%04x
        stx     [ptrdst]
loop:
ptrsrc: .equ    $+1
        ldx     0x0000
        ldaa    [x]
        inx
        stx     [ptrsrc]
ptrdst: .equ    $+1
        ldx     0x0000
        staa    [x]
        inx
        stx     [ptrdst]
        cpx     0x%04x
        bne     loop
        .end
)**";

const char *PCGPage::_asmCodeTmpl2 = R"**(pcgpage.%s.src:
pcgpage.%s.store:
        .macro
        ldx     pcgpage.%s.src
        stx     [ptrsrc1]
        ldx     0x%04x
        stx     [ptrdst1]
loop1:
ptrsrc1:.equ    $+1
        ldx     0x0000
        ldaa    [x]
        inx
        stx     [ptrsrc1]
ptrdst1:.equ    $+1
        ldx     0x0000
        staa    [x]
        inx
        stx     [ptrdst1]
        cpx     0xc100
        bne     loop1
        ldx     [ptrsrc1]
        stx     [ptrsrc2]
        ldx     0xc400
        stx     [ptrdst2]
loop2:
ptrsrc2:.equ    $+1
        ldx     0x0000
        ldaa    [x]
        inx
        stx     [ptrsrc2]
ptrdst2:.equ    $+1
        ldx     0x0000
        staa    [x]
        inx
        stx     [ptrdst2]
        cpx     0x%04x
        bne     loop2
        .end
)**";

//-----------------------------------------------------------------------------
// PCGPageList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCGPageOwner
//-----------------------------------------------------------------------------
PCGPageOwner::~PCGPageOwner()
{
	Clear();
}

void PCGPageOwner::Clear()
{
	for (auto pPCGPage : *this) {
		PCGPage::Delete(pPCGPage);
	}
	clear();
}
