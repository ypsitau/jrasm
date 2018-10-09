//=============================================================================
// Segment.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Segment
//-----------------------------------------------------------------------------
Segment::Segment(const String &name, Segment *pSegmentPrev) :
	_cntRef(1), _name(name), _pSegmentPrev(pSegmentPrev), _pRegionCur(nullptr), _addrOffset(0)
{
	//AddRegion(new Region(0));
}

bool Segment::PrepareRegion()
{
	if (!_regionOwner.empty()) return true;
	if (_pSegmentPrev.IsNull()) {
		ErrorLog::AddError("missing .org directive in %s segment", GetName());
		return false;
	}
	AddRegion(new Region(0)); // create a temporary region with the start address 0.
	return true;
}

void Segment::AddRegion(Region *pRegion)
{
	_regionOwner.push_back(pRegion);
	_pRegionCur = pRegion;
}

bool Segment::AdjustAddress()
{
	if (_regionOwner.empty()) return true;
	Region *pRegionTop = _regionOwner.front();
	if (pRegionTop->GetAddrTop() == 0) {
		Integer addrTop = _pSegmentPrev->GetRegionOwner().GetAddrBtmMax();
		if (addrTop == 0) {
			ErrorLog::AddError("missing .org directive in %s segment", GetName());
			return false;
		}
		pRegionTop->SetAddrTop(addrTop);
	}
	return true;
}

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
void SegmentList::ResetAddrOffset()
{
	for (auto pSegment : *this) {
		pSegment->ResetAddrOffset();
	}
}

RegionOwner *SegmentList::JoinRegion(size_t bytesGapToJoin, UInt8 dataFiller) const
{
	RegionList regionList;
	for (auto pSegment : *this) {
		for (auto pRegion : pSegment->GetRegionOwner()) {
			if (!pRegion->IsBufferEmpty()) regionList.push_back(pRegion);
		}
	}
	regionList.Sort();
	return regionList.Join(bytesGapToJoin, dataFiller);
}

bool SegmentList::AdjustAddress()
{
	for (auto pSegment : *this) {
		if (!pSegment->AdjustAddress()) return false;
	}
	return true;
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
