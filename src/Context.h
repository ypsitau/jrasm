//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Generator.h"

class Expr;

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
class Context {
public:
	class LookupTable : public std::map<String, UInt32> {
	private:
		int _cntRef;
	public:
		DeclareReferenceAccessor(LookupTable);
	public:
		inline LookupTable() : _cntRef(1) {}
	private:
		inline ~LookupTable() {}
	public:
		void Set(const String &label, UInt32 value);
		UInt32 Lookup(const char *label, bool *pFoundFlag) const;
	};
	class LookupTableOwner : public std::vector<LookupTable *> {
	public:
		~LookupTableOwner();
		void Clear();
	};
	typedef LookupTableOwner LookupTableStack;
private:
	std::unique_ptr<Generator> _pGenerator;
	Binary _buff;
	String _errMsg;
	LookupTableStack _lookupTableStack;
public:
	Context(Generator *pGenerator);
	inline const Generator *GetGenerator() const { return _pGenerator.get(); }
	inline const Binary &GetBuffer() { return _buff; }
	inline void PutByte(UInt8 data) { _buff += data; }
	inline void ClearBuffer() { _buff.clear(); }
	inline void ClearError() { _errMsg.clear(); }
	inline bool IsError() const { return !_errMsg.empty(); }
	void SetError(const char *format, ...);
	void SetError(const Expr *pExpr, const char *format, ...);
	void Dump();
};

#endif
