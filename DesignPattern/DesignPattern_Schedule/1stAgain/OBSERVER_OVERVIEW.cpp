/*
>> Intent ----------------------------------------------
- you have multiple state
one of them changes its state
and other state(-> call it 'subscriber') 
wants to know about above one that changed the state 
ex) weather station --> keeps track of change in weather

- "push" vs "poll"
it is basically moving from "poll" architecture
to "push" architecture 

what is 'poll' architecture ?
--> multiple 'subscriber' asking 'have you changed the state' ?
we are asking for the data before we have new data 
--> per sec, or per mil sec, continuously asking 

what is 'push' architecture ?
- asking like 'poll' architecture , is a bad idea 
- rather, one that changed, is responsible for telling
all the subscribers when it has changed
- "push", "I have changed!" only when it changed
- but, before "push", it has to know who are the
'subscribers', therefore 'subscribers' need to register
to this object that changes 

'subscribers' are "Observers"
'changing state' is "Observable" or referred as 'subject'

>> Defintion -------------------------------------------
"" observer pattern defines 'one to many' dependency
between objects 
so that when one object changes its state ,
all of its dependencies are notified ,
updated automatically "" 

>> Diagram --------------------------------------------

>> IObservable --> Interface ( Not Inheritance  !)
(thing that is observed)
- 'has' 0 to many "IObserver"s 
- has to keep track of "who wants to know" thing changed
- have to add "instance" of "IObserver" to 
instance of "IObservable"



*/
class IObservable
{
    virtual void Add(IObserver obs) = 0;
    virtual void Remove(IObserver obs) = 0;
    virtual void Notify() = 0; // call "update methods" of all observer instances
}

class ConcreteObservable : public IObservable
{
    void Add(IObserver obs)
    {
    }
    void Remove(IObserver obs)
    {
    }
    void Notify()
    {
    }
    void getState()
    {
    }
}

// IObserver --> Interface
// (thing that observes)
class IObserver
{
    virtual void update() = 0;
}

class ConcreteObserver : public IObserver
{
    // have to have reference to "ConcreteObservable"
    // pass it by constructor
    // by doing as above processes , we can
    // prevent passing parameter to "update" method
    virtual void update()
    {
    }
}

// Example ---------------------------------------------
#include <iostream>
#include <vector>
class WeatherStation : public IObservable
{
    std::vector<IObserver *> observers;
    int temperature = 0;
    void Add(IObserver *obs)
    {
        this.observers.push_back(obs); //
    }
    void Remove(IObserver obs)
    {
    }
    void Notify()
    {
        for (int i = 0; i < observers.size(); i++)
            observers[i].Update();
    }
    int getTemperature()
    {
        return this.temperature;
    }
}

class PhoneDisplay : public IObserver
{
    WeatherStation station;

public:
    PhoneDisplay(WeatherStation &st) { this.station = st; }
    void update()
    {
        this.station.getTemperature();
    }
    void display()
    {
    }
}

class WindowDisplay : public IObserver
{
    WeatherStation station;

public:
    WindowDisplay(WeatherStation &st) { this.station = st; }
    void update()
    {
        this.station.getTemperature();
    }
    void display()
    {
    }
}

int
main()
{
    WeatherStation station = new WeatherStation();
    PhoneDisplay display = new PhoneDisplay();
    station.add(&display);
}