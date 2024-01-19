#include "Object.h"

void postinitialize_handler(Object* p_object) {
	p_object->_postinitialize();
}

void unregister_core_types()
{
}

void register_core_types()
{
}

void ObjectDB::cleanup()
{
}

ObjectID ObjectDB::add_instance(Object* p_object)
{
	return ObjectID();
}

void ObjectDB::remove_instance(Object* p_object)
{
}

void ObjectDB::setup()
{
}

Object::Object()
{
}

Object::~Object()
{
}

void Object::detach_from_objectdb()
{
}

void Object::_postinitialize()
{
	// _class_name_ptr = _get_class_namev(); // Set the direct pointer, which is much faster to obtain, but can only happen after postinitialize.
	// _initialize_classv();
	// _class_name_ptr = nullptr; // May have been called from a constructor.
	// notification(NOTIFICATION_POSTINITIALIZE);
}
