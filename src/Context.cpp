//=============================================================================
// Context.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context(const String &pathNameSrc) :
	_pParser(new Parser(*this, pathNameSrc)), _pDirectiveOwner(new DirectiveOwner()),
	_phaseCur(PHASE_None), _pExprListResolved(new ExprList())
{
	const char *fileNameSrc = ::ExtractFileName(pathNameSrc.c_str());
	_fileBaseNameSrc = _fileNameJR = ::RemoveExtName(fileNameSrc);
	_dictionaryStack.push_back(new Expr::Dictionary());	// global dictionary
	_segmentOwner.push_back(new Segment("code"));			// code segment
	_segmentOwner.push_back(new Segment("data"));			// data segment
	_segmentOwner.push_back(new Segment("internal"));		// internal segment
	SelectCodeSegment();
	AddDirective(new Directive_CSEG());
	AddDirective(new Directive_DB());
	AddDirective(new Directive_DSEG());
	AddDirective(new Directive_DW());
	AddDirective(new Directive_ENDM());
	AddDirective(new Directive_ENDP());
	AddDirective(new Directive_ENDPCG());
	AddDirective(new Directive_EQU());
	AddDirective(new Directive_FILENAME_JR());
	AddDirective(new Directive_INCLUDE());
	AddDirective(new Directive_ISEG());
	AddDirective(new Directive_MACRO());
	AddDirective(new Directive_MML());
	AddDirective(new Directive_ORG());
	AddDirective(new Directive_PCG());
	AddDirective(new Directive_PROC());
}

bool Context::ParseFile()
{
	FILE *fp = nullptr;
	if (::fopen_s(&fp, _pParser->GetFileNameSrc(), "rt") != 0) {
		ErrorLog::AddError("failed to open file: %s\n", _pParser->GetFileNameSrc());
		return false;
	}
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!_pParser->FeedChar(ch)) break;
		if (ch == '\0') break;
	}
	::fclose(fp);
	return !ErrorLog::HasError();
}

bool Context::Prepare()
{
	SetPhase(PHASE_Include);
	if (!_pParser->GetRoot()->OnPhaseInclude(*this)) return false;
	SetPhase(PHASE_DeclareMacro);
	if (!_pParser->GetRoot()->OnPhaseDeclareMacro(*this)) return false;
	SetPhase(PHASE_ExpandMacro);
	if (!_pParser->GetRoot()->OnPhaseExpandMacro(*this)) return false;
	SetPhase(PHASE_SetupLookup);
	if (!_pParser->GetRoot()->OnPhaseSetupLookup(*this)) return false;
	return true;
}

RegionOwner *Context::Generate(size_t bytesGapToJoin, UInt8 dataFiller)
{
	SetPhase(PHASE_Generate);
	if (!_pParser->GetRoot()->OnPhaseGenerate(*this)) return nullptr;
	return GetSegmentOwner().JoinRegion(bytesGapToJoin, dataFiller);
}

bool Context::DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine)
{
	SetPhase(PHASE_Generate);
	return _pParser->GetRoot()->OnPhaseDisasm(*this, fp, upperCaseFlag, nColsPerLine);
}

void Context::StartRegion(UInt32 addr)
{
	_pSegmentCur->AddRegion(new Region(addr));
	_pSegmentCur->SetAddress(addr);
}

void Context::PushLocalDictionary()
{
	Expr::Dictionary *pDictionary = new Expr::Dictionary(_dictionaryStack.back()->Reference());
	_dictionaryStack.push_back(pDictionary);
}

void Context::PopLocalDictionary()
{
	Expr::Dictionary *pDictionary = _dictionaryStack.back();
	_dictionaryStack.pop_back();
	Expr::Dictionary::Delete(pDictionary);
}

Context::LabelInfoOwner *Context::MakeLabelInfoOwner()
{
	std::unique_ptr<LabelInfoOwner> pLabelInfoOwner(new LabelInfoOwner());
	for (auto iter : *GetDictionaryGlobal()) {
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

void Context::AddDirective(Directive *pDirective)
{
	_pDirectiveOwner->push_back(pDirective);
}

const Directive *Context::FindDirective(const char *symbol) const
{
	return _pDirectiveOwner->FindBySymbol(symbol);
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
