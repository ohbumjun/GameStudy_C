#include<iostream>

using namespace std;

struct Subject
{
	virtual void request() = 0;
	virtual ~Subject(){}
};

struct RealSubject : public Subject
{
	virtual void request() { cout << "RealSubject.request()" << endl; }
};

class Proxy : public Subject
{
	Subject* realSubject;
public :
	Proxy():realSubject(new RealSubject){}
	virtual void request() override { realSubject->request(); }
};

int main()
{
	Proxy p;
	p.request();
	return 0;
}