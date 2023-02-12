#pragma once

#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
#include "MemPtr.h"
#include "RealValue.h"
#include "Object.h"
#include "Flag.h"


class Product
{
	typedef MemPtr<Product, FinalValue> MemberPtr;
	
public:
	Product()
		: productNo(0), weight(0)
	{
		// create a unique product number
	}

	virtual ~Product() {}

	virtual std::string getName() const = 0;
	virtual double getPrice() const = 0;
	virtual double getWeight() const = 0;

	static size_t ownAttribCount()
	{
		return 2;
	}
	static Attribute* ownAttribBegin()
	{
		static Attribute a[] = { Attribute("Product No", TypeT::intT),
								Attribute("Weight", TypeT::doubleT) };
		return a;
	}
	static Attribute* ownAttribEnd()
	{
		return ownAttribBegin() + ownAttribCount();
	}

	// ownAttribBegin ���� ���̴�  
	// ownAttribBegin �� ������ attribute ������ �˷��شٸ�
	// memberBegin �� Base Class Member ������ �˷��ش�.
	// �̷��� ������ �����ִ� ������ �����ϱ� ?
	// 1) Object Level ������ ��� data member ����
	//    �ϳ��� Object �� build �ȴ�.
	// 2) class level ������ base class �� �ٸ� entity �̰�
	//   ���ÿ� meta object protocol �� ���� common base
	//   class �� �Ǿ�� �Ѵ�.
	static MemberPtr* memberBegin()
	{
		static MemberPtr m[] = { MemberPtr(&Product::productNo),
								MemberPtr(&Product::weight) };

		return m;
	}
	static MemberPtr* memberEnd()
	{
		return memberBegin() + ownAttribCount();
	}

	RealValue<int> productNo;
	RealValue<double> weight;

protected:
	Product(const Product& rhs)
		: productNo(rhs.productNo),
		weight(rhs.weight)
	{}
};

// DynaProduct acts as interface for ShoppingCart for dynamic
// MOP classes derived from Product
class DynaProduct : public Product
{
public:
	DynaProduct(Object const* o) : obj(o) {}

	virtual std::string getName() const
	{
		Value v = obj->getValue("Name");
#if defined(_MSC_VER) || defined (__GNUC__)
		return v.get((std::string*)0);
#else
		return v.get<std::string>();
#endif
	}

	virtual double getPrice() const
	{
		Value v = obj->getValue("Price");
#if defined(_MSC_VER) || defined (__GNUC__)
		return v.get((double*)0);
#else
		return v.get<double>();
#endif
	}

	virtual double getWeight() const
	{
		Value v = obj->getValue("Weight");
#if defined(_MSC_VER) || defined (__GNUC__)
		return v.get((double*)0);
#else
		return v.get<double>();
#endif
	}

private:
	Object const* const obj;
};

// DynaProduct �� ��Object �� ������ ������ ���� ?

#endif // PRODUCT_H
