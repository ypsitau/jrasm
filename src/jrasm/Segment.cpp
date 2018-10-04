//=============================================================================
// Segment.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Segment
//-----------------------------------------------------------------------------
bool Segment::CheckRegionReady() const
{
	if (_regionOwner.empty()) {
		ErrorLog::AddError("missing .org directive in %s segment", GetName());
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
void SegmentList::ClearRegion()
{
	for (auto pSegment : *this) {
		pSegment->ClearRegion();
	}
}

RegionOwner *SegmentList::JoinRegion(size_t bytesGapToJoin, UInt8 dataFiller) const
{
	RegionList regionList;
	for (auto pSegment : *this) {
		const RegionOwner &regionOwnerOrg = pSegment->GetRegionOwner();
		regionList.insert(regionList.end(), regionOwnerOrg.begin(), regionOwnerOrg.end());
	}
	regionList.Sort();
	return regionList.Join(bytesGapToJoin, dataFiller);
}

//-----------------------------------------------------------------------------
// SegmentOwner
//-----------------------------------------------------------------------------
SegmentOwner::~SegmentOwner()
{
	Clear();
}

void SegmentOwner::Clear()
{
	for (auto pSegment : *this) {
		Segment::Delete(pSegment);
	}
	clear();
}
