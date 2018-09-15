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
		AutoPtr<LookupTable> _pLookupTableParent;
	public:
		DeclareReferenceAccessor(LookupTable);
	public:
		inline LookupTable(LookupTable *pLookupTableParent = nullptr) :
			_cntRef(1), _pLookupTableParent(pLookupTableParent) {}
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
	UInt32 _addr;
	String _errMsg;
	LookupTableStack _lookupTableStack;
public:
	Context(Generator *pGenerator);
	inline const Generator *GetGenerator() const { return _pGenerator.get(); }
	inline const Binary &GetBuffer() { return _buff; }
	inline void ClearBuffer() { _buff.clear(); }
	inline void ClearError() { _errMsg.clear(); }
	inline bool IsError() const { return !_errMsg.empty(); }
	inline void SetAddress(UInt32 addr) { _addr = addr; }
	inline UInt32 GetAddress() const { return _addr; }
	inline void ForwardAddress(UInt32 bytes) { _addr += bytes; }
	inline LookupTable *GetLookupTable() { return _lookupTableStack.back(); }
	LookupTable *AddLookupTable();
	void RemoveLookupTable();
	void PutByte(UInt8 data);
	void SetError(const char *format, ...);
	void SetError(const Expr *pExpr, const char *format, ...);
	void Dump();
};

#endif
