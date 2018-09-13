//=============================================================================
// Common.h
//=============================================================================
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
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
typedef std::string String;
typedef int8_t		Int8;		// signed 8bit
typedef int16_t		Int16;		// signed 16bit
typedef int32_t		Int32;		// signed 32bit
typedef int64_t		Int64;		// signed 64bit
typedef uint8_t		UInt8;		// unsigned 8bit
typedef uint16_t	UInt16;		// unsigned 16bit
typedef uint32_t	UInt32;		// unsigned 32bit
typedef uint64_t	UInt64;		// unsigned 64bit

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
// Utilities
//-----------------------------------------------------------------------------
String ToLower(const char *str);

#endif
