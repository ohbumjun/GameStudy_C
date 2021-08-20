#include<iostream>
#include<vector>
#include<string>

using namespace std;

struct MilkShake
{
	virtual string Serve() = 0;
	virtual int Price() = 0;
};

class BaseMilkShake : public MilkShake
{
public :
	BaseMilkShake(){}
	~BaseMilkShake(){}
public :
	string Serve() { return "MilkShake"; }
	int Price() { return 30; }
};

class MilkShakeDecorator : public MilkShake
{
protected :
	MilkShake* base;
public :
	MilkShakeDecorator(MilkShake* b):base(b)
	{
	}
public :
	virtual string Serve() { return base->Serve(); }
	virtual int Price() { return base->Price(); }
};

class MangoMilkShakeDecorator : public MilkShakeDecorator
{
public :
	MangoMilkShakeDecorator(MilkShake* b):MilkShakeDecorator(b){}
public :
	virtual string Serve() { return base->Serve() + " with Mango"; }
	virtual int Price() { return base->Price() + 40; }
};

class VanillaMilkShakeDecorator : public MilkShakeDecorator
{
public :
	VanillaMilkShakeDecorator(MilkShake* b): MilkShakeDecorator(b){}
public :
	virtual string Serve() { return base->Serve() + " with Vanilla"; }
	virtual int Price() { return base->Price() + 80; }
};

int main()
{
	BaseMilkShake* base = new BaseMilkShake();
	cout << base->Serve() << endl;
	cout << base->Price() << endl;
	cout << "--------------------" << endl;
	delete base;
	MangoMilkShakeDecorator* mangoBase = new MangoMilkShakeDecorator(new BaseMilkShake());
	cout << mangoBase->Serve() << endl;
	cout << mangoBase->Price() << endl;

	return 0;
}