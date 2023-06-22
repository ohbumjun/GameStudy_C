#pragma once

#include "Flag.h"
#include "Allocable.h"

// #if defined(__WIN32__) || defined(__ANDROID__)
// #include <malloc.h>
// #else

template <typename T>
class GeneralAllocator : Allocable
{
	// T Type 의 객체를 여러 개 할당하는 함수
	inline T* Allocate(size_t count, const char* file = nullptr, size_t line = 0)
	{
		return static_cast<T*>(Allocate(sizeof(T) * count, file, line));
	}

	inline T* Reallocate(T* ptr, size_t count, const char* flie = nullptr, size_t line = 0)
	{
		return static_cast<T*>(Reallocate(ptr, sizeof(T) * count, flie, line));
	}
};

// 1) 고정된 크기의 Buffer 로만 할당을 진행하는 Allocator
// 2) Align 에 맞춰서 할당해주는 Allocator
template <typename T, size_t Capacity>
struct LimitedAllocator final : public GeneralAllocator<T>
{
	uint8 m_Buffer[sizeof(T) * Capacity];

	size_t m_CurPos = 0;
public:
	explicit LimitedAllocator()
		: m_CurPos(0)
	{
	}

	LimitedAllocator(const LimitedAllocator& allocator)
		: m_CurPos(0)
	{
	}

	LimitedAllocator& operator=(const LimitedAllocator& o)
	{
		return *this;
	}

	void* Allocate(size_t size, const char* file, size_t line) override
	{
		if (m_CurPos >= sizeof(T) * Capacity)
		{
			errno = ENOMEM;
			THROW("Not enough memory");
			return nullptr;
		}

		/*
		중요한 전제 조건 : size 가 Alignof(T) 의 배수라고 가정한다. 
		   만약 size 가 align 되어있지 않다면, object 를 특정 align 에 맞추기 위해
		   필요한 padding 을 계산해준다.

		(예시)
		size: 68 bytes
		ALIGNOF(T): 16 bytes

		68 & 15 = 4

		68:  0 1 0 0  0 1 0 0 (binary)
		15:  0 0 0 0  1 1 1 1 (binary)
		-----------------------
		 4:  0 0 0 0  0 1 0 0 (binary)
		*/

		uint16_t extra = size & (ALIGNOF(T) - 1);

		uint8_t* r = &m_Buffer[m_CurPos];

		m_CurPos += size + extra;

		return r;
	}

	void* Reallocate(void* ptr, size_t size, const char* file = nullptr, size_t line = 0) override
	{
		if (ptr == &m_Buffer)
			return ptr;
		THROW("It's not allowed to reallocate not the start pointer in LvFixedAllocator.");
		return nullptr;
	}


	void Free(void* ptr) override
	{
		if (m_Buffer == ptr) m_CurPos = 0;
	}

	bool IsEnough()
	{
		return m_CurPos < sizeof(T) * Capacity;
	}
};

// Align 을 맞춰서 할당해주는 Allocator
template <typename T>
struct AlignedAllocator final : public GeneralAllocator<T>
{
	size_t m_Align = 4;

	explicit AlignedAllocator()
	{
		m_Align = ALIGNOF(T);
	}

	AlignedAllocator(size_t align)
		: m_Align(align)
	{
	}

	AlignedAllocator(const AlignedAllocator& allocator)
	{
		m_Align = allocator.m_Align;
	}

	AlignedAllocator& operator=(const AlignedAllocator& o)
	{
		return *this;
	}

	~AlignedAllocator() {};

	void* Allocate(size_t size, const char* file = nullptr, size_t line = 0) override
	{
		return pr_aligned_alloc(size, align);
	}

	void* Realloc(void* ptr, size_t size, const char* file = nullptr, size_t line = 0) override
	{
		return pr_aligned_realloc(ptr, align, size);
	}

	void Free(void* ptr) override
	{
		pr_aligned_free(ptr);
	}
};