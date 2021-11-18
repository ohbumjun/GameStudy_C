// object wrapping other object
// is a decorator object

// attaches aditional responsibilities to an obj
// 'dynamically' (run-time, compile-time)
// decoreator provide flexible alternatives
// to sub-classing for extending functionally

// Decoreator ���� ���� class �� ��ӹ޴´�.
// �׷��鼭, �ڱ� �ڽ��� interface �̱⵵ �ϴ�.

// 1. ���ڷ����� ����
// ��ü�� ����� �߰��� �� �ִ� ����
// 1. ����� ���� ����� �߰�
// 2. ������ ����� ����� �߰�
// 3. �������̽� �߰��ϱ� 

#include<iostream>

using namespace std;

// Interface �߰�
struct IComponent
{
	virtual void Fire() = 0;
	virtual ~IComponent(){}
};

class I_SpaceShip : public IComponent
{
public :
	int color;
	void Fire() { cout << "Fire Missile" << endl; }
};

class MiddleMissile : public IComponent
{
	IComponent* ss;
public :
	MiddleMissile(IComponent *p) : ss(p){}
	void Fire()
	{
		ss->Fire();
		cout << "Fire Left Missile" << endl;
	}
};

class DownMissile : public IComponent
{
	IComponent* ss;
public:
	DownMissile(IComponent* p) : ss(p) {}
	void Fire()
	{
		ss->Fire();
		cout << "Fire Down  Missile" << endl;
	}
};

// ����� �̿��� ����� �߰�
// ������ ����� �߰�
// �ڵ带 �ۼ��� �� ����� �߰� 
class  SpaceShip
{
public  :
	int color;
	void Fire() { cout << "Fire Missile" << endl; }
};

class LeftMissile : public SpaceShip
{
public :
	void Fire()
	{
		SpaceShip::Fire();
		cout << "Fire Left Missile" << endl;
	}
};

// ������ ����� ����� �߰�
// Ŭ������ �ƴ� ��ü�� ����� �߰��� �� �ִ�. 
class RightMissile : public SpaceShip
{
	// ���� ��ü --> ���� 
	SpaceShip* ss;
public :
	RightMissile(SpaceShip* p) : ss(p){}
	void Fire()
	{
		ss->Fire();
		cout << "Fire Right Missile" << endl;
	}
};

int main()
{
	SpaceShip ss;
	ss.Fire();

	LeftMissile lm;
	lm.Fire();

	RightMissile rm(&lm);
	rm.Fire();
}
