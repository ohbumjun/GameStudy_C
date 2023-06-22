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
	// _aligned_malloc �� malloc �Լ��� �ֿ��� ��������
	// alignment �� �ִ�.
	/*
	size_t size = 16;  // Size of memory to allocate
    size_t alignment = 32;  // Alignment value in bytes

    // Allocate memory with specified size and alignment
    void* memory = _aligned_malloc(size, alignment);

	�� ��ʿ� ���� _aligned_malloc �� Ư�� ũ���� �޸� ���� Ư�� alignment �� �°�
	�Ҵ��ϴ� ���̴�.

	alignemnt ����, �Ҵ�� �޸� ����� ���� �ּ� ��ġ�� �����Ѵ�. 

	�ݸ�, malloc �� Ư�� ũ���� �޸� ���� �Ҵ��ϴ� �͸� �� ��
	Ư�� alignemt �� ���������� �ʴ´�
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

