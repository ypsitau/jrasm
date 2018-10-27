//=============================================================================
// MMLParser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// MMLParser
// see http://ja.wikipedia.org/wiki/Music_Macro_Language for MML syntax
//-----------------------------------------------------------------------------
MMLParser::MMLParser()
{
	Reset();
}

void MMLParser::Reset()
{
	_stat			= STAT_Reset;
	_octave			= 3;				// 1-9
	_lengthDefault	= LENGTH_MAX / 4;	// 1-LENGTH_MAX
	_volume			= 3;				// 0-16
	_tone			= 1;				// 
	_tempo			= 120;				// 
	_operator		= '\0';
	_operatorSub	= '\0';
	_numAccum		= 0;
	_pBuffSharedPrev.reset(nullptr);
}

bool MMLParser::Parse(const char *str, BinaryShared *pBuffShared)
{
	Binary *pBuffPrev = _pBuffSharedPrev.IsNull()?
		&pBuffShared->GetBinary() : &_pBuffSharedPrev->GetBinary();
	for (const char *p = str; ; p++) {
		if (!FeedChar(*p, pBuffShared->GetBinary(), &pBuffPrev)) return false;
		if (*p == '\0') break;
	}
	if (_stat != STAT_Reset) _pBuffSharedPrev.reset(pBuffShared);
	return true;
}

