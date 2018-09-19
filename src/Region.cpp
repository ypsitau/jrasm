//=============================================================================
// Region.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Region
//-----------------------------------------------------------------------------
void Region::AppendZeros(size_t bytes)
{
	while (bytes-- > 0) _buff += '\0';
}

void Region::Dump() const
{
	size_t col = 0;
	for (auto data : _buff) {
		if (col > 0) ::printf(" ");
		::printf("%02x", static_cast<UInt8>(data));
		col++;
		if (col == 32) {
			::printf("\n");
			col = 0;
		}
	}
	if (col > 0) ::printf("\n");
}

//-----------------------------------------------------------------------------
// RegionList
//-----------------------------------------------------------------------------
RegionOwner *RegionList::Join(size_t bytesGapToJoin) const
{
	std::unique_ptr<RegionOwner> pRegionOwner(new RegionOwner());
	const_iterator ppRegion = begin();
	if (ppRegion == end()) return pRegionOwner.release();
	pRegionOwner->push_back((*ppRegion)->Clone());
	ppRegion++;
	for ( ; ppRegion != end(); ppRegion++) {
		const Region *pRegion = *ppRegion;
		Region *pRegionPrev = pRegionOwner->back();
		if (pRegion->GetAddrTop() < pRegionPrev->GetAddrBtm()) {
			ErrorLog::AddError("memory regions are overwrapped at 0x%04x", pRegion->GetAddrTop());
			return nullptr;
		} else if (pRegion->GetAddrTop() - pRegionPrev->GetAddrBtm() < bytesGapToJoin) {
			pRegionPrev->AppendZeros(pRegion->GetAddrTop() - pRegionPrev->GetAddrBtm());
			pRegionPrev->AppendBuffer(pRegion->GetBuffer());
		} else {
			pRegionOwner->push_back(pRegion->Clone());
		}
	}
	return pRegionOwner.release();
}

//-----------------------------------------------------------------------------
// RegionOwner
//-----------------------------------------------------------------------------
RegionOwner::~RegionOwner()
{
	Clear();
}

void RegionOwner::Clear()
{
	for (auto pRegion : *this) {
		Region::Delete(pRegion);
	}
	clear();
}

RegionOwner *RegionOwner::Join()
{
	return nullptr;
}
