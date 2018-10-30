//=============================================================================
// InlineData.h
//=============================================================================
#ifndef __JRASMCORE_INLINEDATA_H__
#define __JRASMCORE_INLINEDATA_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// InlineData
//-----------------------------------------------------------------------------
class InlineData {
public:
	enum Type {
		TYPE_String,
		TYPE_BitPattern,
		TYPE_MML,
	};
private:
	int _cntRef;
	Type _type;
	Binary _buff;
	String _strSrc;
	Integer _addrOffset;
	AutoPtr<Region> _pRegion;
public:
	DeclareReferenceAccessor(InlineData);
public:
	inline InlineData(Type type, const Binary &buff, const String &strSrc) :
		_cntRef(1), _type(type), _buff(buff), _strSrc(strSrc), _addrOffset(0) {}
private:
	inline ~InlineData() {}
public:
	inline Type GetType() const { return _type; }
	inline const Binary &GetBinary() const { return _buff; }
	inline Integer GetInteger() const {
		return _pRegion.IsNull()? _addrOffset : _addrOffset + _pRegion->GetAddrTop();
	}
	bool OnPhaseAssignSymbol(Context &context);
	bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// InlineDataList
//-----------------------------------------------------------------------------
class InlineDataList : public std::vector<InlineData *> {
public:
	InlineData *Lookup(InlineData::Type type, const Binary &buff);
	bool OnPhaseAssignSymbol(Context &context);
	bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// InlineDataOwner
//-----------------------------------------------------------------------------
class InlineDataOwner : public InlineDataList {
public:
	~InlineDataOwner();
	void Clear();
};

#endif
