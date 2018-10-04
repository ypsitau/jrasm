//=============================================================================
// FormatDump.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// FormatDump
//-----------------------------------------------------------------------------
FormatDump::FormatDump()
{
}

bool FormatDump::Write(const char *fileNameOut, const RegionList &regionList)
{
	const int nCols = 16;
	FILE *fp = stdout;
	int iRegion = 0;
	for (auto pRegion : regionList) {
		if (iRegion > 0) ::fprintf(fp, "\n");
		int iCol = 0;
		UInt16 addr = pRegion->GetAddrTop();
		int nColsBlank = addr - addr / nCols * nCols;
		if (nColsBlank > 0) {
			::fprintf(fp, "%04x ", addr);
			for ( ; iCol < nColsBlank; iCol++) ::fprintf(fp, "   ");
		}
		for (auto data : pRegion->GetBuffer()) {
			if (iCol == 0) ::fprintf(fp, "%04x ", addr);
			::fprintf(fp, " %02x", static_cast<UInt8>(data));
			addr++;
			iCol++;
			if (iCol == nCols) {
				::fprintf(fp, "\n");
				iCol = 0;
			}
		}
		if (iCol > 0) ::fprintf(fp, "\n");
		iRegion++;
	}
	return true;
}
