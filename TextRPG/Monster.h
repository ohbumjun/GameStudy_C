#pragma once
#include"Game.h"
class CMonster
{
public :
	CMonster();
	CMonster(const CMonster& monster);
	~CMonster();
private :
	char m_Name[52];
	int  m_Attack;
	int  m_Armor;
	int  m_HP;
	int  m_MP;
	int  m_Level;
	int  m_Exp;
	int  m_Gold;
public :
	int GetAttack() { return m_Attack; }
	int GetArmor() { return m_Armor; }
	int GetGold() { return m_Gold; }
	int GetExp() { return m_Exp; }
public :
	bool Init(const char* Name, int Attack, int Armor, int HP, int MP, int Level, int Exp, int Gold);
	class CMonster* Clone();
};

