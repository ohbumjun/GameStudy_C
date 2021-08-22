#include<iostream>
#include<vector>

using namespace std;

class IObserver;
class Subject
{
private :
	int m_Val;
	vector<IObserver*> m_Views;
public :
	void addObs(IObserver* obs)
	{
		m_Views.push_back(obs);
	}
	int getVal() const { return m_Val; }
	void setVal(int _val)
	{
		m_Val = _val;
		notify();
	}
	virtual void notify();
};

class IObserver
{
private :
	Subject* m_Sub;
	int m_Denom;
public :
	IObserver(Subject* sub, int denom)
	{
		m_Sub = sub;
		m_Denom = denom;
		m_Sub->addObs(this);
	}
	virtual void update() = 0;
protected :
	Subject* getSubject() const { return m_Sub; }
	int getDivisor() const { return m_Denom; }
};

class DivObserver : public IObserver
{
public :
	DivObserver(Subject* sub, int denom) : IObserver(sub,denom)
	{
	}
	void update()
	{
		int v = getSubject()->getVal();
		int d = getDivisor();
		cout << v << " div " << d << "is " << v/d << endl;
	}
};

class MobObserver : public IObserver
{
public :
	MobObserver(Subject* sub, int denom) : IObserver(sub, denom)
	{
	}
	void update()
	{
		int v = getSubject()->getVal();
		int d = getDivisor();
		cout << v << " mob " << d << "is " << v % d << endl;
	}
};

void Subject::notify()
{
	int sz = m_Views.size();
	for (int i = 0; i < sz; i++)
	{
		m_Views[i]->update();
	}
}

int main()
{
	Subject subj;
	DivObserver divObs(&subj, 3);
	MobObserver mobObs(&subj, 4);
	subj.setVal(13);
}