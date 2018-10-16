//=============================================================================
// PCGPageInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGPageInfo
//-----------------------------------------------------------------------------
PCGPageInfo::PCGPageInfo(const String &symbol, PCGType pcgType, int charCodeStart, bool upperCaseFlag) :
	_cntRef(1), _symbol(symbol), _pcgType(pcgType), _charCodeStart(charCodeStart), _upperCaseFlag(upperCaseFlag)
{
}

void PCGPageInfo::AddPCGInfo(PCGInfo *pPCGInfo)
{
	_pcgInfoOwner.push_back(pPCGInfo);
}

bool PCGPageInfo::WriteFile(FILE *fp)
{
	const char *strPCGPAGE = ".pcgpage";
	const char *strEND = ".end";
	if (_upperCaseFlag) {
		strPCGPAGE = ".PCGPAGE";
		strEND = ".END\n";
	}
	::fprintf(fp, "\t%s\t%s,%s,0x%02x\n", strPCGPAGE, GetSymbol(), GetPCGTypeName(), GetCharCodeStart());
	for (auto pPCGInfo : GetPCGInfoOwner()) {
		::fprintf(fp, "\n");
		if (!pPCGInfo->WriteFile(fp)) return false;
	}
	::fprintf(fp, "\n");
	::fprintf(fp, "\t%s\n", strEND);
	return true;
}

//-----------------------------------------------------------------------------
// PCGPageInfoList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCGPageInfoOwner
//-----------------------------------------------------------------------------
PCGPageInfoOwner::~PCGPageInfoOwner()
{
	Clear();
}

void PCGPageInfoOwner::Clear()
{
	for (auto pPCGPageInfo : *this) {
		PCGPageInfo::Delete(pPCGPageInfo);
	}
	clear();
}

void PCGPageInfoOwner::NewPCGPageInfo()
{
	bool upperCaseFlag = true;
	char symbol[256];
	::sprintf_s(symbol, "page%d", static_cast<int>(size()) + 1);
	AutoPtr<PCGPageInfo> pPCGPageInfo(new PCGPageInfo(symbol, PCGTYPE_User, 0x20, upperCaseFlag));
	pPCGPageInfo->NewPCGInfo();
	push_back(pPCGPageInfo.release());
}
