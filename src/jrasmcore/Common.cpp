//=============================================================================
// Common.cpp
//=============================================================================
#include "stdafx.h"

#if defined(JRASM_ON_MSWIN)
const char FileSeparator = '\\';
#else
const char FileSeparator = '/';
#endif

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------
const StringList StringList::Empty;

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------
Binary BitPatternToBinary(const char *str)
{
	Binary buff;
	UInt8 data = 0x00;
	size_t nCols = 0;
	for (const char *p = str; *p != '\0'; p++) {
		char ch = *p;
		data <<= 1;
		if (!(ch == ' ' || ch == '.' || ch == ',' || ch == '_' || ch == '-')) data |= 1;
		nCols++;
		if (nCols == 8) {
			buff += data;
			nCols = 0;
		}
	}
	return buff;
}

String MakePadding(size_t width, const char *padding)
{
	String rtn;
	while (width-- > 0) rtn += padding;
	return rtn;
}

String JustifyLeft(const char *str, size_t width, char padding)
{
	size_t widthBody = ::strlen(str);
	if (width <= widthBody) return String(str);
	String rtn;
	size_t widthRight = width - widthBody;
	rtn += str;
	while (widthRight-- > 0) rtn += padding;
	return rtn;
}

String JustifyRight(const char *str, size_t width, char padding)
{
	size_t widthBody = ::strlen(str);
	if (width <= widthBody) return String(str);
	String rtn;
	size_t widthLeft = width - widthBody;
	while (widthLeft-- > 0) rtn += padding;
	rtn += str;
	return rtn;
}

String ToLower(const char *str)
{
	String rtn;
	for (const char *p = str; *p != '\0'; ) {
		char ch = *p;
		if ('A' <= ch && ch <= 'Z') {
			rtn += ch - 'A' + 'a';
			p++;
		} else {
			rtn += ch;
			p++;
		}
	}
	return rtn;
}

String ToUpper(const char *str)
{
	String rtn;
	for (const char *p = str; *p != '\0'; ) {
		char ch = *p;
		if ('a' <= ch && ch <= 'z') {
			rtn += ch - 'a' + 'A';
			p++;
		} else {
			rtn += ch;
			p++;
		}
	}
	return rtn;
}

String MakeQuotedString(const String &str, char chBorder)
{
	String strDst;
	for (auto ch : str) {
		if (ch == '\0') {
			strDst += "\\0";
		} else if (ch == '\a') {
			strDst += "\\a";
		} else if (ch == '\b') {
			strDst += "\\b";
		} else if (ch == '\f') {
			strDst += "\\f";
		} else if (ch == '\n') {
			strDst += "\\n";
		} else if (ch == '\r') {
			strDst += "\\r";
		} else if (ch == '\t') {
			strDst += "\\t";
		} else if (ch == '\v') {
			strDst += "\\v";
		} else if (ch == chBorder) {
			strDst += '\\';
			strDst += chBorder;
		} else if (ch == '\\') {
			strDst += "\\\\";
		} else if (ch < 0x20 || ch >= 0x7f) {
			char tmp[16];
			::sprintf_s(tmp, "\\x%02x", static_cast<UInt8>(ch));
			strDst += tmp;
		} else {
			strDst += ch;
		}
	}
	return strDst;
}

String CorrectFileSeparator(const char *pathName)
{
	String rtn;
	for (const char *p = pathName; *p != '\0'; p++) {
		char ch = *p;
		rtn += IsFileSeparator(ch)? FileSeparator : ch;
	}
	return rtn;
}

String JoinPathName(const char *pathName1, const char *pathName2)
{
	const char fileSepChar = '/';
	if (*pathName1 == '\0') return pathName2;
	if (*pathName2 == '\0') return pathName1;
	String rtn;
	rtn += pathName1;
	if (!IsFileSeparator(*(pathName1 + ::strlen(pathName1) - 1))) rtn += fileSepChar;
	rtn += pathName2;
	return rtn;
}

void SplitFileName(const char *pathName, String *pDirName, String *pFileName)
{
	const char *p = pathName + ::strlen(pathName);
	for ( ; p > pathName; p--) {
		if (IsFileSeparator(*(p - 1))) break;
	}
	if (pDirName != nullptr) *pDirName = String(pathName, p);
	if (pFileName != nullptr) *pFileName = String(p);
}

String GetDirName(const char *pathName)
{
	String dirName;
	SplitFileName(pathName, &dirName, nullptr);
	return dirName;
}

String GetFileName(const char *pathName)
{
	String fileName;
	SplitFileName(pathName, nullptr, &fileName);
	return fileName;
}

const char *SeekExtName(const char *pathName)
{
	const char *pBtm = pathName + ::strlen(pathName);
	for (const char *p = pBtm; p >= pathName; p--) {
		if (*p == '.') {
			return (p == pathName || IsFileSeparator(*(p - 1)))? pBtm : p;
		}
		if (IsFileSeparator(*p)) break;
	}
	return pBtm;
}

