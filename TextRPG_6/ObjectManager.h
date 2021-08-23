#pragma once
#include"Game.h"

class CObjectManager
{
public :
	CObjectManager();
	~CObjectManager();
private :
	class CPlayer* m_Player;
	class CMonster* m_Monster[3];
	class CItem* m_DropItems[6];
	int m_LevelUpExp[10];
public :
	class CPlayer* GetPlayer()
	{
		return m_Player;
	}
	class CItem* GetDropItem(int Index)
	{
		return m_DropItems[Index];
	}
	int GetLevelUpExp(int Level) { return m_LevelUpExp[Level - 1]; }
public:
	bool Init();
	class CMonster* CloneMonster(const char* Name);
	class CItem* CloneItem(class CItem* Item);
	class CItem* CreateItem(const char* Name, Item_Type Type,
		int Price, int Option, int Sell, const char * Desc );
private:
	static CObjectManager* m_pInst;
public:
	static CObjectManager* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CObjectManager;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

