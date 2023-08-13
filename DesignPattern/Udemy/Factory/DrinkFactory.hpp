#pragma once
#include <string>
#include "HotDrink.h"
#include "TeaFactory.h"
#include "CoffeeFactory.h"
#include <map>
#include <functional>

// more functional approach 

struct HotDrink;

class DrinkFactory
{
  map<string, unique_ptr<HotDrinkFactory>> hot_factories;
public:
  DrinkFactory()
  {
    hot_factories["coffee"] = make_unique<CoffeeFactory>();
    hot_factories["tea"] = make_unique<TeaFactory>();
  }

  unique_ptr<HotDrink> make_drink(const string& name)
  {
    auto drink = hot_factories[name]->make();
    drink->prepare(200); // oops!
    return drink;
  }
};

// let's make simple lambda functions describing
// how a particular drink has made
// let's provide volume of drink we actually 
// want to make 
class DrinkWithVolumeFactory
{
  // function<unique_ptr<HotDrink>()>
  // returns 'function'
  // () : no parameter 
  map<string, function<unique_ptr<HotDrink>()>> factories;
public:
	// we're going to map to a function
	// and this function will define how drink is actually
	// made 
	
	// so instead of using a factory ,
	// we are going to basically use an anonymouse function
	// Below : Constructor
	// 
  DrinkWithVolumeFactory()
  {
  	// provide lambda
  	// [] : 람다  
  	// 아래와 같이 람다함수를 제공하는 것 자체가 factory pattern
	//  
    factories["tea"] = [] {
      auto tea = make_unique<Tea>();
      tea->prepare(200);
      return tea;
    };
  }

  unique_ptr<HotDrink> make_drink(const string& name);
};

// functional factory
inline unique_ptr<HotDrink> DrinkWithVolumeFactory::make_drink(const string& name)
{
  return factories[name]();
}

