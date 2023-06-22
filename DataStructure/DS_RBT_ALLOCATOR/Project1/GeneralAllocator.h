#pragma once

#include "Flag.h"
#include "Allocable.h"

// #if defined(__WIN32__) || defined(__ANDROID__)
// #include <malloc.h>
// #else

template <typename T>
class GeneralAllocator : Allocable
{
	// T Type �� ��ü�� ���� �� �Ҵ��ϴ� �Լ�
	inline T* Allocate(size_t count, const char* file = nullptr, size_t line = 0)
	{
		return static_cast<T*>(Allocate(sizeof(T) * count, file, line));
	}

	inline T* Reallocate(T* ptr, size_t count, const char* flie = nullptr, size_t line = 0)
	{
		return static_cast<T*>(Reallocate(ptr, sizeof(T) * count, flie, line));
	}
};

// 1) ������ ũ���� Buffer �θ� �Ҵ��� �����ϴ� Allocator
// 2) Align �� ���缭 �Ҵ����ִ� Allocator
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
		�߿��� ���� ���� : size �� Alignof(T) �� ������ �����Ѵ�. 
		   ���� size �� align �Ǿ����� �ʴٸ�, object �� Ư�� align �� ���߱� ����
		   �ʿ��� padding �� ������ش�.

		(����)
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

// Align �� ���缭 �Ҵ����ִ� Allocator
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