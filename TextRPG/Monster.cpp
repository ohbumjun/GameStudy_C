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
	// 복사생성자 --> 얕은 복사 
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
	cout << "이름 : "   << m_Name << endl;
	cout << "공격력 : " << m_Attack << endl;
	cout << "방어력 : " << m_Armor << endl;
	cout << "체력 : "   << m_HP << " / " << m_HPMax << endl;
	cout << "마력 : "   << m_MP << " / " << m_MPMax << endl;
	cout << "레벨 : "   << m_Level << endl;
	cout << "경험치 : " << m_Exp  << endl;
	cout << "골드 : "   << m_Gold << endl;
}
