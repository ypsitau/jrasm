//=============================================================================
// Context.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context(const String &pathNameSrc) :
	_pathNameSrc(pathNameSrc), _phaseCur(PHASE_None), _iSavePoint(0),
	_pExprListResolved(new ExprList()), _pExprDictIncluded(new ExprDict())
{
	String fileNameSrc;
	SplitFileName(pathNameSrc.c_str(), &_dirNameSrc, &fileNameSrc);
	_fileBaseNameSrc = _fileNameJR = ::RemoveExtName(fileNameSrc.c_str());
	AutoPtr<ExprDict> pExprDict(new ExprDict());
	pExprDict->Assign("@BYTE", new Expr_Integer(1), false);
	pExprDict->Assign("@WORD", new Expr_Integer(2), false);
	pExprDict->Assign("@DWORD", new Expr_Integer(4), false);
	_exprDictStack.push_back(pExprDict.release());				// global exprDict
	_segmentOwner.push_back(new Segment("code", nullptr));		// code segment
	_segmentOwner.push_back(new Segment("inline",				// inline data segment
										_segmentOwner.back()->Reference()));
	_segmentOwner.push_back(new Segment("data",					// data segment
										_segmentOwner.back()->Reference()));
	_segmentOwner.push_back(new Segment("work",					// work segment
										_segmentOwner.back()->Reference()));
	_pcgCharsBuiltIn.push_back(
		new PCGChar(PCGTYPE_CRAM, 0x00, Binary("\x00\x00\x00\x00\x00\x00\x00\x00", 8)));
	SelectCodeSegment();
	SetupDirNamesInc(_dirNamesInc);
}

void Context::Initialize(Generator *pGenerator)
{
	ErrorLog::Clear();
	Directive::Initialize();
	Operator::Initialize();
	Generator::Initialize(pGenerator);
}

bool Context::ParseFile()
{
	SetPhase(PHASE_Parse);
	Parser parser(_pathNameSrc);
	if (!parser.ParseFile()) return false;
	_pExprRoot.reset(parser.GetExprRoot()->Reference());
	return true;
}

bool Context::Prepare()
{
	SetPhase(PHASE_Preprocess);
	if (!_pExprRoot->OnPhasePreprocess(*this)) return false;
	SetPhase(PHASE_AssignMacro);
	if (!_pExprRoot->OnPhaseAssignMacro(*this)) return false;
	SetPhase(PHASE_ExpandMacro);
	if (!_pExprRoot->OnPhaseExpandMacro(*this)) return false;
	SetPhase(PHASE_AssignSymbol);
	ResetSegment();
	if (!_inlineDataOwner.OnPhaseAssignSymbol(*this)) return false;
	if (!_pExprRoot->OnPhaseAssignSymbol(*this)) return false;
	if (!_segmentOwner.AdjustAddress()) return false;
	return true;
}

RegionOwner *Context::Generate(size_t bytesGapToJoin, UInt8 dataFiller)
{
	SetPhase(PHASE_Generate);
	ResetSegment();
	if (!_inlineDataOwner.OnPhaseGenerate(*this, nullptr)) return nullptr;
	if (!_pExprRoot->OnPhaseGenerate(*this, nullptr)) return nullptr;
	return GetSegmentOwner().JoinRegion(bytesGapToJoin, dataFiller);
}

bool Context::DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine)
{
	SetPhase(PHASE_Generate);
	DisasmDumper disasmDumper(fp, upperCaseFlag, nColsPerLine);
	ResetSegment();
	if (!_pExprRoot->OnPhaseDisasm(*this, disasmDumper, 0)) return false;
	if (!_inlineDataOwner.OnPhaseDisasm(*this, disasmDumper, 0)) return false;
	return true;
}

void Context::PrintSymbolList(FILE *fp, bool upperCaseFlag)
{
	const char *format = upperCaseFlag? "%04X  %s\n" : "%04x  %s\n";
	std::unique_ptr<SymbolInfoOwner> pSymbolInfoOwner(MakeSymbolInfoOwner());
	::fprintf(fp, "[Symbol List]\n");
	if (pSymbolInfoOwner->empty()) {
		::fprintf(fp, "(no symbol)\n");
	} else {
		for (auto pSymbolInfo : *pSymbolInfoOwner) {
			::fprintf(fp, format, pSymbolInfo->GetInteger(), pSymbolInfo->GetSymbol());
		}
	}
}

