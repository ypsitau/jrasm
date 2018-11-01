//=============================================================================
// Segment.h
//=============================================================================
#ifndef __JRASMCORE_SEGMENT_H__
#define __JRASMCORE_SEGMENT_H__

#include "Region.h"

//-----------------------------------------------------------------------------
// Segment
//-----------------------------------------------------------------------------
class Segment {
private:
	int _cntRef;
	String _name;
	bool _generationFlag;
	AutoPtr<Segment> _pSegmentPrev;
	RegionOwner _regionOwner;
	Region *_pRegionCur;
public:
	DeclareReferenceAccessor(Segment);
public:
	Segment(const String &name, bool generationFlag, Segment *pSegmentPrev);
private:
	inline ~Segment() {}
public:
	inline const char *GetName() const { return _name.c_str(); }
	inline bool GetGenerationFlag() const { return _generationFlag; }
	inline const RegionOwner &GetRegionOwner() const { return _regionOwner; }
	inline void SetRegionCur(Region *pRegionCur) { _pRegionCur = pRegionCur; }
	inline Region *GetRegionCur() { return _pRegionCur; }
	inline const Region *GetRegionCur() const { return _pRegionCur; }
	inline Region *FindRegionByAddrTop(Integer addrTop) { return _regionOwner.FindByAddrTop(addrTop); }
	inline Binary &GetBuffer() { return _pRegionCur->GetBuffer(); }
	inline const Binary &GetBuffer() const { return _pRegionCur->GetBuffer(); }
	inline void ResetAddrOffset() { _regionOwner.ResetAddrOffset(); }
	inline Integer GetAddrTop() const { return _pRegionCur->GetAddrTop(); }
	inline Integer GetAddrOffset() const { return _pRegionCur->GetAddrOffset(); }
	inline Integer GetAddress() const { return _pRegionCur->GetAddress(); }
	inline void ForwardAddrOffset(Integer bytes) { _pRegionCur->ForwardAddrOffset(bytes); }
	void AddRegion(Integer addrTop);
	bool AdjustAddress();
	Integer GetAddrBtmMax() const;
};

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
class SegmentList : public std::vector<Segment *> {
public:
	void ResetAddrOffset();
	RegionOwner *JoinRegionToGenerate(size_t bytesGapToJoin, UInt8 dataFiller) const;
	bool AdjustAddress();
};

//-----------------------------------------------------------------------------
// SegmentOwner
//-----------------------------------------------------------------------------
class SegmentOwner : public SegmentList {
public:
	~SegmentOwner();
	void Clear();
};

#endif
