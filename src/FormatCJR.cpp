//=============================================================================
// FormatCJR.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// FormatCJR
//-----------------------------------------------------------------------------
FormatCJR::FormatCJR(const String &fileNameJR) : _fileNameJR(fileNameJR)
{
}

bool FormatCJR::Write(const char *fileNameOut, const RegionList &regionList)
{
	const size_t bytesBlockMax = 256;
	UInt8 blockCount = 0;
	UInt16 addr = 0;
	FILE *fp = stdout;
	if (*fileNameOut != '\0') {
		if (::fopen_s(&fp, fileNameOut, "w") != 0) {
			ErrorLog::AddError("failed to open output file: %s", fileNameOut);
			return false;
		}
	}
	do {
		HeaderBlock headerBlock;
		::memcpy(headerBlock.preamble,	"\x02\x2a", 2);
		headerBlock.blockCount			= blockCount++;
		headerBlock.blockSize 			= 0x1a;
		headerBlock.startAddressH		= 0xff;
		headerBlock.startAddressL		= 0xff;
		::strncpy_s(headerBlock.fileNameJR, _fileNameJR.c_str(), sizeof(headerBlock.fileNameJR));
		headerBlock.binaryFlag 			= 0x01;
		headerBlock.baudrate			= 0x00;
		::memcpy(headerBlock.dummy,		"\xff\xff\xff\xff\xff\xff\xff\xff", 8);
		headerBlock.checkSum			= CalcCheckSum(headerBlock.fileNameJR, &headerBlock.checkSum);
		if (::fwrite(&headerBlock, 1, sizeof(headerBlock), fp) < sizeof(headerBlock)) goto errorDone;
	} while (0);
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
			if (::fwrite(&dataBlockTop, 1, sizeof(dataBlockTop), fp) < sizeof(dataBlockTop)) goto errorDone;
			if (::fwrite(data, 1, bytesBlock, fp) < bytesBlock) goto errorDone;
			if (::fwrite(&dataBlockBtm, 1, sizeof(dataBlockBtm), fp) < sizeof(dataBlockBtm)) goto errorDone;
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
		if (::fwrite(&footerBlock, 1, sizeof(footerBlock), fp) < sizeof(footerBlock)) goto errorDone;
	} while (0);
	if (fp != stdout) ::fclose(fp);
	return true;
errorDone:
	ErrorLog::AddError("error while writing data");
	if (fp != stdout) ::fclose(fp);
	return false;
}

UInt8 FormatCJR::CalcCheckSum(const void *pStart, const void *pEnd)
{
	UInt8 rtn = 0;
	const UInt8 *pbStart = reinterpret_cast<const UInt8 *>(pStart);
	const UInt8 *pbEnd = reinterpret_cast<const UInt8 *>(pEnd);
	for ( ; pbStart != pbEnd; pbStart++) rtn += *pbStart;
	return rtn;
}

