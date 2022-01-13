#pragma once

#include "Game.h"

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

private:
	char    m_Name[32];
	Job     m_Job;
	int     m_Attack;
	int     m_Armor;
	int     m_HP;
	int     m_HPMax;
	int     m_MP;
	int     m_MPMax;
	int     m_Level;
	int     m_Exp;
	int     m_Gold;
	class CItem* m_Equipment[Equip_End];

public:
	int GetAttack()
	{
		return m_Attack;
	}

	int GetArmor()
	{
		return m_Armor;
	}

	int GetGold()
	{
		return m_Gold;
	}

	// �������� �����ϰ� �ִ� ���¿��� �ٸ� ���������� ��ü�� ���ٶ���
	// �����ϰ� �ִ� �������� ��ȯ���ٰ��̴�.
	class CItem* Equip(class CItem* Item);
	class CItem* GetEquipItem(Equip_Type Type)
	{
		return m_Equipment[Type];
	}

public:
	bool Init();
	void Output();
	bool Damage(int Damage);
	void AddGold(int Gold);
	void AddExp(int Exp);
	void Death();
};

