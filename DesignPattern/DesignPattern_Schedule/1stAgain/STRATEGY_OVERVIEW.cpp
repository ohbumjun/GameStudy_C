/*
using composition rather than inheritance !

>> Definition -----------------------------
-> Strategy pattern defines a family of algorithms
encapsulated each one 
and makes them interchangable
- when you use strategy pattern, you use 
family of algorithms
- you have bunch of algorithms and each of them
are interchangable
- you can plug in, out
ex) sometimes you use algorithm A, or use
algorithm B

-> Strategy lets the algorithm
vary independently 
from clients that use it 
- we decoupled the algorithm from the 
one using the algorithm
- we can vary between algorithm A,B,C and
whoever is using algorithm A,B,C is referred to as 
"client"
- "vary" ? == if you want to change contents
of the algorithm, what algorithm does // you don't
have to change the cilent at the same time

ex) implementation of collection --> list
if "list" had sorting algorithm built into it, 
you can never change the sorting algorithm,
but, if you are using "strategy pattern",
you can inject  "sorting strategy",
and "sorting strategy" can vary independently
from the list 

>> Example ---------------------------------------
class Duck // --> super class -> othe class have
to inherti the Duck class
{
    quack()
    display()
    fly()
}

// sub classes are responsible for 
// implementing "their own" version of the
// "display" methods
// on the other hand, 
// "quack" method is shared among classes
// we are using 'inheritance' for code re-use
class WildDuck : public Duck
{
    display()
}
class CityDuck : public Duck
{
    display()
}
class RubberDuck : public Duck
{
    display()
    // RubberDuck cannot fly,
    // but, since it is inheritance from "Duck"
    // it still need to "fly"....
    // That is, the problem here is that
    // in "Inheritance" hierarchy, 
    // some class have to inherit method that
    // does not need due to the fact that
    // they are just children of certain super class
}

How can we solve it ?
we have to extract "methods" from the "duck" class

&& let's say
"WildDuck","CityDuck" is a client 

&& we are creating Interface, 
&& in other words, "abstract strategy"

struct IQuackBehavior
{
    virtual void quack()=0;
}
struct IFlyBehavior
{
    virtual void fly()=0;
}
struct IDisplayBehavior
{
    virtual void display()=0;
}

Now, when you make instance of "Duck",
you have to apply above 3 interfaces
according to the type of "Duck" 
you want to create 

CityDuck, WildDuck both have same "quack,fly" behavior,
&& we achieved that using "inheritance"

Now, how do we apply "different" "quack,fly" beviour ?

// Concrete Behaiour
// OR
// Concrete Strategy
class SimpleQuack : public IQuackBehavior
{quack()}
class NoQuack : public IQuackBehavior
{quack()}
class SimpleFly : public IFlyBehavior
{fly()}
class JetFly : public IFlyBehavior
{fly()}
class DisplayGraphically : public IDisplayBehavior
{display()}
class DisplayText : public IDisplayBehavior
{display()}

these are behavior,strategies.
we can put them in places && 
use them to compose new things that
behave differently

Now if we see above processes, 
we can see that we don't need
"Inheritance structure"

WildDuck,CityDuck does not have to 
inherit "Duck".

we can simply inject concrete strategy
into "WildDuck","CityDuck",
and compose object that acts as
"WildDuck" & "CityDuck"

Given combinations of different strategies, 
we can make different type of "Duck" we 
want to create 

>> Code ----------------------------------------
// Duck --> Client
// -- have to reference to class IBehavior
// -- have to "have" "class IBhavior"
// -- inject "class concrete Behavior : public IBehavior "
class Duck{
    IFlyBehavior m_fb
    IQuackBehavior m_qb
    IDisplayBehaivor m_db
    // you pass in behavior
    public :
        Duck(FlyBehavior fb,
            IQuackBehavior qb,
            IDisplayBehavior db)
            {
                m_fb = fb;
                m_qb = qb;
                m_db = db;
            }
    // you execute that behavior
    void Fly(){m_fb.fly()}
    void Quack{m_qb.quack()}
    void Display{m_db.display();}
}


------------------------------------------------------
정리 : 행위를 클래스로 캡슐화하여
동적으로 행위를 
자유롭게 바꿀 수 있게 해주는 패턴 

*/