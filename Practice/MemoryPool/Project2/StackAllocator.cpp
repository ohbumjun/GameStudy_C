#include "StackAllocator.h"
#include "Utils.h"  /* CalculatePadding */
#include <stdlib.h>     /* malloc, free */
#include <algorithm>    /* max */
#ifdef _DEBUG
#include <iostream>
#endif

StackAllocator::StackAllocator(const std::size_t totalSize)
    : Allocator(totalSize) {

}

void StackAllocator::Init() {

}

StackAllocator::~StackAllocator() {
}

void* StackAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
    const std::size_t currentAddress = (std::size_t)m_start_ptr + m_offset;

    std::size_t padding = Utils::CalculatePaddingWithHeader(
        currentAddress, alignment, sizeof(AllocationHeader));

    if (m_offset + padding + size < m_totalSize)
        return nullptr;

    m_offset += padding;

    const std::size_t nextAddress = currentAddress + padding;
    const std::size_t headerAddress = nextAddress - sizeof(AllocationHeader);
    AllocationHeader allocationHeader(padding);
    AllocationHeader* headerPtr = (AllocationHeader*)headerAddress;
    headerPtr = &allocationHeader;

    m_offset += size;

    #ifdef _DEBUG
        std::cout << "A" << "\t@C " << (void*)currentAddress << "\t@R " << (void*)nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
    #endif

        m_used = m_offset;
        m_peak = std::max(m_peak, m_used);

        return (void*)nextAddress;
}

void StackAllocator::Free(void* ptr) {
    // Move offset back to clear address
    const std::size_t currentAddress = (std::size_t)ptr;
    const std::size_t headerAddress = currentAddress - sizeof(AllocationHeader);
    const AllocationHeader* allocationHeader{ (AllocationHeader*)headerAddress};

    m_offset = currentAddress - allocationHeader->padding - (std::size_t)m_start_ptr;
    m_used = m_offset;
}

void StackAllocator::Reset() {
    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}