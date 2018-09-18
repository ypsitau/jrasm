//=============================================================================
// Chunk.h
//=============================================================================
#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "Common.h"

class ChunkOwner;

//-----------------------------------------------------------------------------
// Chunk
//-----------------------------------------------------------------------------
class Chunk {
private:
	int _cntRef;
	UInt16 _addrTop;
	Binary _buff;
public:
	DeclareReferenceAccessor(Chunk);
public:
	inline Chunk(UInt16 addrTop) : _cntRef(1), _addrTop(addrTop) {}
private:
	inline ~Chunk() {}
public:
	inline UInt16 GetAddrTop() const { return _addrTop; }
	inline Binary &GetBuffer() { return _buff; }
	inline const Binary &GetBuffer() const { return _buff; }
	void Dump() const;
};

//-----------------------------------------------------------------------------
// ChunkList
//-----------------------------------------------------------------------------
class ChunkList : public std::vector<Chunk *> {
public:
	void SortByAddrTop();
};

//-----------------------------------------------------------------------------
// ChunkOwner
//-----------------------------------------------------------------------------
class ChunkOwner : public ChunkList {
public:
	~ChunkOwner();
	void Clear();
	ChunkOwner *Join();
};

#endif
