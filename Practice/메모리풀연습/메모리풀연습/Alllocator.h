#pragma once

class Allocator {
protected :
	size_t m_totalSize;
	size_t m_used;
	size_t m_peak;
public :
	Allocator(size_t totalSize);
	virtual ~Allocator();
	virtual void* Allocate(const size_t allocateSize, const size_t alignment = 0) = 0;
	virtual void Init() = 0;
	virtual void Free(void* ptr) = 0;
};


