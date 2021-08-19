#include<iostream>
#include<string>

using namespace std;

// Interface ( Component )
class MilkShake
{
public :
	virtual string Serve() = 0;
	virtual int Price() = 0;
};

// Concrete Component
class BaseMilkShake : public MilkShake
{
public :
	virtual string Serve() override
	{
		return "MilkShake" ;
	}
	virtual int Price() override
	{
		return 30;
	}
};

// Decorator
class MilkShakeDecorator : public MilkShake
{
protected:
	BaseMilkShake* baseMilkShake;
public :
	MilkShakeDecorator(BaseMilkShake* &b) :baseMilkShake(b){}
	virtual string Serve() override
	{
		return baseMilkShake->Serve();
	}
	virtual int Price() override
	{
		return baseMilkShake->Price();
	}
};

// Mango
class MangoMilkShakeDecorator : public MilkShakeDecorator
{
public :
	MangoMilkShakeDecorator(BaseMilkShake* &b) : MilkShakeDecorator(b){}
	virtual string Serve() override
	{
		return baseMilkShake->Serve() + " decorated with Mango";
	}
	virtual int Price() override
	{
		return baseMilkShake->Price() + 40;
	}
};

// Vanilla
class VanillaMilkShakeDecorator : public MilkShakeDecorator
{
public:
	VanillaMilkShakeDecorator(BaseMilkShake* &b) : MilkShakeDecorator(b) {}
	virtual string Serve() override
	{
		return baseMilkShake->Serve() + " decorated with Vanilla";
	}
	virtual int Price() override
	{
		return baseMilkShake->Price() + 80;
	}
};


int main()
{
	BaseMilkShake* baseMilkShake = new BaseMilkShake();
	MilkShake* basic = new MilkShakeDecorator(baseMilkShake);
	cout << basic->Serve() << endl;
	cout << basic->Price() << endl;
	delete basic;

	MilkShake* mango = new MangoMilkShakeDecorator(baseMilkShake);
	cout << mango->Serve() << endl;
	cout << mango->Price() << endl;

	return 0;
}