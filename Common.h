//=============================================================================
// Common.h
//=============================================================================
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <deque>

typedef std::string String;
typedef int8_t		Int8;		// signed 8bit
typedef int16_t		Int16;		// signed 16bit
typedef int32_t		Int32;		// signed 32bit
typedef int64_t		Int64;		// signed 64bit
typedef uint8_t		UInt8;		// unsigned 8bit
typedef uint16_t	UInt16;		// unsigned 16bit
typedef uint32_t	UInt32;		// unsigned 32bit
typedef uint64_t	UInt64;		// unsigned 64bit

#define ArraySizeOf(x) (sizeof(x) / sizeof(x[0]))

#define BeginPushbackRegion() \
bool __pushbackFlag = false; \
do { \
__pushbackFlag = false

#define EndPushbackRegion() \
} while (__pushbackFlag)

#define Pushback() \
__pushbackFlag = true

#endif
