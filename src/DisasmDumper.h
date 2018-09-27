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
	size_t _nColsPerLine;
	String _paddingLeft;
public:
	DisasmDumper(FILE *fp, bool upperCaseFlag, size_t nColsPerLine);
	inline bool GetUpperCaseFlag() const { return _upperCaseFlag; }
	void DumpLabel(const char *strLabel);
	void DumpCode(const char *strCode, int indentLevelCode);
	void DumpLabelAndCode(const char *strLabel, const char *strCode, int indentLevelCode);
	void DumpDataAndCode(UInt32 addr, const Binary &buff, const char *strCode, int indentLevelCode);
};

#endif
