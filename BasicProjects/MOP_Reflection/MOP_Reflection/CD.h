#pragma once

#ifndef CD_H
#define CD_H

#include <string>
#include "Product.h"
#include "Flag.h"

class Cd : public Product
{
	typedef MemPtr<Cd, FinalValue> MemberPtr;
	//	typedef BaseValue Cd::* MemberPtr;
public:
	Cd(const std::string& composer_,
		const std::string& title_,
		const std::string& musician_,
		double price_)
		: composer(composer_),
		title(title_),
		musician(musician_),
		price(price_)
	{}
	Cd() {}

	std::string getName() const
	{
		std::string name;

		name = composer + ": " + title;

		return name.substr(0, 40);
	}

	double getPrice() const
	{
		return price;
	}

	double getWeight() const
	{
		return 60;
	}

	static size_t ownAttribCount()
	{
		return 4;
	}
	static Attribute* ownAttribBegin()
	{
		static Attribute a[] = { Attribute("Composer", TypeT::stringT),
								Attribute("Title", TypeT::stringT),
								Attribute("Musician", TypeT::stringT),
								Attribute("Price", TypeT::doubleT)
		};
		return a;
	}
	static Attribute* ownAttribEnd()
	{
		return ownAttribBegin() + ownAttribCount();
	}
	static MemberPtr* memberBegin()
	{
		static MemberPtr m[] = { MemberPtr(&Cd::productNo),
								// MemberPtr(&Product::weight),
								MemberPtr(&Product::weight),
								MemberPtr(&Cd::composer),
								MemberPtr(&Cd::title),
								MemberPtr(&Cd::musician),
								MemberPtr(&Cd::price)
		};

		return m;
	}
	static MemberPtr* memberEnd()
	{
		return memberBegin() + 6;
	}

private:
	RealValue<std::string> composer, title, musician;
	RealValue<double> price;
};
#endif // CD_H
