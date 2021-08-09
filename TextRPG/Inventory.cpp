#include "Inventory.h"

CInventory* CInventory::m_pInst = nullptr;
CInventory::CInventory() : m_Count(0),m_Item{}
{
}

CInventory::~CInventory()
{
	for (int i = 0; i < m_Count; i++)
	{
		if (m_Item[i]) delete m_Item[i];
	}
}

void CInventory::Run()
{
}

bool CInventory::Init()
{
	return true;
}
