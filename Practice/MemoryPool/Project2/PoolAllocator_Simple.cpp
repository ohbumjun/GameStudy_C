#include <iostream>
#include "PoolAllocator.h"

using std::cout;
using std::endl;

// new , delete 연산자를 오버로딩하여, Custom Allocator 를 활용할 것이다.
struct Object {
	// Object data -> 16byte 를 활용한다.
	uint64_t data[2]; // 64 bit 크기의 data 2개 -> 8byte 의 data 크기 2개 --> 16byte

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
		// 맨 처음 : new Object() --> Object 크기의,  8개 Chunk 를 가진 Block 하나를 만들어내는 것이다.
		// 이후, new Object() 마다 다음 위치의 주소값을 가리키게 하고 -> 그곳의 메모리를 사용한다.
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