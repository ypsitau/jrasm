//=============================================================================
// ErrorLog.h
//=============================================================================
#ifndef __ERRORLOG_H__
#define __ERRORLOG_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// ErrorLog
//-----------------------------------------------------------------------------
class ErrorLog {
public:
	class Entry {
	private:
		String _fileName;
		int _lineNo;
		String _message;
	public:
		inline Entry(const String &fileName, const String &message) :
			_fileName(fileName), _lineNo(0), _message(message) {}
		inline Entry(const String &fileName, int lineNo, const String &message) :
			_fileName(fileName), _lineNo(lineNo), _message(message) {}
		String GetString() const;
	};
	typedef std::vector<Entry *> EntryList;
	class EntryOwner : public EntryList {
	public:
		~EntryOwner();
		void Clear();
	};
private:
	EntryOwner _entryOwner;
public:
	inline ErrorLog() {}
	inline bool HasError() const { return !_entryOwner.empty(); }
};

#endif
