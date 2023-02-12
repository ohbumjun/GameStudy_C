#pragma once

#ifndef VALUE_H
#define VALUE_H

#include <string>

class Object;
class Value;

// 흔히 생각하는  Value 를 Manage 하는 Interface 
// ex) set, get 함수, clone 함수
class BaseValue
{
public:
	virtual ~BaseValue() {}
	virtual BaseValue* clone(Object*) const = 0;

	virtual Value getFinal() const = 0;
	virtual void set(Value const& v) = 0;

protected:
	BaseValue() {}
	BaseValue(BaseValue const&) {}
	BaseValue& operator=(BaseValue const&) 
	{ 
		return *this; 
	}

private:
	// Type info
};

class ComputingValue : public BaseValue
{
public:
	ComputingValue(Object* o);

protected:
	ComputingValue(ComputingValue const&);

	Object* const obj;
};

class FinalValue : public BaseValue
{
public:
	virtual BaseValue* clone(Object*) const;
	virtual FinalValue* clone() const = 0;

	virtual std::string asString() const = 0;
	virtual Value getFinal() const;

protected:
	FinalValue();
	FinalValue(FinalValue const&);
	FinalValue& operator=(FinalValue const&);
};

template <typename PlainT>
class TypedValue : public FinalValue
{
public:

	virtual operator PlainT() const = 0;

protected:
	TypedValue() {}
	TypedValue(TypedValue const& rhs) : FinalValue(rhs) {}
	TypedValue& operator=(TypedValue const& rhs)
	{
		FinalValue::operator =(rhs);

		return *this;
	}
};

// Value handle
// - handle 로서 body 를 들고 있는 형태가 된다. 
// - 하지만 동시에 실제 value 로서도 동작해야 한다.
//   그렇기 때문에 기본 생성자, 복사생성자를 두는 것.
class Value	
{
public:
	Value(FinalValue const&);
	Value(Value const&);
	explicit Value(FinalValue* = 0);

	~Value();

	Value& operator=(const Value&);

	template <typename PlainT>
#if defined(_MSC_VER) || defined (__GNUC__)
	// version for VC++ and GNU :-{
	PlainT get(PlainT* vcppbug) const
#else
	// version for standard compiler :-}
	PlainT get() const
#endif
	{
		if (v)
		{
			TypedValue<PlainT> const& tv
				= dynamic_cast<TypedValue<PlainT> const&>(*v);
			return tv;	// uses conversion operator
		}
		else
		{
			return PlainT();
		}
	}

	std::string asString() const;

private:
	FinalValue* v;
};


#endif // VALUE_H
