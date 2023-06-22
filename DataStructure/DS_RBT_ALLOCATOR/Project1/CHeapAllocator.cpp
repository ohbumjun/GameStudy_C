#include "CHeapAllocator.h"

CHeapAllocator::CHeapAllocator()
{
}

CHeapAllocator::CHeapAllocator(const CHeapAllocator& allocator)
{

}

CHeapAllocator& CHeapAllocator::operator=(const CHeapAllocator&)
{
    return *this;
}

CHeapAllocator::~CHeapAllocator()
{
}

void* CHeapAllocator::Allocate(size_t size, const char* file, size_t line)
{
    return pr_malloc(size);
}

void* CHeapAllocator::Reallocate(void* ptr, size_t size, const char* file, size_t line)
{
    return pr_realloc(ptr, size);
}

void CHeapAllocator::Free(void* ptr)
{
    pr_free(ptr);
}

void* CHeapAllocator::AlignedAllocate(size_t size, size_t align, const char* file, size_t line)
{
    return pr_aligned_alloc(size, align);
}

void* CHeapAllocator::AlignedReallocate(void* ptr, size_t size, size_t align, const char* file, size_t line)
{
    return nullptr;
}

void CHeapAllocator::AlignedFree(void* ptr)
{
}
