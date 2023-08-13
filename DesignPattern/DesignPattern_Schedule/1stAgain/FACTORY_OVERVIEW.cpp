/*
What is Factory Design Pattern ?

- it creates object for you, rather than initiating
object directly
- FDP is also known as "Virrual Constructor"


How to Implement
- Define an interface or abstract class
for creating an object,
but let subclasses decide which class to initiate 

ex) class Toy{}
ex) class Car,Bike,Plane : public Toy{}
ex) class ToyFactory {Toy* create(){}} --> crate Car,Bike,Plane 



*/