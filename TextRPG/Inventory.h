#pragma once
#include"Game.h"
class CInventory
{
	CInventory();
	~CInventory();
private :
	class CItem* m_Item[INVENTORY_MAX];
	int m_Count;
public : 
	void AddItem(class CItem* Item)
	{
		m_Item[m_Count] = Item;
		++m_Count;
	}
	bool IsFull() { return m_Count == INVENTORY_MAX; }
public :
	bool Init();
	int Menu(class CPlayer *pPlayer);
	void Run();
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
		if (m_pInst)
			delete m_pInst;
	}
};

