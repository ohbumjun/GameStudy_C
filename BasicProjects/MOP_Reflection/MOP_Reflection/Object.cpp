#include "Object.h"
#include "ClassDef.h"
#include "Value.h"

Object::Object(ClassDef const* class_) :
	myClass(class_)
{
};


ClassDef const* Object::instanceOf() const
{
	return myClass;
}

Value Object::getValue(std::string const& name) const
{
	size_t idx = instanceOf()->findAttribute(name); // check for not found
	return getValue(idx);
}


void Object::setValue(std::string const& name, Value const& v)
{
	size_t idx = instanceOf()->findAttribute(name); // check for not found
	setValue(idx, v);
}

BaseValue* Object::getBaseValue(Object* o, size_t idx)
{
	return o->getBaseValue(idx);
}

BaseValue const* Object::getBaseValue(Object const* o, size_t idx)
{
	return o->getBaseValue(idx);
}

