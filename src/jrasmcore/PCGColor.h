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
	int _colorFg;
	int _colorBg;
	int _charCount;
public:
	inline PCGColor(int colorFg, int colorBg, int charCount) :
		_colorFg(colorFg), _colorBg(colorBg), _charCount(charCount) {}
	inline int GetColorFg() const { return _colorFg; }
	inline int GetColorBg() const { return _colorBg; }
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
