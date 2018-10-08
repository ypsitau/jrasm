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
	Integer _addrOrg;
	Integer _addrOffset;
public:
	DeclareReferenceAccessor(Segment);
public:
	inline Segment(const String &name) : _cntRef(1), _name(name), _addrOrg(0), _addrOffset(0) {}
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
	inline void SetAddrOrg(Integer addrOrg) { _addrOrg = addrOrg; }
	inline void SetAddrOffset(Integer addrOffset) { _addrOffset = addrOffset; }
	inline Integer GetAddrOrg() const { return _addrOrg; }
	inline Integer GetAddrOffset() const { return _addrOffset; }
	inline Integer GetAddress() const { return _addrOrg + _addrOffset; }
	inline void ForwardAddrOffset(Integer bytes) { _addrOffset += bytes; }
	bool CheckRegionReady() const;
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
