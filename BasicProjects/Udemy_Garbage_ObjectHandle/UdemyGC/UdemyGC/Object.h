#pragma once
/**************************************************************************/
/*  object.h                                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "SpinLock.h"
#include "ObjectID.h"

void postinitialize_handler(Object* p_object);

class Object
{
public :
	Object();
	virtual ~Object();
	template <class T>
	static T* cast_to(Object* p_object) {
		return dynamic_cast<T*>(p_object);
	}

	template <class T>
	static const T* cast_to(const Object* p_object) {
		return dynamic_cast<const T*>(p_object);
	}

	void detach_from_objectdb();
	inline ObjectID get_instance_id() const { return _instance_id; }
private :
	Object(bool p_reference);

	ObjectID _instance_id;

	friend class RefCounted;
	bool type_is_reference = false;

	friend void postinitialize_handler(Object*);
	void _postinitialize();
};


class ObjectDB {
	// This needs to add up to 63, 1 bit is for reference.
#define OBJECTDB_VALIDATOR_BITS 39
#define OBJECTDB_VALIDATOR_MASK ((uint64_t(1) << OBJECTDB_VALIDATOR_BITS) - 1)
#define OBJECTDB_SLOT_MAX_COUNT_BITS 24
#define OBJECTDB_SLOT_MAX_COUNT_MASK ((uint64_t(1) << OBJECTDB_SLOT_MAX_COUNT_BITS) - 1)
#define OBJECTDB_REFERENCE_BIT (uint64_t(1) << (OBJECTDB_SLOT_MAX_COUNT_BITS + OBJECTDB_VALIDATOR_BITS))

	struct ObjectSlot { // 128 bits per slot.
		uint64_t validator : OBJECTDB_VALIDATOR_BITS;
		uint64_t next_free : OBJECTDB_SLOT_MAX_COUNT_BITS;
		uint64_t is_ref_counted : 1;
		Object* object = nullptr;
	};

	static SpinLock spin_lock;
	static uint32_t slot_count;
	static uint32_t slot_max;
	static ObjectSlot* object_slots;
	static uint64_t validator_counter;

	friend class Object;
	friend void unregister_core_types();
	static void cleanup();

	static ObjectID add_instance(Object* p_object);
	static void remove_instance(Object* p_object);

	friend void register_core_types();
	static void setup();

public:
	typedef void (*DebugFunc)(Object* p_obj);

	inline static Object* get_instance(ObjectID p_instance_id) {
		uint64_t id = p_instance_id;
		uint32_t slot = id & OBJECTDB_SLOT_MAX_COUNT_MASK;

		spin_lock.lock();

		uint64_t validator = (id >> OBJECTDB_SLOT_MAX_COUNT_BITS) & OBJECTDB_VALIDATOR_MASK;

		if (unlikely(object_slots[slot].validator != validator)) {
			spin_lock.unlock();
			return nullptr;
		}

		Object* object = object_slots[slot].object;

		spin_lock.unlock();

		return object;
	}
	static void debug_objects(DebugFunc p_func);
	static int get_object_count();
};