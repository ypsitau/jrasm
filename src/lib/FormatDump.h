//=============================================================================
// FormatDump.h
//=============================================================================
#ifndef __JRASMCORE_FORMATDUMP_H__
#define __JRASMCORE_FORMATDUMP_H__

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
