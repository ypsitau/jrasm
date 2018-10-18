//=============================================================================
// DisasmDumper.h
//=============================================================================
#ifndef __JRASMCORE_DISASMDUMPER_H__
#define __JRASMCORE_DISASMDUMPER_H__

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
	void DumpLabel(const char *strLabel, int indentLevelCode);
	void DumpCode(const char *strCode, int indentLevelCode);
	void DumpLabelAndCode(const char *strLabel, const char *strCode, int indentLevelCode);
	void DumpAddrAndCode(Integer addr, Integer addrEnd, const char *strCode, int indentLevelCode);
	void DumpDataAndCode(Integer addr, const Binary &buff, const char *strCode, int indentLevelCode);
};

#endif
