#include<iostream>

using namespace std;

class CParent
{
public :
	CParent(){}
	~CParent(){}
public :
	virtual void Outputpure() = 0;
	virtual void Outputpure1() = 0
	{

	}
};

class CChild : public CParent
{
public :
	CChild(){}
	~CChild(){}
public :
	virtual void Outputpure() override
	{

	}
	virtual void Outputpure1() override
	{
		cout << "child hello" << endl;
	}
};

int main()
{
	CParent* child = new CChild;
	child->Outputpure1();
	return 0;
}