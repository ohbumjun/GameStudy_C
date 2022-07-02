#include "PoolAllocator.h"
#include <assert.h>
#include <cstdlib>
#include <algorithm>

using namespace  std;

PoolAllocator::PoolAllocator(const size_t totalSize, const size_t chunkSize) :
Allocator(totalSize), m_chunkSize(chunkSize)
{
	assert(m_chunkSize >= 8);
	assert(totalSize % m_chunkSize == 0);
}

PoolAllocator::~PoolAllocator()
{
	free(m_start_ptr);
}

void* PoolAllocator::Allocate(const size_t allocateSize, const size_t alignment)
{
	assert(m_chunkSize == allocateSize);

	Node* PopNode = m_freeList->pop();

	if (PopNode == nullptr)
		return nullptr;

	m_used += m_chunkSize;
	m_peak = max(m_used, m_peak);

	return (void*)PopNode;
}

void PoolAllocator::Free(void* ptr)
{
	m_freeList->push((Node*)ptr);

	m_used -= m_chunkSize;
	m_peak = max(m_used, m_peak);
}

void PoolAllocator::Init()
{
	m_start_ptr = malloc(m_totalSize);
	Reset();
}

void PoolAllocator::Reset()
{
	int chunkN = m_totalSize / m_chunkSize;

	for (int i = 0; i < chunkN; ++i)
	{
		const size_t address = (size_t)m_start_ptr + i * chunkN;
		m_freeList->push((Node*)address);
	}
}
