#include "Alllocator.h"

Allocator::Allocator(size_t totalSize) :
m_totalSize(totalSize), m_used(0),m_peak(0)
{}

Allocator::~Allocator()
{
	m_totalSize = 0;
}

void* Allocator::Allocate(const size_t allocateSize, const size_t alignment)
{}

void Allocator::Init()
{}

void Allocator::Free()
{}