//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Segment.h"

class Expr;

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
class Context {
public:
	class LookupTable : public std::map<String, UInt32> {
	private:
		int _cntRef;
		AutoPtr<LookupTable> _pLookupTableParent;
	public:
		DeclareReferenceAccessor(LookupTable);
	public:
		inline LookupTable(LookupTable *pLookupTableParent = nullptr) :
			_cntRef(1), _pLookupTableParent(pLookupTableParent) {}
	private:
		inline ~LookupTable() {}
	public:
		inline LookupTable *GetParent() { return _pLookupTableParent.get(); }
		static inline bool IsGlobalLabel(const char *label) { return *label != '@'; }
		void Set(const String &label, UInt32 value);
		bool IsDefined(const char *label) const;
		UInt32 Lookup(const char *label, bool *pFoundFlag) const;
		LookupTable *GetRoot();
		inline const LookupTable *GetRoot() const {
			return const_cast<LookupTable *>(this)->GetRoot();
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
		struct LessThan_Addr {
			inline bool operator()(const LabelInfo *pLabelInfo1, const LabelInfo *pLabelInfo2) const {
				return pLabelInfo1->GetAddr() < pLabelInfo2->GetAddr();
			}
		};
		struct LessThan_Label {
			inline bool operator()(const LabelInfo *pLabelInfo1, const LabelInfo *pLabelInfo2) const {
				return ::strcmp(pLabelInfo1->GetLabel(), pLabelInfo2->GetLabel()) < 0;
			}
		};
	private:
		UInt16 _addr;
		String _label;
	public:
		inline LabelInfo(UInt16 addr, const String &label) : _addr(addr), _label(label) {}
		inline UInt16 GetAddr() const { return _addr; }
		inline const char *GetLabel() const { return _label.c_str(); }
	};
	class LabelInfoList : public std::vector<LabelInfo *> {
	public:
		void SortByAddr() { std::sort(begin(), end(), LabelInfo::LessThan_Addr()); }
		void SortByLabel() { std::sort(begin(), end(), LabelInfo::LessThan_Label()); }
		static size_t GetLabelLenMax(const_iterator ppLabelInfo, const_iterator ppLabelInfoEnd);
	};
	class LabelInfoOwner : public LabelInfoList {
	public:
		~LabelInfoOwner();
		void Clear();
	};
private:
	String _fileNameJR;
	Segment *_pSegmentCur;
	SegmentOwner _segmentOwner;
	bool _preparationFlag;
	LookupTableStack _lookupTableStack;
public:
	Context();
	inline void SetFileNameJR(const String &fileNameJR) { _fileNameJR = fileNameJR; }
	inline const char *GetFileNameJR() const { return _fileNameJR.c_str(); }
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
	inline void SetPreparationFlag(bool preparationFlag) { _preparationFlag = preparationFlag; }
	inline bool GetPreparationFlag() const { return _preparationFlag; }
	inline LookupTable *GetLookupTable() { return _lookupTableStack.back(); }
	inline const LookupTable *GetLookupTable() const { return _lookupTableStack.back(); }
	inline LookupTable *GetLookupTableRoot() { return _lookupTableStack.front(); }
	inline const LookupTable *GetLookupTableRoot() const { return _lookupTableStack.front(); }
	inline bool CheckRegionReady() const { return _pSegmentCur->CheckRegionReady(); }
	void StartRegion(UInt32 addr);
	LookupTable *AddLookupTable();
	void RemoveLookupTable();
	LabelInfoOwner *MakeLabelInfoOwner() const;
};

#endif
