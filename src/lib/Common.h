//=============================================================================
// Common.h
//=============================================================================
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <map>

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#define strcasecmp _stricmp
#else
typedef int errno_t;
inline errno_t fopen_s(FILE **fp, const char *filename, const char *mode)
{
	return (((*fp) = ::fopen(filename, mode)) == nullptr)? -1 : 0;
}
#define sprintf_s sprintf
#define vsprintf_s vsprintf
#define strncpy_s strncpy
#endif

#define ArraySizeOf(x) (sizeof(x) / sizeof(x[0]))

#define DeclareReferenceAccessor(T) \
inline static T *Reference(const T *p) { \
	if (p == nullptr) return nullptr; \
	T *pCasted = const_cast<T *>(p); \
	pCasted->_cntRef++; \
	return pCasted; \
} \
inline T *Reference() const { return Reference(this); } \
inline static void Delete(T *p) { \
	if (p == nullptr) return; \
	p->_cntRef--; \
	if (p->_cntRef <= 0) delete p; \
} \
inline int GetCntRef() const { return _cntRef; }

#define BeginPushbackRegion() \
bool __pushbackFlag = false; \
do { \
__pushbackFlag = false

#define EndPushbackRegion() \
} while (__pushbackFlag)

#define Pushback() \
__pushbackFlag = true

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef int8_t		Int8;		// signed 8bit
typedef int16_t		Int16;		// signed 16bit
typedef int32_t		Int32;		// signed 32bit
typedef int64_t		Int64;		// signed 64bit
typedef uint8_t		UInt8;		// unsigned 8bit
typedef uint16_t	UInt16;		// unsigned 16bit
typedef uint32_t	UInt32;		// unsigned 32bit
typedef uint64_t	UInt64;		// unsigned 64bit

typedef Int32		Number;

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------
typedef std::string String;
typedef String Binary;

class StringList : public std::vector<String> {
public:
	static const StringList Empty;
};

//-----------------------------------------------------------------------------
// AutoPtr
// The destroyer executes "T::Delete(p)" instead of "delete p".
// Class T must have a static function named Delete() that deletes the pointed
// object in the argument.
//-----------------------------------------------------------------------------
template<typename T> class AutoPtr {
private:
	T *_p;
public:
	inline AutoPtr(T *p = nullptr) : _p(p) {}
	inline ~AutoPtr() { T::Delete(_p); }
	inline T &operator*() { return *_p; }
	inline T &operator*() const { return *_p; }
	inline T *operator->() { return _p; }
	inline T *operator->() const { return _p; }
	inline void reset(T *p = nullptr) { T::Delete(_p); _p = p; }
	inline T *get() { return _p; }
	inline T *get() const { return _p; }
	inline T *release() { T *p = _p; _p = nullptr; return p; }
	inline bool IsNull() const { return _p == nullptr; }
	inline T *Reference() const { return T::Reference(_p); }
private:
	inline void operator=(const T *p) {}
};

//-----------------------------------------------------------------------------
// StringRef
//-----------------------------------------------------------------------------
class StringShared {
private:
	int _cntRef;
	String _str;
public:
	DeclareReferenceAccessor(StringShared)
public:
	inline StringShared() : _cntRef(1) {}
	inline StringShared(const String &str) : _cntRef(1), _str(str) {}
private:
	inline ~StringShared() {}
public:
	inline const char *GetString() const { return _str.c_str(); }
	inline const String &GetStringSTL() const { return _str; }
};

//-----------------------------------------------------------------------------
// LessThan/GreaterThan
//-----------------------------------------------------------------------------
struct LessThan_StringICase {
	inline bool operator()(const String &str1, const String &str2) const {
		return ::strcasecmp(str1.c_str(), str2.c_str()) < 0;
	}
};

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------
extern const char FileSeparator;

inline bool IsFileSeparator(char ch) { return ch == '\\' || ch == '/'; }

String MakePadding(size_t width, const char *padding = " ");
String JustifyLeft(const char *str, size_t width, char padding = ' ');
String JustifyRight(const char *str, size_t width, char padding = ' ');
String ToLower(const char *str);
String ToUpper(const char *str);
String MakeQuotedString(const String &str, char chBorder);
String CorrectFileSeparator(const char *pathName);
String JoinPathName(const char *pathName1, const char *pathName2);
void SplitFileName(const char *pathName, String *pDirName, String *pFileName);
const char *SeekExtName(const char *pathName);
String RemoveExtName(const char *pathName);

#endif
