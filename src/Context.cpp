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
	String fileNameSrc;
	SplitFileName(pathNameSrc.c_str(), &_dirNameSrc, &fileNameSrc);
	_fileBaseNameSrc = _fileNameJR = ::RemoveExtName(fileNameSrc.c_str());
	_exprDictStack.push_back(new ExprDict());			// global exprDict
	_segmentOwner.push_back(new Segment("code"));		// code segment
	_segmentOwner.push_back(new Segment("data"));		// data segment
	_segmentOwner.push_back(new Segment("internal"));	// internal segment
	SelectCodeSegment();
}

bool Context::Prepare()
{
	SetPhase(PHASE_Parse);
	Parser parser(_pathNameSrc);
	if (!parser.ParseFile()) return false;
	_pExprRoot.reset(parser.GetRoot()->Reference());
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
	if (!_pExprRoot->OnPhaseGenerate(*this, nullptr)) return nullptr;
	return GetSegmentOwner().JoinRegion(bytesGapToJoin, dataFiller);
}

bool Context::DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine)
{
	SetPhase(PHASE_Generate);
	DisasmDumper disasmDumper(fp, upperCaseFlag, nColsPerLine);
	return _pExprRoot->OnPhaseDisasm(*this, disasmDumper, 0);
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

Context::SymbolInfoOwner *Context::MakeSymbolInfoOwner()
{
	std::unique_ptr<SymbolInfoOwner> pSymbolInfoOwner(new SymbolInfoOwner());
	for (auto iter : GetExprDictGlobal()) {
		const String &symbol = iter.first;
		const Expr *pExpr = iter.second;
		AutoPtr<Expr> pExprResolved(pExpr->Resolve(*this));
		if (!pExprResolved.IsNull() && pExprResolved->IsTypeNumber()) {
			UInt32 num = dynamic_cast<Expr_Number *>(pExprResolved.get())->GetNumber();
			pSymbolInfoOwner->push_back(new SymbolInfo(num, symbol));
		}
	}
	pSymbolInfoOwner->SortByNumber();
	return pSymbolInfoOwner.release();
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
// Context::SymbolInfoList
//-----------------------------------------------------------------------------
size_t Context::SymbolInfoList::GetSymbolLenMax(const_iterator ppSymbolInfo, const_iterator ppSymbolInfoEnd)
{
	size_t symbolLenMax = 0;
	for ( ; ppSymbolInfo != ppSymbolInfoEnd; ppSymbolInfo++) {
		const SymbolInfo *pSymbolInfo = *ppSymbolInfo;
		size_t symbolLen = ::strlen(pSymbolInfo->GetSymbol());
		if (symbolLenMax < symbolLen) symbolLenMax = symbolLen;
	}
	return symbolLenMax;
}

//-----------------------------------------------------------------------------
// Context::SymbolInfoOwner
//-----------------------------------------------------------------------------
Context::SymbolInfoOwner::~SymbolInfoOwner()
{
	Clear();
}

void Context::SymbolInfoOwner::Clear()
{
	for (auto pSymbolInfo : *this) {
		delete pSymbolInfo;
	}
	clear();
}
