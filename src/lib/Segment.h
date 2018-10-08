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
	Integer _addrOffset;
public:
	DeclareReferenceAccessor(Segment);
public:
	inline Segment(const String &name) : _cntRef(1), _name(name), _addrOffset(0) {}
private:
	inline ~Segment() {}
public:
	inline const char *GetName() const { return _name.c_str(); }
	inline RegionOwner &GetRegionOwner() { return _regionOwner; }
	inline const RegionOwner &GetRegionOwner() const { return _regionOwner; }
	inline Region *GetRegionCur() { return _regionOwner.back(); }
	inline const Region *GetRegionCur() const { return _regionOwner.back(); }
	inline void AddRegion(Region *pRegion) { _regionOwner.push_back(pRegion); }
	inline Binary &GetBuffer() { return GetRegionCur()->GetBuffer(); }
	inline const Binary &GetBuffer() const { return GetRegionCur()->GetBuffer(); }
	inline void ClearRegion() { _regionOwner.Clear(); }
	inline void SetAddrOffset(Integer addrOffset) { _addrOffset = addrOffset; }
	inline Integer GetAddrTop() const { return GetRegionCur()->GetAddrTop(); }
	inline Integer GetAddrOffset() const { return _addrOffset; }
	inline Integer GetAddress() const { return GetAddrTop() + _addrOffset; }
	inline void ForwardAddrOffset(Integer bytes) { _addrOffset += bytes; }
	bool PrepareRegion() const;
};

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
class SegmentList : public std::vector<Segment *> {
public:
	void ClearRegion();
	RegionOwner *JoinRegion(size_t bytesGapToJoin, UInt8 dataFiller) const;
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
