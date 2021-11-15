#include <iostream>

// Proxy Pattern¿« ªÁ∑ 
// Shared_ptr, Unique_ptr

using namespace std;

class Subject
{
public :
	virtual void request() = 0;
	virtual ~Subject(){}
};

class RealSubject : public Subject
{
public :
	virtual void request() override
	{
		cout <<  "real Sub Requset" << endl;
	}
};

class Proxy : public Subject
{
private :
	Subject* realSubject;
public :
	Proxy() : realSubject(new RealSubject()){}
	~Proxy() { delete realSubject; }
	virtual void request()
	{
		realSubject->request();
	}
};

int main()
{
	Proxy p;
	p.request();
}