#include <iostream>
#include <cstdlib>


using std::cout;
using std::endl;


struct Chunk {
	/**
	 * When a chunk is free, the `next` contains the
	 * address of the next chunk in a list.
	 *
	 * When it's allocated, this space is used by
	 * the user.

	 ��, Allocation Pointer ��, �� ���� Allocate ��
	 ������ ����Ű�� �ְ� �Ѵ�.
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
	PoolAllocator(size_t chunksPerBlock) : mChunksPerBlock(chunksPerBlock){}
	void* allocate(size_t size);
	void deallocate(void* ptr, size_t size);
private :
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
	if (m_Alloc = nullptr)
		m_Alloc = allocateBlock(size);

	// ���� Allocation Pointer �� ��ġ�� �����Ѵ�
	Chunk* freeChunk = m_Alloc;

	// ���� Chunk�� �̵�Ų��.
	// ����, ���̻��� chunk�� �������� �ʴٸ�, m_Alloc �� nullptr �� �� ���̴�.
	// �׷��� �Ǹ�, ���� ��û ���� ���ο� Block �� ����� �ȴ�.
	m_Alloc = m_Alloc->next;

	return freeChunk;
}

// Chunk �� Chunk ��� �տ� �ִ´�.
inline void PoolAllocator::deallocate(void* chunk, size_t size)
{
	// �޸� ������ �༮�� Allocation Pointer �� �տ� ���̰� �ȴ�
	reinterpret_cast<Chunk*>(chunk)->next = m_Alloc;

	// �ٽ� Allocation Pointer �� ��ġ�� ������ �ű��.
	m_Alloc = reinterpret_cast<Chunk*>(chunk);
}

// Allocate New Block From OS
// Returns Chunk Pointer Set To Beginning Of Block
inline Chunk* PoolAllocator::allocateBlock(size_t chunkSize)
{
	// chunkSize --> ex) Attack Effect Class ũ�� 

	size_t blockSize = chunkSize * mChunksPerBlock;

	Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

	// Once The Block Is Allocated, We Need To Chain All
	// The Chunks In This Block
	Chunk* chunk = blockBegin;

	for (int i = 0; i < mChunksPerBlock - 1; ++i)
	{
		chunk->next = reinterpret_cast<Chunk*>((char*)(chunk)+chunkSize);
		chunk = chunk->next;
	}

	chunk->next = nullptr;

	return blockBegin;
}


// new , delete �����ڸ� �����ε��Ͽ�, Custom Allocator �� Ȱ���� ���̴�.
struct Object {
	// Object data -> 16byte �� Ȱ���Ѵ�.
	uint64_t data[2]; // 64 bit ũ���� data 2�� -> 8byte �� data ũ�� 2�� --> 16byte

	static PoolAllocator allocator;

	static void* operator new (size_t size)
	{
		return allocator.allocate(size);
	}

	static void operator delete (void* ptr, size_t size)
	{
		return allocator.deallocate(ptr, size);
	}
};

PoolAllocator Object::allocator{ 8 };

int main()
{
	// Allocate 10 pointers to our "Object" Instances
	constexpr int arryaSize = 10;

	Object* objects[arryaSize];

	// Two `uint64_t`, 16 bytes.
	cout << "size(Object) = " << sizeof(Object) << endl << endl;

	for (int i = 0; i < arryaSize; ++i)
	{
		// �� ó�� : new Object() --> Object ũ����,  8�� Chunk �� ���� Block �ϳ��� ������ ���̴�.
		// ����, new Object() ���� ���� ��ġ�� �ּҰ��� ����Ű�� �ϰ� -> �װ��� �޸𸮸� ����Ѵ�.
		objects[i] = new Object();
		cout << "new [" << i << "] = " << objects[i] << endl;
	}

	for (int i = arryaSize - 1; i >= 0; --i)
	{
		cout << "delete [" << i << "] = " << objects[i] << endl;
		delete objects[i];
	}

	return 0;
}
