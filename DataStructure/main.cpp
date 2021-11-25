#include<iostream>

// 기능 추가 : 상속, 포함
// 상속 : 추가변수가 필요 없다
// 포함 : 다형성을 이용할 수 있다

struct ICompont
{
	virtual void Fire() = 0;
};

struct SpaceShip : ICompont
{
	void Fire()
	{
		std::cout << "Fire SpaceShip" << std::endl;
	}
};

struct LeftMissile : ICompont
{
	ICompont* ss;
	LeftMissile(ICompont* c) : ss(c){}
	void Fire()
	{
		ss->Fire();
		std::cout << "Fire LeftMissile" << std::endl;
	}
};

struct RightMissle : ICompont
{
	ICompont* ss;
	RightMissle(ICompont* c) : ss(c) {}
	void Fire()
	{
		ss->Fire();
		std::cout << "Fire RightMissle" << std::endl;
	}
};

int main()
{
	SpaceShip ss;
	ss.Fire();
	LeftMissile lm(&ss);
	lm.Fire();
	RightMissle rm(&lm);
	rm.Fire();
	return 0;
}