/*
--------------------------------------------------
>> Intent ?

- Creating an Object is expensive operation than
copy an object
- All objects "Initial State" is common and
takes time to build 

>> Example ? 
- In games, guns fire bullets,
but before it come out from gun
it needs to be prepared. 
- bullet prepared have common initial state

>> Diagram ? 

// will create either "SimpleBullet" or "GoodBullet"
// according to "type"
class BulletFactory
{
    unique_ptr<Bullet> createBullte(type){}
}

class Bullet
{
    fire(direction){}
    unique_ptr<Bulloet> clone(){}
}

class SimpleBullet : public Bullet
{
    unique_ptr<Bullet> clone()
    {
        return make_unique<SimpleBullet>(*this);
    }
}
class GoodBullet : public Bullet
{
    unique_ptr<Bullet> clone()
    {
        return make_unique<GoodBullet>(*this);
    }
}

>> What is Prototype Pattern ?

- creating new objects( instance ) by 
cloning (copying) other objects
- you create an object which is locked as an
default instance of some family of objects 
- then load it. while potentially overriding 
the defaults
- it eliminates the factory classes or 
builders for any possible subclass

즉 쉽게 말하면, 최근에 생성한 객체를 복사해서
이후에는, 새로운 객체를 생성하는 것이 아니라,
객체를 복제해서 return 하는 형태이다. 

*/

#include <iostream>
#include <map>
#include <string>
#include <memory>

using namespace std;

/** Bullet is the base Prototype */
class Bullet
{
protected:
    string _bulletName;
    float _direction;

public:
    Bullet() {}
    Bullet(string bulletName)
        : _bulletName(bulletName)
    {
    }
    virtual ~Bullet() {}
    virtual std::unique_ptr<Bullet> clone() = 0;
    void fire(float direction)
    {
        _direction = direction;

        cout << "Name        : " << _bulletName << endl
             << "Direction   : " << _direction << endl
             << endl;
    }
};

/** SimpleBullet is a Concrete Prototype */
class SimpleBullet : public Bullet
{

public:
    SimpleBullet(string bulletName) : Bullet(bulletName)
    {
    }

    unique_ptr<Bullet> clone() override
    {
        // 복사 생성자 호출
        return make_unique<SimpleBullet>(*this);
    }
};

/** GoodBullet is the Concrete Prototype */
class GoodBullet : public Bullet
{

public:
    GoodBullet(string bulletName)
        : Bullet(bulletName)
    {
    }

    unique_ptr<Bullet> clone() override
    {
        return std::make_unique<GoodBullet>(*this);
    }
};

/** Opaque Bullet type, 
 avoids exposing concrete implementations */
enum BulletType
{
    SIMPLE,
    GOOD
};

/** BulletFactory is the client */
class BulletFactory
{
private:
    map<BulletType, unique_ptr<Bullet>> m_Bullets;

public:
    BulletFactory()
    {
        m_Bullets[SIMPLE] = make_unique<SimpleBullet>("Simple Bullet");
        m_Bullets[GOOD] = make_unique<GoodBullet>("Good Bullet");
    }

    unique_ptr<Bullet> createBullet(BulletType BulletType)
    {
        return m_Bullets[BulletType]->clone();
    }
};

int main()
{
    BulletFactory bulletFactory;

    auto Bullet = bulletFactory.createBullet(SIMPLE);
    Bullet->fire(90);

    Bullet = bulletFactory.createBullet(GOOD);
    Bullet->fire(100);
}