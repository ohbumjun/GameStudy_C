// object wrapping other object
// is a decorator object

// attaches aditional responsibilities to an obj
// 'dynamically' (run-time, compile-time)
// decoreator provide flexible alternatives
// to sub-classing for extending functionally

// Decoreator 또한 본래 class 를 상속받는다.
// 그러면서, 자기 자신이 interface 이기도 하다.

// 1. 데코레이터 패턴
// 객체에 기능을 추가할 수 있는 패턴
// 1. 상속을 통한 기능의 추가
// 2. 포함을 사용한 기능의 추가
// 3. 인터페이스 추가하기 

#include<iostream>

using namespace std;

// Interface 추가
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

// 상속을 이용한 기능의 추가
// 정적인 기능의 추가
// 코드를 작성할 때 기능이 추가 
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

// 포함을 사용한 기능의 추가
// 클래스가 아닌 객체에 기능을 추가할 수 있다. 
class RightMissile : public SpaceShip
{
	// 원본 객체 --> 포함 
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
