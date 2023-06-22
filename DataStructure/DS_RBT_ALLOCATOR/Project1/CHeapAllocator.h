#pragma once

#include "Allocable.h"

class DefaultAllocator
{
};

struct CHeapAllocator : AlignedAllocable
{
	explicit CHeapAllocator();

	explicit CHeapAllocator(const CHeapAllocator& allocator);

	CHeapAllocator& operator=(const CHeapAllocator&);

	~CHeapAllocator();

	void* Allocate(size_t size, const char* file = nullptr, size_t line = 0) override;

	template<typename T>
	void* Allocate(const char* file = nullptr, const size_t line = 0) { return Allocate(sizeof(T), file, line); }

	// template<typename T, typename... TArgs>
	// T* New(const char* file, const size_t line, TArgs... args)
	// {
	// 	return new(Alloc<T>(file, line))T(args...);
	// }

	void* Reallocate(void* ptr, size_t size, const char* file = nullptr, size_t line = 0) override;

	template<typename T>
	void* Realloc(void* ptr, const char* file = nullptr, const size_t line = 0) { return Realloc(ptr, sizeof(T), file, line); }

	void Free(void* ptr) override;

	// New, Delete ดย
	// template<typename T>
	// void Delete(T* o)
	// {
	// 	o->~T();
	// 	Free(o);
	// }

	void* AlignedAllocate(size_t size, size_t align, const char* file = nullptr, size_t line = 0) override;

	void* AlignedReallocate(void* ptr, size_t size, size_t align, const char* file = nullptr, size_t line = 0) override;

	void AlignedFree(void* ptr) override;
};

