#pragma once

#include "Flag.h"
#include "Allocable.h"
#include "CHeapAllocator.h"
#include "GeneralAllocator.h"

#include <type_traits>
#include <functional>

static const int INITIAL_CAPACITY = 10;

template <size_t InlineAlign = 4, typename TAllocator = CHeapAllocator>
struct LvPoolAllocator final : Allocable
{
	struct Area
	{
		struct Fragment
		{
			char* m_Address;

			uint32 m_Size;
		};

		const uint32 m_SizeBytes = sizeof(uint32);

		const int m_MinSize = m_SizeBytes * 3;

		char* m_Memory;

		uint32 m_Cursor;

		uint32 m_Chunk;

		bool m_IsAligned;

		std::unordered_set<uintptr> m_FreeAddresses;
	};
};

