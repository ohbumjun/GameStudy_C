#include "Monster.h"

using namespace std;

CMonster::CMonster():
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

CMonster::CMonster(const CMonster* Monster)
{
    *this = Monster;
}

CMonster::~CMonster()
{
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

CMonster* CMonster::Clone()
{
    return new CMonster(*this);
}

void CMonster::Output()
{
	cout << "이름 : "   << m_Name << endl;
	cout << "공격력 : " << m_Attack << endl;
	cout << "방어력 : " << m_Armor << endl;
	cout << "체력 : "   << m_HP << " / " << m_HPMax << endl;
	cout << "마력 : "   << m_MP << " / " << m_MPMax << endl;
	cout << "경험치 : " << m_Exp << endl;
	cout << "골드 : "   << m_Gold << endl;
	cout << "레벨 : "   << m_Level << endl;
}

bool CMonster::Damage(int Damage)
{
	m_HP -= Damage;
	if (m_HP < 0)
	{
		return true;
	}
	return false;
}