bool MMLParser::FeedChar(int ch, Binary &buff, Binary **ppBuffPrev)
{
	if ('a' <= ch && ch <= 'z') ch = ch - 'a' + 'A';
	BeginPushbackRegion();
	switch (_stat) {
	case STAT_Reset: {
		if (IsEOD(ch)) {
			// nothing to do
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_Begin;
		}
		break;
	}
	case STAT_Begin: {
		if (IsEOD(ch)) {
			// nothing to do
		} else if (IsWhite(ch)) {
			// nothing to do
		} else if ('A' <= ch && ch <= 'G') {
			_operator = ch;
			_operatorSub = '\0';
			_numAccum = 0;
			_cntDot = 0;
			_stat = STAT_Note;
		} else if (ch == 'R') {
			_operator = ch;
			_numAccum = 0;
			_cntDot = 0;
			_stat = STAT_RestLengthPre;
		} else if (ch == '&') {
			_operator = ch;
		} else if (ch == 'O') {
			_operator = ch;
			_numAccum = 0;
			_stat = STAT_OctavePre;
		} else if (ch == '>') {
			_operator = ch;
			if (_octave < 255) _octave++;
		} else if (ch == '<') {
			_operator = ch;
			if (_octave > 0) _octave--;
		} else if (ch == ';') {
			buff += '\0';
			Reset();
		} else if (ch == 'L') {
			_operator = ch;
			_numAccum = 0;
			_cntDot = 0;
			_stat = STAT_LengthPre;
			/*
			  } else if (ch == 'V') {
			  _operator = ch;
			  _numAccum = 0;
			  _stat = STAT_VolumePre;
			  } else if (ch == '@') {
			  _operator = ch;
			  _numAccum = 0;
			  _stat = STAT_TonePre;
			  } else if (ch == 'T') {
			  _operator = ch;
			  _numAccum = 0;
			  _stat = STAT_TempoPre;
			*/
		} else {
			if (::isprint(ch)) {
				::sprintf_s(_strErr, "invalid character: %c", ch);
			} else {
				::sprintf_s(_strErr, "invalid character: 0x%02x", ch);
			}
			return false;
		}
		break;
	}
	case STAT_Note: {		// -------- Note --------
		if (ch == '#' || ch == '+' || ch == '-') {
			_operatorSub = ch;
			_stat = STAT_NoteLengthPre;
		} else if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_NoteLength;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_NoteFix;
		}
		break;
	}
	case STAT_NoteLengthPre: {
		if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_NoteLength;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_NoteFix;
		}
		break;
	}
	case STAT_NoteLength: {
		if (IsDigit(ch)) {
			_numAccum = _numAccum * 10 + (ch - '0');
		} else if (ch == '.') {
			_cntDot++;
		} else {
			Pushback();
			_stat = STAT_NoteFix;
		}
		break;
	}
	case STAT_NoteFix: {
		static const unsigned char noteTbl[] = {
			9, 11, 0, 2, 4, 5, 7,
		};
		int note = noteTbl[_operator - 'A'] + _octave * 12;
		if (_operatorSub == '#' || _operatorSub == '+') {
			if (note < 127) note++;
		} else if (_operatorSub == '-') {
			if (note > 0) note--;
		} else {
			// nothing to do
		}
		int length = CalcLength(_numAccum, _cntDot, _lengthDefault);
		if (note < 12 || note > 71) {
			SetError("MML note is out of range");
			return false;
		}
		UInt8 noteDev = static_cast<UInt8>(note + 1 - 12);
		do {
			Binary &buffPrev = **ppBuffPrev;
			size_t sizeBuffPrev = buffPrev.size();
			if (sizeBuffPrev % 2 == 0 && sizeBuffPrev >= 2 && buffPrev.back() == noteDev) {
				// A short rest is inserted between two same notes succeeding.
				UInt8 lengthDev = buffPrev[sizeBuffPrev - 2];
				if (lengthDev > 1) buffPrev[sizeBuffPrev - 2] = lengthDev - 1;
				buffPrev += '\x01';
				buffPrev += '\0';
			}
		} while (0);
		buff += static_cast<UInt8>(length);
		buff += noteDev;
		*ppBuffPrev = &buff;
		Pushback();
		_stat = STAT_Begin;
		break;
	}
	case STAT_RestLengthPre: {// -------- Rest --------
		if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_RestLength;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_RestFix;
		}
		break;
	}
	case STAT_RestLength: {
		if (IsDigit(ch)) {
			_numAccum = _numAccum * 10 + (ch - '0');
		} else if (ch == '.') {
			_cntDot++;
		} else {
			Pushback();
			_stat = STAT_RestFix;
		}
		break;
	}
	case STAT_RestFix: {
		int length = CalcLength(_numAccum, _cntDot, _lengthDefault);
		buff += static_cast<UInt8>(length);
		buff += '\0';
		*ppBuffPrev = &buff;
		Pushback();
		_stat = STAT_Begin;
		break;
	}
	case STAT_OctavePre: {	// -------- Octave --------
		if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_Octave;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_OctaveFix;
		}
		break;
	}
	case STAT_Octave: {
		if (IsDigit(ch)) {
			_numAccum = _numAccum * 10 + (ch - '0');
		} else {
			Pushback();
			_stat = STAT_OctaveFix;
		}
		break;
	}
	case STAT_OctaveFix: {
		_octave = _numAccum;
		Pushback();
		_stat = STAT_Begin;
		break;
	}
	case STAT_LengthPre: {	// -------- Length --------
		if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_Length;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_LengthFix;
		}
		break;
	}
	case STAT_Length: {
		if (IsDigit(ch)) {
			_numAccum = _numAccum * 10 + (ch - '0');
		} else if (ch == '.') {
			_cntDot++;
		} else {
			Pushback();
			_stat = STAT_LengthFix;
		}
		break;
	}
	case STAT_LengthFix: {
		_lengthDefault = CalcLength(_numAccum, _cntDot, _lengthDefault);
		Pushback();
		_stat = STAT_Begin;
		break;
	}
	case STAT_VolumePre: {	// -------- Volume --------
		if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_Volume;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_VolumeFix;
		}
		break;
	}
	case STAT_Volume: {
		if (IsDigit(ch)) {
			_numAccum = _numAccum * 10 + (ch - '0');
		} else {
			Pushback();
			_stat = STAT_VolumeFix;
		}
		break;
	}
	case STAT_VolumeFix: {
		_volume = _numAccum;
		Pushback();
		_stat = STAT_Begin;
		break;
	}
	case STAT_TonePre: {		// ------- Tone --------
		if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_Tone;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_ToneFix;
		}
		break;
	}
	case STAT_Tone:{
		if (IsDigit(ch)) {
			_numAccum = _numAccum * 10 + (ch - '0');
		} else {
			Pushback();
			_stat = STAT_ToneFix;
		}
		break;
	}
	case STAT_ToneFix: {
		_tone = _numAccum;
		Pushback();
		_stat = STAT_Begin;
		break;
	}
	case STAT_TempoPre: {	// -------- Tempo --------
		if (IsDigit(ch)) {
			Pushback();
			_stat = STAT_Tempo;
		} else if (IsWhite(ch)) {
			// nothing to do
		} else {
			Pushback();
			_stat = STAT_TempoFix;
		}
		break;
	}
	case STAT_Tempo: {
		if (IsDigit(ch)) {
			_numAccum = _numAccum * 10 + (ch - '0');
		} else {
			Pushback();
			_stat = STAT_TempoFix;
		}
		break;
	}
	case STAT_TempoFix: {
		_tempo = _numAccum;
		Pushback();
		_stat = STAT_Begin;
		break;
	}
	}
	EndPushbackRegion();
	return true;
}

int MMLParser::CalcLength(int numDisp, int cntDot, int lengthDefault)
{
	if (numDisp <= 0) return lengthDefault;
	int length = LENGTH_MAX / numDisp;
	for (int lengthDiv = length / 2; lengthDiv > 0 && cntDot > 0; lengthDiv /= 2, cntDot--) {
		length += lengthDiv;
	}
	return length;
}

void MMLParser::SetError(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	::vsprintf_s(_strErr, format, ap);
}
