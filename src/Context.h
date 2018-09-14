//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Generator.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
class Context {
private:
	String _fileNameSrc;
	Binary _buff;
	std::unique_ptr<Generator> _pGenerator;
	String _errMsg;
public:
	Context(const String &fileNameSrc, Generator *pGenerator);
	inline const char *GetFileNameSrc() const { return _fileNameSrc.c_str(); }
	inline const Generator *GetGenerator() const { return _pGenerator.get(); }
	inline const Binary &GetBuffer() { return _buff; }
	inline void PutByte(UInt8 data) { _buff += data; }
	inline void ClearBuffer() { _buff.clear(); }
	void SetError(int lineNo, const char *format, ...);
	void Dump();
};

#endif
