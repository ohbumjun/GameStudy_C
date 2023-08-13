/*
>> Factory Pattern을 사용하는 이유 << 
- Object creation logic becomes too complicated  
- Constructor is not descriptive 
	> 자기 class 이름써야한다
	- Name mandated by name of containing type
	- Cannot overload with same sets of arguments with different names
- Object creation can be outsourced to 	 
	- seperate function ( factory method )
	- may exist in seperate class ( factory ) 
	- can create hierarchy of factories with Abstract Factory

<Factory>
A Component responsible solely for the wholesale 
creation of objects 
*/

/*

<Summary>
1) A factory method is a static 
method that creates objects
2) A factory can take care of object creation
3) A factory can be external or reside inside
the object as an inner class 
4) Hierchies of factories can be used to create
relate objects

*/
