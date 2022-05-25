#include "StackAllocator.h"
#include "../Utils.h"  /* CalculatePadding */
#include <stdlib.h>     /* malloc, free */
#include <algorithm>    /* max */
#ifdef _DEBUG
#include <iostream>
#endif

using namespace std;

StackAllocator::StackAllocator(const std::size_t totalSize)
    : Allocator(totalSize) {

}

void StackAllocator::Init() {

}

StackAllocator::~StackAllocator() {
}

void* StackAllocator::Allocate(const std::size_t size, const std::size_t alignment) {

const size_t currentAddress = (size_t)m_start_ptr + m_offset;

  size_t padding = Utils::CalculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));

    if (m_offset + size + padding > m_totalSize)
    return nullptr;

m_offset += padding;

size_t nextAddress = currentAddress + padding;
size_t headerAddress = nextAddress - sizeof(AllocationHeader);

AllocationHeader allocatorHeader(padding);

AllocationHeader* headerPtr = (AllocationHeader*)headerAddress;

headerPtr = &allocatorHeader;

m_offset += size;

m_used = m_offset;

m_peak = std::max(m_peak, m_used);

return (void*)nextAddress;
};

void StackAllocator::Free(void* ptr) {
    // Move Offset Back
    size_t currentAddress = (size_t)ptr;
    size_t headerAddress = currentAddress - sizeof(AllocationHeader);
    AllocationHeader* allocationHeader((AllocationHeader*)headerAddress);

    m_offset = currentAddress - allocationHeader->padding - (size_t)m_start_ptr;

    m_used = m_offset;
}

void StackAllocator::Reset() {
    m_used = 0;
    m_peak = 0;
    m_offset = 0;
}