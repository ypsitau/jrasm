//=============================================================================
// CJRFormat.cpp
//=============================================================================
#include "CJRFormat.h"

//-----------------------------------------------------------------------------
// CJRFormat
//-----------------------------------------------------------------------------
bool CJRFormat::Write(FILE *fp, const char *fileName, UInt16 address, const UInt8 *data, size_t bytes)
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
		::strncpy(headerBlock.fileName,	fileName, sizeof(headerBlock.fileName));
		headerBlock.binaryFlag 			= 0x01;
		headerBlock.baudrate			= 0x00;
		::memcpy(headerBlock.dummy,		"\xff\xff\xff\xff\xff\xff\xff\xff", 8);
		headerBlock.checkSum			= CalcCheckSum(headerBlock.fileName, &headerBlock.checkSum);
		if (::fwrite(&headerBlock, 1, sizeof(headerBlock), fp) < sizeof(headerBlock)) return false;
	} while (0);
	for (size_t bytesDone = 0; bytesDone < bytes; ) {
		DataBlockTop dataBlockTop;
		DataBlockBtm dataBlockBtm;
		size_t bytesBlock = bytes - bytesDone;
		if (bytesBlock > bytesBlockMax) bytesBlock = bytesBlockMax;
		::memcpy(dataBlockTop.preamble,	"\x02\x2a", 2);
		dataBlockTop.blockCount 		= blockCount++;
		dataBlockTop.blockSize 			= static_cast<UInt8>(bytesBlock);
		dataBlockTop.startAddressH		= static_cast<UInt8>(address >> 8);
		dataBlockTop.startAddressL		= static_cast<UInt8>(address >> 0);
		dataBlockBtm.checkSum			= CalcCheckSum(data, data + bytesBlock);
		if (::fwrite(&dataBlockTop, 1, sizeof(dataBlockTop), fp) < sizeof(dataBlockTop)) return false;
		if (::fwrite(data, 1, bytesBlock, fp) < bytesBlock) return false;
		if (::fwrite(&dataBlockBtm, 1, sizeof(dataBlockBtm), fp) < sizeof(dataBlockBtm)) return false;
		bytesDone += bytesBlock;
		address += bytesBlock;
		data += bytesBlock;
	}
	do {
		FooterBlock footerBlock;
		::memcpy(footerBlock.preamble,	"\x02\x2a", 2);
		footerBlock.blockCount 			= 0xff;
		footerBlock.blockSize 			= 0xff;
		footerBlock.startAddressH		= static_cast<UInt8>(address >> 8);
		footerBlock.startAddressL		= static_cast<UInt8>(address >> 0);
		if (::fwrite(&footerBlock, 1, sizeof(footerBlock), fp) < sizeof(footerBlock)) return false;
	} while (0);
	return true;
}

UInt8 CJRFormat::CalcCheckSum(const void *pStart, const void *pEnd)
{
	UInt8 rtn = 0;
	const UInt8 *pbStart = reinterpret_cast<const UInt8 *>(pStart);
	const UInt8 *pbEnd = reinterpret_cast<const UInt8 *>(pEnd);
	for ( ; pbStart != pbEnd; pbStart++) rtn += *pbStart;
	return rtn;
}

