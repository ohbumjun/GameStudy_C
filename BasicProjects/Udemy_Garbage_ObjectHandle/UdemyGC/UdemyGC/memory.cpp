#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

SafeNumeric<uint64_t> Memory::alloc_count;

void* Memory::alloc_static(size_t p_bytes, bool p_pad_align) {
#ifdef DEBUG_ENABLED
	bool prepad = true;
#else
	bool prepad = p_pad_align;
#endif

	void* mem = malloc(p_bytes + (prepad ? PAD_ALIGN : 0));

	// ERR_FAIL_NULL_V(mem, nullptr);

	alloc_count.increment();

	if (prepad) {
		uint64_t* s = (uint64_t*)mem;
		*s = p_bytes;

		uint8_t* s8 = (uint8_t*)mem;

#ifdef DEBUG_ENABLED
		uint64_t new_mem_usage = mem_usage.add(p_bytes);
		max_usage.exchange_if_greater(new_mem_usage);
#endif
		return s8 + PAD_ALIGN;
	}
	else {
		return mem;
	}
}

void* Memory::realloc_static(void* p_memory, size_t p_bytes, bool p_pad_align) {
	if (p_memory == nullptr) {
		return alloc_static(p_bytes, p_pad_align);
	}

	uint8_t* mem = (uint8_t*)p_memory;

#ifdef DEBUG_ENABLED
	bool prepad = true;
#else
	bool prepad = p_pad_align;
#endif

	if (prepad) {
		mem -= PAD_ALIGN;
		uint64_t* s = (uint64_t*)mem;

#ifdef DEBUG_ENABLED
		if (p_bytes > *s) {
			uint64_t new_mem_usage = mem_usage.add(p_bytes - *s);
			max_usage.exchange_if_greater(new_mem_usage);
		}
		else {
			mem_usage.sub(*s - p_bytes);
		}
#endif

		if (p_bytes == 0) {
			free(mem);
			return nullptr;
		}
		else {
			*s = p_bytes;

			mem = (uint8_t*)realloc(mem, p_bytes + PAD_ALIGN);
			// ERR_FAIL_NULL_V(mem, nullptr);

			s = (uint64_t*)mem;

			*s = p_bytes;

			return mem + PAD_ALIGN;
		}
	}
	else {
		mem = (uint8_t*)realloc(mem, p_bytes);

		// ERR_FAIL_COND_V(mem == nullptr && p_bytes > 0, nullptr);

		return mem;
	}
}

void Memory::free_static(void* p_ptr, bool p_pad_align) {
	// ERR_FAIL_NULL(p_ptr);

	uint8_t* mem = (uint8_t*)p_ptr;

#ifdef DEBUG_ENABLED
	bool prepad = true;
#else
	bool prepad = p_pad_align;
#endif

	alloc_count.decrement();

	if (prepad) {
		mem -= PAD_ALIGN;

#ifdef DEBUG_ENABLED
		uint64_t* s = (uint64_t*)mem;
		mem_usage.sub(*s);
#endif

		free(mem);
	}
	else {
		free(mem);
	}
}

uint64_t Memory::get_mem_available() {
	return -1; // 0xFFFF...
}

uint64_t Memory::get_mem_usage() {
#ifdef DEBUG_ENABLED
	return mem_usage.get();
#else
	return 0;
#endif
}

uint64_t Memory::get_mem_max_usage() {
#ifdef DEBUG_ENABLED
	return max_usage.get();
#else
	return 0;
#endif
}