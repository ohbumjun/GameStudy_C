#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "ClassDef.h"
#include <vector>

class Value;
class BaseValue;

// Class 에 정의된 Attribute에 대한 value 들고 있기
// Object 의 Interface 클래스
class Object
{
public:
	Object(ClassDef const* class_);
	virtual ~Object() {}

	virtual Object* clone() const = 0;

	ClassDef const* instanceOf() const;

	virtual Value getValue(size_t idx) const = 0;
	virtual Value getValue(std::string const& name) const;
	virtual void setValue(size_t idx, Value const&) = 0;
	virtual void setValue(std::string const& name, Value const&);

protected:
	Object(const Object& rhs) : myClass(rhs.myClass) {}
	virtual BaseValue* getBaseValue(size_t idx) = 0;
	virtual BaseValue const* getBaseValue(size_t idx) const = 0;
	
	
	// the static function is present to avoid getBaseValue to be public
	static BaseValue* getBaseValue(Object*, size_t idx);
	static BaseValue const* getBaseValue(Object const*, size_t idx);

private:
	ClassDef const* const myClass;
};

#endif // OBJECT_H
