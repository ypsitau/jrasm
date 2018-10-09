//=============================================================================
// Segment.h
//=============================================================================
#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include "Region.h"

//-----------------------------------------------------------------------------
// Segment
//-----------------------------------------------------------------------------
class Segment {
private:
	int _cntRef;
	String _name;
	AutoPtr<Segment> _pSegmentPrev;
	RegionOwner _regionOwner;
	Region *_pRegionCur;
	Integer _addrOffset;
public:
	DeclareReferenceAccessor(Segment);
public:
	Segment(const String &name, Segment *pSegmentPrev);
private:
	inline ~Segment() {}
public:
	inline const char *GetName() const { return _name.c_str(); }
	inline const RegionOwner &GetRegionOwner() const { return _regionOwner; }
	inline void SetRegionCur(Region *pRegionCur) { _pRegionCur = pRegionCur; }
	inline Region *GetRegionCur() { return _pRegionCur; }
	inline const Region *GetRegionCur() const { return _pRegionCur; }
	inline Region *FindRegionByAddrTop(Integer addrTop) { return _regionOwner.FindByAddrTop(addrTop); }
	inline Binary &GetBuffer() { return GetRegionCur()->GetBuffer(); }
	inline const Binary &GetBuffer() const { return GetRegionCur()->GetBuffer(); }
	inline void ResetAddrOffset() { _addrOffset = 0; }
	inline Integer GetAddrTop() const { return GetRegionCur()->GetAddrTop(); }
	inline Integer GetAddrOffset() const { return _addrOffset; }
	inline Integer GetAddress() const { return GetAddrTop() + _addrOffset; }
	inline void ForwardAddrOffset(Integer bytes) { _addrOffset += bytes; }
	bool PrepareRegion();
	void AddRegion(Region *pRegion);
	bool AdjustAddress();
};

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
class SegmentList : public std::vector<Segment *> {
public:
	void ResetAddrOffset();
	RegionOwner *JoinRegion(size_t bytesGapToJoin, UInt8 dataFiller) const;
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
