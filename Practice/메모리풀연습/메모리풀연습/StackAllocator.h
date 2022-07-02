#pragma once

#include <cstddef>
#include "Alllocator.h"

class StackAllocator : public Allocator {
public :
	StackAllocator(size_t totalSize);
	~StackAllocator();
	virtual void* Allocate(const size_t allocateSize, const size_t alignment) override;
	virtual void Init() override;
	virtual void Free(void* ptr) override;
	virtual void Reset();
private :
	void* m_start_ptr = nullptr;
	size_t m_offset;
private :
	struct AllocationHeader {
		char padding;
	public :
		AllocationHeader(size_t pad) : padding(pad){}
	};
};