String RemoveExtName(const char *pathName)
{
	return String(pathName, SeekExtName(pathName));
}

void SplitPathList(const char *str, StringList &strList)
{
	enum {
		STAT_SkipSpace, STAT_Field,
	} stat = STAT_SkipSpace;
	char ch = '\0';
	String field;
	int cntSpace = 0;
	bool eatNextFlag = true;
	do {
		if (eatNextFlag) ch = *str++;
		eatNextFlag = true;
		if (stat == STAT_SkipSpace) {
			if (ch == ' ' || ch == '\t') {
				// nothing to do
			} else {
				field.clear();
				cntSpace = 0;
				eatNextFlag = false;
				stat = STAT_Field;
			}
		} else if (stat == STAT_Field) {
			if (IsPathSeparator(ch) || ch == '\0') {
				strList.push_back(field);
				stat = STAT_SkipSpace;
			} else if (ch == ' ') {
				cntSpace++;
			} else {
				while (cntSpace-- > 0) field.push_back(' ');
				field.push_back(ch);
			}
		}
	} while (ch != '\0');
}

String EliminateBottomDirName(const char *pathName)
{
	size_t len = ::strlen(pathName);
	if (len == 0) return String("");
	const char *pEnd = pathName + len;
	if (pathName < pEnd && IsFileSeparator(*(pEnd - 1))) {
		pEnd--;
	}
	if (pathName < pEnd) pEnd--;
	const char *pBegin = pEnd;
	for ( ; pathName < pBegin; pBegin--) {
		if (IsFileSeparator(*pBegin)) break;
	}
	return String(pathName, pBegin);
}

bool IsAbsPathName(const char *pathName)
{
	return IsFileSeparator(*pathName) ||
		(isalpha(*pathName) && *(pathName + 1) == ':');
}

String MakeAbsPathName(const char *fileName, const char *dirNameBase, char chSeparator)
{
	bool cutLastSepFlag = false;
	String pathName;
	if (IsAbsPathName(fileName)) {
		// nothing to do
	} else if (dirNameBase == nullptr) {
		pathName += GetCurDir();
	} else {
		pathName += dirNameBase;
		if (pathName.empty() || !IsFileSeparator(pathName[pathName.size() - 1])) {
			pathName += FileSeparator;
		}
	}
	pathName += fileName;
	return RegulatePathName(pathName.c_str(), chSeparator, cutLastSepFlag);
}

String RegulatePathName(const char *pathName, char chSeparator, bool cutLastSepFlag)
{
	enum {
		STAT_Field, STAT_FileSeparator,
	} stat = STAT_Field;
	String prefix;
	char driveLetter = '\0';
	const char *p = pathName;
	if (isalpha(*p) && *(p + 1) == ':') {
		driveLetter = *p;
		p += 2;
		if (IsFileSeparator(*p)) {
			prefix += chSeparator;
			p++;
		}
		while (IsFileSeparator(*p)) p++;
	} else {
		while (IsFileSeparator(*p)) {
			prefix += chSeparator;
			p++;
		}
	}
	StringList fields;
	String field;
	bool sepTailFlag = false;
	for ( ; ; p++) {
		char ch = *p;
		BeginPushbackRegion();
		switch (stat) {
		case STAT_Field: {
			if (IsFileSeparator(ch) || ch == '\0') {
				if (field == ".") {
					// nothing to do
				} else if (field == "..") {
					if (fields.empty()) {
						if (prefix.empty()) {
							fields.push_back(field);
						}
					} else if (fields.back() == "..") {
						fields.push_back(field);
					} else {
						fields.pop_back();
					}
				} else if (!field.empty()) {
					fields.push_back(field);
				}
				field.clear();
				stat = STAT_FileSeparator;
			} else {
				field += ch;
			}
			break;
		}
		case STAT_FileSeparator: {
			if (ch == '\0') {
				sepTailFlag = true;
			} else if (IsFileSeparator(ch)) {
				// nothing to do
			} else {
				Pushback();
				stat = STAT_Field;
			}
			break;
		}
		}
		EndPushbackRegion();
		if (ch == '\0') break;
	}
	String rtn;
	if (driveLetter != '\0') {
		rtn += driveLetter;
		rtn += ':';
	}
	rtn += prefix;
	for (StringList::iterator pField = fields.begin(); pField != fields.end(); pField++) {
		if (pField != fields.begin()) rtn += chSeparator;
		rtn += *pField;
	}
	if (sepTailFlag && !rtn.empty() && !IsFileSeparator(rtn[rtn.size() - 1])) {
		rtn += chSeparator;
	}
	return rtn;
}

#if defined(JRASM_ON_MSWIN)

