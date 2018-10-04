//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Segment.h"
#include "Expr.h"
#include "Macro.h"
#include "PCGPage.h"

class Parser;

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
class Context {
public:
	enum Phase {
		PHASE_None,
		PHASE_Parse,
		PHASE_Preprocess,
		PHASE_AssignMacro,
		PHASE_ExpandMacro,
		PHASE_AssignSymbol,
		PHASE_Generate,
	};
public:
	class SymbolInfo {
	public:
		struct LessThan_Number {
			inline bool operator()(const SymbolInfo *pSymbolInfo1, const SymbolInfo *pSymbolInfo2) const {
				return pSymbolInfo1->GetNumber() < pSymbolInfo2->GetNumber();
			}
		};
		struct LessThan_Symbol {
			inline bool operator()(const SymbolInfo *pSymbolInfo1, const SymbolInfo *pSymbolInfo2) const {
				return ::strcmp(pSymbolInfo1->GetSymbol(), pSymbolInfo2->GetSymbol()) < 0;
			}
		};
	private:
		Number _num;
		String _symbol;
	public:
		inline SymbolInfo(Number num, const String &symbol) : _num(num), _symbol(symbol) {}
		inline Number GetNumber() const { return _num; }
		inline const char *GetSymbol() const { return _symbol.c_str(); }
	};
	class SymbolInfoList : public std::vector<SymbolInfo *> {
	public:
		void SortByNumber() { std::sort(begin(), end(), SymbolInfo::LessThan_Number()); }
		void SortBySymbol() { std::sort(begin(), end(), SymbolInfo::LessThan_Symbol()); }
		static size_t GetSymbolLenMax(const_iterator ppSymbolInfo, const_iterator ppSymbolInfoEnd);
	};
	class SymbolInfoOwner : public SymbolInfoList {
	public:
		~SymbolInfoOwner();
		void Clear();
	};
private:
	String _pathNameSrc;
	String _dirNameSrc;
	String _fileBaseNameSrc;
	String _fileNameJR;
	Segment *_pSegmentCur;
	SegmentOwner _segmentOwner;
	Phase _phaseCur;
	ExprDictStack _exprDictStack;
	MacroDict _macroDict;
	AutoPtr<Expr> _pExprRoot;
	AutoPtr<PCGPage> _pPCGPageCur;
	PCGCharOwner _pcgCharsBuiltIn;
	std::unique_ptr<ExprList> _pExprListResolved;
public:
	Context(const String &pathNameSrc);
	inline void SetFileNameJR(const String &fileNameJR) { _fileNameJR = fileNameJR; }
	inline const char *GetDirNameSrc() const { return _dirNameSrc.c_str(); }
	inline const char *GetFileNameJR() const { return _fileNameJR.c_str(); }
	inline const char *GetFileBaseNameSrc() const { return _fileBaseNameSrc.c_str(); }
	inline void SelectCodeSegment() { _pSegmentCur = _segmentOwner[0]; }
	inline void SelectDataSegment() { _pSegmentCur = _segmentOwner[1]; }
	inline void SelectInternalSegment() { _pSegmentCur = _segmentOwner[2]; }
	inline Binary &GetSegmentBuffer() { return _pSegmentCur->GetBuffer(); }
	inline const Binary &GetSegmentBuffer() const { return _pSegmentCur->GetBuffer(); }
	inline void ResetSegment() { _segmentOwner.ClearRegion(); }
	inline Number GetAddress() const { return _pSegmentCur->GetAddress(); }
	inline void ForwardAddress(Number bytes) { _pSegmentCur->ForwardAddress(bytes); }
	inline SegmentOwner &GetSegmentOwner() { return _segmentOwner; }
	inline const SegmentOwner &GetSegmentOwner() const { return _segmentOwner; }
	inline void SetPhase(Phase phase) { _phaseCur = phase; }
	inline bool IsPhase(Phase phase) const { return _phaseCur == phase; }
	inline ExprDict &GetExprDictCurrent() { return *_exprDictStack.back(); }
	inline const ExprDict &GetExprDictCurrent() const { return *_exprDictStack.back(); }
	inline ExprDict &GetExprDictGlobal() { return *_exprDictStack.front(); }
	inline const ExprDict &GetExprDictGlobal() const { return *_exprDictStack.front(); }
	inline MacroDict &GetMacroDict() { return _macroDict; }
	inline const MacroDict &GetMacroDict() const { return _macroDict; }
	inline bool CheckSegmentRegionReady() const { return _pSegmentCur->CheckRegionReady(); }
	inline bool DoesExistLocalExprDict() const { return _exprDictStack.size() > 1; }
	inline void SetPCGPageCur(PCGPage *pPCGPage) { _pPCGPageCur.reset(pPCGPage); }
	inline PCGPage *GetPCGPageCur() { return _pPCGPageCur.get(); }
	inline const PCGCharOwner &GetPCGCharsBuiltIn() const { return _pcgCharsBuiltIn; }
	bool ParseFile();
	bool Prepare();
	RegionOwner *Generate(size_t bytesGapToJoin, UInt8 dataFiller);
	bool DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine);
	void StartRegion(Number addr);
	void BeginScope();
	void EndScope();
	String MakeFileNameOut(const char *extName) const;
	SymbolInfoOwner *MakeSymbolInfoOwner();
	void StartToResolve();
	bool CheckCircularReference(const Expr *pExpr);
};

#endif