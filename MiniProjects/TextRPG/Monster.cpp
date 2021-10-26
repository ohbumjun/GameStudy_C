#include "Monster.h"

using namespace std;

CMonster::CMonster() :
	m_Name{},
	m_Attack(0),
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
CMonster::CMonster(CMonster& monster)
{
	*this = monster;
}
;

CMonster::~CMonster()
{
}

void CMonster::Output()
{
	cout << "���� : " << m_Name << endl;
	cout << "���ݷ� : " << m_Attack << endl;
	cout << "���� : " << m_Armor << endl;
	cout << "ü�� : " << m_HP << " / " << m_HPMax << endl;
	cout << "���� : " << m_MP << " / " << m_MPMax << endl;
	cout << "����ġ : " << m_Exp << endl;
	cout << "���   : " << m_Gold << endl;
	cout << "����   : " << m_Level << endl;
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

bool CMonster::Damage(int Damage)
{
	m_HP -= Damage;
	if (m_HP <= 0) return true;
    return false;
}

bool CMonster::Init(const char* Name, int Attack, int Armor, int HP, int MP, int Exp, int Gold, int Level)
{
	strcpy_s(m_Name, Name);
	m_Attack = Attack;
	m_Armor = Armor;
	m_HP = HP;
	m_HPMax = HP;
	m_MP = MP;
	m_MPMax = MP;
	m_Exp = Exp;
	m_Gold = Gold;
	m_Level = Level;
    return true;
}
