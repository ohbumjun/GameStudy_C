/*
[ �ó����� ]
������ ĥ���� Ŭ������ �ִ�. 
ĥ������ ������ �а� ���Ѿ� �Ѵ�. 
�׷��� �� �� Ŭ������ ������ �������̽��� Ʋ���ٴ� ������ �ִ�.

[ ��� ]
���� Ŭ������ ��ӹް� ĥ���� Ŭ������ ��� ��Ų��. 
�׸��� ���� Ŭ���� �������̽��� ���ļ� ���� ��Ű�� �ȴ�.

*/

#include<iostream>
#include<string>

using namespace std;

// Target Interface
class Duck
{
public :
	virtual void Fly(void) = 0;
	virtual void Quack(void) = 0;
};

class MallardDuck : public Duck
{
public :
	virtual void Fly(void) { cout << "û�������� ���� ���� �־��" << endl; }
	virtual void Quack() { cout << "��" << endl; }
};

// Adaptee interface
class Turkey
{
public :
	virtual void Fly() = 0;
	virtual void Gobble() = 0;
};

class WildTurkey : public Turkey
{
public :
	virtual void Fly() { cout << "ĥ������ ���� ���� ���ƿ�" << endl; }
	virtual void Gobble() { cout << "���" << endl; }
};

class TurkeyAdapter : public Duck
{
private :
	Turkey* m_pTurkey;
public :
	TurkeyAdapter(Turkey* pTurkey) 
	{
		m_pTurkey = pTurkey;
	}
public :
	virtual void Fly() {
		cout << "���� �����ε�, ĥ������ ������" << endl;
		m_pTurkey->Fly(); 
	}
	virtual void Quack() { m_pTurkey->Gobble(); }

};

int main()
{
	Duck* pMallardDuck = new MallardDuck;
	pMallardDuck->Fly();
	pMallardDuck->Quack();

	Turkey* pWildTurkey = new WildTurkey;
	pWildTurkey->Fly();
	pWildTurkey->Gobble();

	TurkeyAdapter* pTurkeyAdapter = new TurkeyAdapter(pWildTurkey);
	pTurkeyAdapter->Fly();
	pTurkeyAdapter->Quack();

	return 0;
}