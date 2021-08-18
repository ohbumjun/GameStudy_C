#include "Player.h"

using namespace std;

CPlayer::CPlayer() :
	m_Attack(0),
	m_Job(Job::Archer),
	m_Armor(0),
	m_HP(0),
	m_HPMax(0),
	m_MP(0),
	m_MPMax(0),
	m_Exp(0),
	m_Gold(0),
	m_Level(0)
{

}
CPlayer::~CPlayer()
{

}

void CPlayer::AddExp(int Exp)
{
}

void CPlayer::Output()
{
	cout << "직업 :" << endl;
	cout << "공격력 : " << endl;
	cout << "방어력 : " << endl;
	cout << "체력 : " << endl;
	cout << "마력 : " << endl;
	cout << "경험치 : " << endl;
	cout << "골드 : " << endl;
	cout << "레벨 : " << endl;

	m_Attack(0),
		m_Job(Job::Archer),

		m_Armor(0),
		m_HP(0),
		m_HPMax(0),
		m_MP(0),
		m_MPMax(0),
		m_Exp(0),
		m_Gold(0),
		m_Level(0)
}

void CPlayer::Death()
{
	m_Exp -= m_Exp * 0.1;
	m_Gold -= m_Exp * 0.1;
}
bool CPlayer::Init()
{
	int _Menu;
	while (true)
	{
		cout << "1. 기사" << endl;
		cout << "2. 궁수" << endl;
		cout << "3. 마법사" << endl;
		cout << "직업을 선택하세요" << endl;
		cin >> _Menu;
		if (1 <= _Menu && _Menu <= 3) break;
	}
	switch ((Job)_Menu)
	{
	case Job::Knight:
		m_Job = Job::Knight;
		m_Attack = 400;
		m_Armor = 300;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	case Job::Archer:
		m_Job = Job::Archer;
		m_Attack = 400;
		m_Armor = 300;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	case Job::Magician:
		m_Job = Job::Magician;
		m_Attack = 400;
		m_Armor = 300;
		m_HP = 2000;
		m_HPMax = 2000;
		m_MP = 1000;
		m_MPMax = 1000;
		break;
	}
	m_Exp   = 1;
	m_Gold  = 1500;
	m_Level = 1;

	return true;
}
void CPlayer::AddGold(int Gold)
{
	m_Gold += Gold;
}
