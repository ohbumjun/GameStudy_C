/*
>> Intent -------------------------------------------------
- You have one object that wraps another object ,
and that object wraps another object ... and so on.
- object wrapping other object is a decorator object 

>> Definition ---------------------------------------------
- attaches a "additional" responsibilities to an object
- "dynamically" (its about run-time || compile-time )
- decorators provide flexible alternative
to sub-classing for extending functionality
ex) if you define subclass A,B,C
you only have A,B,C
you cannot somehow compose a new class at a runtime, 
that is a composition of "A&B" or "A&C"

BUT, with decorator A,B,C
you can compose "A&B" , "A&C" ... etc 

>> Diagram Example ---------------------------------------
class Beverage 
// -> Abstract Class :class that cannot be instantiated
// Why use Abstract Class, rather than Interface ? 
// - Instance cannot have implementation!! cannot implement
//   or define specific work in member functions 
// - Abstract Class is used for defining 
//   basic implementations
// - but, according to the cases, it doesn't really matter whether you 
// make Abstract Class or Interface , up to context
{
    getDescription();
    cost();
}

// various types of Beverage exist
class Decaffeine : public Beverage{}
class Espresso   : public Beverage{}
class ColdDrink  : public Beverage{}

But instead of introducing
multiple classes that represents
all of the different combinations 
of beverages ex) mildEspresso, coldmilkdecaffeine

we will introduce "Addon" "Decorator"

class AddonDecorator
{
    getDescription();
}

and "AddonDecorator"
"is a Beverage == behave as if it's 
Beverage" &
"has a Beverage"

There are many different "Addon" decorators
ex)
class CaramelDecorator 
{
    getDescription();
    cost();
}
class SoyDecorator 
{
    getDescription();
    cost();
}

ex) wrapping structure 
--> SoyDecorator(CaramelDecorator(Beverage()))
--> SoyDecorator(CaramelDecorator(Expresso(CaramelDecorator(ColdDrink())))

And another important characteristic is that
"Decorator" refers to what is is "wrapping"
ex) Espresson with Caramel
= CaramelDecorator(Espresso())
-> "CaramelDecorator" refers to "Espresso"

>> Generalized Diagram ----------------------------------

class Component
{
    methodA(){}
    methodB(){}
}
class Concrete Component : public Component
{
    methodA(){}
    methodB(){}
}

class Decorator : public Component
{
    methodA(){}
    methodB(){}
}

// below classes are "additional" responsibilities
// and you can attach below decorators to 
// ConcreteComponent 
class ConcreteDecoratorA : public Decorator
{
    methodA(){}
    methodB(){}
}
class ConcreteDecoratorA : public Decorator
{
    methodA(){}
    methodB(){}
}

>> Another Example ---------------------------------------
(abstract)class Beverage 
{
    public (abstract)cost();
}

class Espresso : Beverage
{
    public int cost(){return 1;}
}

(abstract) class AddOnDecorator : Beverage
{
    public (abstract) cost();
}

class CaramelDecorator : AddOnDecorator
{
    Beverage beverage;
    int cost;
    public CaramelDecorator(Beverage b) 
    {this.beverage = b;}
    public int cost(){
        // cannot return its own cost 
        // it has to decorate sth 
        // ex) another decorator || another beverage 
        return this.beverage.cost + this.cost;
    }
}

------------------------------------------------------
Decorator 정리
--> 객체에 기능을 추가하는 원리이다.

ex)

struct IComponent {}
struct A : IComponent {}
struct B : IComponent {}
struct C : IComponent {}

A에 B 기능, B에 C 기능 추가해가는 원리라고 생각하기.
여기서 중요한 점은, 모두가 같은
interface를 따른다는 점이다 !

그리고 자신의 interface는 지니기도 한다

"is a interface" & "has a interface"

참고. 보통 객체지향에서는 둘중 하나를 선택해야 한다
"상속" vs "포함"

만일, 객체 간의 공통적인 것을"부모"레벨에서 구현가능한 경우 --> "상속"으로 처리한다.
반면, 그렇지 못하다면, "포함"으로 처리한다. 

*/