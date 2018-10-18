//=============================================================================
// Region.h
//=============================================================================
#ifndef __JRASMCORE_REGION_H__
#define __JRASMCORE_REGION_H__

#include "Common.h"

class Region;
class RegionOwner;

//-----------------------------------------------------------------------------
// RegionList
//-----------------------------------------------------------------------------
class RegionList : public std::vector<Region *> {
public:
	void Sort();
	void ResetAddrOffset();
	RegionOwner *Join(size_t bytesGapToJoin, UInt8 dataFiller) const;
	Region *FindByAddrTop(Integer addrTop);
	Integer GetAddrBtmMax() const;
};

//-----------------------------------------------------------------------------
// RegionOwner
//-----------------------------------------------------------------------------
class RegionOwner : public RegionList {
public:
	~RegionOwner();
	void Clear();
};

//-----------------------------------------------------------------------------
// Region
//-----------------------------------------------------------------------------
class Region {
public:
	struct LessThan {
		inline bool operator()(const Region *pRegion1, const Region *pRegion2) const {
			return pRegion1->GetAddrTop() < pRegion2->GetAddrTop();
		}
	};
private:
	int _cntRef;
	Integer _addrTop;
	Integer _addrOffset;
	Binary _buff;
	RegionOwner _regionsIngredient;
public:
	DeclareReferenceAccessor(Region);
public:
	Region(Integer addrTop);
	Region(const Region &region);
private:
	inline ~Region() {}
public:
	inline void SetAddrTop(Integer addrTop) { _addrTop = addrTop; }
	inline void ResetAddrOffset() { _addrOffset = 0; }
	inline void ForwardAddrOffset(Integer bytes) { _addrOffset += bytes; }
	inline Integer GetAddrTop() const { return _addrTop; }
	inline Integer GetAddrOffset() const { return _addrOffset; }
	inline Integer GetAddress() const { return _addrTop + _addrOffset; }
	inline Integer GetAddrBtm() const { return _addrTop + _addrOffset; }
	inline Integer GetBytes() const { return static_cast<Integer>(_buff.size()); }
	inline bool IsBufferEmpty() const { return _buff.empty(); }
	inline Binary &GetBuffer() { return _buff; }
	inline const Binary &GetBuffer() const { return _buff; }
	inline Region *Clone() const { return new Region(*this); }
	inline void AppendBuffer(const Binary &buff) { _buff += buff; }
	inline const RegionOwner &GetRegionsIngredient() const { return _regionsIngredient; }
	void AddRegionIngredient(Region *pRegion);
	void AppendFiller(UInt8 dataFiller, size_t bytes);
	void Dump() const;
};

#endif
