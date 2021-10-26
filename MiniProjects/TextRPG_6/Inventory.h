#pragma once
#include"Game.h"

class CInventory
{
public :
	CInventory();
	~CInventory();
private :
	int m_Size;
	class CItem* m_Item[INVENTORY_MAX];
public:
	void Run();
	bool Init();
	int Menu(class CPlayer *Player);
public :
	void AddItem(class CItem* Item)
	{
		m_Item[m_Size] = Item;
		++m_Size;
	}
	bool IsFull() const
	{
		return m_Size == INVENTORY_MAX;
	}
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

