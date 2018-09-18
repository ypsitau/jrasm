//=============================================================================
// Context.h
//=============================================================================
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Common.h"

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
		inline LookupTable *GetParent() { return _pLookupTableParent.get(); }
		static inline bool IsGlobalLabel(const char *label) { return *label != '@'; }
		void Set(const String &label, UInt32 value);
		bool IsDefined(const char *label) const;
		UInt32 Lookup(const char *label, bool *pFoundFlag) const;
		LookupTable *GetRoot();
		inline const LookupTable *GetRoot() const {
			return const_cast<LookupTable *>(this)->GetRoot();
		}
	};
	class LookupTableOwner : public std::vector<LookupTable *> {
	public:
		~LookupTableOwner();
		void Clear();
	};
	typedef LookupTableOwner LookupTableStack;
private:
	Binary _buff;
	UInt32 _addr;
	LookupTableStack _lookupTableStack;
public:
	Context();
	inline Binary &GetBuffer() { return _buff; }
	inline const Binary &GetBuffer() const { return _buff; }
	inline void ClearBuffer() { _buff.clear(); }
	inline void SetAddress(UInt32 addr) { _addr = addr; }
	inline UInt32 GetAddress() const { return _addr; }
	inline void ForwardAddress(UInt32 bytes) { _addr += bytes; }
	inline LookupTable *GetLookupTable() { return _lookupTableStack.back(); }
	inline LookupTable *GetLookupTableRoot() { return _lookupTableStack.front(); }
	LookupTable *AddLookupTable();
	void RemoveLookupTable();
	void PutByte(UInt8 data);
	void Dump();
};

#endif
