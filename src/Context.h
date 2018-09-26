//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Segment.h"
#include "Expr.h"
#include "Macro.h"

class Parser;

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
class Context {
public:
	enum Phase {
		PHASE_None,
		PHASE_Include,
		PHASE_DeclareMacro,
		PHASE_ExpandMacro,
		PHASE_SetupExprDict,
		PHASE_Generate,
	};
public:
	class LabelInfo {
	public:
		struct LessThan_Number {
			inline bool operator()(const LabelInfo *pLabelInfo1, const LabelInfo *pLabelInfo2) const {
				return pLabelInfo1->GetNumber() < pLabelInfo2->GetNumber();
			}
		};
		struct LessThan_Label {
			inline bool operator()(const LabelInfo *pLabelInfo1, const LabelInfo *pLabelInfo2) const {
				return ::strcmp(pLabelInfo1->GetLabel(), pLabelInfo2->GetLabel()) < 0;
			}
		};
	private:
		UInt32 _num;
		String _label;
	public:
		inline LabelInfo(UInt32 num, const String &label) : _num(num), _label(label) {}
		inline UInt16 GetNumber() const { return _num; }
		inline const char *GetLabel() const { return _label.c_str(); }
	};
	class LabelInfoList : public std::vector<LabelInfo *> {
	public:
		void SortByNumber() { std::sort(begin(), end(), LabelInfo::LessThan_Number()); }
		void SortByLabel() { std::sort(begin(), end(), LabelInfo::LessThan_Label()); }
		static size_t GetLabelLenMax(const_iterator ppLabelInfo, const_iterator ppLabelInfoEnd);
	};
	class LabelInfoOwner : public LabelInfoList {
	public:
		~LabelInfoOwner();
		void Clear();
	};
private:
	String _pathNameSrc;
	String _fileBaseNameSrc;
	String _fileNameJR;
	Segment *_pSegmentCur;
	SegmentOwner _segmentOwner;
	Phase _phaseCur;
	ExprDictStack _exprDictStack;
	MacroDict _macroDict;
	AutoPtr<Expr> _pExprRoot;
	std::unique_ptr<ExprList> _pExprListResolved;
public:
	Context(const String &pathNameSrc);
	inline void SetFileNameJR(const String &fileNameJR) { _fileNameJR = fileNameJR; }
	inline const char *GetFileNameJR() const { return _fileNameJR.c_str(); }
	inline const char *GetFileBaseNameSrc() const { return _fileBaseNameSrc.c_str(); }
	inline String MakeFileNameOut(const char *extName) const { return _fileBaseNameSrc + extName; }
	inline void SelectCodeSegment() { _pSegmentCur = _segmentOwner[0]; }
	inline void SelectDataSegment() { _pSegmentCur = _segmentOwner[1]; }
	inline void SelectInternalSegment() { _pSegmentCur = _segmentOwner[2]; }
	inline Binary &GetBuffer() { return _pSegmentCur->GetBuffer(); }
	inline const Binary &GetBuffer() const { return _pSegmentCur->GetBuffer(); }
	inline void ResetSegment() { _segmentOwner.ClearRegion(); }
	inline UInt32 GetAddress() const { return _pSegmentCur->GetAddress(); }
	inline void ForwardAddress(UInt32 bytes) { _pSegmentCur->ForwardAddress(bytes); }
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
	inline bool CheckRegionReady() const { return _pSegmentCur->CheckRegionReady(); }
	inline bool DoesExistLocalExprDict() const { return _exprDictStack.size() > 1; }
	bool ParseFile();
	bool Prepare();
	RegionOwner *Generate(size_t bytesGapToJoin, UInt8 dataFiller);
	bool DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine);
	void StartRegion(UInt32 addr);
	void PushLocalExprDict();
	void PopLocalExprDict();
	LabelInfoOwner *MakeLabelInfoOwner();
	void StartToResolve();
	bool CheckCircularReference(const Expr *pExpr);
};

#endif
