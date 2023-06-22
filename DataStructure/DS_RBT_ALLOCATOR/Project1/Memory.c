#include <malloc.h>

void* pr_malloc(size_t size)
{
	return malloc(size);
}

void* pr_realloc(void* ptr, size_t size)
{
	return realloc(ptr, size);
}

void* pr_calloc(size_t number, size_t size)
{
	return calloc(number, size);
}

void pr_free(void* ptr)
{
	free(ptr);
}

void* pr_aligned_alloc(size_t size, size_t alignment)
{
	// _aligned_malloc 와 malloc 함수의 주요한 차이점은
	// alignment 에 있다.
	/*
	size_t size = 16;  // Size of memory to allocate
    size_t alignment = 32;  // Alignment value in bytes

    // Allocate memory with specified size and alignment
    void* memory = _aligned_malloc(size, alignment);

	위 사례와 같이 _aligned_malloc 은 특정 크기의 메모리 블럭을 특정 alignment 에 맞게
	할당하는 것이다.

	alignemnt 값은, 할당된 메모리 블록의 시작 주소 위치를 결정한다. 

	반면, malloc 은 특정 크기의 메모리 블럭을 할당하는 것만 할 뿐
	특정 alignemt 를 맞춰주지는 않는다
	*/
	return _aligned_malloc(size, alignment);
}

void* pr_aligned_realloc(void* ptr, size_t alignment, size_t new_size)
{
	return _aligned_realloc(ptr, new_size, alignment);
}

void pr_aligned_free(void* pointer)
{
	_aligned_free(pointer);
}

