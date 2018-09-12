//=============================================================================
// jrasm.h
//=============================================================================
#ifndef __JRASM_H__
#define __JRASM_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>

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

#define DeclarePushback(T, n) \
T __pushbackBuff[n];  \
int __idxPushbackStore; \
int __idxPushbackLoad;

#define InitializePushback() \
__idxPushbackStore = 0; \
__idxPushbackLoad = 0

#define BeginPushbackRegion(var) \
__pushbackBuff[__idxPushbackStore++] = var; \
if (__idxPushbackStore >= ArraySizeOf(__pushbackBuff)) { __idxPushbackStore = 0; }  \
while (__idxPushbackLoad != __idxPushbackStore) { \
var = __pushbackBuff[__idxPushbackLoad++]; \
if (__idxPushbackLoad >= ArraySizeOf(__pushbackBuff)) { __idxPushbackLoad = 0; }

#define EndPushbackRegion() \
}

#define Pushback(value) do { \
	__pushbackBuff[__idxPushbackStore++] = (value); \
	if (__idxPushbackStore >= ArraySizeOf(__pushbackBuff)) { __idxPushbackStore = 0; }  \
} while (0)

#endif
