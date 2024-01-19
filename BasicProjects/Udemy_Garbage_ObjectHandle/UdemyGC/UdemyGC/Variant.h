#pragma once

#include "ObjectID.h"
#include "Object.h"
#include "ref_counted.h"

// double ¾Æ´Ï¸é float
typedef double real_t;

class Variant
{
public:
	struct Pools {
		union BucketSmall {
			BucketSmall() {}
			~BucketSmall() {}
		};
		union BucketMedium {
			BucketMedium() {}
			~BucketMedium() {}
		};
		union BucketLarge {
			BucketLarge() {}
			~BucketLarge() {}
		};

		// static PagedAllocator<BucketSmall, true> _bucket_small;
		// static PagedAllocator<BucketMedium, true> _bucket_medium;
		// static PagedAllocator<BucketLarge, true> _bucket_large;
	};
	struct ObjData {
		ObjectID id;
		Object* obj = nullptr;
	};

	union {
		bool _bool;
		int64_t _int;
		double _float;
		void* _ptr; //generic pointer
		uint8_t _mem[sizeof(ObjData) > (sizeof(real_t) * 4) ? sizeof(ObjData) : (sizeof(real_t) * 4)]{ 0 };
	} _data alignas(8);


	inline ObjData& _get_obj();
	inline const ObjData& _get_obj() const;

	void reference(const Variant& p_variant);
	static bool initialize_ref(Object* p_object);

	inline Variant() {}
	inline ~Variant() {
		clear();
	}
	void clear();
	void _clear_internal();
	Variant(const ObjectID& p_id);
	Variant(const Variant& p_variant); 
	Variant(const Object* p_object);
	void operator=(const Variant& p_variant); // only this is enough for all the other types

};

Variant::ObjData& Variant::_get_obj() {
	return *reinterpret_cast<ObjData*>(&_data._mem[0]);
}

const Variant::ObjData& Variant::_get_obj() const {
	return *reinterpret_cast<const ObjData*>(&_data._mem[0]);
}
