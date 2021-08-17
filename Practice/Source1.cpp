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
		cout << "parent pure1" << endl;
	}
};

class CChild : public CParent
{
public :
	CChild(){}
	~CChild(){}
private :
	void Outputpure(){}
	void Outputpure1()
	{
		cout << "child pure1" << endl;
	}
};


int main()
{
	CParent* child = new CChild;
	child->Outputpure1();
	return 0;
}