#include "PoolAllocator.h"

PoolAllocator::PoolAllocator(const std::size_t totalSize, const std::size_t chunkSize):
Allocator(totalSize), m_chunkSize(chunkSize)
{
	assert(m_chunkSize >= 8);
	assert(m_TotalSize % m_chunkSize == 0);
}

void* PoolAllocator::Allocate(const std::size_t allocationSize, const std::size_t alignment)
{
	assert(allocationSize == m_chunkSize); //

	Node* PopNode = m_freeList.pop();

	if (PopNode == nullptr)
	{
		assert(false);
	}

	m_used += m_chunkSize;
	m_peak = max(m_used, m_peak);

	return (void*)PopNode;
}

void PoolAllocator::Init()
{
	m_start_ptr = malloc(m_TotalSize);
	Reset();
}

void PoolAllocator::Free(void* ptr)
{
	m_used -= m_chunkSize;
	m_freeList.push((Node*)ptr);
}

void PoolAllocator::Reset()
{
	int nChunks = m_TotalSize / m_chunkSize;

	for (int i = 0; i < nChunks; ++i)
	{
		const size_t address = (std::size_t)m_start_ptr + i * m_chunkSize;
		m_freeList.push((Node*)address);
	}
}

PoolAllocator::~PoolAllocator()
{
	free(m_start_ptr);
}
