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
	Binary _buff;
	std::unique_ptr<Generator> _pGenerator;
public:
	inline Context(Generator *pGenerator) : _pGenerator(pGenerator) {}
	inline const Generator *GetGenerator() const { return _pGenerator.get(); }
	inline const Binary &GetBuffer() { return _buff; }
	inline void PutByte(UInt8 data) { _buff += data; }
	inline void ClearBuffer() { _buff.clear(); }
	void Dump();
};

#endif
