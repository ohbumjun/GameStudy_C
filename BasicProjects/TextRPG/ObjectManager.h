#pragma once
#include "Game.h"

class CObjectManager
{
	CObjectManager();
	~CObjectManager();
private : 
	class CPlayer* m_Player;
	class CMonster* m_Monster[3];
	class CItem* m_DropItem[6];
	int m_LevelUpExp[LEVEL_MAX];
public :
	class CPlayer* GetPlayer() { return m_Player; }
	int GetLevelUpExp(int Level)
	{
		return m_LevelUpExp[Level - 1];
	}
	class CItem* GetDropItem(int Index)
	{
		return m_DropItem[Index];
	}
public:
	class CMonster* CloneMonster(const char* Name);
	bool Init();
	class CItem* CreateItem(const char* Name, Item_Type Type, int Option, int Price, int Sell, const char* Desc);
private:
	static CObjectManager* m_pInst;
public:
	static CObjectManager* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CObjectManager;
		return m_pInst;
	}
	static void DestoryInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

