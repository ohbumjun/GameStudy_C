#pragma once
#include"Game.h"

class CObjectManager
{
	CObjectManager();
	~CObjectManager();
private :
	class CPlayer* pPlayer;
	class CMonster* m_Monster[3];
	int LevelUpExp[LEVEL_MAX];
	class CItem* m_DropItem[6];
public :
	CPlayer* GetPlayer()
	{
		return pPlayer;
	}
	CItem* CreateItem(const char* Name, Item_Type Type, int Option, int Price,
		int Sell , const char* Desc);
	class CMonster* CloneMonster(const char*name);
private:
	static CObjectManager* m_pInst;
public:
	static CObjectManager* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CObjectManager;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
public:
	bool Init();
};

