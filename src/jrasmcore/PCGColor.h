//=============================================================================
// PCGColor.h
//=============================================================================
#ifndef __JRASMCORE_PCGCOLOR_H__
#define __JRASMCORE_PCGCOLOR_H__

//-----------------------------------------------------------------------------
// PCGColor
//-----------------------------------------------------------------------------
class PCGColor {
private:
	int _colorCode;
	int _charCount;
public:
	inline PCGColor(int colorCode, int charCount) : _colorCode(colorCode), _charCount(charCount) {}
	inline int GetColorCode() const { return _colorCode; }
	inline int GetCharCount() const { return _charCount; }
};

//-----------------------------------------------------------------------------
// PCGColorList
//-----------------------------------------------------------------------------
class PCGColorList : public std::vector<PCGColor *> {
};

//-----------------------------------------------------------------------------
// PCGColorOwner
//-----------------------------------------------------------------------------
class PCGColorOwner : public PCGColorList {
public:
	~PCGColorOwner();
	void Clear();
};

#endif