bool Context::PrintMemoryUsage(FILE *fp, bool upperCaseFlag)
{
#if 0
	const char *formatRoot	= "%04x-%04x   %5dbytes\n";
	const char *formatChild	= " %04x-%04x  %5dbytes\n";
	if (upperCaseFlag) {
		formatRoot	= "%04X-%04X   %5dbytes\n";
		formatChild	= " %04X-%04X  %5dbytes\n";
	}
	size_t bytesGapToJoin = 128;
	UInt8 dataFiller = 0x00;
	std::unique_ptr<RegionOwner> pRegionOwner(Generate(bytesGapToJoin, dataFiller));
	if (pRegionOwner.get() == nullptr) return false;
	::fprintf(fp, "[Memory Usage]\n");
	for (auto pRegion : *pRegionOwner) {
		::fprintf(fp, formatRoot, pRegion->GetAddrTop(), pRegion->GetAddrBtm() - 1, pRegion->GetBytes());
		for (auto pRegionIngredient : pRegion->GetRegionsIngredient()) {
			::fprintf(fp, formatChild,
					  pRegionIngredient->GetAddrTop(), pRegionIngredient->GetAddrBtm() - 1,
					  pRegionIngredient->GetBytes());
		}
	}
#endif
	return true;
}

void Context::PrintPCGUsage(FILE *fp, bool upperCaseFlag)
{
	if (GetPCGPageOwner().empty()) return;
	::fprintf(fp, "[PCG Usage]\n");
	for (auto pPCGPage : GetPCGPageOwner()) {
		::fprintf(fp, "PCG Page: %s\n", pPCGPage->GetSymbol());
		for (auto pPCGRange : pPCGPage->GetPCGRangeOwner()) {
			int nChars = pPCGRange->GetCharCodeCur() - pPCGRange->GetCharCodeStart();
			if (nChars == 0) continue;
			::fprintf(fp, "  %s:0x%02x-0x%02x (%dchars)\n",
					  pPCGRange->GetPCGTypeName(upperCaseFlag),
					  pPCGRange->GetCharCodeStart(), pPCGRange->GetCharCodeCur() - 1, nChars);
		}
	}
}

bool Context::AddRegion(Integer addrTop)
{
	Region *pRegion = _pSegmentCur->FindRegionByAddrTop(addrTop);
	if (pRegion == nullptr) {
		_pSegmentCur->AddRegion(addrTop);
	} else if (!pRegion->IsBufferEmpty()) {
		ErrorLog::AddError("memory regions are overwrapped at 0x%04x", addrTop);
		return false;
	} else {
		_pSegmentCur->SetRegionCur(pRegion);
		pRegion->ResetAddrOffset();
	}
	//_pSegmentCur->ResetAddrOffset();
	return true;
}

void Context::BeginScope()
{
	ExprDict *pExprDict = new ExprDict(_exprDictStack.back()->Reference());
	_exprDictStack.push_back(pExprDict);
}

void Context::EndScope()
{
	ExprDict *pExprDict = _exprDictStack.back();
	_exprDictStack.pop_back();
	ExprDict::Delete(pExprDict);
}

String Context::MakeFileNameOut(const char *extName) const
{
	if (_fileBaseNameSrc == "-") return String("output") + extName;
	return _fileBaseNameSrc + extName;
}

Context::SymbolInfoOwner *Context::MakeSymbolInfoOwner()
{
	std::unique_ptr<SymbolInfoOwner> pSymbolInfoOwner(new SymbolInfoOwner());
	for (auto iter : GetExprDictGlobal()) {
		const String &symbol = iter.first;
		const Expr *pExpr = iter.second;
		AutoPtr<Expr> pExprResolved(pExpr->Resolve(*this));
		if (!pExprResolved.IsNull() && pExprResolved->IsTypeInteger()) {
			Integer num = dynamic_cast<Expr_Integer *>(pExprResolved.get())->GetInteger();
			pSymbolInfoOwner->push_back(new SymbolInfo(num, symbol));
		}
	}
	pSymbolInfoOwner->SortByInteger();
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

const Expr *Context::FindExprIncluded(const char *pathNameIncluded)
{
	ExprDict::iterator iter = _pExprDictIncluded->find(pathNameIncluded);
	return (iter == _pExprDictIncluded->end())? nullptr : iter->second;
}

void Context::AddExprIncluded(const char *pathNameIncluded, Expr *pExpr)
{
	_pExprDictIncluded->insert(std::make_pair(pathNameIncluded, pExpr));
}

InlineData *Context::CreateInlineData(InlineData::Type type, const Binary &buff, const String &strSrc)
{
	InlineData *pInlineData = _inlineDataOwner.Lookup(type, buff);
	if (pInlineData == nullptr) {
		pInlineData = new InlineData(type, buff, strSrc);
		_inlineDataOwner.push_back(pInlineData);
	}
	return pInlineData;
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
