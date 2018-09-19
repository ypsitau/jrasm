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
public:
	struct LessThan {
		inline bool operator()(const Chunk *pChunk1, const Chunk *pChunk2) const {
			return pChunk1->GetAddrTop() < pChunk2->GetAddrTop();
		}
	};
private:
	int _cntRef;
	UInt16 _addrTop;
	Binary _buff;
public:
	DeclareReferenceAccessor(Chunk);
public:
	inline Chunk(UInt16 addrTop) : _cntRef(1), _addrTop(addrTop) {}
	inline Chunk(const Chunk &chunk) : _cntRef(1), _addrTop(chunk._addrTop), _buff(chunk._buff) {}
private:
	inline ~Chunk() {}
public:
	inline UInt16 GetAddrTop() const { return _addrTop; }
	inline UInt16 GetAddrBtm() const { return static_cast<UInt16>(_addrTop + _buff.size()); }
	inline Binary &GetBuffer() { return _buff; }
	inline const Binary &GetBuffer() const { return _buff; }
	inline Chunk *Clone() const { return new Chunk(*this); }
	inline void AppendBuffer(const Binary &buff) { _buff += buff; }
	void AppendZeros(size_t bytes);
	void Dump() const;
};

//-----------------------------------------------------------------------------
// ChunkList
//-----------------------------------------------------------------------------
class ChunkList : public std::vector<Chunk *> {
public:
	inline void Sort() { std::sort(begin(), end(), Chunk::LessThan()); }
	ChunkOwner *Join(size_t bytesGapToJoin) const;
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
