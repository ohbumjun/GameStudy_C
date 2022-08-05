#pragma once

#include "Alllocator.h"
#include "StackLinkedList.h"

class PoolAllocator : public Allocator {
private :
	struct FreeHeader{};
	typedef StackLinkedList<FreeHeader>::Node Node;
	StackLinkedList<FreeHeader>* m_freeList;
	void* m_start_ptr;
	size_t m_chunkSize;
public :
	PoolAllocator(const size_t totalSize, 
	const size_t chunkSize);
	virtual ~PoolAllocator();
	virtual void* Allocate(const size_t allocateSize, const size_t alignment) override;
	virtual void Free(void* ptr) override;
	virtual void Init() override;
	virtual void Reset();
};


