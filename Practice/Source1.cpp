#include<iostream>

template<typename T>
struct IVisitor;

template<typename T>
struct IAcceptor
{
	virtual void accept(IVisitor*) = 0;
};

template<typename T>



template<typename T>
struct IVisitor
{
	virtual void visit(T& a) = 0;
};


using namespace std;

int main()
{
	return 0;
}