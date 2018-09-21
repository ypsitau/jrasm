//=============================================================================
// FormatDump.h
//=============================================================================
#ifndef __FORMATDUMP_H__
#define __FORMATDUMP_H__

#include "Region.h"

//-----------------------------------------------------------------------------
// FormatDump
//-----------------------------------------------------------------------------
class FormatDump {
public:
	FormatDump();
	bool Write(const char *fileNameOut, const RegionList &regionList);
};

#endif
