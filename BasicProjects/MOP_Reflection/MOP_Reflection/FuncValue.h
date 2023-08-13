#ifndef FUNC_VALUE_H_
#define FUNC_VALUE_H_

#include <string>
#include "Value.h"

class Object;

template <typename ReturnPlainT>
class FuncValue : public ComputingValue
{
	typedef ReturnPlainT(*FunctionT)(Object*);
public:
	FuncValue(Object* o, FunctionT getFunction)
		: ComputingValue(o), getF(getFunction)
	{}

	BaseValue* clone(Object* o) const
	{
		return new FuncValue(o, getF);
	}

	Value getFinal() const
	{
		// RealValue(PlainT v) 를 호출할 거이다.
		return RealValue<ReturnPlainT>(getF(obj));
	}

	void set(Value const& v)
	{
		throw "not supported";
	}

private:
	FunctionT getF;
};

#endif // FUNC_VALUE_H_
