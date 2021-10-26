#include "GameManager.h"
#include "Inventory.h"
#include "MapManager.h"
#include "ObjectManager.h"
#include "StoreManager.h"

using namespace std;

CGameManager* CGameManager::m_pInst = nullptr;
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

bool CGameManager::Init()
{
    if (!CObjectManager::GetInst()->Init()) return false;
    if (!CStoreManager::GetInst()->Init()) return false;
    if (!CInventory::GetInst()->Init()) return false;
    if (!CMapManager::GetInst()->Init()) return false;
    return true;
}

Main_Menu CGameManager::Menu()
{
    system("cls");
    cout << "1. ��" << endl;
    cout << "2. ����" << endl;
    cout << "3. �κ��丮" << endl;
    cout << "4. ����" << endl;
    cout << "�޴��� �����ϼ���" << endl;
    int _Menu;
    cin >> _Menu;

    if (_Menu <= 0 || _Menu > 4) return Main_Menu::None;
    return (Main_Menu)_Menu;
}
