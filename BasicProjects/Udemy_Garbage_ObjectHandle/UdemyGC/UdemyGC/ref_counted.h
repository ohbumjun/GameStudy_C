#pragma once

#include "Object.h"
#include "ObjectID.h"
#include "safe_refcount.h"
#include "Variant.h"

class RefCounted : public Object {
	SafeRefCount refcount;
	SafeRefCount refcount_init;

protected:
	static void _bind_methods();

public:
	inline bool is_referenced() const { return refcount_init.get() != 1; }
	bool init_ref();
	bool reference(); // returns false if refcount is at zero and didn't get increased
	bool unreference();
	int get_reference_count() const;

	RefCounted();
	~RefCounted() {}
};

// ex) Ref<Texture>
template <class T>
class Ref {
	T* reference = nullptr;

	void ref(const Ref& p_from) {
		if (p_from.reference == reference) {
			return;
		}

		unref();

		reference = p_from.reference;
		if (reference) {
			reference->reference();
		}
	}

	void ref_pointer(T* p_ref) {
		// ERR_FAIL_NULL(p_ref);

		if (p_ref->init_ref()) {
			reference = p_ref;
		}
	}

	//virtual RefCounted * get_reference() const { return reference; }
public:
	inline bool operator==(const T* p_ptr) const {
		return reference == p_ptr;
	}
	inline bool operator!=(const T* p_ptr) const {
		return reference != p_ptr;
	}

	inline bool operator<(const Ref<T>& p_r) const {
		return reference < p_r.reference;
	}
	inline bool operator==(const Ref<T>& p_r) const {
		return reference == p_r.reference;
	}
	inline bool operator!=(const Ref<T>& p_r) const {
		return reference != p_r.reference;
	}

	inline T* operator*() const {
		return reference;
	}

	inline T* operator->() const {
		return reference;
	}

	inline T* ptr() const {
		return reference;
	}

	operator Variant() const {
		return Variant(reference);
	}

	void operator=(const Ref& p_from) {
		ref(p_from);
	}

	template <class T_Other>
	void operator=(const Ref<T_Other>& p_from) {
		RefCounted* refb = const_cast<RefCounted*>(static_cast<const RefCounted*>(p_from.ptr()));
		if (!refb) {
			unref();
			return;
		}
		Ref r;
		r.reference = Object::cast_to<T>(refb);
		ref(r);
		r.reference = nullptr;
	}

	void operator=(const Variant& p_variant) {
		Object* object = p_variant.get_validated_object();

		if (object == reference) {
			return;
		}

		unref();

		if (!object) {
			return;
		}

		T* r = Object::cast_to<T>(object);
		if (r && r->reference()) {
			reference = r;
		}
	}

	template <class T_Other>
	void reference_ptr(T_Other* p_ptr) {
		if (reference == p_ptr) {
			return;
		}
		unref();

		T* r = Object::cast_to<T>(p_ptr);
		if (r) {
			ref_pointer(r);
		}
	}

	Ref(const Ref& p_from) {
		ref(p_from);
	}

	template <class T_Other>
	Ref(const Ref<T_Other>& p_from) {
		RefCounted* refb = const_cast<RefCounted*>(static_cast<const RefCounted*>(p_from.ptr()));
		if (!refb) {
			unref();
			return;
		}
		Ref r;
		r.reference = Object::cast_to<T>(refb);
		ref(r);
		r.reference = nullptr;
	}

	Ref(T* p_reference) {
		if (p_reference) {
			ref_pointer(p_reference);
		}
	}

	Ref(const Variant& p_variant) {
		Object* object = p_variant.get_validated_object();

		if (!object) {
			return;
		}

		T* r = Object::cast_to<T>(object);
		if (r && r->reference()) {
			reference = r;
		}
	}

	inline bool is_valid() const { return reference != nullptr; }
	inline bool is_null() const { return reference == nullptr; }

	void unref() {
		// TODO: this should be moved to mutexes, since this engine does not really
		// do a lot of referencing on references and stuff
		// mutexes will avoid more crashes?

		if (reference && reference->unreference()) {
			memdelete(reference);
		}
		reference = nullptr;
	}

	void instantiate() {
		ref(memnew(T));
	}

	Ref() {}

	~Ref() {
		unref();
	}
};

class WeakRef : public RefCounted {

	ObjectID ref;

protected:
	static void _bind_methods();

public:
	Variant get_ref() const;
	void set_obj(Object* p_object);
	void set_ref(const Ref<RefCounted>& p_ref);

	WeakRef() {}
};


