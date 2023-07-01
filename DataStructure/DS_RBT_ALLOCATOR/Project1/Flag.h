#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <bitset>

#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned long ulong;

/* detect Windows 95/98/NT/2000/XP/Vista/7 platform (윈도우 여부 파악) */
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)) && !defined(__CYGWIN__)
#if !defined(__WIN32__)
#define __WIN32__
#endif
#endif

/* detect x86 64 bit platform (32 혹은 64비트 시스템 여부 파악) */
#if defined(__x86_64__) || defined(__ia64__) || defined(_M_X64)
#if !defined(__X86__)
#define __X86__
#define __PLATFORM64__
#endif
#if !defined(__X86_64__)
#define __X86_64__
#define __PLATFORM64__
#endif
#endif

#if defined(__WIN32__)
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

#ifdef __PLATFORM64__
	typedef uint64 uintptr;
#else
	typedef uint32 uintptr;
#endif

#define ALIGNOF(...) __alignof(__VA_ARGS__)

#define THROW(fmt, ...) do { fprintf(stderr, fmt "\n", ##__VA_ARGS__); __debugbreak(); } while (false)