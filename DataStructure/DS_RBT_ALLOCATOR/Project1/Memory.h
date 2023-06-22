#pragma once

#ifndef PR_MEMORY_H
#define PR_MEMORY_H

#include <malloc.h>

void* pr_malloc(size_t size);
void* pr_realloc(void* ptr, size_t size);
void* pr_calloc(size_t number, size_t size);
void  pr_free(void* ptr);
void* pr_aligned_alloc(size_t size, size_t alignment);
void* pr_aligned_realloc(void* ptr, size_t alignment, size_t new_size);
void  pr_aligned_free(void* pointer);

#endif // MY_HEADER_H