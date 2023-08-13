/*
Factory Method Pattern ----------------------------
- Factory Method
- Abstract Factory

Why do we need concept "factory" ?
- in our code, we use many objects
- we will make class, instantiate them
- question is, "where do we instantiate them" ?

So, The Point here is that, 
when you instantiate an object,
let's "encapsulate that instantiation", so that 
we can "make it uniform" 
across all places 

You can use that "Factory" whenever
you want to instantiate ,
and "Factory" is responsible for
instantiation.

>> Why Is It useful ? ---------------------------------
1) Instantiation might be very complex
ex) in order to instatiate an Animal,
you might need computation

2) Polymorphism
- if you have a factory, that wraps you construction
and that factory is an Instance 
- you can swap that instance with 
other instance at a run-time

ex) 
class A,B,C : public class "I"
--> A,B,C are same type , 

and, at certain point of your program,
you want to instantiate one of those.

upon, entrying the line where you
instantiate, you "don't know"
what you are going to instantiate 
ex) flip a coin

That is, we want to randomly create
either A,B or C

Then, now we need some logic of deciding
which one we want to instantiate 

Depending on your scenario, 
Depending on what you are building , 
you might have different business logic
that determines what thing you
want to create .
What thing you want to instantiate .

And That logic can be encapsulate,
be coded into,
"Factory"

"Factory" is responsible for holding
the business logic 
of creation of some particular type ,
of creation of some particular way 
of creating instance
ex) above case "class I"

If there are 2 factories,
they bot create "class I", 
but in different way 

or they create "different sub types"

----------------------------------------------------
>> Example
class Dog,Cat,Duck : class Animal

class AnimalFactory {
    virtual Animal createAnimal()=0;
}
class RandomAnimalFactory : AnimalFactory {
    --> randomly creates Animal
    Animal createAnimal(){}
}

// other factory
class BalancedAnimalFactory  : AnimalFactory
{
    // this factory has "state"
    // by saving previous animal instances it created
    // it balances the creation of different types
    // of animals
    Animal createAnimal(){}
}

>> Definition ---------------------------------------
--> Factory Pattern, defines an interface 
for creating an object.
- In the end, you want an "object"
- you don't want to necessarily know
"how" you make that object 
"why" ~
"what" parameters are needed to construct an object

--> But, lets subclasses decide
which class to instantiate

--> Factory lets Classes defer instantiation
to Sub Classes
ex) AnimalFactory --> defer to Random,Balanced ~ 

>> Diagram ------------------------------------------
"class Product{}" <<- created by "class Creator{}"
class ConcreteProdcut : class Product{}

// class Creator{} ? ex) AnimalFactory
// class ConcreteCreator{} ? ex) RandomAnimalFactory
class ConcreteCreator : class Creator{}


>> Another Example ----------------------------------
ex) Game 
we have many characters ... 

we have factory --> create "Ork", create "Dragon"
or you can have randomize "size","location" of character..

Level1 --> A Factory
Level2 --> B Factory ...

A,B,C ... --> all same Interface !
ex) pass parameter to "Factory",
and according to "param",
create different Characters. etc.
*/
/*
한국어 설명
--> 추상 팩토리 패턴에서는, "자식"이 존재하고,
"핵심"은 , 객체 생성 부분을 "자식"에 위임하는 것이다.
즉, "상속"을 활용하는 것이다

즉, 객체 생성을 위한 인터페이스만 정의해두고
실제 객체 생성은, 자식 클래스에게 위임하는 원리이다. 

class IEdit{}
class XPEdit  : IEdit{}
class MacEdit : IEdit{}

class IButton{}
class XPButton : IButton{}
class MacButton : IButton{}

class IFactory {}
class XPFactory : IFactory
{ 
    // "has-a" relationship
    class IEdit;
    class IButton;
}
class MacFactory : IFactory
{
    class IEdit;
    class IButton;
}

*/