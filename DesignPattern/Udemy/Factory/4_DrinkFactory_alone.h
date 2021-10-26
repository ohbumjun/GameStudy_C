#pragma once

#include "HotDrink.hpp"
#include "HotDrink.hpp"
#include<map>

class DrinkFactory
{
	map<string, unique_ptr<HotDrinkFactory>> hot_factories;
	public :
		DrinkFactory(){
							// Factory ����ü�� ���� unique ptr ��ȯ  
			hot_factories["coffee"] = make_unique<CoffeeFactory>();
			hot_factories["tea"] = make_unique<TeaFactory>();
		}
	unique_ptr<HotDrink> make_drink(const string& name)
	{
		// drink : �ش� ���� ����ü �� ���� unique_ptr 
		auto drink = hot_factories[name]->make();
		drink->prepare(200);
		return drink;
	}
}
