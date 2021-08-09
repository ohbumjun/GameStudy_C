#include "Player.h"

using namespace std;

CPlayer::CPlayer() :
	m_Name{},
	m_Job(Job::Archer),
	m_Attack(0),
	m_Armor(0),
	m_HP(0),
	m_HPMax(0),
	m_MP(0),
	m_MPMax(0),
	m_Exp(0),
	m_Gold(0),
	m_Level(0),
	m_Equipment{}
{
}

CPlayer::~CPlayer()
{
	for (int i = 0; i < Equip_End; i++)
	{
		if (m_Equipment[i]) delete m_Equipment[i];
	}
}

void CPlayer::AddExp(int Exp)
{
}
bool CPlayer::Init()
{
	int _Menu;
	cout << "이름을 입력하세요 :" << endl;
	cin >> m_Name;
	while (true)
	{
		system("cls");
		cout << "1. 기사" << endl;
		cout << "2. 궁수" << endl;
		cout << "3. 마법사" << endl;
		cin >> _Menu;
		if (_Menu >= 1 || _Menu <= 3) break;
	}
	switch (_Menu)
	{
	case (int)Job::Knight:
		m_Job = Job::Knight;
		m_Attack = 500;
		m_Armor = 500;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	case (int)Job::Archer:
		m_Job = Job::Archer;
		m_Attack = 500;
		m_Armor = 500;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	case (int)Job::Magician:
		m_Job = Job::Magician;
		m_Attack = 500;
		m_Armor = 500;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	}

	 m_Exp = 0;
	 m_Gold = 1000;
	 m_Level = 1;
	
	return true;
}
void Output()
{
	Job m_Job;
	int m_Attack;
	int m_Armor;
	int m_HP;
	int m_HPMax;
	int m_MP;
	int m_MPMax;
	int m_Exp;
	int m_Gold;
	int m_Level;
	class CItem* m_Equipment[Equip_End];
}

CItem* CPlayer::Equip(Equip_Type Type)
{
	return nullptr;
}

bool CPlayer::Damage(int Damage)
{
	return false;
}
