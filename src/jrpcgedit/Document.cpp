//=============================================================================
// Document.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Document
//-----------------------------------------------------------------------------
Document::Document() : _cntRef(1)
{
	_pageInfoOwner.push_back(new PageInfo("page1", PCGTYPE_User, 0x20));
}

bool Document::ReadFile(const char *pathName)
{
	Context context(pathName);
	if (!context.ParseFile()) return false;
	const Expr *pExprRoot = context.GetExprRoot();
	for (auto pExpr : pExprRoot->GetExprChildren()) {
		if (!pExpr->IsTypeDirective(Directive::PCGPAGE)) continue;
		String symbol;
		PCGType pcgType;
		int charCodeStart;
		if (!Directive_PCGPAGE::ExtractParams(pExpr, &symbol, &pcgType, &charCodeStart)) return false;
		_pageInfoOwner.push_back(new PageInfo(symbol, pcgType, charCodeStart));
		pExpr->Print();
		for (auto pExprChild : pExpr->GetExprChildren()) {
			pExprChild->Print();
		}
	}
	return true;
}

bool Document::WriteFile(const char *pathName)
{
	return true;
}

