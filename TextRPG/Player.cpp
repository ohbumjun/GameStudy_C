#include "Player.h"
#include "Item.h"

using namespace std;

CPlayer::CPlayer() :
	m_Name{},
	m_Job(Job::Knight),
	m_Attack(0),
	m_Armor(0),
	m_HP(0),
	m_MP(0),
	m_MPMax(0),
	m_HPMax(0),
	m_Exp(0),
	m_Gold(0),
	m_Level(0),
	m_Equipment{}
{

}
CPlayer::~CPlayer()
{

}

bool CPlayer::Init()
{
	int _Menu;
	
	cout << "�̸��� �Է��ϼ��� :";
	cin >> m_Name;
	cout << endl;

	while (true)
	{
		system("cls");
		cout << "������ �����ϼ��� :" << endl ;
		cout << "1.���" << endl;
		cout << "2.�ü�" << endl;
		cout << "3.������" << endl;
		cin >> _Menu;
		if (_Menu >= 1 && _Menu <= 3) break;
	}
	
	switch (_Menu)
	{
	case 0 :
		m_Job = Job::Knight;
		m_Attack = 300;
		m_Armor = 150;
		m_HP = 500;
		m_MP = 500;
		m_MPMax = 500;
		m_HPMax = 500;
		break;
	case 1:
		m_Job = Job::Archer;
		m_Attack = 300;
		m_Armor = 150;
		m_HP = 500;
		m_MP = 500;
		m_MPMax = 500;
		m_HPMax = 500;
		break;
	case 2:
		m_Job = Job::Magicion;
		m_Attack = 300;
		m_Armor = 150;
		m_HP = 500;
		m_MP = 500;
		m_MPMax = 500;
		m_HPMax = 500;
		break;
	}
	m_Exp = 1;
	m_Level = 1;
	m_Gold = 1000;
	return true;
}

bool CPlayer::Damage(int Damage)
{
	m_HP -= Damage;
	if (m_HP <= 0)
	{
		m_HP = m_HPMax;
		return true;
	}
	return false;
}

void CPlayer::Death()
{
	m_Exp -= 0.1 * m_Exp;
	m_Gold -= 0.1 * m_Gold;
	m_HP = m_HPMax;
	m_MP = m_MPMax;
}

void CPlayer::Output()
{
	cout << "�̸� : " << m_Name << endl;
	cout << "���� : ";
	switch (m_Job)
	{
	case Job::Knight:
		cout << "���" ;
		break;
	case Job::Archer:
		cout << "�ü�";
		break;
	case Job::Magicion:
		cout << "������";
		break;
	}
	cout << endl;
	cout << "���ݷ� : " << m_Attack;
	if (m_Equipment[IT_Weapon])
		cout << " + " << m_Equipment[IT_Weapon]->GetOption() ;
	cout << endl;

	cout << "���� : " << m_Armor;
	if (m_Equipment[IT_Armor])
		cout << " + " << m_Equipment[IT_Armor]->GetOption();
	cout << endl;

	cout << "ü�� : " << m_HP << " / " << m_HPMax << endl;
	cout << "���� : " << m_MP << " / " << m_MPMax << endl;
	cout << "����ġ : " << m_Exp << endl;
	cout << "��� : " << m_Gold << endl;
	cout << "���� : " << m_Level << endl;

	
}