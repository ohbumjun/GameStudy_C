#include "StackAllocator.h"
#include "Utils.h"
#include <cstdlib>
#include <algorithm>

StackAllocator::StackAllocator(size_t totalSize) :
Allocator(totalSize), m_offset(0), m_start_ptr(nullptr)
{}

StackAllocator::~StackAllocator()
{
	free(m_start_ptr);
	m_offset = 0;
}

void* StackAllocator::Allocate(const size_t allocateSize, const size_t alignment)
{
	const size_t currentAddress = (size_t)m_start_ptr + m_offset;
	const size_t padding = Utils::CalculatePaddingWithHeader(currentAddress,
		alignment, sizeof(AllocationHeader));

	// 더이상 할당될 공간 X
	if (m_offset + padding + allocateSize > m_totalSize)
		return nullptr;

	const size_t nextAddress = currentAddress + padding;
	const size_t headerAddress = nextAddress - sizeof(AllocationHeader);

	AllocationHeader allocHeader(padding);
	AllocationHeader* allocHeaderAddress = (AllocationHeader*)headerAddress;
	allocHeaderAddress = &allocHeader;

	m_offset += (padding + allocateSize);

	m_used = m_offset;
	m_peak = max(m_peak, m_used);

	return (void*)nextAddress;
}

void StackAllocator::Init()
{
	if (m_start_ptr != nullptr)
		free(m_start_ptr);

	m_start_ptr = malloc(m_totalSize);
	Reset();
}

void StackAllocator::Free(void* ptr)
{
	const size_t currentAddress = (size_t)ptr;
	const size_t headerAddress = currentAddress - sizeof(AllocationHeader);
	AllocationHeader* allocHeader = (AllocationHeader*)headerAddress;

	m_offset = currentAddress - allocHeader->padding - (size_t)m_start_ptr;
	m_used = m_offset;
}

void StackAllocator::Reset()
{
	m_offset = 0;
	m_peak = 0;
	m_used = 0;
}
