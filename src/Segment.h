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
	RegionOwner _regionOwner;
public:
	DeclareReferenceAccessor(Segment);
public:
	inline Segment(const String &name) : _cntRef(1), _name(name) {}
private:
	inline ~Segment() {}
public:
	inline const char *GetName() const { return _name.c_str(); }
	inline RegionOwner &GetRegionOwner() { return _regionOwner; }
	inline const RegionOwner &GetRegionOwner() const { return _regionOwner; }
	inline void AddRegion(Region *pRegion) { _regionOwner.push_back(pRegion); }
	inline Binary &GetBuffer() { return _regionOwner.back()->GetBuffer(); }
	inline const Binary &GetBuffer() const { return _regionOwner.back()->GetBuffer(); }
	inline void ClearRegion() { _regionOwner.Clear(); }
	bool CheckRegionReady() const;
};

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
class SegmentList : public std::vector<Segment *> {
public:
	void ClearRegion();
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
