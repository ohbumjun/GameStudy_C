/*
< Motivation >

- For some components it only makes sense to have
one in the system
	ex) DB repository
	ex) Object Factory
- Constructor call is expensive
	- we only do it once
	- we provide everyone with the same instance
- Want to prevent anyone creating additional copies

< Definition >
Singleton
- A component which is instantiated only once

*/
