#pragma once

#ifndef DYNAOBJECT_H
#define DYNAOBJECT_H

#include "Object.h"
#include "Value.h"
#include <vector>

// 실제  Object 에 대한 Implementation
// Object 클래스를 base 클래스로 정의하고
// DyanObject 를 real MOP class 인터페이스를 닮은
// 형태로 만들 것이다. 
class DynaObject : public Object
{
public:
	DynaObject(ClassDef const*);
	DynaObject(DynaObject const&);
	~DynaObject();

#if defined(__BORLANDC__)
	virtual Value getValue(std::string const& name) const { return Object::getValue(name); }
	virtual void setValue(std::string const& name, Value const& v) { Object::setValue(name, v); }
#else
	using Object::getValue;
	using Object::setValue;
#endif
	virtual Value getValue(size_t idx) const;
	virtual void setValue(size_t idx, Value const&);
	virtual void replaceValue(size_t idx, BaseValue const&);
	void replaceValue(std::string const& name, BaseValue const&);

	Object* privObject() const;
	void setPrivate(Object*);

protected:
	virtual BaseValue* getBaseValue(size_t idx);
	virtual BaseValue const* getBaseValue(size_t idx) const;
	void init(DynaObject* self);
	void copy(DynaObject* self, DynaObject const& rhs);

private:
	typedef std::vector<BaseValue*> ValueContainer;

	DynaObject& operator=(DynaObject const&); // forbidden
	ValueContainer values;
	Object* privateObj;
};

#endif // DYNAOBJECT_H
