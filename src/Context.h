//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
class Context {
private:
	Binary _buff;
public:
	inline const Binary &GetBuffer() { return _buff; }
	inline void PutByte(UInt8 data) { _buff += data; }
};

#endif
