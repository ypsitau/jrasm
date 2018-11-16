//=============================================================================
// PCGData.h
//=============================================================================
#ifndef __JRASMCORE_PCGDATA_H__
#define __JRASMCORE_PCGDATA_H__

#include "PCGChar.h"
#include "PCGColor.h"

class Expr;

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
class PCGData {
private:
	int _cntRef;
	String _symbol;
	int _wdChar;
	int _htChar;
	PCGStride _pcgStride;
	std::unique_ptr<PCGColorOwner> _pPCGColorOwner;
	PCGCharOwner _pcgCharOwner;
public:
	DeclareReferenceAccessor(PCGData);
public:
	inline PCGData(const String &symbol, int wdChar, int htChar,
				   const PCGStride &pcgStride, PCGColorOwner *pPCGColorOwner) :
		_cntRef(1), _symbol(symbol), _wdChar(wdChar), _htChar(htChar),
		_pcgStride(pcgStride), _pPCGColorOwner(pPCGColorOwner) {}
private:
	inline ~PCGData() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline int GetWdChar() const { return _wdChar; }
	inline int GetHtChar() const { return _htChar; }
	inline int GetStrideX() const { return _pcgStride.strideX; }
	inline int GetStrideY() const { return _pcgStride.strideY; }
	inline void AddPCGChar(PCGChar *pPCGChar) { _pcgCharOwner.push_back(pPCGChar); }
	Expr *ComposeExpr() const;
private:
	String ComposeSource(bool putZeroFlag) const;
};

//-----------------------------------------------------------------------------
// PCGDataList
//-----------------------------------------------------------------------------
class PCGDataList : public std::vector<PCGData *> {
public:
	PCGData *FindBySymbol(const char *symbol) const;
};

//-----------------------------------------------------------------------------
// PCGDataOwner
//-----------------------------------------------------------------------------
class PCGDataOwner : public PCGDataList {
public:
	~PCGDataOwner();
	void Clear();
};

#endif
