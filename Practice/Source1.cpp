#include<iostream>

using namespace std;

struct IComponent
{
	virtual void Fire() = 0;
	virtual ~IComponent() {};
};

class SpaceShip : public IComponent
{
public :
	int color;
	void Fire() { cout << "Fire Missile" << endl; }
};

class LeftMissile : public IComponent
{
	IComponent* ss;
public :
	LeftMissile(IComponent*p):ss(p){}
	void Fire()
	{
		ss->Fire();
		cout << "Left Missile" << endl;
	}
};

class RightMissile : public IComponent
{
	IComponent* ss;
public :
	RightMissile(IComponent*p):ss(p){}
	void Fire()
	{
		ss->Fire();
		cout << "Right Missile" << endl;
	}
};

int main()
{
	SpaceShip ss;
	ss.Fire();

	LeftMissile lm(&ss);
	lm.Fire();

	RightMissile rm(&lm);
	rm.Fire();
}