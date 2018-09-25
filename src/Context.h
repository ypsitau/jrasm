//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Segment.h"

class Parser;
class Expr;
class ExprList;
class Directive;
class DirectiveOwner;

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
		PHASE_SetupLookup,
		PHASE_Generate,
	};
public:
	struct LessThan_StringICase {
		inline bool operator()(const String &str1, const String &str2) const {
			return ::strcasecmp(str1.c_str(), str2.c_str()) < 0;
		}
	};
	class LookupTable : public std::map<String, Expr *, LessThan_StringICase> {
	private:
		int _cntRef;
		AutoPtr<LookupTable> _pLookupTableParent;
	public:
		DeclareReferenceAccessor(LookupTable);
	public:
		inline LookupTable(LookupTable *pLookupTableParent = nullptr) :
			_cntRef(1), _pLookupTableParent(pLookupTableParent) {}
	private:
		~LookupTable();
	public:
		inline LookupTable *GetParent() { return _pLookupTableParent.get(); }
		void Associate(const String &label, Expr *pExpr, bool forceGlobalFlag);
		bool IsDefined(const char *label) const;
		const Expr *Lookup(const char *label) const;
		LookupTable *GetGlobal();
		inline const LookupTable *GetGlobal() const {
			return const_cast<LookupTable *>(this)->GetGlobal();
		}
	};
	class LookupTableList : public std::vector<LookupTable *> {
	};
	class LookupTableOwner : public LookupTableList {
	public:
		~LookupTableOwner();
		void Clear();
	};
	typedef LookupTableOwner LookupTableStack;
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
	std::unique_ptr<Parser> _pParser;
	std::unique_ptr<DirectiveOwner> _pDirectiveOwner;
	String _fileNameJR;
	String _fileBaseNameSrc;
	Segment *_pSegmentCur;
	SegmentOwner _segmentOwner;
	Phase _phaseCur;
	LookupTableStack _lookupTableStack;
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
	inline LookupTable *GetLookupTable() { return _lookupTableStack.back(); }
	inline const LookupTable *GetLookupTable() const { return _lookupTableStack.back(); }
	inline LookupTable *GetLookupTableGlobal() { return _lookupTableStack.front(); }
	inline const LookupTable *GetLookupTableGlobal() const { return _lookupTableStack.front(); }
	inline bool CheckRegionReady() const { return _pSegmentCur->CheckRegionReady(); }
	inline bool DoesExistLocalLookupTable() const { return _lookupTableStack.size() > 1; }
	bool ParseFile();
	bool Prepare();
	RegionOwner *Generate(size_t bytesGapToJoin, UInt8 dataFiller);
	bool DumpDisasm(FILE *fp, bool upperCaseFlag, size_t nColsPerLine);
	void StartRegion(UInt32 addr);
	void PushLocalLookupTable();
	void PopLocalLookupTable();
	LabelInfoOwner *MakeLabelInfoOwner();
	void StartToResolve();
	bool CheckCircularReference(const Expr *pExpr);
	const Directive *FindDirective(const char *symbol) const;
};

#endif
