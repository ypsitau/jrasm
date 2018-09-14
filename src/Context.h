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
	std::unique_ptr<Generator> _pGenerator;
	Binary _buff;
	String _errMsg;
public:
	Context(Generator *pGenerator);
	inline const Generator *GetGenerator() const { return _pGenerator.get(); }
	inline const Binary &GetBuffer() { return _buff; }
	inline void PutByte(UInt8 data) { _buff += data; }
	inline void ClearBuffer() { _buff.clear(); }
	inline void ClearError() { _errMsg.clear(); }
	inline bool IsError() const { return !_errMsg.empty(); }
	void SetError(const char *fileName, int lineNo, const char *format, ...);
	void Dump();
};

#endif
