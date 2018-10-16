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
		AutoPtr<PCGPageInfo> pPCGPageInfo(PCGPageInfo::CreateFromExpr(context, pExpr));
		if (pPCGPageInfo.IsNull()) return false;
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
