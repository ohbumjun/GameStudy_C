#pragma once

#include "safe_refcount.h"

#ifndef PAD_ALIGN
#define PAD_ALIGN 16 //must always be greater than this at much
#endif

class Memory {
	static SafeNumeric<uint64_t> alloc_count;

public:
	static void* alloc_static(size_t p_bytes, bool p_pad_align = false);
	static void* realloc_static(void* p_memory, size_t p_bytes, bool p_pad_align = false);
	static void free_static(void* p_ptr, bool p_pad_align = false);

	static uint64_t get_mem_available();
	static uint64_t get_mem_usage();
	static uint64_t get_mem_max_usage();
};

#define memalloc(m_size) Memory::alloc_static(m_size)
#define memrealloc(m_mem, m_size) Memory::realloc_static(m_mem, m_size)
#define memfree(m_mem) Memory::free_static(m_mem)

inline void postinitialize_handler(void*) {}

template <class T>
inline T* _post_initialize(T* p_obj) {
	postinitialize_handler(p_obj);
	return p_obj;
}

#define memnew(m_class) _post_initialize(new ("") m_class)

#define memnew_allocator(m_class, m_allocator) _post_initialize(new (m_allocator::alloc) m_class)
#define memnew_placement(m_placement, m_class) _post_initialize(new (m_placement) m_class)

template <class T>
void memdelete(T* p_class) {
	if (!predelete_handler(p_class)) {
		return; // doesn't want to be deleted
	}
	if (!std::is_trivially_destructible<T>::value) {
		p_class->~T();
	}

	Memory::free_static(p_class, false);
}

template <class T, class A>
void memdelete_allocator(T* p_class) {
	if (!predelete_handler(p_class)) {
		return; // doesn't want to be deleted
	}
	if (!std::is_trivially_destructible<T>::value) {
		p_class->~T();
	}

	A::free(p_class);
}

#define memdelete_notnull(m_v) \
	{                          \
		if (m_v) {             \
			memdelete(m_v);    \
		}                      \
	}

#define memnew_arr(m_class, m_count) memnew_arr_template<m_class>(m_count)
