#include "MapManager.h"

CMapManager* CMapManager::m_pInst = nullptr;
void CMapManager::Run()
{
}

bool CMapManager::Init()
{
    return true;
}

Map_Menu CMapManager::Menu()
{
    return Map_Menu();
}
