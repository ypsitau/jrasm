//=============================================================================
// PCGPageInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGPageInfo
//-----------------------------------------------------------------------------
PCGPageInfo::PCGPageInfo(const String &symbol, PCGRangeOwner *pPCGRangeOwner, bool upperCaseFlag) :
	_cntRef(1), _symbol(symbol), _upperCaseFlag(upperCaseFlag), _pPCGRangeOwner(pPCGRangeOwner)
{
}

void PCGPageInfo::AddPCGDataInfo(PCGDataInfo *pPCGDataInfo)
{
	_pcgInfoOwner.push_back(pPCGDataInfo);
}

bool PCGPageInfo::WriteFile(FILE *fp)
{
	const char *strPCGPAGE = ".pcgpage";
	const char *strEND = ".end";
	if (_upperCaseFlag) {
		strPCGPAGE = ".PCGPAGE";
		strEND = ".END\n";
	}
	::fprintf(fp, "\t%s\t%s,%s\n", strPCGPAGE, GetSymbol(), _pPCGRangeOwner->ComposeSource(_upperCaseFlag).c_str());
	for (auto pPCGDataInfo : GetPCGDataInfoOwner()) {
		::fprintf(fp, "\n");
		if (!pPCGDataInfo->WriteFile(fp)) return false;
	}
	::fprintf(fp, "\n");
	::fprintf(fp, "\t%s\n", strEND);
	return true;
}

PCGPageInfo *PCGPageInfo::CreateFromExpr(Context &context, const Expr *pExpr)
{
	bool upperCaseFlag = true;
	String symbol;
	std::unique_ptr<PCGRangeOwner> pPCGRangeOwner;
	if (!Directive_PCGPAGE::ExtractParams(pExpr, &symbol, &pPCGRangeOwner)) return nullptr;
	AutoPtr<PCGPageInfo> pPCGPageInfo(new PCGPageInfo(symbol, pPCGRangeOwner.release(), upperCaseFlag));
	bool firstFlag = true;
	for (auto pExprChild : pExpr->GetExprChildren()) {
		if (!pExprChild->IsTypeDirective(Directive::PCG)) continue;
		String symbol;
		int wdChar, htChar;
		int stepX, stepY;
		std::unique_ptr<PCGColorOwner> pPCGColorOwner;
		Binary buff;
		if (!Directive_PCG::ExtractParams(context, pExprChild, &symbol,
										  &wdChar, &htChar, &stepX, &stepY,
										  &pPCGColorOwner, buff)) return nullptr;
		AutoPtr<PCGDataInfo> pPCGDataInfo(
			new PCGDataInfo(symbol, PCGDataInfo::Pattern::CreateFromBuff(
							wdChar, htChar, buff), stepX, stepY,
							pPCGColorOwner.release(), upperCaseFlag));
		pPCGDataInfo->SetSelectedFlag(firstFlag);
		firstFlag = false;
		pPCGPageInfo->AddPCGDataInfo(pPCGDataInfo.release());
	}
	if (pPCGPageInfo->IsEmptyPCGDataInfo()) pPCGPageInfo->NewPCGDataInfo(true);
	return pPCGPageInfo.release();
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
	std::unique_ptr<PCGRangeOwner> pPCGRangeOwner(new PCGRangeOwner());
	pPCGRangeOwner->push_back(new PCGRange(PCGTYPE_User, 0x20, 0x60));
	AutoPtr<PCGPageInfo> pPCGPageInfo(new PCGPageInfo(symbol, pPCGRangeOwner.release(), upperCaseFlag));
	pPCGPageInfo->NewPCGDataInfo(true);
	push_back(pPCGPageInfo.release());
}
