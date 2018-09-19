//=============================================================================
// Segment.h
//=============================================================================
#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include "Chunk.h"

//-----------------------------------------------------------------------------
// Segment
//-----------------------------------------------------------------------------
class Segment {
private:
	int _cntRef;
	ChunkOwner _chunkOwner;
public:
	DeclareReferenceAccessor(Segment);
public:
	inline Segment() : _cntRef(1) {}
private:
	inline ~Segment() {}
public:
	inline ChunkOwner &GetChunkOwner() { return _chunkOwner; }
	inline const ChunkOwner &GetChunkOwner() const { return _chunkOwner; }
};

//-----------------------------------------------------------------------------
// SegmentList
//-----------------------------------------------------------------------------
class SegmentList : public std::vector<Segment *> {
public:
	ChunkOwner *JoinChunk(size_t bytesGapToJoin) const;
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
