#include "Monster.h"

CMonster::CMonster()
{
}

CMonster::CMonster(const CMonster& monster)
{
	// ��������� --> ���� ���� 
	*this = monster;
}

CMonster::~CMonster()
{
}

bool CMonster::Init(const char* Name, int Attack, int Armor, int HP, int MP, int Level, int Exp, int Gold)
{
	strcpy_s(m_Name, Name);
	m_Armor = Armor;
	m_Attack = Attack;
	m_HP = HP;
	m_MP = MP;
	m_Level = Level;
	m_Exp = Exp;
	m_Gold = Gold;

	return true;
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}
