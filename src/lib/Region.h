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
	Integer _addrTop;
	Integer _addrOffset;
	Binary _buff;
	std::unique_ptr<RegionOwner> _pRegionsIngredient;
public:
	DeclareReferenceAccessor(Region);
public:
	Region(Integer addrTop);
	Region(const Region &region);
private:
	inline ~Region() {}
public:
	inline void SetAddrTop(Integer addrTop) { _addrTop = addrTop; }
	inline void ClearAddrOffset() { _addrOffset = 0; }
	inline Integer GetAddrTop() const { return _addrTop; }
	inline Integer GetAddrOffset() const { return _addrTop; }
	inline void ForwardAddrOffset(Integer bytes) { _addrOffset += bytes; }
	inline Integer GetAddrBtm() const { return static_cast<Integer>(_addrTop + _buff.size()); }
	inline Integer GetBytes() const { return static_cast<Integer>(_buff.size()); }
	inline bool IsBufferEmpty() const { return _buff.empty(); }
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

#endif
