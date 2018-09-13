//=============================================================================
// Element.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Element
//-----------------------------------------------------------------------------
Element::Element(Type type) : _cntRef(1), _type(type), _pElemChildren(new ElementOwner())
{
}

Element::~Element()
{
}

void Element::AddChild(Element *pElement)
{
	_pElemChildren->push_back(pElement);
}

//-----------------------------------------------------------------------------
// ElementList
//-----------------------------------------------------------------------------
String ElementList::ToString() const
{
	String rtn;
	for (auto pElement : *this) {
		if (!rtn.empty()) rtn += ",";
		rtn += pElement->ToString();
	}
	return rtn;
}

void ElementList::Print() const
{
	for (auto pElement : *this) {
		::printf("%s\n", pElement->ToString().c_str());
	}
}

//-----------------------------------------------------------------------------
// Element_Inst
//-----------------------------------------------------------------------------
String Element_Inst::ToString() const
{
	String str = _symbol;
	str += " ";
	str += GetChildren().ToString();
	return str;
}

//-----------------------------------------------------------------------------
// Element_Number
//-----------------------------------------------------------------------------
String Element_Number::ToString() const
{
	char buff[128];
	::sprintf_s(buff, "0x%x", _num);
	return buff;
}

//-----------------------------------------------------------------------------
// Element_Symbol
//-----------------------------------------------------------------------------
String Element_Symbol::ToString() const
{
	return _str;
}

//-----------------------------------------------------------------------------
// Element_String
//-----------------------------------------------------------------------------
String Element_String::ToString() const
{
	String str;
	str = "\"";
	str += _str;
	str += "\"";
	return str;
}

//-----------------------------------------------------------------------------
// Element_BinOp
//-----------------------------------------------------------------------------
String Element_BinOp::ToString() const
{
	String str;
	str = GetLeft()->ToString();
	str += " + ";
	str += GetRight()->ToString();
	return str;
}

//-----------------------------------------------------------------------------
// Element_Bracket
//-----------------------------------------------------------------------------
String Element_Bracket::ToString() const
{
	String str;
	str = "[";
	str += GetChildren().ToString();
	str += "]";
	return str;
}

//-----------------------------------------------------------------------------
// Element_Parenthesis
//-----------------------------------------------------------------------------
String Element_Parenthesis::ToString() const
{
	String str;
	str = "(";
	str += GetChildren().ToString();
	str += ")";
	return str;
}

//-----------------------------------------------------------------------------
// ElementOwner
//-----------------------------------------------------------------------------
ElementOwner::~ElementOwner()
{
	Clear();
}

void ElementOwner::Clear()
{
	for (auto pElement : *this) {
		Element::Delete(pElement);
	}
	clear();
}
