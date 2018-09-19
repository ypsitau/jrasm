//=============================================================================
// Segment.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
ChunkOwner *SegmentList::JoinChunk(size_t bytesGapToJoin) const
{
	ChunkList chunkList;
	for (auto pSegment : *this) {
		const ChunkOwner &chunkOwnerOrg = pSegment->GetChunkOwner();
		chunkList.insert(chunkList.end(), chunkOwnerOrg.begin(), chunkOwnerOrg.end());
	}
	chunkList.Sort();
	return chunkList.Join(bytesGapToJoin);
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
