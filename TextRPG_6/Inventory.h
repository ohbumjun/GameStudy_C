#pragma once
#include"Game.h"
class CInventory
{
public:
	void Run();
	bool Init();
	int Menu(class CPlayer *Player);
	class CItem* m_Item[INVENTORY_MAX];
private:
	static CInventory* m_pInst;
public:
	static CInventory* GetInst()
	{
		if (m_pInst == nullptr)
			m_pInst = new CInventory;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

