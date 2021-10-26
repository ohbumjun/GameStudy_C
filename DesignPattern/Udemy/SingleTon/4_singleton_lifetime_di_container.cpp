
/*
Instead of defining your singleton 
explicitly by hiding the constructors and
having the get method, 

what you can do is you can
rely on inversion of control container
used in a typical dependency injection framework

to basically make a singleton for you 
------------------------------------------------
Because what happens in these containers is 
you allow yourself to configure different components
with different lifetimes

------------------------------------------------
Singleton is nothing more than a 
specification of lifetime ?, 
specifically requires that only one
component exist at any one point in time
------------------------------------------------
*/

// using dependency injection framework
struct IFoo
{
	virtual string name() = 0;
};

struct Foo : IFoo
{
	static int id;
	Foo(){++id;}
	string name() override {
		return "foo " + lexical_cast<string>(id) ;
	}
}

int Foo:id = 0;

struct Bar
{
	// shared_ptr : 소유권을 여러 군데에서 공유한다  
	// 혹은, auto ptr1 = std::make_shared<IFoo>(); 
	std::shared_ptr<IFoo> foo;
};

int main()
{
	return 0;
}


