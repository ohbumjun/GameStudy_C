#include "GameManager.h"
#include"ObjectManager.h"
#include"Inventory.h"
#include"StoreManager.h"
#include"MapManager.h"

using namespace std;

CGameManager* CGameManager::m_pInst = nullptr;
bool CGameManager::Init()
{
	if (!CMapManager::GetInst()->Init()) return false;
	if (!CObjectManager::GetInst()->Init()) return false;
	if (!CInventory::GetInst()->Init()) return false;
	if (!CStoreManager::GetInst()->Init()) return false;
	return true;
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
			CStoreManager::GetInst()->Run();
			break;
		case Main_Menu::Inventory:
			CInventory::GetInst()->Run();
			break;
		case Main_Menu::Exit:
			return;
		}
	}
}

Main_Menu CGameManager::Menu()
{
	system("cls");
	cout << "1. 맵" << endl;
	cout << "2. 상점" << endl;
	cout << "3. 인벤토리" << endl;
	cout << "4. 종료" << endl;
	cout << "메뉴를 선택하세요 : ";
	int _Menu;
	cin >> _Menu;
	if (_Menu <= (int)Main_Menu::None || _Menu > (int)Main_Menu::Exit)
		return Main_Menu::None;
	return (Main_Menu)_Menu;
}
