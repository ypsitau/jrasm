//=============================================================================
// PCGPage.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
bool PCGPage::GenerateCharCode(int *pCharCode, PCGType *pPCGType)
{
	for ( ; _iPCGRangeCur < _pPCGRangeOwner->size(); _iPCGRangeCur++) {
		PCGRange *pPCGRange = (*_pPCGRangeOwner)[_iPCGRangeCur];
		if (pPCGRange->GetCharCodeCur() < pPCGRange->GetCharCodeEnd()) {
			*pCharCode = pPCGRange->GetCharCodeCur();
			*pPCGType = pPCGRange->GetPCGType();
			pPCGRange->ForwardCharCodeCur();
			return true;
		}
	}
	ErrorLog::AddError("number of characters exceeds the range specified by .PCGPAGE");
	return false;
}

PCGChar *PCGPage::CreatePCGChar(const Binary &buff, int)
{
	const PCGChar *pPCGCharFound = _pcgCharOwner.FindSamePattern(buff);
	if (pPCGCharFound != nullptr) return pPCGCharFound->Reference();
	int charCode;
	PCGType pcgType;
	if (!GenerateCharCode(&charCode, &pcgType)) return nullptr;
	AutoPtr<PCGChar> pPCGChar(new PCGChar(pcgType, charCode, buff));
	_pcgCharOwner.push_back(pPCGChar->Reference());
	return pPCGChar.release();
}

Expr *PCGPage::ComposeExpr() const
{
	char buff[1024];
	String asmCode;
	UInt16 dstAddrStart = 0x0000;
	UInt16 dstAddrEnd = 0x0000;
	const char *formatDW = "        .dw     0x%04x,0x%04x\n";
	::sprintf_s(buff, _asmCodeTmpl, GetSymbol(), GetSymbol(), GetSymbol());
	asmCode += buff;
	for (auto pPCGRange : GetPCGRangeOwner()) {
		int charCodeStart = pPCGRange->GetCharCodeStart();
		int charCodeCur = pPCGRange->GetCharCodeCur();
		if (pPCGRange->GetPCGType() == PCGTYPE_CRAM) {
			dstAddrStart = 0xd000 + charCodeStart * 8;
			dstAddrEnd = 0xd000 + charCodeCur * 8;
			::sprintf_s(buff, formatDW, dstAddrStart, dstAddrEnd);
			asmCode += buff;
		} else if (charCodeStart < 64 && charCodeCur <= 64) { // _pcgType == PCGTYPE_User
			dstAddrStart = 0xc000 + (charCodeStart - 32) * 8;
			dstAddrEnd = 0xc000 + (charCodeCur - 32) * 8;
			::sprintf_s(buff, formatDW, dstAddrStart, dstAddrEnd);
			asmCode += buff;
		} else if (charCodeStart < 64 && charCodeCur > 64) { // _pcgType == PCGTYPE_User
			dstAddrStart = 0xc000 + (charCodeStart - 32) * 8;
			dstAddrEnd = 0xc100;
			::sprintf_s(buff, formatDW, dstAddrStart, dstAddrEnd);
			asmCode += buff;
			dstAddrStart = 0xc400;
			dstAddrEnd = 0xc400 + (charCodeCur - 64) * 8;
			::sprintf_s(buff, formatDW, dstAddrStart, dstAddrEnd);
			asmCode += buff;
		} else { // charCodeStart >= 64 && _pcgType == PCGTYPE_User
			dstAddrStart = 0xc400 + (charCodeStart - 64) * 8;
			dstAddrEnd = 0xc400 + (charCodeCur - 64) * 8;
			::sprintf_s(buff, formatDW, dstAddrStart, dstAddrEnd);
			asmCode += buff;
		}
	}
	asmCode += "        .dw     0x0000\n";
	asmCode += "done:   .end\n";
	Parser parser("***PCGPage.cpp***");
	if (!parser.ParseString(asmCode.c_str())) return nullptr;
	return parser.GetExprRoot()->Reference();
}

// DON'T use .SAVE macro here
const char *PCGPage::_asmCodeTmpl = R"**(pcgpage.%s.src:
pcgpage.%s.store:
        .macro
        ldx     pcgpage.%s.src
        stx     [srcp]
        ldx     entries
loop_nextentry:
        stx     [save2_x]
        stx     [save1_x]
        ldx     [x]
        beq     done
        stx     [dstp]
save1_x:
        .equ    $+1
        ldx     0x0000
        ldx     [x+2]
        stx     [dstp_end]
loop_copy:
srcp:
        .equ    $+1
        ldx     0x0000
        ldaa    [x]
        ldab    [x+1]
        inx
        inx
        stx     [srcp]
dstp:
        .equ    $+1
        ldx     0x0000
        staa    [x]
        stab    [x+1]
        inx
        inx
        stx     [dstp]
dstp_end:
        .equ    $+1
        cpx     0x0000
        bne     loop_copy
save2_x:
        .equ    $+1
        ldx     0x0000
        inx
        inx
        inx
        inx
        bra     loop_nextentry
entries:
)**";

//-----------------------------------------------------------------------------
// PCGPageList
//-----------------------------------------------------------------------------
PCGPage *PCGPageList::FindBySymbol(const char *symbol) const
{
	for (auto pPCGPage : *this) {
		if (::strcasecmp(pPCGPage->GetSymbol(), symbol) == 0) return pPCGPage;
	}
	return nullptr;
}

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
