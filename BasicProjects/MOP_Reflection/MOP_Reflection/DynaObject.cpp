
#include "DynaObject.h"
#include "ClassDef.h"
#include "Value.h"

DynaObject::DynaObject(ClassDef const* class_)
	: Object(class_), values(class_->getAttributeCount(), 0), privateObj(0)
{
	// we can't initialize 'values' here, as 'this' is not a valid pointer
}

DynaObject::DynaObject(DynaObject const& rhs)
	: Object(rhs), values(rhs.instanceOf()->getAttributeCount(), 0)
{
	// we can't initialize 'values' here, as 'this' is not a valid pointer
}

DynaObject::~DynaObject()
{
	for (int idx = 0; idx != values.size(); ++idx)
	{
		delete values[idx];
	}
}

Value DynaObject::getValue(size_t idx) const
{
	return values[idx]->getFinal();
}

void DynaObject::setValue(size_t idx, Value const& v)
{
	values[idx]->set(v);
}

void DynaObject::replaceValue(size_t idx, BaseValue const& v)
{
	BaseValue* oldV = values[idx];
	values[idx] = v.clone(this);
	delete oldV;
}

void DynaObject::replaceValue(std::string const& name, BaseValue const& v)
{
	size_t idx = instanceOf()->findAttribute(name); // check for not found
	replaceValue(idx, v);
}

Object* DynaObject::privObject() const
{
	return privateObj;
}

void DynaObject::setPrivate(Object* priv)
{
	privateObj = priv;
}

BaseValue* DynaObject::getBaseValue(size_t idx)
{
	return values[idx];
}

BaseValue const* DynaObject::getBaseValue(size_t idx) const
{
	return values[idx];
}

void DynaObject::init(DynaObject* self)
{
	Object const* def = instanceOf()->defObject();
	if (def) // def is not necessarily a DynaObject!
	{
		for (int idx = 0; idx != values.size(); ++idx)
		{
			values[idx] = Object::getBaseValue(def, idx)->clone(self);
		}
	}
	else
	{
		ClassDef::AttrIterator a;
		ValueContainer::iterator i = values.begin();

		// 해당 Class 의 Attribute 들에 대한 기본값 세팅
		for (a = instanceOf()->attribBegin();
			a != instanceOf()->attribEnd();
			++a, ++i)
		{
			*i = a->getType().newValue();
		}
	}
}

void DynaObject::copy(DynaObject* self, DynaObject const& rhs)
{
	for (int idx = 0; idx != values.size(); ++idx)
	{
		values[idx] = rhs.values[idx]->clone(self);
	}
}

