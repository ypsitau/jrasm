//=============================================================================
// ErrorLog.h
//=============================================================================
#ifndef __ERRORLOG_H__
#define __ERRORLOG_H__

#include "Common.h"

class Expr;

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
		inline bool DoesMatch(const char *fileName, int lineNo, const char *message) {
			return _fileName == fileName && _lineNo == lineNo && _message == message;
		}
		String GetString() const;
	};
	class EntryList : public std::vector<Entry *> {
	public:
		bool DoesExist(const char *fileName, int lineNo, const char *message);
	};
	class EntryOwner : public EntryList {
	public:
		~EntryOwner();
		void Clear();
	};
private:
	EntryOwner _entryOwner;
	static ErrorLog _instance;
public:
	inline ErrorLog() {}
	inline EntryOwner &GetEntryOwner() { return _entryOwner; }
	inline static bool HasError() { return !_instance.GetEntryOwner().empty(); }
	inline static void Clear() { _instance._entryOwner.Clear(); }
	static void AddError(const Expr *pExpr, const char *format, ...);
	static void AddError(const char *format, ...);
	static void AddError(const String &fileName, int lineNo, const char *format, ...);
	static void AddErrorV(const String &fileName, int lineNo, const char *format, va_list ap);
	static void Print(FILE *fp);
};

#endif
