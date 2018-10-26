//=============================================================================
// MMLParser.h
//=============================================================================
#ifndef __JRASMCORE_MMLPARSER_H__
#define __JRASMCORE_MMLPARSER_H__

class MMLParser {
public:
	enum { LENGTH_MAX = 256 };
public:
	class Handler {
	public:
		virtual void OnMMLNote(MMLParser &parser, unsigned char note, int length) = 0;
		virtual void OnMMLRest(MMLParser &parser, int length) = 0;
	};
private:
	enum Stat {
		STAT_Begin,
		STAT_Note, STAT_NoteLengthPre, STAT_NoteLength, STAT_NoteFix,
		STAT_RestLengthPre, STAT_RestLength, STAT_RestFix,
		STAT_OctavePre, STAT_Octave, STAT_OctaveFix,
		STAT_LengthPre, STAT_Length, STAT_LengthFix,
		STAT_VolumePre, STAT_Volume, STAT_VolumeFix,
		STAT_TonePre, STAT_Tone, STAT_ToneFix,
		STAT_TempoPre, STAT_Tempo, STAT_TempoFix,
	};
private:
	Handler &_handler;
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
public:
	MMLParser(Handler &handler);
	void Reset();
	bool FeedChar(int ch);
	inline const char *GetError() const { return _strErr; }
	inline int GetOctave() const { return _octave; }
	inline int GetVolume() const { return _volume; }
	inline int GetTone() const { return _tone; }
	inline int GetTempo() const { return _tempo; }
private:
	inline static bool IsEOD(int ch) { return ch == '\0' || ch < 0; }
	inline static bool IsWhite(int ch) { return ch == ' ' || ch == '\t'; }
	inline static bool IsDigit(int ch) { return '0' <= ch && ch <= '9'; }
	static int CalcLength(int numDisp, int cntDot, int lengthDefault);
};

#endif
