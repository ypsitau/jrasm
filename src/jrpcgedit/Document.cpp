//=============================================================================
// Document.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Document
//-----------------------------------------------------------------------------
Document::Document() : _cntRef(1)
{
	_pageInfoOwner.push_back(new PageInfo());
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
		pExpr->Print();
	}
	return true;
}

bool Document::WriteFile(const char *pathName)
{
	return true;
}

