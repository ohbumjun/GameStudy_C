#include "GameManager.h"
#include "ObjectManager.h"
#include "Inventory.h"
#include "MapManager.h"
#include "StoreManagaer.h"

using namespace std;

CGameManager* CGameManager::m_pInst = nullptr;

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{
}

Main_Menu CGameManager::Menu()
{
	cout << "1. 맵" << endl;
	cout << "2. 상점" << endl;
	cout << "3. 인벤토리" << endl;
	cout << "4. 종료" << endl;

	int _Menu;
	cin >> _Menu;
	if (_Menu <= 0 || _Menu > 4) return Main_Menu::None;

	return (Main_Menu)_Menu;
}

void CGameManager::Run()
{
	while (true)
	{
		switch (Menu())
		{
		case Main_Menu::Map:
			CMapManager::GetInst()->Run();
			break;
		case Main_Menu::Store:
			CStoreManagaer::GetInst()->Run();
			break;
		case Main_Menu::Inventory:
			CInventory::GetInst()->Run();
			break;
		case Main_Menu::Exit:
			return;
		}
	}
}

bool CGameManager::Init()
{
	// object
	if (!CObjectManager::GetInst()->Init()) return false;
	// store
	if (!CStoreManagaer::GetInst()->Init()) return false;
	// inventory
	if (!CInventory::GetInst()->Init()) return false;
	// map 
	if (!CMapManager::GetInst()->Init()) return false;
	return true;
}
