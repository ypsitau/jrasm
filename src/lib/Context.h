//=============================================================================
// Context.h
//=============================================================================
#ifndef __JRASMCORE_CONTEXT_H__
#define __JRASMCORE_CONTEXT_H__

#include "Segment.h"
#include "Expr.h"
#include "Macro.h"
#include "PCGPage.h"

class Parser;
class Generator;

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
		struct LessThan_Integer {
			inline bool operator()(const SymbolInfo *pSymbolInfo1, const SymbolInfo *pSymbolInfo2) const {
				return pSymbolInfo1->GetInteger() < pSymbolInfo2->GetInteger();
			}
		};
		struct LessThan_Symbol {
			inline bool operator()(const SymbolInfo *pSymbolInfo1, const SymbolInfo *pSymbolInfo2) const {
				return ::strcmp(pSymbolInfo1->GetSymbol(), pSymbolInfo2->GetSymbol()) < 0;
			}
		};
	private:
		Integer _num;
		String _symbol;
	public:
		inline SymbolInfo(Integer num, const String &symbol) : _num(num), _symbol(symbol) {}
		inline Integer GetInteger() const { return _num; }
		inline const char *GetSymbol() const { return _symbol.c_str(); }
	};
	class SymbolInfoList : public std::vector<SymbolInfo *> {
	public:
		void SortByInteger() { std::sort(begin(), end(), SymbolInfo::LessThan_Integer()); }
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
	int _iSavePoint;
	ExprDictStack _exprDictStack;
	MacroDict _macroDict;
	AutoPtr<Expr> _pExprRoot;
	PCGPageOwner _pcgPageOwner;
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
	inline Segment *GetSegmentCur() { return _pSegmentCur; }
	inline const Segment *GetSegumentCur() const { return _pSegmentCur; }
	inline Binary &GetSegmentBuffer() { return _pSegmentCur->GetBuffer(); }
	inline const Binary &GetSegmentBuffer() const { return _pSegmentCur->GetBuffer(); }
	inline void ResetSegment() { _segmentOwner.ResetAddrOffset(); }
	inline Integer GetAddress() const { return _pSegmentCur->GetAddress(); }
	inline void ForwardAddrOffset(Integer bytes) { _pSegmentCur->ForwardAddrOffset(bytes); }
	inline SegmentOwner &GetSegmentOwner() { return _segmentOwner; }
	inline const SegmentOwner &GetSegmentOwner() const { return _segmentOwner; }
	inline void SetPhase(Phase phase) { _phaseCur = phase; }
	inline bool IsPhase(Phase phase) const { return _phaseCur == phase; }
	inline int NextSavePoint() { return ++_iSavePoint; }
	inline ExprDict &GetExprDictCurrent() { return *_exprDictStack.back(); }
	inline const ExprDict &GetExprDictCurrent() const { return *_exprDictStack.back(); }
	inline ExprDict &GetExprDictGlobal() { return *_exprDictStack.front(); }
	inline const ExprDict &GetExprDictGlobal() const { return *_exprDictStack.front(); }
	inline MacroDict &GetMacroDict() { return _macroDict; }
	inline const MacroDict &GetMacroDict() const { return _macroDict; }
	inline bool DoesExistLocalExprDict() const { return _exprDictStack.size() > 1; }
	inline const Expr *GetExprRoot() { return _pExprRoot.get(); }
	inline const PCGPageOwner &GetPCGPageOwner() const { return _pcgPageOwner; }
	inline void AddPCGPage(PCGPage *pPCGPage) { _pcgPageOwner.push_back(pPCGPage); }
	inline PCGPage *GetPCGPageCur() { return _pcgPageOwner.back(); }
	inline const PCGCharOwner &GetPCGCharsBuiltIn() const { return _pcgCharsBuiltIn; }
	static void Initialize(Generator *pGenerator);
	bool ParseFile();
	bool Prepare();
	RegionOwner *Generate(size_t bytesGapToJoin, UInt8 dataFiller);
	bool DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine);
	void PrintSymbolList(FILE *fp, bool upperCaseFlag);
	bool PrintMemoryUsage(FILE *fp, bool upperCaseFlag);
	void PrintPCGUsage(FILE *fp, bool upperCaseFlag);
	bool AddRegion(Integer addrTop);
	void BeginScope();
	void EndScope();
	String MakeFileNameOut(const char *extName) const;
	SymbolInfoOwner *MakeSymbolInfoOwner();
	void StartToResolve();
	bool CheckCircularReference(const Expr *pExpr);
};

#endif
