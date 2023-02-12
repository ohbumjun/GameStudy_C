#pragma once

#ifndef REAL_VALUE_H_
#define REAL_VALUE_H_

// BaseValue 라는 interface 에 대한 
// implementation 을 세팅하기.
// - 서로 다른 형태의 value 가 존재하기 때문에
//   template 을 사용할 것이다.

#include <string>
#include <sstream>
#include "Value.h"

template <typename PlainT>
class RealValue : public TypedValue<PlainT>
{
public:
	RealValue(PlainT v)
		: val(v)
	{}

	RealValue() : val(0) {}

	FinalValue* clone() const
	{
		return new RealValue(*this);
	}

	std::string asString() const
	{
		std::ostringstream os;
		os << val;
		return os.str();
	}

	operator PlainT() const	// conversion to plain type
	{
		return val;
	}

	void set(Value const& v)
	{
#if defined(_MSC_VER) || defined (__GNUC__)
		val = v.get(&val);
#else
		val = v.get<PlainT>();
#endif
	}

private:
	PlainT val;
};


template <>
class RealValue<std::string> : public TypedValue<std::string>, public std::string
{
public:
	RealValue(std::string const& s)
		: std::basic_string<char>(s)
	{}

	RealValue(char const* s)
		: std::basic_string<char>(s)
	{}

	RealValue() {}

	FinalValue* clone() const
	{
		return new RealValue(*this);
	}

	std::string asString() const
	{
#ifndef __BORLANDC__
		return *this;
#else
		std::string const& ref = *this;
		return ref;
#endif
	}

	// this conversion will only be used through the virtual function call
	operator std::string() const	// conversion to plain type
	{
#ifndef __BORLANDC__
		return *this;
#else
		std::string const& ref = *this;
		return ref;
#endif
	}

	void set(Value const& v)
	{
#if defined(_MSC_VER) || defined (__GNUC__)
		*this = v.get((std::string*)0);
#else
		* this = v.get<std::string>();
#endif
	}
};

class Object;

template <>
inline std::string RealValue<Object*>::asString() const
{
	//	return obj->instanceOf()->className() + " *";
	return "Object *";
}


#endif // REAL_VALUE_H_
