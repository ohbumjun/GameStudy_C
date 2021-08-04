#include "Monster.h"

using namespace std;

CMonster::CMonster() :
	m_Name{},
	m_Armor(0),
	m_Attack(0),
	m_HP(0),
	m_HPMax(0),
	m_MP(0),
	m_MPMax(0),
	m_Level(0),
	m_Exp(0),
	m_Gold(0)
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
	m_HPMax = HP;
	m_MP = MP;
	m_MPMax = MP;
	m_Level = Level;
	m_Exp = Exp;
	m_Gold = Gold;

	return true;
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

bool CMonster::Damage(int Damage)
{
	m_HP -= Damage;
	if (m_HP < Damage)
	{
		return true;
	}
	return false;
}

void CMonster::Output()
{
	cout << "�̸� : "   << m_Name << endl;
	cout << "���ݷ� : " << m_Attack << endl;
	cout << "���� : " << m_Armor << endl;
	cout << "ü�� : "   << m_HP << " / " << m_HPMax << endl;
	cout << "���� : "   << m_MP << " / " << m_MPMax << endl;
	cout << "���� : "   << m_Level << endl;
	cout << "����ġ : " << m_Exp  << endl;
	cout << "��� : "   << m_Gold << endl;
}
