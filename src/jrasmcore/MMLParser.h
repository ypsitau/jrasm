//=============================================================================
// MMLParser.h
//=============================================================================
#ifndef __JRASMCORE_MMLPARSER_H__
#define __JRASMCORE_MMLPARSER_H__

#include "Common.h"

class MMLParser {
public:
	enum { LENGTH_MAX = 0x60 };
private:
	enum Stat {
		STAT_Reset, STAT_Begin,
		STAT_Note, STAT_NoteLengthPre, STAT_NoteLength, STAT_NoteFix,
		STAT_RestLengthPre, STAT_RestLength, STAT_RestFix,
		STAT_OctavePre, STAT_Octave, STAT_OctaveFix,
		STAT_LengthPre, STAT_Length, STAT_LengthFix,
		STAT_VolumePre, STAT_Volume, STAT_VolumeFix,
		STAT_TonePre, STAT_Tone, STAT_ToneFix,
		STAT_TempoPre, STAT_Tempo, STAT_TempoFix,
	};
private:
	Stat _stat;
	int _octave;
	int _lengthDefault;
	int _volume;
	int _tone;
	int _tempo;
	int _operator;
	int _operatorSub;
	int _numAccum;
	int _cntDot;
	char _strErr[128];
	AutoPtr<BinaryShared> _pBuffSharedPrev;
public:
	MMLParser();
	inline const char *GetError() const { return _strErr; }
	inline int GetOctave() const { return _octave; }
	inline int GetVolume() const { return _volume; }
	inline int GetTone() const { return _tone; }
	inline int GetTempo() const { return _tempo; }
	void Reset();
	bool Parse(const char *str, BinaryShared *pBuffShared);
	void SetError(const char *format, ...);
private:
	inline static bool IsEOD(int ch) { return ch == '\0' || ch < 0; }
	inline static bool IsWhite(int ch) { return ch == ' ' || ch == '\t'; }
	inline static bool IsDigit(int ch) { return '0' <= ch && ch <= '9'; }
	bool FeedChar(int ch, Binary &buff, Binary **ppBuffPrev);
	static int CalcLength(int numDisp, int cntDot, int lengthDefault);
};

#endif
