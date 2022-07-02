#include <list>
#include <iostream>

using namespace std;

class IObserver;

// Observable
class Subject {
private :
	int value;
	list<IObserver*> views;

public :
	Subject() {};
	void addObs(IObserver* obs) { views.emplace_back(obs); }
	int getVal() const { return value; }
	void setVal(int val)
	{
		value = val;
		notify();
	}
	virtual void notify();
};

class IObserver {
private :
	Subject* sub;
	int denom;
public :
	IObserver(Subject* s, int val) : sub(s), denom(val)
{
		sub->addObs(this);
}

	virtual void update() = 0;

protected:
	Subject* getSubject() { return sub; }
	int getDivisor() { return denom; }
};

void Subject::notify()
{
	auto iter = views.begin();
	auto iterEnd = views.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->update();
	}
}

class DivObserver : public IObserver {
public :
	DivObserver(Subject* mod, int div) : IObserver(mod, div){}
	void update() override
{
		int v = getSubject()->getVal();
		int d = getDivisor();
		cout << v << " div " << d << "is " << v / d << endl;
}
};