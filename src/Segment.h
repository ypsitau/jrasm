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
	RegionOwner _regionOwner;
public:
	DeclareReferenceAccessor(Segment);
public:
	inline Segment() : _cntRef(1) {}
private:
	inline ~Segment() {}
public:
	inline RegionOwner &GetRegionOwner() { return _regionOwner; }
	inline const RegionOwner &GetRegionOwner() const { return _regionOwner; }
};

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
class SegmentList : public std::vector<Segment *> {
public:
	RegionOwner *JoinRegion(size_t bytesGapToJoin) const;
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
