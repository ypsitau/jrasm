//=============================================================================
// Document.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Document
//-----------------------------------------------------------------------------
Document::Document() : _cntRef(1), _pPageInfoOwner(new PageInfoOwner())
{
	_pPageInfoOwner->NewPageInfo();
}

bool Document::ReadFile(const char *pathName)
{
	Context context(pathName);
	if (!context.ParseFile()) return false;
	std::unique_ptr<PageInfoOwner> pPageInfoOwner(new PageInfoOwner());
	const Expr *pExprRoot = context.GetExprRoot();
	for (auto pExpr : pExprRoot->GetExprChildren()) {
		if (!pExpr->IsTypeDirective(Directive::PCGPAGE)) continue;
		String symbol;
		PCGType pcgType;
		int charCodeStart;
		if (!Directive_PCGPAGE::ExtractParams(pExpr, &symbol, &pcgType, &charCodeStart)) return false;
		AutoPtr<PageInfo> pPageInfo(new PageInfo(symbol, pcgType, charCodeStart));
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
			pPageInfo->AddPCGInfo(new PCGInfo(symbol, PCGInfo::Pattern::CreateFromBuff(wdChar, htChar, buff)));
		}
		if (pPageInfo->IsEmptyPCGInfo()) pPageInfo->NewPCGInfo();
		pPageInfoOwner->push_back(pPageInfo.release());
	}
	if (pPageInfoOwner->empty()) pPageInfoOwner->NewPageInfo();
	_pPageInfoOwner.reset(pPageInfoOwner.release());
	return true;
}

bool Document::WriteFile(const char *pathName)
{
	return true;
}
