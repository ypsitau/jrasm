//=============================================================================
// FormatCJR.h
//=============================================================================
#ifndef __JRASMCORE_FORMATCJR_H__
#define __JRASMCORE_FORMATCJR_H__

#include "Region.h"

//-----------------------------------------------------------------------------
// FormatCJR
//-----------------------------------------------------------------------------
class FormatCJR {
public:
	struct HeaderBlock {
		UInt8 preamble[2];		// {0x02, 0x2a}
		UInt8 blockCount;
		UInt8 blockSize;		// 0x1a (size between fileNameJR and dummy)
		UInt8 startAddressH;	// 0xff
		UInt8 startAddressL;	// 0xff
		char fileNameJR[16];
		UInt8 binaryFlag;		// 0: BASIC, 1: Binary
		UInt8 baudrate;
		UInt8 dummy[8];			// {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
		UInt8 checkSum;
	};
	struct DataBlockTop {
		UInt8 preamble[2];		// {0x02, 0x2a}
		UInt8 blockCount;
		UInt8 blockSize;
		UInt8 startAddressH;
		UInt8 startAddressL;
	};
	struct DataBlockBtm {
		UInt8 checkSum;
	};
	struct FooterBlock {
		UInt8 preamble[2];		// {0x02, 0x2a}
		UInt8 blockCount;		// 0xff
		UInt8 blockSize;		// 0xff
		UInt8 startAddressH;
		UInt8 startAddressL;
	};
private:
	String _fileNameJR;
public:
	FormatCJR(const String &fileNameJR);
	bool Write(const char *fileNameOut, const RegionList &regionList);
	static UInt8 CalcCheckSum(const void *pStart, size_t bytes);
	static UInt8 CalcCheckSum(const void *pStart, const void *pEnd);
};

#endif
