#pragma once

#include <assert.h>
#include <algorithm>
#include <stdlib.h> 
#include "Allocator.h"
#include "StackLinkedList.h"
#include <memory>

class PoolAllocator : public Allocator {
	private :
	struct FreeHeader{};
	typedef CStackLinkedList<FreeHeader>::Node Node;
	CStackLinkedList<FreeHeader> m_freeList;
	void* m_start_ptr;
	size_t m_chunkSize;
public :
	PoolAllocator(const std::size_t totalSize, const std::size_t chunkSize);
	virtual void* Allocate(const std::size_t allocationSize, const std::size_t alignment) override;
	virtual void Init() override;
	virtual void Free(void* ptr) override;
	virtual void Reset();
	virtual  ~PoolAllocator();
};