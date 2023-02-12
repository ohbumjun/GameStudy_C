#include "SimpleObject.h"

SimpleObject::SimpleObject(ClassDef const* class_)
	: DynaObject(class_)
{
	init(this);
}

SimpleObject::SimpleObject(SimpleObject const& rhs)
	: DynaObject(rhs)
{
	copy(this, rhs);
}

Object* SimpleObject::newObject(ClassDef const* myClass)
{
	return new SimpleObject(myClass);
}

Object* SimpleObject::clone() const
{
	return new SimpleObject(*this);
}

