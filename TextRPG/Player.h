#pragma once
#include"Game.h"

class CPlayer
{
public :
	CPlayer();
	~CPlayer();
private :
	char m_Name[52];
	Job  m_Job;
	int  m_Attack;
	int  m_Armor;
	int  m_HP;
	int  m_MP;
	int  m_HPMax;
	int  m_MPMax;
	int  m_Level;
	int  m_Exp;
	int  m_Gold;
	class CItem* m_Equipment[Equip_End];
public :
	class CItem* Equip(class CItem* Item);
	class CItem* GetEquipItem(Equip_Type Type)
	{
		return m_Equipment[Type];
	}
public :
	int GetAttack() { return m_Attack; }
	int GetArmor() { return m_Armor; }
	int GetExp() { return m_Exp; }
	int GetGold() { return m_Gold; }
public :
	bool Init();
	void Output();
	void Death();
	void AddGold(int Gold);
	void AddExp(int Exp);
	bool Damage(int Damage);
};

