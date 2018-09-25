//=============================================================================
// Context.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context() : _phaseCur(PHASE_None), _pExprListResolved(new ExprList())
{
	_fileNameJR = "JRASM_PRODUCT";
	_lookupTableStack.push_back(new LookupTable());		// global lookup table
	_segmentOwner.push_back(new Segment("code"));		// code segment
	_segmentOwner.push_back(new Segment("data"));		// data segment
	_segmentOwner.push_back(new Segment("internal"));	// internal segment
	SelectCodeSegment();
}

bool Context::ParseFile(Parser &parser)
{
	FILE *fp = nullptr;
	if (::fopen_s(&fp, parser.GetFileNameSrc(), "rt") != 0) {
		ErrorLog::AddError("failed to open file: %s\n", parser.GetFileNameSrc());
		return false;
	}
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!parser.FeedChar(ch)) break;
		if (ch == '\0') break;
	}
	::fclose(fp);
	return !ErrorLog::HasError();
}

bool Context::Prepare(Parser &parser)
{
	SetPhase(PHASE_Include);
	if (!parser.GetRoot()->OnPhaseInclude(*this)) return false;
	SetPhase(PHASE_DeclareMacro);
	if (!parser.GetRoot()->OnPhaseDeclareMacro(*this)) return false;
	SetPhase(PHASE_ExpandMacro);
	if (!parser.GetRoot()->OnPhaseExpandMacro(*this)) return false;
	SetPhase(PHASE_SetupLookup);
	if (!parser.GetRoot()->OnPhaseSetupLookup(*this)) return false;
	return true;
}

RegionOwner *Context::Generate(Parser &parser, size_t bytesGapToJoin, UInt8 dataFiller)
{
	SetPhase(PHASE_Generate);
	if (!parser.GetRoot()->OnPhaseGenerate(*this)) return nullptr;
	return GetSegmentOwner().JoinRegion(bytesGapToJoin, dataFiller);
}

bool Context::DumpDisasm(Parser &parser, FILE *fp, bool upperCaseFlag, size_t nColsPerLine)
{
	SetPhase(PHASE_Generate);
	return parser.GetRoot()->OnPhaseDisasm(*this, fp, upperCaseFlag, nColsPerLine);
}

void Context::StartRegion(UInt32 addr)
{
	_pSegmentCur->AddRegion(new Region(addr));
	_pSegmentCur->SetAddress(addr);
}

void Context::PushLocalLookupTable()
{
	LookupTable *pLookupTable = new LookupTable(_lookupTableStack.back()->Reference());
	_lookupTableStack.push_back(pLookupTable);
}

void Context::PopLocalLookupTable()
{
	LookupTable *pLookupTable = _lookupTableStack.back();
	_lookupTableStack.pop_back();
	LookupTable::Delete(pLookupTable);
}

Context::LabelInfoOwner *Context::MakeLabelInfoOwner()
{
	std::unique_ptr<LabelInfoOwner> pLabelInfoOwner(new LabelInfoOwner());
	for (auto iter : *GetLookupTableGlobal()) {
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
// Context::LookupTable
//-----------------------------------------------------------------------------
Context::LookupTable::~LookupTable()
{
	for (auto iter : *this) {
		Expr::Delete(iter.second);
	}
}

void Context::LookupTable::Associate(const String &label, Expr *pExpr, bool forceGlobalFlag)
{
	LookupTable *pLookupTable = forceGlobalFlag? GetGlobal() : this;
	pLookupTable->insert(std::make_pair(label, pExpr));
}

bool Context::LookupTable::IsDefined(const char *label) const
{
	return find(label) != end();
}

const Expr *Context::LookupTable::Lookup(const char *label) const
{
	const_iterator iter = find(label);
	if (iter != end())	return iter->second;
	return _pLookupTableParent.IsNull()? nullptr : _pLookupTableParent->Lookup(label);
}

Context::LookupTable *Context::LookupTable::GetGlobal()
{
	LookupTable *pLookupTable = this;
	for ( ; pLookupTable->GetParent() != nullptr; pLookupTable = pLookupTable->GetParent()) ;
	return pLookupTable;
}

//-----------------------------------------------------------------------------
// Context::LookupTableOwner
//-----------------------------------------------------------------------------
Context::LookupTableOwner::~LookupTableOwner()
{
	Clear();
}

void Context::LookupTableOwner::Clear()
{
	for (auto pLookupTable : *this) {
		LookupTable::Delete(pLookupTable);
	}
	clear();
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
