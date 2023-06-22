#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <bitset>

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned long ulong;

#if defined(__WIN32__)
	typedef long long int64;
	typedef unsigned long long uint64;
#endif


#define ALIGNOF(...) __alignof(__VA_ARGS__)

#define THROW(fmt, ...) do { fprintf(stderr, fmt "\n", ##__VA_ARGS__); __debugbreak(); } while (false)