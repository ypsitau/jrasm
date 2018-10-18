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

#if 1
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

#else

Expr *PCGPage::ComposeExpr() const
{
	char asmCode[1024];
	const char *asmCodeTmpl = "";
	UInt16 dstAddrStart = 0x0000;
	UInt16 dstAddrEnd = 0x0000;
	int charCodeStart = _pPCGRangeOwner->front()->GetCharCodeStart();
	int charCodeCur = _pPCGRangeOwner->front()->GetCharCodeCur();
	if (GetPCGType() == PCGTYPE_CRAM) {
		asmCodeTmpl = _asmCodeTmpl1;
		dstAddrStart = 0xd000 + charCodeStart * 8;
		dstAddrEnd = 0xd000 + charCodeCur * 8;
	} else if (charCodeStart < 64 && charCodeCur <= 64) { // _pcgType == PCGTYPE_User
		asmCodeTmpl = _asmCodeTmpl1;
		dstAddrStart = 0xc000 + (charCodeStart - 32) * 8;
		dstAddrEnd = 0xc000 + (charCodeCur - 32) * 8;
	} else if (charCodeStart < 64 && charCodeCur > 64) { // _pcgType == PCGTYPE_User
		asmCodeTmpl = _asmCodeTmpl2;
		dstAddrStart = 0xc000 + (charCodeStart - 32) * 8;
		dstAddrEnd = 0xc400 + (charCodeCur - 64) * 8;
	} else { // charCodeStart >= 64 && _pcgType == PCGTYPE_User
		asmCodeTmpl = _asmCodeTmpl1;
		dstAddrStart = 0xc400 + (charCodeStart - 64) * 8;
		dstAddrEnd = 0xc400 + (charCodeCur - 64) * 8;
	}
	::sprintf_s(asmCode, asmCodeTmpl, GetSymbol(), GetSymbol(), GetSymbol(), dstAddrStart, dstAddrEnd);
	Parser parser("***PCGPage.cpp***");
	if (!parser.ParseString(asmCode)) return nullptr;
	return parser.GetExprRoot()->Reference();
}
#endif

const char *PCGPage::_asmCodeTmpl = R"**(pcgpage.%s.src:
pcgpage.%s.store:
        .macro
        ldx     pcgpage.%s.src
        stx     [srcp]
        ldx     entries
loop_nextentry:
        .save   x
        .save   x
        ldx     [x]
        beq     done
        stx     [dstp]
        .end
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
        .end
        inx
        inx
        inx
        inx
        bra     loop_nextentry
entries:
)**";

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
