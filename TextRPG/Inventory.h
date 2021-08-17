#pragma once
#include"Game.h"

class CInventory
{
	CInventory();
	~CInventory();
private :
	int m_Count;
	class CItem* m_Item[INVENTORY_MAX];
public :
	bool IsFull() { return m_Count == INVENTORY_MAX; }
	int GetItemCount() { return m_Count; }
	void AddItem(class CItem* Item)
	{
		m_Item[m_Count] = Item;
		++m_Count;
	}
public:
	void Run();
	int Menu(class CPlayer* player);
	bool Init();
private:
	static CInventory* m_pInst;
public:
	static CInventory* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CInventory;
		return m_pInst;
	}
	static void DestoryInst()
	{
		if (m_pInst) delete m_pInst;
	}
};

