/*
1) Liskov's Substitution Principle
>> Definition -----------------------------------
Subtitutability is a principle in OOP
if S is a sub-type of T , 
then object of "type T" may be replace 
with object of "type S"

That is , 
object of "type T" may be subtituted with
any object of "subtype S"

---------------------------------------------------

SubType Requirement

Let '*(x)' is property provable about object 'X' of
type T.
- "*" : property, of instance of T class

Then '*(y)' should be true for objects 'Y' of
type S, where S is a subtype of T.
- That is, instance of S class, which is a subtype
of T, should also have property "*"

In other words , 
if 'T' can do 'A', 
'S' must be able to do 'A' also !

It tell us when we should use Inheritance, 
ans when we should not use the Inheritance. 

*/