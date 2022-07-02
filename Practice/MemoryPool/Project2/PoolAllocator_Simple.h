#pragma once

#include <cstdlib>
#include <iostream>

struct Chunk {
	/**
	 * When a chunk is free, the `next` contains the
	 * address of the next chunk in a list.
	 *
	 * When it's allocated, this space is used by
	 * the user.

	 즉, Allocation Pointer 가, 그 다음 Allocate 할
	 공간을 가리키고 있게 한다.
	 */
	Chunk* next;
};

/**
 * The allocator class.
 *
 * Features:
 *
 *   - Parametrized by number of chunks per block
 *   - Keeps track of the allocation pointer
 *   - Bump-allocates chunks
 *   - Requests a new larger block when needed
 *
 */

class PoolAllocator {
public:
	PoolAllocator(size_t chunksPerBlock) : mChunksPerBlock(chunksPerBlock) {}
	void* allocate(size_t size);
	void deallocate(void* ptr, size_t size);
private:
	/**
   * Number of chunks per larger block.
   */
	size_t mChunksPerBlock;

	// Allocation Pointer
	Chunk* m_Alloc = nullptr;

	// Allocate Larger Block (Pool) for Chunks
	Chunk* allocateBlock(size_t size);

};

inline void* PoolAllocator::allocate(size_t size)
{
	// No Chunks Left In The Current Block, Or Any Block Exist Yet
	// Allocate New One
	if (m_Alloc == nullptr)
		m_Alloc = allocateBlock(size);

	// 현재 Allocation Pointer 의 위치를 리턴한다
	Chunk* freeChunk = m_Alloc;

	// 다은 Chunk로 이동킨다.
	// 만약, 더이상의 chunk가 남아있지 않다면, m_Alloc 은 nullptr 이 될 것이다.
	// 그렇게 되면, 다음 요청 때는 새로운 Block 을 만들게 된다.
	m_Alloc = m_Alloc->next;

	return freeChunk;
}

// Chunk 를 Chunk 목록 앞에 넣는다.
inline void PoolAllocator::deallocate(void* chunk, size_t size)
{
	// 메모리 해제된 녀석은 Allocation Pointer 의 앞에 놓이게 된다
	reinterpret_cast<Chunk*>(chunk)->next = m_Alloc;

	// 다시 Allocation Pointer 의 위치를 앞으로 옮긴다.
	m_Alloc = reinterpret_cast<Chunk*>(chunk);
}

// Allocate New Block From OS
// Returns Chunk Pointer Set To Beginning Of Block
inline Chunk* PoolAllocator::allocateBlock(size_t chunkSize)
{
	std::cout << "New Block Allocated" << std::endl;

	// chunkSize --> ex) Attack Effect Class 크기 

	size_t blockSize = chunkSize * mChunksPerBlock;

	Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

	// Once The Block Is Allocated, We Need To Chain All
	// The Chunks In This Block
	Chunk* chunk = blockBegin;

	for (size_t i = 0; i < mChunksPerBlock - 1; ++i)
	{
		chunk->next = reinterpret_cast<Chunk*>((char*)(chunk)+chunkSize);
		chunk = chunk->next;
	}

	chunk->next = nullptr;

	return blockBegin;
}
