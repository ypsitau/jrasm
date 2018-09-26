//=============================================================================
// Context.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context(const String &pathNameSrc) :
	_pathNameSrc(pathNameSrc), _phaseCur(PHASE_None), _pExprListResolved(new ExprList())
{
	const char *fileNameSrc = ::ExtractFileName(pathNameSrc.c_str());
	_fileBaseNameSrc = _fileNameJR = ::RemoveExtName(fileNameSrc);
	_exprDictStack.push_back(new ExprDict());			// global exprDict
	_segmentOwner.push_back(new Segment("code"));		// code segment
	_segmentOwner.push_back(new Segment("data"));		// data segment
	_segmentOwner.push_back(new Segment("internal"));	// internal segment
	SelectCodeSegment();
}

bool Context::ParseFile()
{
	Parser parser(_pathNameSrc);
	FILE *fp = nullptr;
	if (::fopen_s(&fp, parser.GetPathNameSrc(), "rt") != 0) {
		ErrorLog::AddError("failed to open file: %s\n", parser.GetPathNameSrc());
		return false;
	}
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!parser.FeedChar(ch)) break;
		if (ch == '\0') break;
	}
	::fclose(fp);
	_pExprRoot.reset(parser.GetRoot()->Reference());
	return !ErrorLog::HasError();
}

bool Context::Prepare()
{
	SetPhase(PHASE_Include);
	if (!_pExprRoot->OnPhaseInclude(*this)) return false;
	SetPhase(PHASE_DeclareMacro);
	if (!_pExprRoot->OnPhaseDeclareMacro(*this)) return false;
	SetPhase(PHASE_ExpandMacro);
	if (!_pExprRoot->OnPhaseExpandMacro(*this)) return false;
	SetPhase(PHASE_SetupExprDict);
	if (!_pExprRoot->OnPhaseSetupExprDict(*this)) return false;
	return true;
}

RegionOwner *Context::Generate(size_t bytesGapToJoin, UInt8 dataFiller)
{
	SetPhase(PHASE_Generate);
	if (!_pExprRoot->OnPhaseGenerate(*this)) return nullptr;
	return GetSegmentOwner().JoinRegion(bytesGapToJoin, dataFiller);
}

bool Context::DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine)
{
	SetPhase(PHASE_Generate);
	return _pExprRoot->OnPhaseDisasm(*this, fp, upperCaseFlag, nColsPerLine);
}

void Context::StartRegion(UInt32 addr)
{
	_pSegmentCur->AddRegion(new Region(addr));
	_pSegmentCur->SetAddress(addr);
}

void Context::PushLocalExprDict()
{
	ExprDict *pExprDict = new ExprDict(_exprDictStack.back()->Reference());
	_exprDictStack.push_back(pExprDict);
}

void Context::PopLocalExprDict()
{
	ExprDict *pExprDict = _exprDictStack.back();
	_exprDictStack.pop_back();
	ExprDict::Delete(pExprDict);
}

Context::LabelInfoOwner *Context::MakeLabelInfoOwner()
{
	std::unique_ptr<LabelInfoOwner> pLabelInfoOwner(new LabelInfoOwner());
	for (auto iter : GetExprDictGlobal()) {
		const String &label = iter.first;
		const Expr *pExpr = iter.second;
		AutoPtr<Expr> pExprResolved(pExpr->Resolve(*this));
		if (!pExprResolved.IsNull() && pExprResolved->IsTypeNumber()) {
			UInt32 num = dynamic_cast<Expr_Number *>(pExprResolved.get())->GetNumber();
			pLabelInfoOwner->push_back(new LabelInfo(num, label));
		}
	}
	pLabelInfoOwner->SortByNumber();
	return pLabelInfoOwner.release();
}

void Context::StartToResolve()
{
	_pExprListResolved->clear();
}

bool Context::CheckCircularReference(const Expr *pExpr)
{
	if (std::find(_pExprListResolved->begin(), _pExprListResolved->end(),
				  const_cast<Expr *>(pExpr)) != _pExprListResolved->end()) {
		ErrorLog::AddError(pExpr, "circular reference is detected");
		return true;
	}
	_pExprListResolved->push_back(const_cast<Expr *>(pExpr));
	return false;
}

//-----------------------------------------------------------------------------
// Context::LabelInfoList
//-----------------------------------------------------------------------------
size_t Context::LabelInfoList::GetLabelLenMax(const_iterator ppLabelInfo, const_iterator ppLabelInfoEnd)
{
	size_t labelLenMax = 0;
	for ( ; ppLabelInfo != ppLabelInfoEnd; ppLabelInfo++) {
		const LabelInfo *pLabelInfo = *ppLabelInfo;
		size_t labelLen = ::strlen(pLabelInfo->GetLabel());
		if (labelLenMax < labelLen) labelLenMax = labelLen;
	}
	return labelLenMax;
}

//-----------------------------------------------------------------------------
// Context::LabelInfoOwner
//-----------------------------------------------------------------------------
Context::LabelInfoOwner::~LabelInfoOwner()
{
	Clear();
}

void Context::LabelInfoOwner::Clear()
{
	for (auto pLabelInfo : *this) {
		delete pLabelInfo;
	}
	clear();
}
