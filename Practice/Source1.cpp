#include<iostream>

using namespace std;

class CParent
{
public:
	CParent()
	{
	}
	~CParent()
	{
	}
public:
	virtual void Outputpure1() = 0;
	virtual void Outputpure2() = 0
	{
		cout << "parent out 2" << endl;
	}
};

class CChild : public CParent
{
public:
	CChild()
	{

	}
	~CChild()
	{

	}
	virtual void Outputpure1() override
	{
	}
	virtual void Outputpure2() override
	{
		cout << "child out 2" << endl;
	}
};

int main()
{
	CChild* ch = new CChild;
	ch->Outputpure2();
	return 0;
}