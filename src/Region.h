//=============================================================================
// Region.h
//=============================================================================
#ifndef __REGION_H__
#define __REGION_H__

#include "Common.h"

class RegionOwner;

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
	UInt16 _addrTop;
	Binary _buff;
	std::unique_ptr<RegionOwner> _pRegionsIngredient;
public:
	DeclareReferenceAccessor(Region);
public:
	Region(UInt16 addrTop);
	Region(const Region &region);
private:
	inline ~Region() {}
public:
	inline UInt16 GetAddrTop() const { return _addrTop; }
	inline UInt16 GetAddrBtm() const { return static_cast<UInt16>(_addrTop + _buff.size()); }
	inline UInt16 GetBytes() const { return _buff.size(); }
	inline Binary &GetBuffer() { return _buff; }
	inline const Binary &GetBuffer() const { return _buff; }
	inline Region *Clone() const { return new Region(*this); }
	inline void AppendBuffer(const Binary &buff) { _buff += buff; }
	inline const RegionOwner &GetRegionsIngredient() const { return *_pRegionsIngredient; }
	void AddRegionIngredient(Region *pRegion);
	void AppendFiller(UInt8 dataFiller, size_t bytes);
	void Dump() const;
};

//-----------------------------------------------------------------------------
// RegionList
//-----------------------------------------------------------------------------
class RegionList : public std::vector<Region *> {
public:
	inline void Sort() { std::sort(begin(), end(), Region::LessThan()); }
	RegionOwner *Join(size_t bytesGapToJoin, UInt8 dataFiller) const;
};

//-----------------------------------------------------------------------------
// RegionOwner
//-----------------------------------------------------------------------------
class RegionOwner : public RegionList {
public:
	~RegionOwner();
	void Clear();
};

#endif
