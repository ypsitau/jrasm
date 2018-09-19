//=============================================================================
// Chunk.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Chunk
//-----------------------------------------------------------------------------
void Chunk::AppendZeros(size_t bytes)
{
	while (bytes-- > 0) _buff += '\0';
}

void Chunk::Dump() const
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
// ChunkList
//-----------------------------------------------------------------------------
ChunkOwner *ChunkList::Join(size_t bytesGapToJoin) const
{
	std::unique_ptr<ChunkOwner> pChunkOwner(new ChunkOwner());
	const_iterator ppChunk = begin();
	if (ppChunk == end()) return pChunkOwner.release();
	pChunkOwner->push_back((*ppChunk)->Clone());
	ppChunk++;
	for ( ; ppChunk != end(); ppChunk++) {
		const Chunk *pChunk = *ppChunk;
		Chunk *pChunkPrev = pChunkOwner->back();
		if (pChunk->GetAddrTop() < pChunkPrev->GetAddrBtm()) {
			ErrorLog::AddError("memory regions are overwrapped at 0x%04x", pChunk->GetAddrTop());
			return nullptr;
		} else if (pChunk->GetAddrTop() - pChunkPrev->GetAddrBtm() < bytesGapToJoin) {
			pChunkPrev->AppendZeros(pChunk->GetAddrTop() - pChunkPrev->GetAddrBtm());
			pChunkPrev->AppendBuffer(pChunk->GetBuffer());
		} else {
			pChunkOwner->push_back(pChunk->Clone());
		}
	}
	return pChunkOwner.release();
}

//-----------------------------------------------------------------------------
// ChunkOwner
//-----------------------------------------------------------------------------
ChunkOwner::~ChunkOwner()
{
	Clear();
}

void ChunkOwner::Clear()
{
	for (auto pChunk : *this) {
		Chunk::Delete(pChunk);
	}
	clear();
}

ChunkOwner *ChunkOwner::Join()
{
	return nullptr;
}
