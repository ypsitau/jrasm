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
	char str[1024];
	UInt16 srcAddrStart = 0;
	UInt16 dstAddrStart = 0xd000 + _charCodeStart * 8;
	UInt16 dstAddrEnd = 0xd000 + GetCharCodeCur() * 8;
	::sprintf_s(str, _asmCodeForStore,
				_symbol.c_str(), dstAddrEnd, srcAddrStart, dstAddrStart);
	Parser parser(pathNameSrc);
	if (!parser.ParseString(str)) return nullptr;
	return parser.GetRoot()->Reference();
}

const char *PCGPage::_asmCodeForStore = R"**(pcgpage.%s.store:
        .macro
loop:
        ldx     [ptrsrc]
        ldaa    [x]
        inx
        stx     [ptrsrc]
        ldx     [ptrdst]
        staa    [x]
        inx
        stx     [ptrdst]
        cpx     0x%04x
        bne     loop
        bra     done
ptrsrc: .dw     0x%04x
ptrdst: .dw     0x%04x
done:
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
