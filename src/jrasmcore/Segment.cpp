//=============================================================================
// Segment.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Segment
//-----------------------------------------------------------------------------
Segment::Segment(const String &name, Segment *pSegmentPrev) :
	_cntRef(1), _name(name), _pSegmentPrev(pSegmentPrev)
{
	_pRegionCur = new Region(0);
	_regionOwner.push_back(_pRegionCur);
}

void Segment::AddRegion(Integer addrTop)
{
	if (_pRegionCur->GetAddrTop() == 0) {
		_pRegionCur->SetAddrTop(addrTop);
	} else {
		_pRegionCur = new Region(addrTop);
		_regionOwner.push_back(_pRegionCur);
	}
}

bool Segment::AdjustAddress()
{
	Region *pRegionTop = _regionOwner.front();
	if (pRegionTop->GetAddrTop() == 0) {
		Integer addrTop = 0;
		if (_pSegmentPrev.IsNull() || (addrTop = _pSegmentPrev->GetAddrBtmMax()) == 0) {
			ErrorLog::AddError("missing .org directive in %s segment", GetName());
			return false;
		}
		pRegionTop->SetAddrTop(addrTop);
	}
	return true;
}

Integer Segment::GetAddrBtmMax() const
{
	Integer addrBtmMaxPrev = _pSegmentPrev.IsNull()? 0 : _pSegmentPrev->GetAddrBtmMax();
	Integer addrBtmMax = GetRegionOwner().GetAddrBtmMax();
	return (addrBtmMax > addrBtmMaxPrev)? addrBtmMax : addrBtmMaxPrev;
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

RegionOwner *SegmentList::JoinRegionToGenerate(size_t bytesGapToJoin, UInt8 dataFiller) const
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
