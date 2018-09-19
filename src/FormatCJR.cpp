//=============================================================================
// FormatCJR.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// FormatCJR
//-----------------------------------------------------------------------------
bool FormatCJR::Write(FILE *fp, const char *fileNameJR, const RegionList &regionList)
{
	const size_t bytesBlockMax = 256;
	UInt8 blockCount = 0;
	do {
		HeaderBlock headerBlock;
		::memcpy(headerBlock.preamble,	"\x02\x2a", 2);
		headerBlock.blockCount			= blockCount++;
		headerBlock.blockSize 			= 0x1a;
		headerBlock.startAddressH		= 0xff;
		headerBlock.startAddressL		= 0xff;
		::strncpy_s(headerBlock.fileNameJR, fileNameJR, sizeof(headerBlock.fileNameJR));
		headerBlock.binaryFlag 			= 0x01;
		headerBlock.baudrate			= 0x00;
		::memcpy(headerBlock.dummy,		"\xff\xff\xff\xff\xff\xff\xff\xff", 8);
		headerBlock.checkSum			= CalcCheckSum(headerBlock.fileNameJR, &headerBlock.checkSum);
		if (::fwrite(&headerBlock, 1, sizeof(headerBlock), fp) < sizeof(headerBlock)) return false;
	} while (0);
	UInt16 addr = 0;
	for (auto pRegion : regionList) {
		addr = pRegion->GetAddrTop();
		const char *data = pRegion->GetBuffer().data();
		size_t bytes = pRegion->GetBuffer().size();
		for (size_t bytesDone = 0; bytesDone < bytes; ) {
			DataBlockTop dataBlockTop;
			DataBlockBtm dataBlockBtm;
			size_t bytesBlock = bytes - bytesDone;
			if (bytesBlock > bytesBlockMax) bytesBlock = bytesBlockMax;
			::memcpy(dataBlockTop.preamble,	"\x02\x2a", 2);
			dataBlockTop.blockCount 		= blockCount++;
			dataBlockTop.blockSize 			= static_cast<UInt8>(bytesBlock);
			dataBlockTop.startAddressH		= static_cast<UInt8>(addr >> 8);
			dataBlockTop.startAddressL		= static_cast<UInt8>(addr >> 0);
			dataBlockBtm.checkSum			= CalcCheckSum(data, data + bytesBlock);
			if (::fwrite(&dataBlockTop, 1, sizeof(dataBlockTop), fp) < sizeof(dataBlockTop)) return false;
			if (::fwrite(data, 1, bytesBlock, fp) < bytesBlock) return false;
			if (::fwrite(&dataBlockBtm, 1, sizeof(dataBlockBtm), fp) < sizeof(dataBlockBtm)) return false;
			bytesDone += bytesBlock;
			addr += static_cast<UInt16>(bytesBlock);
			data += bytesBlock;
		}
	}
	do {
		FooterBlock footerBlock;
		::memcpy(footerBlock.preamble,	"\x02\x2a", 2);
		footerBlock.blockCount 			= 0xff;
		footerBlock.blockSize 			= 0xff;
		footerBlock.startAddressH		= static_cast<UInt8>(addr >> 8);
		footerBlock.startAddressL		= static_cast<UInt8>(addr >> 0);
		if (::fwrite(&footerBlock, 1, sizeof(footerBlock), fp) < sizeof(footerBlock)) return false;
	} while (0);
	return true;
}

UInt8 FormatCJR::CalcCheckSum(const void *pStart, const void *pEnd)
{
	UInt8 rtn = 0;
	const UInt8 *pbStart = reinterpret_cast<const UInt8 *>(pStart);
	const UInt8 *pbEnd = reinterpret_cast<const UInt8 *>(pEnd);
	for ( ; pbStart != pbEnd; pbStart++) rtn += *pbStart;
	return rtn;
}

