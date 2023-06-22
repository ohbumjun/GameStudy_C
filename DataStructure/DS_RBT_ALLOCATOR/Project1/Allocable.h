#pragma once

#include "Memory.h"

class Allocable
{
	// file, line : memory leak tracking 을 위함이다.
	virtual void* Allocate(size_t size, const char* flie = nullptr, size_t line = 0) = 0;

	virtual void* Reallocate(void* ptr, size_t size, const char* flie = nullptr, size_t line = 0) = 0;

	virtual void Free(void* ptr) = 0;
};


struct AlignedAllocable : Allocable
{
	virtual void* AlignedAllocate(size_t size, size_t align, const char* flie = nullptr, size_t line = 0) = 0;

	virtual void* AlignedReallocate(void* ptr, size_t size, size_t align, const char* flie = nullptr, size_t line = 0) = 0;

	virtual void AlignedFree(void* ptr) = 0;
};