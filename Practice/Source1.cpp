#include<iostream>
#include<map>
#include<string>

using namespace std;

class Bullet
{
protected :
	string _bulletName;
public :
	Bullet(string bulletName) : _bulletName(bulletName){}
	virtual ~Bullet(){}
	virtual unique_ptr<Bullet> Clone() = 0;
	void fire()
	{
		cout << "Name : " << _bulletName << endl;
	}
};

class SimpleBullet : public Bullet
{
public :
	SimpleBullet(string bulletName) : Bullet(bulletName){}
	virtual unique_ptr<Bullet> Clone() override
	{
		// 복사 생성자 호출 
		return make_unique<SimpleBullet>(*this);
	}
};

class GoodBullet : public Bullet
{
public :
	GoodBullet(string bulletName) : Bullet(bulletName) {}
	virtual unique_ptr<Bullet> Clone() override
	{
		// 복사 생성자 호출 
		return make_unique<GoodBullet>(*this);
	}
};

enum BulletType
{
	SIMPLE,
	GOOD
};

class BulletFactory
{
private :
	map<BulletType, unique_ptr<Bullet>> m_Bullets;
public :
	BulletFactory()
	{
		m_Bullets[GOOD] = make_unique<GoodBullet>("Good");
		m_Bullets[SIMPLE] = make_unique<SimpleBullet>("Simple");
	}
	unique_ptr<Bullet> createBullet(BulletType Type)
	{
		return m_Bullets[Type]->Clone();
	}
};

int main()
{
	BulletFactory bf;
	auto Bullet = bf.createBullet(GOOD);
	Bullet->fire();
	return 0;
}