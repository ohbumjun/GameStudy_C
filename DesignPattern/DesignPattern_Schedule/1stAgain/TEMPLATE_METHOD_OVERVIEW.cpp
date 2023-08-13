/*
turn blueprint, template, class
into
acutal instanciation ,
you can take the blueprint and
make sth out of blueprint
---------------------------------
"template" might have
multiple different
instantiation 

More specifically
1) you have "template"
2) you have "subclasses" to create
specialization , concretion
of that particular template 

--------------------------------
ex) we have an algorithm that
produces "poster"

--> case 1) band poster
--> case 2) politician poster
--> case 3) ~~

along above cases,
when you make a "poster",
there are some portion that varies, ex) actual picture
other portions that does not vary 
ex) shape of picture(all round shape)

Likewise, 
there are some parts in algorithm that "vary"
and others that "don't vary"

>> Definition ----------------------------------
---> Template method pattern defines skeleton
of algorithm ,
defering some steps , 
to sub classes
- some portion of algoritm may not vary across
different instantiation.
-- 'In'variant, not changing --> in "base class"
-- changing --> in "sub class"
ex) subclass for "policitian"
ex) subclass for "band"

---> Template method lets subclasses 
redefine certain steps of algorithm, 
without changing the algorithm structure. 

------------------------------------------------------
>> Diagram

class AbstractClass
{
    // has method called "template method"
    virtual TemplateMethod(){
        ...
        Operation_1();
        ...
        Operation_2();
        ...

        // so what does "TemplateMethod" do ?
        // it has "implementation", "algorithm"
        // but !
        // there are pieces of algorithm 
        // that "can" be defined in 
        // TemplateMethod
        // ------------------------------------
        // other pieces of algorithm 
        // that "can't" be defined in 
        // TemplateMethod
        // on the Level of "AbstractClass" 

        // these "pieces" are deferred to 
        // "sub classes" 
        // defer implementation of "pieces"
        // that cannot be defined to "subclass"
    }

    // No implementation in "Abstract class"
    // but "must" have implementation in "Concrete class"
    virutal Operation_1()=0;
    virtual Operation_2()=0;

}

- you "can never" instantiate "AbstractClass"
ex) new AbstractClass (x)

class ConcreteClass : public AbstractClass
{
    Operation_1(){}
    Operation_2(){}
}
- you "have to" instantiate "ConcreteClass"


Template Method pattern allows you to change the
behavior of "AbstractClass"
by introducing new "ConcreteClass"

-----------------------------------------------------
What is the difference between "Strategy pattern"
and "Template Method Pattern" ?

1) Assumption about the Future

>> Strategy Pattern
- it defines "family" of algorithm.
- ex) class Run{} (Interface)
- ex) --> class FastRun{}, class SlowRun{}, class NormalRun{} ... 
- they can be used interchangebly. 

The massive power of "Strategy Pattern" is a 
"Dependency Injection"

Every Algorithm does a "single thing".
there is a only one method in each class.
That is it.

And they all follow one interface, 
which specifies only single method.

So, there are very few assumptions about the future.

Whereas !! 
Let's see "Template Method Pattern" ,
difference between "AbstractClass" &
"ConcreteClass" is existence of 
"Template Method".

So we are making assumption about the future.
Assumption about how the structure of the
algorithm will remain invariant overtime. 

음..쉽게 말해서 variant 부분.
변하는 부분에 있어서, 더 다양한 로직이 들어갈
가능성이 많다는 의미로 해석할 수 있다. 

2) Inheritance vs Composition

If you see "Template Method Pattern" .
Let's assume "TemplateMethod" in Abstract Class
will remain "Invariant",
Entire Structure of Abstract Class
will remain "Invariant".

virtual TemplateMethod(){
    ...             // Invariant
    Operation_1();  // Variant
    ...             // Invariant
    Operation_2();  // Variant
    ...             // Invariant
}

Actually, we can compose above "TemplateMethod"
with "Strategy" pattern

We should prefer Composition over Inheritance.

Strategy --> Composition --> prioritized !
Template Method --> Inheritance 

BUT !
if we are certain about the structure
of "TemplateMethod".

ex) know which part is "Invariant" or "Variant" ?
use "TemplateMethod" !!

---------------------------------------------------
ex)
class Record
{
    void save()
    {
        this.beforeSave();
        db.query // invariant
        if(ok){this.afterSave();}
        else{this.failedSave();}
    }
    virtual beforeSave()=0;
    virtual afterSave()=0;
    virtual failedSave()=0;
}

class User : public Record
{
    string username;
    void beforeSave(){};
    void afterSave(){};
    void failedSave(){};
}

class Post : public Record
{
    void beforeSave(){};
    void afterSave(){};
    void failedSave(){};
}


*/