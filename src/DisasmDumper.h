//=============================================================================
// DisasmDumper.h
//=============================================================================
#ifndef __DISASMDUMPER_H__
#define __DISASMDUMPER_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// DisasmDumper
//-----------------------------------------------------------------------------
class DisasmDumper {
private:
	FILE *_fp;
	bool _upperCaseFlag;
	size_t _nColsPerLineMax;
public:
	DisasmDumper(FILE *fp, bool upperCaseFlag, size_t nColsPerLineMax);
	void Dump(UInt32 addr, const Binary &buff, size_t nColsPerLine, const char *strCode) const;
};

#endif
