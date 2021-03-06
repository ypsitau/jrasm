//=============================================================================
// CommandLine.h
//=============================================================================
#ifndef __JRASMCORE_COMMANDLINE_H__
#define __JRASMCORE_COMMANDLINE_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// CommandLine
//-----------------------------------------------------------------------------
class CommandLine {
public:
	enum Type {
		TYPE_Flag,
		TYPE_Value,
	};
	struct Info {
		const char *keyLong;
		char keyShort;
		Type type;
	};
	typedef std::map<String, Info> InfoMapByKeyLong;
	typedef std::map<char, Info> InfoMapByKeyShort;
	typedef std::map<String, StringList *> Map;
private:
	InfoMapByKeyLong _infoMapByKeyLong;
	InfoMapByKeyShort _infoMapByKeyShort;
	Map _map;
public:
	inline CommandLine() {}
	~CommandLine();
	CommandLine &AddInfo(const Info *infoTbl, int cntInfo);
	bool Parse(int &argc, const char *argv[], String &strErr);
	bool IsSet(const char *key);
	const char *GetString(const char *key, const char *defValue) const;
	const StringList &GetStringList(const char *key) const;
	long GetLong(const char *key, long defValue) const;
private:
	const char *_GetString(const char *key) const;
};

#endif
