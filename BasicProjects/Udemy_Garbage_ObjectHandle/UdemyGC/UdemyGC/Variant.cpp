#include "Variant.h"
#include "memory.h"

void Variant::reference(const Variant& p_variant)
{
	memnew_placement(_data._mem, ObjData);

	if (p_variant._get_obj().obj && p_variant._get_obj().id.is_ref_counted()) {
		RefCounted* ref_counted = static_cast<RefCounted*>(p_variant._get_obj().obj);
		if (!ref_counted->reference()) {
			_get_obj().obj = nullptr;
			_get_obj().id = ObjectID();
			// break;
		}
	}

	_get_obj().obj = const_cast<Object*>(p_variant._get_obj().obj);
	_get_obj().id = p_variant._get_obj().id;
}

bool Variant::initialize_ref(Object* p_object)
{
	RefCounted* ref_counted = const_cast<RefCounted*>(static_cast<const RefCounted*>(p_object));
	if (!ref_counted->init_ref()) {
		return false;
	}
	return true;
}

void Variant::clear()
{
	if (unlikely(false)) { // Make it fast for types that don't need deinit.
		_clear_internal();
	}
}

void Variant::_clear_internal()
{
	if (_get_obj().id.is_ref_counted()) {
		// We are safe that there is a reference here.
		RefCounted* ref_counted = static_cast<RefCounted*>(_get_obj().obj);
		if (ref_counted->unreference()) {
			memdelete(ref_counted);
		}
	}
	_get_obj().obj = nullptr;
	_get_obj().id = ObjectID();
}

Variant::Variant(const ObjectID& p_id)
{
	// type = INT;
	_data._int = p_id;
}

Variant::Variant(const Variant& p_variant)
{
}

Variant::Variant(const Object* p_object)
{
	// type = OBJECT;

	memnew_placement(_data._mem, ObjData);

	if (p_object) {
		if (p_object->is_ref_counted()) {
			RefCounted* ref_counted = const_cast<RefCounted*>(static_cast<const RefCounted*>(p_object));
			if (!ref_counted->init_ref()) {
				_get_obj().obj = nullptr;
				_get_obj().id = ObjectID();
				return;
			}
		}

		_get_obj().obj = const_cast<Object*>(p_object);
		_get_obj().id = p_object->get_instance_id();
	}
	else {
		_get_obj().obj = nullptr;
		_get_obj().id = ObjectID();
	}
}

void Variant::operator=(const Variant& p_variant)
{
}

Object* Variant::get_validated_object() const {
	if (type == OBJECT) {
		return ObjectDB::get_instance(_get_obj().id);
	}
	else {
		return nullptr;
	}
}