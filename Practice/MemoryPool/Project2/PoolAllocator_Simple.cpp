#include <iostream>
#include "PoolAllocator.h"

using std::cout;
using std::endl;

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

/*
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
*/