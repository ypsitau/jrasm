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
		AutoPtr<PCGPageInfo> pPCGPageInfo(new PCGPageInfo(symbol, pcgType, charCodeStart));
		pExpr->Print();
		for (auto pExprChild : pExpr->GetExprChildren()) {
			if (!pExprChild->IsTypeDirective(Directive::PCG)) continue;
			String symbol;
			int wdChar, htChar;
			int stepX, stepY;
			Binary buff;
			pExprChild->Print();
			if (!Directive_PCG::ExtractParams(context, pExprChild, &symbol,
											  &wdChar, &htChar, &stepX, &stepY, buff)) return false;
			pPCGPageInfo->AddPCGInfo(new PCGInfo(symbol, PCGInfo::Pattern::CreateFromBuff(wdChar, htChar, buff)));
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
	return true;
}