bool DoesExist(const char *pathName)
{
	WIN32_FILE_ATTRIBUTE_DATA attrData;
	return ::GetFileAttributesEx(pathName, GetFileExInfoStandard, &attrData) != 0;
}

String GetEnv(const char *name, bool *pFoundFlag)
{
	String nameEnc = name;
	DWORD len = ::GetEnvironmentVariable(nameEnc.c_str(), nullptr, 0);
	if (len == 0) {
		if (pFoundFlag != nullptr) *pFoundFlag = false;
		return String("");
	}
	char *buff = new char [len];
	::GetEnvironmentVariable(nameEnc.c_str(), buff, len);
	String rtn(buff);
	delete[] buff;
	if (pFoundFlag != nullptr) *pFoundFlag = true;
	return rtn;
}

void PutEnv(const char *name, const char *value)
{
	::SetEnvironmentVariable(name, value);
}

String GetCurDir()
{
	char pathName[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, pathName);
	String rtn = pathName;
	if (rtn.empty() || !IsFileSeparator(rtn[rtn.size() - 1])) {
		rtn += FileSeparator;
	}
	return rtn;
}

void SetupDirNamesInc(StringList &dirNamesInc)
{
	String str = GetEnv("JRASMPATH");
	if (!str.empty()) {
		SplitPathList(str.c_str(), dirNamesInc);
	}
	dirNamesInc.push_back(GetDirName(GetExecutablePath().c_str()) + "\\inc");
}

String GetExecutablePath()
{
	char pathName[1024];
	::GetModuleFileName(nullptr, pathName, ArraySizeOf(pathName)); // Win32 API
	return pathName;
}

#else

bool DoesExist(const char *pathName)
{
	struct stat stat;
	return ::stat(pathName, &stat) == 0;
}

String GetEnv(const char *name, bool *pFoundFlag)
{
	const char *str = ::getenv(name);
	if (str == nullptr) {
		if (pFoundFlag != nullptr) *pFoundFlag = false;
		return String("");
	}
	if (pFoundFlag != nullptr) *pFoundFlag = true;
	return str;
}

void PutEnv(const char *name, const char *value)
{
	int overwrite = 1;
	::setenv(name, value, overwrite);
}

String GetCurDir()
{
	//char *pathName = ::get_current_dir_name();
	char *pathName = ::getcwd(nullptr, 0);
	String rtn = pathName;
	::free(pathName);
	if (rtn.empty() || !IsFileSeparator(rtn[rtn.size() - 1])) {
		rtn += FileSeparator;
	}
	return rtn;
}

String _ReadLink(const char *pathName)
{
	size_t bufsize = 128;
	for (int i = 0; i < 8; i++) {
		char *buf = new char [bufsize + 1];
		int size = ::readlink(pathName, buf, bufsize);
		if (size >= 0) {
			buf[size] = '\0';
			String rtn = buf;
			delete[] buf;
			return rtn;
		}
		delete[] buf;
		if (errno != EFAULT) return String("");
		bufsize *= 2;
	}
	return String("");
}

void SetupDirNamesInc(StringList &dirNamesInc)
{
	String str = GetEnv("JRASMPATH");
	if (!str.empty()) {
		SplitPathList(str.c_str(), dirNamesInc);
	}
	dirNamesInc.push_back(GetDirName(GetExecutablePath().c_str()) + "../share/jrasm/inc");
}

String _GetExecutablePath();

String GetExecutablePath()
{
	String pathName = _GetExecutablePath();
	for (int i = 0; i < 100; i++) {
		struct stat sb;
		int rtn = ::lstat(pathName.c_str(), &sb);
		if (rtn < 0 || !S_ISLNK(sb.st_mode)) break;
		String linkName = _ReadLink(pathName.c_str());
		if (linkName.empty()) break; // this must not happen.
		String dirNameBase = EliminateBottomDirName(pathName.c_str());
		if (dirNameBase.empty()) dirNameBase = "/";
		pathName = MakeAbsPathName(linkName.c_str(), dirNameBase.c_str(), FileSeparator);
	}
	return pathName;
}

#endif

#if defined(JRASM_ON_DARWIN)

String _GetExecutablePath()
{
	uint32_t bufsize = 1024;
	for (int i = 0; i < 2; i++) {
		char *buf = new char [bufsize];
		if (::_NSGetExecutablePath(buf, &bufsize) == 0) {
			String rtn = buf;
			delete[] buf;
			return rtn;
		}
		delete[] buf;
	}
	return String("");
}

#elif defined(JRASM_ON_LINUX)
	
String _GetExecutablePath()
{
	String rtn = _ReadLink("/proc/self/exe");
	return RegulatePathName(rtn.c_str(), FileSeparator, false);
}

#else

String _GetExecutablePath()
{
	return String("/usr/bin/gura");
}

#endif
