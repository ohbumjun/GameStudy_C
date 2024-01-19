#pragma once

#include <atomic>
#include <type_traits>

#include "typedefs.h"


template <class T>
class SafeNumeric {
	std::atomic<T> value;

	static_assert(std::atomic<T>::is_always_lock_free);

public:
	inline void set(T p_value) {
		value.store(p_value, std::memory_order_release);
	}

	inline T get() const {
		return value.load(std::memory_order_acquire);
	}

	inline T increment() {
		return value.fetch_add(1, std::memory_order_acq_rel) + 1;
	}

	// Returns the original value instead of the new one
	inline T postincrement() {
		return value.fetch_add(1, std::memory_order_acq_rel);
	}

	inline T decrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel) - 1;
	}

	// Returns the original value instead of the new one
	inline T postdecrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel);
	}

	inline T add(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel) + p_value;
	}

	// Returns the original value instead of the new one
	inline T postadd(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel);
	}

	inline T sub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel) - p_value;
	}

	inline T bit_or(T p_value) {
		return value.fetch_or(p_value, std::memory_order_acq_rel);
	}
	inline T bit_and(T p_value) {
		return value.fetch_and(p_value, std::memory_order_acq_rel);
	}

	inline T bit_xor(T p_value) {
		return value.fetch_xor(p_value, std::memory_order_acq_rel);
	}

	// Returns the original value instead of the new one
	inline T postsub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel);
	}

	inline T exchange_if_greater(T p_value) {
		while (true) {
			T tmp = value.load(std::memory_order_acquire);
			if (tmp >= p_value) {
				return tmp; // already greater, or equal
			}

			if (value.compare_exchange_weak(tmp, p_value, std::memory_order_acq_rel)) {
				return p_value;
			}
		}
	}

	inline T conditional_increment() {
		while (true) {
			T c = value.load(std::memory_order_acquire);
			if (c == 0) {
				return 0;
			}
			if (value.compare_exchange_weak(c, c + 1, std::memory_order_acq_rel)) {
				return c + 1;
			}
		}
	}

	inline explicit SafeNumeric<T>(T p_value = static_cast<T>(0)) {
		set(p_value);
	}
};


class SafeRefCount {
	SafeNumeric<uint32_t> count;

#ifdef DEV_ENABLED
	inline void _check_unref_safety() {
		// This won't catch every misuse, but it's better than nothing.
		CRASH_COND_MSG(count.get() == 0,
			"Trying to unreference a SafeRefCount which is already zero is wrong and a symptom of it being misused.\n"
			"Upon a SafeRefCount reaching zero any object whose lifetime is tied to it, as well as the ref count itself, must be destroyed.\n"
			"Moreover, to guarantee that, no multiple threads should be racing to do the final unreferencing to zero.");
	}
#endif

public:
	inline bool ref() { // true on success
		return count.conditional_increment() != 0;
	}

	inline uint32_t refval() { // none-zero on success
		return count.conditional_increment();
	}

	inline bool unref() { // true if must be disposed of
#ifdef DEV_ENABLED
		_check_unref_safety();
#endif
		return count.decrement() == 0;
	}

	inline uint32_t unrefval() { // 0 if must be disposed of
#ifdef DEV_ENABLED
		_check_unref_safety();
#endif
		return count.decrement();
	}

	inline uint32_t get() const {
		return count.get();
	}

	inline void init(uint32_t p_value = 1) {
		count.set(p_value);
	}
};
