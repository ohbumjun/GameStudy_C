#include "Inventory.h"
#include "Item.h"

CInventory* CInventory::m_pInst = nullptr;

CInventory::CInventory() :
	m_Item{},m_Size(0)
{
}

CInventory::~CInventory()
{
	for (int i = 0; i < INVENTORY_MAX; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

bool CInventory::Init()
{
	return true;
}
void CInventory::Run()
{
}
CItem* CInventory::Equip(CItem* Item)
{
	Item_Type m_Type = Item->GetItemType();
	return nullptr;
}


