#include "FreeListAllocator.h"
#include "../Utils.h"  /* CalculatePaddingWithHeader */
#include <stdlib.h>     /* malloc, free */
#include <cassert>   /* assert		*/
#include <limits>  /* limits_max */
#include <algorithm>    // std::max

#ifdef _DEBUG
#include <iostream>
#endif

FreeListAllocator::FreeListAllocator(const std::size_t totalSize, const PlacementPolicy pPolicy)
: Allocator(totalSize) {
    m_pPolicy = pPolicy;
}

void FreeListAllocator::Init() {
    if (m_start_ptr != nullptr)
    {
        free(m_start_ptr);
        m_start_ptr = nullptr;
    };
    m_start_ptr = malloc(m_totalSize);
    Reset();
}

FreeListAllocator::~FreeListAllocator() {
    free(m_start_ptr);
    m_start_ptr = nullptr;
}

void* FreeListAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
    size_t allocationHeaderSize = sizeof(FreeListAllocator::AllocationHeader);
    size_t freeHeaderSize = sizeof(FreeListAllocator::FreeHeader);
    assert("Allocation size must be bigger" && size >= sizeof(Node));
     assert("Alignment must be 8 at least" && alignment >= 8);

     // List 를 순회하면서, Memory 를 할당할 수 있는 충분한 Block이 존재하는지 확인한다.
     size_t padding;
     Node* affectNode, *previousNode;

     Find(size, alignment, padding, previousNode, affectNode);
     assert(affectNode != nullptr && "not enough memory");

     size_t alignmentPadding = padding - allocationHeaderSize;
     size_t requiredSize = size + padding;

     size_t rest = affectNode->data.blockSize - requiredSize;

     if (rest > 0)
     {
         // We Have To Split Block Into Data Block,
         // And Free Block Of Size "rest"
         Node* newFreeNode = (Node*)((size_t)affectNode + requiredSize);
         newFreeNode->data.blockSize = rest;
         m_freeList.insert(affectNode, newFreeNode);
     }
     m_freeList.remove(previousNode, affectNode);
}

void FreeListAllocator::Find(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    switch (m_pPolicy)
    {
    case FIND_FIRST:
        /* code */
        break;
    case FIND_BEST :
        break;
    }
};

void FreeListAllocator::FindFirst(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    Node* it = m_freeList.head;
    Node* itPrev = nullptr;

    while (it != nullptr)
    {
        padding = Utils::CalculatePaddingWithHeader((size_t)it, alignment, 
        sizeof(FreeListAllocator::AllocationHeader));
        
        size_t requiredSpace = size + padding;

        if (it->data.blockSize >= requiredSpace)
            break;

        itPrev = it;
        it = it->next;
    }

    previousNode = itPrev;
    foundNode = it;
};

void FreeListAllocator::FindBest(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    size_t smallestDiff = std::numeric_limits<size_t>max();
    Node* bestBlock = nullptr;
    Node* it = m_freeList.head, *itPrev = nullptr;

    while (it != nullptr)
    {
        padding = Utils::CalculatePaddingWithHeader((size_t)it, alignment, sizeof(FreeListAllocator::AllocationHeader));
        size_t requiredSpace = size + padding;

        if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff))
        {
            bestBlock = it;
        }

        itPrev = it;
        it = it->next;
    }

    previousNode = itPrev;
    foundNode = bestBlock;
}

void FreeListAllocator::Free(void* ptr) {
    
}

void FreeListAllocator::Coalescence(Node* previousNode, Node * freeNode) {   
    
}

void FreeListAllocator::Reset() {
}