#include "StoreManager.h"

CStoreManager* CStoreManager::m_pInst = nullptr;
void CStoreManager::Run()
{
}

bool CStoreManager::Init()
{
	return true;
}

Store_Menu CStoreManager::Menu()
{
	return Store_Menu();
}
