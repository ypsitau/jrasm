//=============================================================================
// Document.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Document
//-----------------------------------------------------------------------------
Document::Document() : _cntRef(1), _pPCGPageInfoOwner(new PCGPageInfoOwner())
{
	_pPCGPageInfoOwner->NewPCGPageInfo();
}

bool Document::ReadFile(const char *pathName)
{
	bool upperCaseFlag = true;
	Context context(pathName);
	if (!context.ParseFile()) return false;
	std::unique_ptr<PCGPageInfoOwner> pPCGPageInfoOwner(new PCGPageInfoOwner());
	const Expr *pExprRoot = context.GetExprRoot();
	for (auto pExpr : pExprRoot->GetExprChildren()) {
		if (!pExpr->IsTypeDirective(Directive::PCGPAGE)) continue;
		String symbol;
		PCGType pcgType;
		int charCodeStart;
		if (!Directive_PCGPAGE::ExtractParams(pExpr, &symbol, &pcgType, &charCodeStart)) return false;
		AutoPtr<PCGPageInfo> pPCGPageInfo(new PCGPageInfo(symbol, pcgType, charCodeStart, upperCaseFlag));
		bool firstFlag = true;
		for (auto pExprChild : pExpr->GetExprChildren()) {
			if (!pExprChild->IsTypeDirective(Directive::PCG)) continue;
			String symbol;
			int wdChar, htChar;
			int stepX, stepY;
			Binary buff;
			if (!Directive_PCG::ExtractParams(context, pExprChild, &symbol,
											  &wdChar, &htChar, &stepX, &stepY, buff)) return false;
			AutoPtr<PCGInfo> pPCGInfo(
				new PCGInfo(symbol, PCGInfo::Pattern::CreateFromBuff(
								wdChar, htChar, buff), stepX, stepY, upperCaseFlag));
			pPCGInfo->SetSelectedFlag(firstFlag);
			firstFlag = false;
			pPCGPageInfo->AddPCGInfo(pPCGInfo.release());
		}
		if (pPCGPageInfo->IsEmptyPCGInfo()) pPCGPageInfo->NewPCGInfo();
		pPCGPageInfoOwner->push_back(pPCGPageInfo.release());
	}
	if (pPCGPageInfoOwner->empty()) pPCGPageInfoOwner->NewPCGPageInfo();
	_pPCGPageInfoOwner.reset(pPCGPageInfoOwner.release());
	return true;
}

bool Document::WriteFile(const char *pathName)
{
	bool rtn = true;
	FILE *fp = stdout;
	for (auto pPCGPageInfo : GetPCGPageInfoOwner()) {
		if (!pPCGPageInfo->WriteFile(fp)) {
			rtn = false;
			break;
		}
	}
	return rtn;
}
