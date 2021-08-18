#pragma once
#include"Game.h"

class CMonster
{
public :
	CMonster();
	CMonster(const CMonster* Monster);
	~CMonster();
private:
	char m_Name[52];
	int m_Attack;
	int m_Armor;
	int m_HP;
	int m_HPMax;
	int m_MP;
	int m_MPMax;
	int m_Exp;
	int m_Gold;
	int m_Level;
public :
	int GetAttack() { return m_Attack; }
	int GetArmor() { return m_Armor; }
	int GetGold() { return m_Gold; }
	int GetExp() { return m_Exp; }
public :
	bool Init(const char* Name, int Attack, int Armor, int HP, int HPMax,
		int Exp, int Gold, int Level);
	CMonster* CloneMonster();
	void Output();
};

