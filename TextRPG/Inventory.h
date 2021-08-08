#pragma once
#include"Game.h"

class CInventory
{
	CInventory();
	~CInventory();
private :
	class CItem* m_Item[INVENTORY_MAX];
	int m_Size;
private:
	static CInventory* m_pInst;
public:
	static CInventory* GetInst()
	{
		if (m_pInst == nullptr) m_pInst = new CInventory;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
	}
public :
	bool IsFull()
	{
		return m_Size == INVENTORY_MAX;
	}
	void AddItem(CItem* Item)
	{
		m_Item[m_Size] = Item;
		++m_Size;
	}

public:
	bool Init();
	void Run();
	class CItem* Equip(class CItem* Item);
};

