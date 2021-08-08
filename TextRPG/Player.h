#pragma once
#include"Game.h"

class CPlayer
{
public :
	CPlayer();
	~CPlayer();
private :
	char m_Name[52];
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
public :
	int GetAttack() { return m_Attack; }
	int GetArmor() { return m_Armor; }
	int GetGold() { return m_Gold; }
	CItem* GetEquipment(Equip_Type Type)
	{
		return m_Equipment[Type];
	}
	void AddGold(int Gold) { m_Gold += Gold; }
	void AddExp(int Exp) { m_Exp += Exp; }
public :
	bool Init();
	bool Damage(int Damage);
	void Death();
	void Output();
};

