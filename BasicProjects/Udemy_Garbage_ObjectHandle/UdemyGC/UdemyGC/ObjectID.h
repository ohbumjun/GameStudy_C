#pragma once

#include "typedefs.h"

class ObjectID {
	uint64_t id = 0;

public:
	inline bool is_ref_counted() const { return (id & (uint64_t(1) << 63)) != 0; }
	inline bool is_valid() const { return id != 0; }
	inline bool is_null() const { return id == 0; }
	inline operator uint64_t() const { return id; }
	inline operator int64_t() const { return id; }
	inline
	inline bool operator==(const ObjectID& p_id) const { return id == p_id.id; }
	inline bool operator!=(const ObjectID& p_id) const { return id != p_id.id; }
	inline bool operator<(const ObjectID& p_id) const { return id < p_id.id; }
	inline
	inline void operator=(int64_t p_int64) { id = p_int64; }
	inline void operator=(uint64_t p_uint64) { id = p_uint64; }
	inline
	inline ObjectID() {}
	inline explicit ObjectID(const uint64_t p_id) { id = p_id; }
	inline explicit ObjectID(const int64_t p_id) { id = p_id; }
};


