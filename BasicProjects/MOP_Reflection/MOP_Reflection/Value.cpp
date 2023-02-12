#include "Value.h"

ComputingValue::ComputingValue(Object* o)
	: obj(o)
{}

ComputingValue::ComputingValue(ComputingValue const& rhs)
	: BaseValue(rhs), obj(rhs.obj)
{}

BaseValue* FinalValue::clone(Object*) const
{
	// override 한 파생클래스에서의 clone 함수 호출
	return clone();
}

std::string FinalValue::asString() const
{
	return std::string();
}

Value FinalValue::getFinal() const
{
	return *this;
}

FinalValue::FinalValue()
{
}

FinalValue::FinalValue(FinalValue const& rhs)
	: BaseValue(rhs)
{}

FinalValue& FinalValue::operator=(FinalValue const& rhs)
{
	// TODO: 여기에 return 문을 삽입합니다.
	BaseValue::operator =(rhs);

	return *this;
}

Value::Value(FinalValue const& fv)
	: v(fv.clone())
{}

Value::Value(Value const& rhs)
	: v(rhs.v ? rhs.v->clone() : 0)
{}

Value::Value(FinalValue* fv)
	: v(fv)
{}

Value::~Value()
{
	delete v;
}

Value& Value::operator=(const Value& rhs)
{
	// TODO: 여기에 return 문을 삽입합니다.
	 // this is not a typical pimpl assignment, but a set()
	if (v)
	{
		if (rhs.v)
		{ // fine, all v's exist
			v->set(rhs);
		}
		else
		{ // the other v doesn't exist, so we must delete our own :-(
			FinalValue* old = v;
			v = 0;
			delete old;
		}
	}
	else
	{ // we don't have a v, so just copy the other
		v = (rhs.v ? rhs.v->clone() : 0);
	}

	return *this;
}

std::string Value::asString() const
{
	if (v)
	{
		return v->asString();
	}
	else
	{
		return std::string();
	}
}
