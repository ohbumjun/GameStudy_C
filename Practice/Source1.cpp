#include<vector>
#include<iostream>

using namespace std;

class IObserver;
class Subject
{
private :
	int value;
	vector<IObserver*> views;
public :
	Subject(){}
	void addObs(IObserver* obs) { views.emplace_back(obs); }
	int getVal()const { return value; }
	void setVal(int val)
	{
		value = val;
		notify();
	}
	virtual void notify();
};


class IObserver
{
private :
	Subject* sub;
	int denom;
public :
	IObserver(Subject* s, int val) :
		sub(s),denom(val)
	{
		sub->addObs(this);
	}
	virtual void update() = 0;
protected :
	Subject* getSubject() { return sub; }
	int getDivisor() { return denom; }
};

void Subject::notify()
{
	int sz = views.size();
	for (int i = 0; i < sz; i++)
		views[i]->update();
}

class DivObserver : public IObserver
{
public :
	DivObserver(Subject* mod, int div) :
		IObserver(mod, div){}
	void update()
	{
		int v = getSubject()->getVal();
		int d = getDivisor();
		cout << v << " div " << d << "is " << v/d  << endl;
	}
};

class MobObserver : public IObserver
{
public :
	MobObserver(Subject* sub, int val):
		IObserver(sub,val){}
	void update()
	{
		int v = getSubject()->getVal();
		int d = getDivisor();
		cout << v << " mod " << d << "is " << v % d << endl;
	}
};

int main()
{
	Subject sub;
	DivObserver divObs1(&sub, 4);
	MobObserver divObs2(&sub, 4);
	sub.setVal(14);
	return 0;
}