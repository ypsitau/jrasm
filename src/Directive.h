//=============================================================================
// Directive.h
//=============================================================================
#ifndef __DIRECTIVE_H__
#define __DIRECTIVE_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
class Directive {
public:
	static const Directive *CSEG;
	static const Directive *DB;
	static const Directive *DSEG;
	static const Directive *DW;
	static const Directive *END;
	static const Directive *EQU;
	static const Directive *INCLUDE;
	static const Directive *MML;
	static const Directive *ORG;
	static const Directive *PCG;
	static const Directive *PROC;
private:
	String _symbol;
public:
	inline Directive(const String &symbol) {}
	static void Initialize();
	inline const char *GetSymbol() const { return _symbol.c_str(); }
};

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
class Directive_CSEG : public Directive {
public:
	inline Directive_CSEG() : Directive(".cseg") {}
};

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
class Directive_DB : public Directive {
public:
	inline Directive_DB() : Directive(".db") {}
};

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
class Directive_DSEG : public Directive {
public:
	inline Directive_DSEG() : Directive(".dseg") {}
};

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
class Directive_DW : public Directive {
public:
	inline Directive_DW() : Directive(".dw") {}
};

//-----------------------------------------------------------------------------
// Directive_END
//-----------------------------------------------------------------------------
class Directive_END : public Directive {
public:
	inline Directive_END() : Directive(".end") {}
};

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
class Directive_EQU : public Directive {
public:
	inline Directive_EQU() : Directive(".equ") {}
};

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
class Directive_INCLUDE : public Directive {
public:
	inline Directive_INCLUDE() : Directive(".include") {}
};

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
class Directive_MML : public Directive {
public:
	inline Directive_MML() : Directive(".mml") {}
};

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
class Directive_ORG : public Directive {
public:
	inline Directive_ORG() : Directive(".org") {}
};

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
class Directive_PCG : public Directive {
public:
	inline Directive_PCG() : Directive(".pcg") {}
};

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
class Directive_PROC : public Directive {
public:
	inline Directive_PROC() : Directive(".proc") {}
};

#endif
