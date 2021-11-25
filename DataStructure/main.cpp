#include<iostream>

// Real Subject 와 같은 Interface
// Real Subject 에 대한 Ref 유지
// Real Subject 에 대한 소멸, 생성 담당
// - solve a problem that are access related
// - it adds additional behavior just before you make an call to it

class Subject
{
	virtual void request() = 0;
};

class RealSubject : public Subject
{
public:
	void request()
	{
		std::cout << "Real Subject Requset" << std::endl;
	}
};

class Proxy : public Subject
{
	RealSubject* rs;
public :
	Proxy() : rs(new RealSubject){}
	~Proxy() { delete rs; }
	virtual void request() override
	{
		rs->request();
	}
};

int main()
{
	Proxy p;
	p.request();
}