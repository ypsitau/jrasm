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
	FILE *fp = ::fopen(pathName, "rt");
	if (fp == nullptr) {
		ErrorLog::AddError("failed to open file %s", pathName);
		return false;
	}
	_strSrcSegs.clear();
	int lineNoPrev = 1;
	for (auto pExpr : pExprRoot->GetExprChildren()) {
		if (!pExpr->IsTypeDirective(Directive::PCGPAGE)) continue;
		int lineNoStart = pExpr->GetLineNo();
		int lineNoEnd = pExpr->GetExprChildren().empty()?
			lineNoStart : pExpr->GetExprChildren().back()->GetLineNo();
		String strSrcSeg = ReadLines(fp, lineNoStart - lineNoPrev);
		_strSrcSegs.push_back(strSrcSeg);
		SkipLines(fp, lineNoEnd - lineNoStart + 1);
		lineNoPrev = lineNoEnd + 1;
	}
	_strSrcSegs.push_back(ReadRest(fp));
	::fclose(fp);
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
	StringList::const_iterator pStrSrcSegs = _strSrcSegs.begin();
	FILE *fp = ::fopen(pathName, "wt");
	if (fp == nullptr) {
		ErrorLog::AddError("failed to open file %s", pathName);
		return false;
	}
	if (pStrSrcSegs != _strSrcSegs.end()) {
		::fprintf(fp, "%s", pStrSrcSegs->c_str());
		pStrSrcSegs++;
	}
	for (PCGPageInfoOwner::iterator ppPCGPageInfo = GetPCGPageInfoOwner().begin();
		 ppPCGPageInfo != GetPCGPageInfoOwner().end(); ppPCGPageInfo++) {
		PCGPageInfo *pPCGPageInfo = *ppPCGPageInfo;
		if (!pPCGPageInfo->WriteFile(fp)) {
			rtn = false;
			break;
		}
		if (pStrSrcSegs != _strSrcSegs.end()) {
			::fprintf(fp, "%s", pStrSrcSegs->c_str());
			pStrSrcSegs++;
		} else if (ppPCGPageInfo + 1 != GetPCGPageInfoOwner().end()) {
			::fprintf(fp, "\n");
		}
	}
	for ( ; pStrSrcSegs != _strSrcSegs.end(); pStrSrcSegs++) {
		::fprintf(fp, "%s", pStrSrcSegs->c_str());
	}
	::fclose(fp);
	return rtn;
}

String Document::ReadLines(FILE *fp, int nLines)
{
	String rtn;
	if (nLines == 0) return rtn;
	for (;;) {
		int ch = ::fgetc(fp);
		if (ch < 0) break;
		rtn += static_cast<UInt8>(ch);
		if (ch == '\n' && --nLines <= 0) break;
	}
	return rtn;
}

String Document::ReadRest(FILE *fp)
{
	String rtn;
	for (;;) {
		int ch = ::fgetc(fp);
		if (ch < 0) break;
		rtn += static_cast<UInt8>(ch);
	}
	return rtn;
}

void Document::SkipLines(FILE *fp, int nLines)
{
	if (nLines == 0) return;
	for (;;) {
		int ch = ::fgetc(fp);
		if (ch < 0 || (ch == '\n' && --nLines <= 0)) break;
	}
}
