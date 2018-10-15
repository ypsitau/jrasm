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
	Parser parser(pathName);
	if (!parser.ParseFile()) {
		ErrorLog::Print(stderr);
		return false;
	}
	::printf("ReadFile\n");
	const Expr *pExprRoot = parser.GetRoot();
	for (auto pExpr : pExprRoot->GetExprChildren()) {
		if (!pExpr->IsTypeDirective(Directive::PCGPAGE)) continue;
		pExpr->GetExprOperands();
		pExpr->Print();
		for (auto pExprChild : pExpr->GetExprChildren()) {
			pExprChild->Print();
		}
		//_pageInfoOwner.push_back(new PageInfo());
	}
	return true;
}

bool Document::WriteFile(const char *pathName)
{
	return true;
}

