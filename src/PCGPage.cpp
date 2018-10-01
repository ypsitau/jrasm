//=============================================================================
// PCGPage.cpp
//=============================================================================
#include "stdafx.h"


//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
const PCGPattern *PCGPage::AddPCGPattern(const Binary &buff)
{
	AutoPtr<PCGPattern> pPCGPattern(new PCGPattern(_pcgType, GetCharCodeCur(), buff));
	_pcgPatternOwner.push_back(pPCGPattern->Reference());
	return pPCGPattern.release();
}

Expr *PCGPage::GenerateExpr(const char *pathNameSrc) const
{
	char asmCode[1024];
	UInt16 srcAddrStart = 0;
	UInt16 dstAddrStart = 0xd000 + _charCodeStart * 8;
	UInt16 dstAddrEnd = 0xd000 + GetCharCodeCur() * 8;
	::sprintf_s(asmCode, _asmCodeTmpl, GetSymbol(), srcAddrStart, dstAddrStart, dstAddrEnd);
	Parser parser(pathNameSrc);
	if (!parser.ParseString(asmCode)) return nullptr;
	return parser.GetRoot()->Reference();
}

const char *PCGPage::_asmCodeTmpl = R"**(pcgpage.%s.store:
        .macro
        ldx     0x%04x
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
