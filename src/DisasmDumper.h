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
	String _paddingLeft;
public:
	DisasmDumper(FILE *fp, bool upperCaseFlag, size_t nColsPerLineMax);
	void DumpLabel(const char *strLabel) const;
	void DumpCode(const char *strCode) const;
	void DumpLabelAndCode(const char *strLabel, const char *strCode) const;
	void DumpDataAndCode(UInt32 addr, const Binary &buff, size_t nColsPerLine, const char *strCode) const;
};

#endif
