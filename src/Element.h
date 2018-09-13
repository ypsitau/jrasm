//=============================================================================
// Element.h
//=============================================================================
#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "Common.h"

class ElementOwner;

//-----------------------------------------------------------------------------
// Element
//-----------------------------------------------------------------------------
class Element {
public:
	enum Type {
		TYPE_Inst,
		TYPE_A,
		TYPE_B,
		TYPE_X,
		TYPE_Number,
		TYPE_Symbol,
		TYPE_String,
		TYPE_BinOp,
		TYPE_Bracket,
		TYPE_Parenthesis,
	};
private:
	int _cntRef;
	Type _type;
	std::auto_ptr<ElementOwner> _pElemChildren;
public:
	DeclareReferenceAccessor(Element);
public:
	Element(Type type);
protected:
	virtual ~Element();
public:
	bool IsType(Type type) const { return _type == type; }
	void AddChild(Element *pElement);
	ElementOwner &GetChildren() { return *_pElemChildren; }
	const ElementOwner &GetChildren() const { return *_pElemChildren; }
	//virtual bool Reduce() = 0;
	virtual String ToString() const = 0;
};

//-----------------------------------------------------------------------------
// ElementList
//-----------------------------------------------------------------------------
class ElementList : public std::vector<Element *> {
public:
	String ToString() const;
	void Print() const;
};

//-----------------------------------------------------------------------------
// ElementStack
//-----------------------------------------------------------------------------
typedef std::vector<Element *> ElementStack;

//-----------------------------------------------------------------------------
// ElementOwner
//-----------------------------------------------------------------------------
class ElementOwner : public ElementList {
public:
	~ElementOwner();
	void Clear();
};

//-----------------------------------------------------------------------------
// Element_Inst
//-----------------------------------------------------------------------------
class Element_Inst : public Element {
private:
	String _symbol;
public:
	inline Element_Inst(const String &symbol) : Element(TYPE_Inst), _symbol(symbol) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Element_A
//-----------------------------------------------------------------------------
class Element_A : public Element {
public:
	inline Element_A() : Element(TYPE_A) {}
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Element_B
//-----------------------------------------------------------------------------
class Element_B : public Element {
public:
	inline Element_B() : Element(TYPE_B) {}
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Element_X
//-----------------------------------------------------------------------------
class Element_X : public Element {
public:
	inline Element_X() : Element(TYPE_X) {}
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Element_Number
//-----------------------------------------------------------------------------
class Element_Number : public Element {
private:
	UInt32 _num;
public:
	inline Element_Number(UInt32 num) : Element(TYPE_Number), _num(num) {}
	inline UInt32 GetNumber() const { return _num; }
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Element_Symbol
//-----------------------------------------------------------------------------
class Element_Symbol : public Element {
private:
	String _str;
	UInt32 _num;
	bool _validNumFlag;
public:
	inline Element_Symbol(const String &str) :
		Element(TYPE_Symbol), _str(str), _num(0), _validNumFlag(false) {}
	inline void SetNumber(UInt32 num) { _num = num; _validNumFlag = true; }
	inline void InvalidateNumber() { _validNumFlag = false; }
	inline UInt32 GetNumber() const { return _num; }
	inline bool IsValidNumber() const { return _validNumFlag; }
	inline bool MatchCase(const char *str) const { return ::strcmp(_str.c_str(), str) == 0; }
	inline bool MatchICase(const char *str) const { return ::strcasecmp(_str.c_str(), str) == 0; }
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Element_String
//-----------------------------------------------------------------------------
class Element_String : public Element {
private:
	String _str;
public:
	inline Element_String(const String &str) : Element(TYPE_String), _str(str) {}
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Element_BinOp
//-----------------------------------------------------------------------------
class Element_BinOp : public Element {
public:
	inline Element_BinOp(Element *pElementL, Element *pElementR) : Element(TYPE_BinOp) {
		GetChildren().push_back(pElementL);
		GetChildren().push_back(pElementR);
	}
	const Element *GetLeft() const { return GetChildren()[0]; }
	const Element *GetRight() const { return GetChildren()[1]; }
	virtual String ToString() const;
};

class Element_BinOp_Add : public Element_BinOp {
public:
	inline Element_BinOp_Add(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_BinOp_Sub : public Element_BinOp {
public:
	inline Element_BinOp_Sub(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_BinOp_Mul : public Element_BinOp {
public:
	inline Element_BinOp_Mul(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_BinOp_Div : public Element_BinOp {
public:
	inline Element_BinOp_Div(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_Bracket : public Element {
public:
	inline Element_Bracket() : Element(TYPE_Bracket) {}
	virtual String ToString() const;
};

class Element_Parenthesis : public Element {
public:
	inline Element_Parenthesis() : Element(TYPE_Parenthesis) {}
	virtual String ToString() const;
};

#endif
