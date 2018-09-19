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
	class LookupTableOwner : public std::vector<LookupTable *> {
	public:
		~LookupTableOwner();
		void Clear();
	};
	typedef LookupTableOwner LookupTableStack;
private:
	UInt32 _addr;
	Segment *_pSegmentCur;
	SegmentOwner _segmentOwner;
	bool _preparationFlag;
	LookupTableStack _lookupTableStack;
public:
	Context();
	inline void SelectCodeSegment() { _pSegmentCur = _segmentOwner[0]; }
	inline void SelectDataSegment() { _pSegmentCur = _segmentOwner[1]; }
	inline Binary &GetBuffer() { return _pSegmentCur->GetBuffer(); }
	inline const Binary &GetBuffer() const { return _pSegmentCur->GetBuffer(); }
	inline void ResetSegment() { _segmentOwner.ClearRegion(); }
	inline void StartRegion(UInt32 addr) { _pSegmentCur->AddRegion(new Region(addr)), _addr = addr; }
	inline UInt32 GetAddress() const { return _addr; }
	inline void ForwardAddress(UInt32 bytes) { _addr += bytes; }
	inline void SetPreparationFlag(bool preparationFlag) { _preparationFlag = preparationFlag; }
	inline bool GetPreparationFlag() const { return _preparationFlag; }
	inline LookupTable *GetLookupTable() { return _lookupTableStack.back(); }
	inline LookupTable *GetLookupTableRoot() { return _lookupTableStack.front(); }
	inline bool CheckRegionReady() const { return _pSegmentCur->CheckRegionReady(); }
	LookupTable *AddLookupTable();
	void RemoveLookupTable();
};

#endif
